/*
 *  GPUCalculationEngine.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 * Contains code from Apple's FBOBunies Example. License for that example is included below
 *
 *  File: main.c
 * 
 * Abstract: FBO bunnies
 * 
 * Version: 1.0
 * 
 * Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 * Computer, Inc. ("Apple") in consideration of your agreement to the
 * following terms, and your use, installation, modification or
 * redistribution of this Apple software constitutes acceptance of these
 * terms.  If you do not agree with these terms, please do not use,
 * install, modify or redistribute this Apple software.
 * 
 * In consideration of your agreement to abide by the following terms, and
 * subject to these terms, Apple grants you a personal, non-exclusive
 * license, under Apple's copyrights in this original Apple software (the
 * "Apple Software"), to use, reproduce, modify and redistribute the Apple
 * Software, with or without modifications, in source and/or binary forms;
 * provided that if you redistribute the Apple Software in its entirety and
 * without modifications, you must retain this notice and the following
 * text and disclaimers in all such redistributions of the Apple Software. 
 * Neither the name, trademarks, service marks or logos of Apple Computer,
 * Inc. may be used to endorse or promote products derived from the Apple
 * Software without specific prior written permission from Apple.  Except
 * as expressly stated in this notice, no other rights or licenses, express
 * or implied, are granted by Apple herein, including but not limited to
 * any patent rights that may be infringed by your derivative works or by
 * other works in which the Apple Software may be incorporated.
 * 
 * The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 * MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 * THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 * OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 * 
 * IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 * MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 * AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 * STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * Copyright © 2006 Apple Computer, Inc., All Rights Reserved
 */

#include <assert.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <string>
#include <iostream>
#include <fstream>

#include "GPUCalculationEngine.h"
#include "GPUGeometryModel.h"
#include "SimpleDesignByContract.h"
#include "MathHelper.h"

using namespace hdsim;
using namespace std;

#ifndef NDEBUG

template<class T> void writeColorBufferToCSVFile(const char *fileName, T *colors, int width, int height)
{
   // Write debug output
   ofstream myfile;
   myfile.open(fileName);
   
   for (int indexY = 0; indexY < height; indexY++)
   {
      for (int indexX = 0; indexX < width; indexX++)
      {
         myfile << colors[indexX + width*indexY];
         if (indexX < width - 1)
            myfile << ",";
      }
      
      myfile << endl;
   }
   
   myfile.close();  
}

#endif


bool hdsim::isWholeWorldSubstring(const char *searchIn, const char *searchFor)
{
   PRECONDITION(searchIn);
   PRECONDITION(searchFor);
   
   // Note that strstr doesn't account for terminator
   char *substringStartPosition = strstr(searchIn, searchFor);
   
   if (!substringStartPosition)
      return false;
   
   int searchInLength = strlen(searchIn);
   int searchForLength = strlen(searchFor);
   
   // Find how far in index it was found
   int fromStart = substringStartPosition - searchIn;
   
   // And make sure that position is last position in allExtensions string, or that next position is space
	bool delimitedAtEnd = (searchInLength == (fromStart + searchForLength))  ||  (*(substringStartPosition + searchForLength) == ' ');

   // Check that there is nothing before, or that what is before is space
   bool delimitedAtBegin = (substringStartPosition == searchIn)  ||  (*(substringStartPosition - 1) == ' ');

   return delimitedAtBegin  &&  delimitedAtEnd;
}

/**
 * Check is given OpenGL extension enabled
 */
static bool isOpenGLExtensionSupported(const char *extensionName)
{
   return hdsim::isWholeWorldSubstring(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)), extensionName);
}

/**
 * Check did OpenGL error occured. It would print what error was to stderr
 *
 * @return Was there an error
 * 
 * This function started as macro from Apples example and was subsequently modified
 */
static bool getAndResetGLErrorStatus()
{
	GLenum err = glGetError(); 
   
   if (err == GL_NO_ERROR)
      return false;
   
	while (err != GL_NO_ERROR) 
   {
		cerr << "glError: " << err << " detail: " << reinterpret_cast<const char *>(gluErrorString(err)) << " caught at " << __FILE__ << " line " << __LINE__ << endl;
		err = glGetError();
   }
   
   return true;
}

GPUCalculationEngine::GPUCalculationEngine() : wasInitialized_(false), offScreenMemory_(0)
{
}

GPUCalculationEngine::~GPUCalculationEngine() 
{
   if (wasInitialized_)
   {
   	destroyFrameBuffer();
   }
}

void GPUCalculationEngine::calculateEngine(const AbstractModel *model) 
{
   PRECONDITION(model);

   if (!wasInitialized_)
   {
      initialize(model);
   }
     
   const GPUGeometryModel *geometryModel = dynamic_cast<const GPUGeometryModel *>(model);
   CHECK(geometryModel, "This calculation engine operates only with the geometry model");

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Error binding frameBuffer");
   
   GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
   {
      cerr << "Error in calculateEngine's glCheckFramebufferStatusEXT, FBO status " <<  status << endl;
      
      // Try to restore everything, and cleanup errors
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
      getAndResetGLErrorStatus();
      return;
   }
   
   // Position camera
   glMatrixMode(GL_PROJECTION);
   CHECK(!getAndResetGLErrorStatus(), "Error setting projection matrix mode");

   glLoadIdentity();
   CHECK(!getAndResetGLErrorStatus(), "Error in glLoadIdentity");
   
   // Set camera and planes to Z_CORRECTION_FACTOR*z to avoid problems due to Z buffer aliasing
   glOrtho(geometryModel->getRenderedAreaMinX(), geometryModel->getRenderedAreaMaxX(), 
           geometryModel->getRenderedAreaMinY(), geometryModel->getRenderedAreaMaxY(),
           geometryModel->getRenderedAreaMinZ(), geometryModel->getRenderedAreaMaxZ());

   if (getAndResetGLErrorStatus())
   {
      // Try to restore everything, and cleanup errors
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
      getAndResetGLErrorStatus();
      return;
   }
   
   glMatrixMode(GL_MODELVIEW);
   CHECK(!getAndResetGLErrorStatus(), "Error setting modelView matrix mode");
   
   glLoadIdentity();
   CHECK(!getAndResetGLErrorStatus(), "Error in glLoadIdentity");
   
   gluLookAt(// Eye position
             0, 0, geometryModel->getRenderedAreaMaxZ(),
             // Center position
             0, 0, 0, 
             // Up position
             0, 1, 0); 
   
   CHECK(!getAndResetGLErrorStatus(), "Error in gluLookAt");
   
   glEnable(GL_DEPTH_TEST);
   CHECK(!getAndResetGLErrorStatus(), "Error in enabling depth test");
   
   glDepthMask(GL_TRUE);
   CHECK(!getAndResetGLErrorStatus(), "Depth mask failed");
   
   glClearDepth(1);
   CHECK(!getAndResetGLErrorStatus(), "Error setting value for clearing depth buffer");

   glClear(GL_DEPTH_BUFFER_BIT);
   CHECK(!getAndResetGLErrorStatus(), "Error clearing depth buffer");

   glDepthFunc(GL_LESS);
   CHECK(!getAndResetGLErrorStatus(), "Error setting depth function");
   
	glDrawBuffer(GL_NONE);
   CHECK(!getAndResetGLErrorStatus(), "Error disabling color drawing");
   
   glReadBuffer(GL_NONE);
   CHECK(!getAndResetGLErrorStatus(), "Error disabling read buffer for color");

   // All bound, now lets go ahead and draw
   // This might benefit from the display list approach
   // Orientation is counterclockwise here
   glBegin(GL_TRIANGLES);
   
   for (int indexTriangle = 0; indexTriangle < geometryModel->getNumTriangles(); indexTriangle++)
   {
      TriangleByPointIndexes triangle = geometryModel->getTriangle(indexTriangle);
      
      Point point = geometryModel->getPoint(triangle.getIndex1());
      glVertex3d(point.getX(), point.getY(), point.getZ());
      
      point = geometryModel->getPoint(triangle.getIndex2());
      glVertex3d(point.getX(), point.getY(), point.getZ());
      
      point = geometryModel->getPoint(triangle.getIndex3());
      glVertex3d(point.getX(), point.getY(), point.getZ());
   }
   glEnd();
   CHECK(!getAndResetGLErrorStatus(), "Error in glEnd");
   
   glReadPixels(0, 0, width_, height_, GL_DEPTH_COMPONENT, GL_FLOAT, renderedDepth_);   
   CHECK(!getAndResetGLErrorStatus(), "Error in glReadPixels");
   
   // Unbind frame buffer so that we could do rendering to different windows
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   CHECK(!getAndResetGLErrorStatus(), "Error binding frameBuffer");
}

bool GPUCalculationEngine::initFrameBuffer(int width, int height) 
{
   PRECONDITION(width > 0  &&  height > 0);
   
   // Following code is based on Apple OpenGL Programming Guide for Mac OS X, page 46
   const int PIXEL_MEM_SIZE = 32;
   const int BYTES_PER_PIXEL = PIXEL_MEM_SIZE/8;
   
   CGLPixelFormatAttribute openGLAttributes[] =
   {
      kCGLPFAOffScreen, 
      kCGLPFAColorSize, 
      static_cast<CGLPixelFormatAttribute>(PIXEL_MEM_SIZE), 
      static_cast<CGLPixelFormatAttribute>(0)
   }; 
   
   CGLPixelFormatObj pixelFormatObj; 
   GLint pixelFormatID;
   
   CGLChoosePixelFormat(openGLAttributes, &pixelFormatObj, &pixelFormatID);	
   CHECK(!getAndResetGLErrorStatus(), "GL Error Occured");
   
   CGLCreateContext(pixelFormatObj, 0, &cglContext_);
   CHECK(!getAndResetGLErrorStatus(), "GL Error Occured");

   CGLDestroyPixelFormat(pixelFormatObj); 
   CHECK(!getAndResetGLErrorStatus(), "GL Error Occured");

   CGLSetCurrentContext(cglContext_);
   CHECK(!getAndResetGLErrorStatus(), "GL Error Occured");

   offScreenMemory_ = new char[width * height * BYTES_PER_PIXEL];
   CGLSetOffScreen(cglContext_, width, height, width * BYTES_PER_PIXEL, offScreenMemory_);
   CHECK(!getAndResetGLErrorStatus(), "GL Error Occured");

   // Now, lets setup FBO objects (if supported)
	if (!isOpenGLExtensionSupported("GL_EXT_framebuffer_object"))
   {
      cerr << "GL_EXT_framebuffer_object is not supported" << endl;
      return false;
   }
   
   GLenum status;
   
   glGenFramebuffersEXT(1, &frameBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Can't generate framebuffer");
   
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Can't bind framebuffer");

   glGenRenderbuffersEXT(1, &renderBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Can't create renderbuffer");
   
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Can't bind renderbuffer");
   
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
   CHECK(!getAndResetGLErrorStatus(), "Can't create renderbuffer depth storage");
   
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Can't attach renderbuffer to framebuffer");
  
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);
  
   status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
   {
      cerr << "Error in glCheckFramebufferStatusEXT, FBO status " <<  status << endl;
      return false;
   }

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   
#pragma warning "START - DISABLE THIS DEBUGGING PART"   
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);

   glEnable(GL_DEPTH_TEST);
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);
   
   // Position camera
   glMatrixMode(GL_PROJECTION);
   CHECK(!getAndResetGLErrorStatus(), "Error setting projection matrix mode");
   
   glLoadIdentity();
   CHECK(!getAndResetGLErrorStatus(), "Error in glLoadIdentity");
   
   // Set camera and planes to Z_CORRECTION_FACTOR*z to avoid problems due to Z buffer aliasing
   glOrtho(-.5, .5, -.5, .5, 0, 5.5);
   CHECK(!getAndResetGLErrorStatus(), "Error");
   
   glMatrixMode(GL_MODELVIEW);
   CHECK(!getAndResetGLErrorStatus(), "Error");
   glLoadIdentity();
   CHECK(!getAndResetGLErrorStatus(), "Error");
   
   gluLookAt(// Eye position
             0, 0, 5,
             // Center position
             0, 0, 0, 
             // Up position
             0, 1, 0); 
   CHECK(!getAndResetGLErrorStatus(), "Error");

   glClearDepth(1);
   CHECK(!getAndResetGLErrorStatus(), "Error");
   glClear(GL_DEPTH_BUFFER_BIT);
   CHECK(!getAndResetGLErrorStatus(), "Error");
   glBegin(GL_TRIANGLES);
      glColor3i(0, 0, 0);
      glVertex3f(0, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 1, 0);
   glEnd();
   CHECK(!getAndResetGLErrorStatus(), "Error");
   
   GLfloat *colors = new GLfloat[width*height];
   
   glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, colors);
   CHECK(!getAndResetGLErrorStatus(), "Error");
   
   writeColorBufferToCSVFile("test.csv", colors, width, height);
   
   delete [] colors;
   
   // glOrtho NEAR and FAR cleaping plane are in the respect to eye point, not in the respect to the "center of the world"
#pragma warning "STOP - DISABLE THIS DEBUGGING PART"      
   
   return true;
}

bool GPUCalculationEngine::destroyFrameBuffer()
{
   PRECONDITION(wasInitialized_);
   
   // Don't use framebuffer extension any more
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   CHECK(!getAndResetGLErrorStatus(), "Error in glBindFramebufferEXT");
   
	glDeleteRenderbuffersEXT(1, &renderBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Error in renderBufferID_");

   glDeleteFramebuffersEXT(1, &frameBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Error in frameBufferID_");

   delete [] offScreenMemory_;
	delete [] renderedDepth_;
   
   CGLSetCurrentContext(0); 
   CHECK(!getAndResetGLErrorStatus(), "Error in CGLSetCurrentContext");
   
   CGLClearDrawable(cglContext_); 
   CHECK(!getAndResetGLErrorStatus(), "Error in CGLClearDrawable");

   CGLDestroyContext(cglContext_);
   CHECK(!getAndResetGLErrorStatus(), "Error in CGLDestroyContext");
   
	return !getAndResetGLErrorStatus();
}

void GPUCalculationEngine::initialize(const AbstractModel *model)
{
   PRECONDITION(model);
   
   const GPUGeometryModel *geometryModel = dynamic_cast<const GPUGeometryModel *>(model);
   CHECK(geometryModel, "This class works only with GPUGeometryModel");
   
   width_ = geometryModel->getSizeX();
   height_ = geometryModel->getSizeY();
   
   // Now we need to extract calculated Z buffer
   renderedDepth_ = new GLfloat[width_ * height_];
   
   bool status = initFrameBuffer(geometryModel->getSizeX(), geometryModel->getSizeY());
   CHECK(status, "Can't initialize frame buffer");
   wasInitialized_ = true;
}

void GPUCalculationEngine::deInitialize()
{
   destroyFrameBuffer();
   wasInitialized_ = false;
}

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

#include "GPUCalculationEngine.h"
#include "GPUGeometryModel.h"
#include "SimpleDesignByContract.h"

#include <assert.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <string>
#include <iostream>

using namespace hdsim;
using namespace std;

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
   return hdsim::isWholeWorldSubstring(extensionName, reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
}

/**
 * Check did OpenGL error occured. It would print what error was to stderr
 *
 * @return Was there an error
 * 
 * This function started as macro from Apples example and was subsequently modified
 */
static bool checkGLError()
{
	GLenum err = glGetError(); 
   
   if (err == GL_NO_ERROR)
      return false;
   
	while (err != GL_NO_ERROR) 
   {
		cerr << "glError: " << reinterpret_cast<const char *>(gluErrorString(err)) << " caught at " << __FILE__ << " line " << __LINE__;
		err = glGetError();
   }
   
   return true;
}

GPUCalculationEngine::GPUCalculationEngine() : wasInitialized_(false), offScreenMemory_(0)
{
}

GPUCalculationEngine::~GPUCalculationEngine() 
{
   destroyFrameBuffer();
}

void GPUCalculationEngine::calculate(const GPUGeometryModel *model)
{
	PRECONDITION(model);
   PRECONDITION(wasInitialized_);

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);
   
   GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
   {
      cerr << "Error, FBO status " <<  status;
      
      // Try to restore everything, and cleanup errors
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
      checkGLError();
      return;
   }

   // Position camera
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(model->getRenderedAreaMinX(), model->getRenderedAreaMaxX(), 
           model->getRenderedAreaMinY(), model->getRenderedAreaMaxY(),
           model->getBoundMaxZ(), model->getBoundMinZ());
   if (checkGLError())
   {
      // Try to restore everything, and cleanup errors
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
      checkGLError();
      return;
   }
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   gluLookAt(// Eye position
             0, 0, model->getBoundMaxZ(),
             // Center position
             0, 0, 0, 
             // Up position
             0, 1, 0); 

   glEnable(GL_DEPTH_TEST);
   glClearDepth(model->getBoundMinZ());
   glDepthFunc(GL_GEQUAL);
   
   // All bound, now lets go ahead and draw
   // This might benefit from the display list approach
   // Orientation is counterclockwise here
   glBegin(GL_TRIANGLES);
	   for (int indexTriangle = 0; indexTriangle < model->getNumTriangles(); indexTriangle++)
      {
         TriangleByPointIndexes triangle = model->getTriangle(indexTriangle);
         
         Point point = model->getPoint(triangle.getIndex1());
         glVertex3d(point.getX(), point.getY(), point.getZ());
         
         point = model->getPoint(triangle.getIndex2());
         glVertex3d(point.getX(), point.getY(), point.getZ());
         
         point = model->getPoint(triangle.getIndex3());
         glVertex3d(point.getX(), point.getY(), point.getZ());
      }
   glEnd();
   
   glReadPixels(0, 0, width_, height_, GL_DEPTH_COMPONENT, GL_DOUBLE, renderedDepth_);
     
   // And restore state
}

void GPUCalculationEngine::calculateEngine(const AbstractModel *model) 
{
   PRECONDITION(model);
   PRECONDITION(wasInitialized_);
   
   const GPUGeometryModel *geometryModel = dynamic_cast<const GPUGeometryModel *>(model);
   CHECK(geometryModel, "This calculation engine operates only with the geometry model");
   calculate(geometryModel);
}

bool GPUCalculationEngine::initFrameBuffer(int width, int height) 
{
   PRECONDITION(isOpenGLExtensionSupported("GL_EXT_framebuffer_object"));
   PRECONDITION(wasInitialized_);
   PRECONDITION(width > 0  &&  height < 0);
   
   // Following code is based on Apple OpenGL Programming Guide for Mac OS X, page 46
   const int PIXEL_MEM_SIZE = 32;
   
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
   CHECK(checkGLError(), "GL Error Occured");
   
   CGLCreateContext (pixelFormatObj, 0, &cglContext_);
   CHECK(checkGLError(), "GL Error Occured");

   CGLDestroyPixelFormat(pixelFormatObj); 
   CHECK(checkGLError(), "GL Error Occured");

   CGLSetCurrentContext(cglContext_);
   CHECK(checkGLError(), "GL Error Occured");

   offScreenMemory_ = new char[width * height * PIXEL_MEM_SIZE/8];
   CGLSetOffScreen(cglContext_, width, height, width * PIXEL_MEM_SIZE * 4, offScreenMemory_);
   CHECK(checkGLError(), "GL Error Occured");

   // Now, lets setup FBO objects
   
   GLenum status;
   
   glGenFramebuffersEXT(1, &frameBufferID_);
   CHECK(checkGLError(), "GL Error Occured");

   glGenRenderbuffersEXT(1, &renderBufferID_);
   CHECK(checkGLError(), "GL Error Occured");
   
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferID_);
   CHECK(checkGLError(), "GL Error Occured");

   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
   CHECK(checkGLError(), "GL Error Occured");

   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBufferID_);
   CHECK(checkGLError(), "GL Error Occured");
   
   // Allocate
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
   CHECK(checkGLError(), "GL Error Occured");
   
   // And bind
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferID_);
   CHECK(checkGLError(), "GL Error Occured");
   
   status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
      cerr << "Error, FBO status " <<  status;
   
   return false;
}

bool GPUCalculationEngine::destroyFrameBuffer()
{
   PRECONDITION(wasInitialized_);
   
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   glPopMatrix();
   glPopAttrib();
   CHECK(checkGLError(), "GL Error Occured");
   
   CGLSetCurrentContext(0); 
   CGLClearDrawable(cglContext_); 
   CGLDestroyContext(cglContext_);
   
   // Don't use framebuffer extension any more
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDeleteRenderbuffersEXT(1, &renderBufferID_);
   glDeleteFramebuffersEXT(1, &frameBufferID_);
   
   delete [] offScreenMemory_;
	delete [] renderedDepth_;
   
	return !checkGLError();
}

void GPUCalculationEngine::initialize(AbstractModel *model)
{
   PRECONDITION(model);
   
   GPUGeometryModel *geometryModel = dynamic_cast<GPUGeometryModel *>(model);
   CHECK(geometryModel, "This class works only with GPUGeometryModel");
   
   width_ = geometryModel->getSizeX();
   height_ = geometryModel->getSizeY();
   
   // Now we need to extract calculated Z buffer
   renderedDepth_ = new double[width_ * height_];
   
   initFrameBuffer(geometryModel->getSizeX(), geometryModel->getSizeY());
   wasInitialized_ = true;
}

void GPUCalculationEngine::deInitialize()
{
   destroyFrameBuffer();
   wasInitialized_ = false;
}

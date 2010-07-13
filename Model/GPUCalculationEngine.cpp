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

#include <CoreFoundation/CFBundle.h>

#include "GPUCalculationEngine.h"
#include "GPUGeometryModel.h"
#include "MathHelper.h"
#include "OGLUtils.h"
#include "SimpleDesignByContract.h"

using namespace hdsim;
using namespace std;

// Default shader to use if no other is available. It must exist in current working directory
static const char *NULL_SHADER_NAME = "./NullOpFragmentShader.fs";

GPUCalculationEngine::GPUCalculationEngine() : wasInitialized_(false), width_(0), height_(0), renderedDepth_(0), timeSlice_(0)
{
}

GPUCalculationEngine::~GPUCalculationEngine() 
{
   if (wasInitialized_)
   {
   	destroyFrameBuffer();
   }
}

bool GPUCalculationEngine::getPathToShaderFileAdopt(const GPUGeometryModel *model, std::string *path) const
{
   const int MAX_PATH_SIZE = 1024;
   char pathToFile[MAX_PATH_SIZE];

   if (strlen(model->getPathToShaderSource()) > MAX_PATH_SIZE - 1)
		return false;
   
   // If shader is not set, then use shader in the same directory
   if (strlen(model->getPathToShaderSource()) == 0)
   {
		strcpy(pathToFile, NULL_SHADER_NAME);
   }
   else
   {
      strcpy(pathToFile, model->getPathToShaderSource());      
   }
   
   *path = pathToFile;
   
   return true;
}

void GPUCalculationEngine::calculateEngine(const AbstractModel *model) 
{
   PRECONDITION(model);
   
   CGLContextObj currenCGLContext;
   
   if (!saveOpenGLState(&currenCGLContext))
   {
      cerr << "Error saving CGL Context" << endl;
      return;
   }
     
   if (!wasInitialized_)
   {
      currenCGLContext = CGLGetCurrentContext();
      initialize(model);
	}      
   
   const GPUGeometryModel *geometryModel = dynamic_cast<const GPUGeometryModel *>(model);
   CHECK(geometryModel, "This calculation engine operates only with the geometry model");
   
   CGLError error = CGLSetCurrentContext(cglContext_);
   if (error != kCGLNoError)
   {
      stringstream message;
      message << "Error in setting CGL context " << CGLErrorString(error);
      LOG(message.str().c_str());
      return;
   }

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);
   CHECK(!getAndResetGLErrorStatus(), "Error binding frameBuffer");

   CHECK(shader_.setShaderActive(true), "Can't set shader");
	CHECK(shader_.setShaderVariable(Shader::TIMESLICE_NAME, getTimeSlice()), "Can't reset shader");
   
   GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
   {
      GLenum err = glGetError(); 
      
      cerr << "Error in calculateEngine's glCheckFramebufferStatusEXT, FBO status " <<  status << "glError: " << err << " detail: " << reinterpret_cast<const char *>(gluErrorString(err)) << endl;
      
      // Try to restore everything, and cleanup errors
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
      getAndResetGLErrorStatus();
      restoreOpenGLState(currenCGLContext);
      return;
   }

   glMatrixMode(GL_PROJECTION);
   CHECK(!getAndResetGLErrorStatus(), "Error setting GL_PROJECTION");
   
   // Position camera
   glLoadIdentity();
   CHECK(!getAndResetGLErrorStatus(), "Error in glLoadIdentity");
   
   // Position of the near clip plane from the camera
   static const double NEAR_CLIP_PLANE_POSITION = 1.0;
   
   double farClipPlanePosition = geometryModel->getRenderedAreaMaxZ() - geometryModel->getRenderedAreaMinZ() +  
   										NEAR_CLIP_PLANE_POSITION + FLOATING_POINTS_LOW_PRECISION_EQUAL_DELTA;
   
   // Set camera and planes to Z_CORRECTION_FACTOR*z to avoid problems due to Z buffer aliasing
   glOrtho(geometryModel->getRenderedAreaMinX(), geometryModel->getRenderedAreaMaxX(), 
           geometryModel->getRenderedAreaMinY(), geometryModel->getRenderedAreaMaxY(),
           NEAR_CLIP_PLANE_POSITION, farClipPlanePosition);
   

   if (getAndResetGLErrorStatus())
   {
      // Try to restore everything, and cleanup errors
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
      getAndResetGLErrorStatus();
      restoreOpenGLState(currenCGLContext);
      return;
   }
   
   glMatrixMode(GL_MODELVIEW);
   CHECK(!getAndResetGLErrorStatus(), "Error setting modelView matrix mode");
   
   glLoadIdentity();
   CHECK(!getAndResetGLErrorStatus(), "Error in glLoadIdentity");
   
   gluLookAt(// Eye position
             0, 0, geometryModel->getRenderedAreaMaxZ() + NEAR_CLIP_PLANE_POSITION + FLOATING_POINTS_LOW_PRECISION_EQUAL_DELTA,
             // Center position
             0, 0, 0, 
             // Up position
             0, 1, 0); 
   
   CHECK(!getAndResetGLErrorStatus(), "Error in gluLookAt");
   
   // Set viewport so that one rod matches one pixel
   glViewport(0, 0, geometryModel->getSizeX(), geometryModel->getSizeY());
   CHECK(!getAndResetGLErrorStatus(), "Failed to setup viewport so that one rod matches one pixel");   

   status = prepareForDepthBufferDrawing();
   CHECK(status, "Preparation for drawing to depth buffer failed");
   
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
   
   CHECK(shader_.setShaderActive(false), "Can't reset shader");
   
   // Unbind frame buffer so that we could do rendering to different windows
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   CHECK(!getAndResetGLErrorStatus(), "Error binding frameBuffer");
  
   if (!restoreOpenGLState(currenCGLContext))
   {
      cerr << "Error in restoring OpenGL state" << endl;
   }
}

bool GPUCalculationEngine::initFrameBuffer(int width, int height) 
{
   PRECONDITION(width > 0  &&  height > 0);

   bool status = initOpenGLOffScreenRender(width, height, &cglContext_, &frameBufferID_, &colorBufferID_, &depthBufferID_);
   CHECK(status, "Initialization of offscreen rendering failed");
   
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   if (getAndResetGLErrorStatus())
   {
      cerr << "Error binding framebuffer" << endl;
      return false;
   }
   
   return true;
}

bool GPUCalculationEngine::destroyFrameBuffer()
{
   PRECONDITION(wasInitialized_);

   delete [] renderedDepth_;
   return destroyOpenGLOffScreenRender(cglContext_, frameBufferID_, colorBufferID_, depthBufferID_);
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

   string pathToShaderSource;

   status = getPathToShaderFileAdopt(geometryModel, &pathToShaderSource);
   CHECK(status, "Can't find shader source");
      
   status = shader_.initializeWithFragmentShaderOnly(pathToShaderSource.c_str());
   CHECK(status, "Can't initilize shader");
   
   wasInitialized_ = true;
}

void GPUCalculationEngine::deInitialize()
{
   destroyFrameBuffer();
   wasInitialized_ = false;
}

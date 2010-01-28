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

GPUCalculationEngine::GPUCalculationEngine() 
{
   assert(0);
}

GPUCalculationEngine::GPUCalculationEngine(const GPUCalculationEngine &rhs) 
{
   assert(0);  
}

GPUCalculationEngine &GPUCalculationEngine::operator=(const GPUCalculationEngine &rhs) 
{
   assert(0);  
}
   
GPUCalculationEngine::~GPUCalculationEngine() 
{
   assert(0);
}

void GPUCalculationEngine::positionCamera(int sizeX, int sizeY, double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
{
   assert(0);
}

void GPUCalculationEngine::calculate(GPUGeometryModel *model)
{
	PRECONDITION(model);
   
   /**
   // Initialize framebuffer extension
   
   // Create framebuffer
   void glGenFramebuffersEXT(GLsizei n, GLuint* ids)
   
   void glBindFramebufferEXT(GLenum target, GLuint id)
   
   // Create renderbuffer
   
   void glGenRenderbuffersEXT(GLsizei n, GLuint* ids)
   
   // And bind
   void glBindRenderbufferEXT(GLenum target, GLuint id)
   
   // Allocate
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, model->getSizeX(), model->getSizeY());
   
   void glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
                                     GL_DEPTH_ATTACHMENT_EXT,
                                     GL_RENDERBUFFER_EXT,
                                     GLuint renderbufferId)
   
   GLenum glCheckFramebufferStatusEXT(GLenum target)
   
   void glDeleteRenderbuffersEXT(GLsizei n, const Gluint* ids)
   
   // Delete framebuffer, not needed any more
   void glDeleteFramebuffersEXT(GLsizei n, const GLuint* ids)
    */
   
   assert(0);
}

void GPUCalculationEngine::calculateEngine(AbstractModel *model) {
   GPUGeometryModel *geometryModel = dynamic_cast<GPUGeometryModel *>(model);
   CHECK(geometryModel, "This calculation engine operates only with the geometry model");
   
   // Now, we need to do appropriate setup
   positionCamera(geometryModel->getSizeX(), geometryModel->getSizeY(),
                  geometryModel->getBoundMinX(), geometryModel->getBoundMaxX(),
                  geometryModel->getBoundMinY(), geometryModel->getBoundMaxY(),
                  geometryModel->getBoundMinZ(), geometryModel->getBoundMaxZ());
   
   calculate(geometryModel);
}

bool GPUCalculationEngine::initFrameBuffer(int width, int height) 
{
   PRECONDITION(isOpenGLExtensionSupported("GL_EXT_framebuffer_object"));
	
	// constant state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
   
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	
   GLenum status;
   
   glGenFramebuffersEXT(1, &frameBufferID_);
   glGenRenderbuffersEXT(1, &renderBufferID_);
   
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferID_);
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBufferID_);
   
   // Allocate
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
   
   // And bind
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferID_);
   
   status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
      cerr << "Error, FBO status " <<  status;
   return false;
}

bool GPUCalculationEngine::destroyFrameBuffer()
{
   glPopAttrib();
   
   // Don't use framebuffer extension any more
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDeleteRenderbuffersEXT(1, &renderBufferID_);
   glDeleteFramebuffersEXT(1, &frameBufferID_);
	return !checkGLError();
}


/* ======================================
 
//  Demonstrate a few ways to render-to-texture via FBO:
//  * cache rendering results (imposters)
//  * dynamic cubic environment map
//  * fullscreen shader effects
//
//  See the specification for full details:
//  http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt


typedef struct glExtension {
   char		*name;
   GLfloat		promoted;
   GLboolean	supported;
} glExtension;

typedef struct fbodesc {
	GLint  wide, high;
	GLenum color0;
	GLenum filter;
	GLenum depth;
} fbodesc;


// globals
glExtension	extension[] = {
	{"GL_EXT_framebuffer_object",   0.0, 0}
};

fbodesc fbos[NUMFBOS] = {
	{  512,  512, GL_TEXTURE_2D,           GL_LINEAR_MIPMAP_NEAREST, GL_RENDERBUFFER_EXT },
	{  128,  128, GL_TEXTURE_CUBE_MAP_ARB, GL_LINEAR,                GL_RENDERBUFFER_EXT },
	{ 1024, 1024, GL_TEXTURE_2D,           GL_LINEAR_MIPMAP_LINEAR,  GL_TEXTURE_2D       },
};

// extension index
enum {
	EXT_framebuffer_object
};

#pragma mark -

#pragma mark Window reshaping
void reshape(int width, int height, int windowaspect, int ortho) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
}


void display() {

#pragma mark Render inside FBO
	{
		glDisable(GL_TEXTURE_2D);
		if (mode >= 2) {	
			// render bunny to FBO
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb[FBO_BUNNY]);
			if (mode >= 4)
				glClearColor(0, 0, 0, 0);
			else
				glClearColor(0.5, 0.5, 0.5, 1);		
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			glTranslatef(0, 0, -1.35);
			drawbunny(rotx, roty, rotz, 1);	
		}
	}
	  
	// redirect rendering either to the window or DOF FBO
	if (mode >= 8) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb[FBO_DOF]);
		reshape(fbos[FBO_DOF].wide, fbos[FBO_DOF].high, 1, 0);
	}
	
	{
		// render real bunny geometry
		glClearColor(.3, .4, .5, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
		drawbunny(rotx, roty, rotz, 1);
	 
 */

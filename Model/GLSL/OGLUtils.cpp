/*
 * HoloSim, visualization and control of the moxel based environment.
 *
 * Copyright (C) 2010 Veljko Krunic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Contains code from Apple's FBOBunies Example. License for that example is included below
 *
 * In the case of the conflict of licenses for this file, you could still use other HoloSim code under the terms
 * of GNU Affero General Public License version 3 after the removal of the conflict.
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

#include "OGLUtils.h"

using namespace hdsim;
using namespace std;

#ifndef __APPLE__
#error This file is Mac specific, needs to be fixed for other platforms
#endif

template<class T> void hdsim::writeBufferToCSVFile(const char *fileName, T *array, int width, int height)
{
   // Write debug output
   ofstream myfile;
   myfile.open(fileName);
   
   for (int indexY = 0; indexY < height; indexY++)
   {
      for (int indexX = 0; indexX < width; indexX++)
      {
         myfile << array[indexX + width*indexY];
         if (indexX < width - 1)
            myfile << ",";
      }
      
      myfile << endl;
   }
   
   myfile.close();  
}

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

bool hdsim::isOpenGLExtensionSupported(const char *extensionName)
{
   return isWholeWorldSubstring(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)), extensionName);
}

bool hdsim::saveOpenGLState(CGLContextObj *savedContext)
{
   *savedContext = CGLGetCurrentContext();

   // If we have no OpenGL state, then don't do OpenGL operations
   if (!*savedContext)
   {
      return true;
   }
   
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error pushing attributes");
		return false;      
   }
   
   glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error pushing client attributes");
		return false;      
   }
   
   glMatrixMode(GL_MODELVIEW);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting GL_MODELVIEW");
		return false;      
   }
   
   glPushMatrix();
   if (getAndResetGLErrorStatus())
   {
      LOG("Error saving GL_MODELVIEW");
		return false;      
   }
   
   glMatrixMode(GL_PROJECTION);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting GL_PROJECTION");
		return false;      
   }
   
   glPushMatrix();
   if (getAndResetGLErrorStatus())
   {
      LOG("Error pushing GL_PROJECTION");
		return false;      
   }
   
   return true;
}

bool hdsim::restoreOpenGLState(CGLContextObj savedContext)
{
   // If there was not a saved context, don't worry
   if (!savedContext)
   {
      return true;
   }

   CGLError error = CGLSetCurrentContext(savedContext);
   if (error != kCGLNoError)
	{
      stringstream message;
     	message << "Error in setting CGL context " << CGLErrorString(error);
      LOG(message.str().c_str());
	   return false;
	}
   
   // Now, lets restore old state
   glMatrixMode(GL_PROJECTION);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting GL_PROJECTION mode");
		return false;      
   }
   
   glPopMatrix();
   if (getAndResetGLErrorStatus())
   {
      LOG("Error restoring projection GL_PROJECTION");
		return false;      
   }
   
   glMatrixMode(GL_MODELVIEW);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting GL_MODELVIEW mode");
		return false;      
   }
   
   glPopMatrix();
   if (getAndResetGLErrorStatus())
   {
      LOG("Error in restoring GL_MODELVIEW");
		return false;      
   }
   
   glPopClientAttrib();
   if (getAndResetGLErrorStatus())
   {
      LOG("Error restoring client attributes");
		return false;      
   }
   
   glPopAttrib();
   if (getAndResetGLErrorStatus())
   {
      LOG("Error restoring attributes");
		return false;      
   }
   
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error unbinding framebuffer");
		return false;      
   }
   
   return true;
}

bool hdsim::getAndResetGLErrorStatus()
{
	GLenum err = glGetError(); 
   
   if (err == GL_NO_ERROR)
      return false;
   
	while (err != GL_NO_ERROR) 
   {
      stringstream message;
      message << "glError: " << err << " detail: " << reinterpret_cast<const char *>(gluErrorString(err));
      LOG(message.str().c_str());
		err = glGetError();
   }
   
   return true;
}

bool hdsim::prepareForDepthBufferDrawing()
{
   glEnable(GL_DEPTH_TEST);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error in enabling depth test");
		return false;      
   }
   
   glDepthMask(GL_TRUE);
   if (getAndResetGLErrorStatus())
   {
      LOG("Depth mask failed");
		return false;
   }
   
   glClearDepth(1);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting value for clearing depth buffer");
		return false;      
   }

   glClearColor(0, 0, 0, 0);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting value for clearing depth buffer");
		return false;      
   }
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error clearing depth buffer");
		return false;      
   }
   
   glDepthFunc(GL_LESS);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting depth function");
		return false;      
   }
   
	glDrawBuffer(GL_NONE);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error disabling color drawing");
		return false;      
   }
   
   glReadBuffer(GL_NONE);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error disabling read buffer for color");
		return false;      
   }
   
   // As we would do read pixel later, we must setup pixel alignment to 1 to prevent overwritting buffer
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting pixel store for unpack alignment");
		return false;      
   }

   glPixelStorei(GL_PACK_ALIGNMENT, 1);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error setting pixel store for pack alignment");
		return false;      
   }
   
   return true;
}

// THIS FUNCTION HAS LEAKS IN THE CASE OF ERROR. SHOULD BE FIXED FOR PRODUCTION QUALITY CODE
bool hdsim::initOpenGLOffScreenRender(int width, int height, CGLContextObj *cglContext, GLuint *frameBufferID, GLuint *colorBufferID, GLuint *depthBufferID)
{
   // Following code is based on Apple OpenGL Programming Guide for Mac OS X, page 46. Although we would be using
   // renderbuffer and not pbuffer, we still need a context for the renderers so that we could check renderers
   // capabilities etc. With that being said, once when we have render buffer setup, we don't need to set this context
   // as a current context
   const int PIXEL_MEM_SIZE = 32;
   
   CGLPixelFormatAttribute openGLAttributes[] =
   {
      kCGLPFAPBuffer, 
      kCGLPFAAccelerated,
      kCGLPFANoRecovery,
      kCGLPFAMinimumPolicy,
      kCGLPFAColorSize, 
      static_cast<CGLPixelFormatAttribute>(PIXEL_MEM_SIZE), 
      static_cast<CGLPixelFormatAttribute>(0)
   }; 
   
   CGLPixelFormatObj pixelFormatObj; 
   GLint pixelFormatID;
   
   CGLError error;
   
   error = CGLChoosePixelFormat(openGLAttributes, &pixelFormatObj, &pixelFormatID);	
   if (error != kCGLNoError)
   {
      stringstream message;
      message << "Error choosing pixel format " << CGLErrorString(error);
      LOG(message.str().c_str());
      return false;
   }
   
   error = CGLCreateContext(pixelFormatObj, 0, cglContext);
   if (error != kCGLNoError)
   {
      CGLDestroyPixelFormat(pixelFormatObj); 

      stringstream message;
      message << "Error creating context " << CGLErrorString(error);
      LOG(message.str().c_str());
		return false;      
   }
   
   error = CGLDestroyPixelFormat(pixelFormatObj); 
   if (error != kCGLNoError)
   {
      stringstream message;
      message << "Error destroying pixel format " << CGLErrorString(error);
      LOG(message.str().c_str());
		return false;      
   }
   
   error = CGLSetCurrentContext(*cglContext);
   if (error != kCGLNoError)
   {
      stringstream message;
      message << "Error setting current context " << CGLErrorString(error);
      LOG(message.str().c_str());
		return false;      
   }
   
   // Now, lets setup FBO objects (if supported)
	if (!isOpenGLExtensionSupported("GL_EXT_framebuffer_object"))
   {
      LOG("GL_EXT_framebuffer_object is not supported");
      return false;
   }
   
   GLenum status;
   
   glGenFramebuffersEXT(1, frameBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't generate framebuffer");
      return false;
   }
   
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *frameBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't bind framebuffer");
      return false;
   }

   // Provide depth buffer renderbuffer
   glGenRenderbuffersEXT(1, depthBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't create renderbuffer");
      return false;
   }
   
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, *depthBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't bind renderbuffer");
      return false;
   }
   
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't create renderbuffer depth storage");
      return false;
   }
   
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, *depthBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't attach renderbuffer to framebuffer");
      return false;
   }

   // Provide color buffer renderbuffer
   glGenRenderbuffersEXT(1, colorBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't create renderbuffer");
      return false;
   }
   
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, *colorBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't bind renderbuffer");
      return false;
   }
   
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA8, width, height);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't create renderbuffer depth storage");
      return false;
   }
   
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, *colorBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't attach renderbuffer to framebuffer");
      return false;
   }

   status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
   {
      stringstream message;
      message << "Error in glCheckFramebufferStatusEXT, FBO status " <<  status;
      LOG(message.str().c_str());
      return false;
   }

   // Don't set framebuffer as active yet
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   if (getAndResetGLErrorStatus())
   {
      LOG("Can't unbind framebuffer");
      return false;
   }

   return true;
}

bool hdsim::changeCGLContext(CGLContextObj context)
{
   CGLError error = CGLSetCurrentContext(context);
   if (error != kCGLNoError)
   {
      stringstream message;
      message << "Error setting current context " << CGLErrorString(error);
      LOG(message.str().c_str());
		return false;      
   }
   
   return true;
}

bool hdsim::destroyOpenGLOffScreenRender(CGLContextObj cglContext, GLuint frameBufferID, GLuint colorBufferID, GLuint depthBufferID)
{
   // Don't use framebuffer extension any more
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error in glBindFramebufferEXT");
      return false;
   }
   
	glDeleteRenderbuffersEXT(1, &frameBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error in deleting framebuffer");
      return false;
   }
   
   glDeleteRenderbuffersEXT(1, &colorBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error in color renderbuffer");
      return false;
   }
   
	glDeleteRenderbuffersEXT(1, &depthBufferID);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error in deleting depth buffer");
      return false;
   }
   
   CGLError error;
   
   error = CGLSetCurrentContext(0); 
	if (error != kCGLNoError)
	{
      stringstream message;
      message << "Error in reseting current context " << CGLErrorString(error);
      LOG(message.str().c_str());
      return false;
   }
   
   error = CGLDestroyContext(cglContext);
	if (error != kCGLNoError)
	{
      stringstream message;
      message << "Error in CGLDestroyContext " << CGLErrorString(error);
      LOG(message.str().c_str());
      return false;
   }
   
	return true;
}


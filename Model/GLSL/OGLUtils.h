/*
 *  GPUCalculationEngine.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef OGL_UTILS_H_
#define OGL_UTILS_H_

#include <string>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/OpenGL.h>

#include "AbstractModel.h"
#include "GPUGeometryModel.h"

using namespace std;

namespace hdsim {
   
   /**
    * Check is this whole world substring
    *
    * @param searchFor - string to search for
    * @param searchIn - String to search in for searchFor
    */
   bool isWholeWorldSubstring(const char *searchIn, const char *searchFor);
   
   /**
    * Check did OpenGL error occured. It would print what error was to stderr
    *
    * @return Was there an error
    * 
    * This function started as macro from Apples example and was subsequently modified
    */
   bool getAndResetGLErrorStatus();
   
   /**
    * Write content of the array arranged in rectangle width*height to file
    * 
    * @param fileName Name of the file
    * @param array Array to use
    * @param width Width of the array
    * @param height Height of the array
    */
   template<class T> void writeBufferToCSVFile(const char *fileName, T *array, int width, int height);
   
   /**
    * Check is given OpenGL extension enabled
    */
   bool isOpenGLExtensionSupported(const char *extensionName);
   
   /**
    * Save complete OpenGL state
    *
    * @param savedContext - (OUT) Context variable that was saved
    * 
    * @return Was state saving success
    */
   bool saveOpenGLState(CGLContextObj *savedContext);
   
   /**
    * Restore OpenGL state
    *
    * @param savedContext - Context variable that was saved
    * 
    * @return Was state restoration success
    */
   bool restoreOpenGLState(CGLContextObj savedContext);
   
   /**
    * Prepare for depth buffer rendering
    *
    * @return Was preparation Success
    */
   bool prepareForDepthBufferDrawing();
   
   /**
    * Initialize OpenGL context so that offscreen rendering can happen
    *
    * @param width Width of the offscreen drawing region to create
    * @param height Height of the offscreen drawing region to create
    * @param cglContext (OUT) Created CGL context to use
    * @param frameBufferID (OUT) ID of the created framebuffer
    * @param colorBufferID (OUT) ID of the color buffer
    * @param depthBufferID (OUT) ID of the depth buffer
    *
    * @return Was OpenGL offscreen rendering success
    */
   bool initOpenGLOffScreenRender(int width, int height, CGLContextObj *cglContext, GLuint *frameBufferID, GLuint *colorBufferID, GLuint *depthBufferID);
   
   /**
    * Cleanup for depth buffer rendering to framebuffer
    *
    * @param cglContext context to use
    * @param frameBufferID Frame buffer ID to use
    * @param colorBufferID Color buffer ID to use
    * @param depthBufferID Depth buffer ID to use
    *
    * @return Was destroying contextes success
    */
   bool destroyOpenGLOffScreenRender(CGLContextObj cglContext, GLuint frameBufferID, GLuint colorBufferID, GLuint depthBufferID);
   
   /**
    * Change CGL context
    *
    * @param context New context to set
    *
    * @return Was change success
    */
   bool changeCGLContext(CGLContextObj context);
}

#endif

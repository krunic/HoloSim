/*
 *  Shader.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 2/20/10.
 *  Copyright © 2010 Veljko Krunic. All rights reserved.
 *
 * This code is based on OpenGL for Apple's Demo GLSLBasicCocoaDL. License for
 * the original code is included below
 * 
 *	File: Shader.m
 *
 *  Abstract: Rudimentary class to instantiate a shader object
 *            NOTE: This class does not validate the program object
 * 			 
 *  Disclaimer: IMPORTANT:  This Apple software is supplied to you by
 *  Inc. ("Apple") in consideration of your agreement to the following terms, 
 *  and your use, installation, modification or redistribution of this Apple 
 *  software constitutes acceptance of these terms.  If you do not agree with 
 *  these terms, please do not use, install, modify or redistribute this 
 *  Apple software.
 *  
 *  In consideration of your agreement to abide by the following terms, and
 *  subject to these terms, Apple grants you a personal, non-exclusive
 *  license, under Apple's copyrights in this original Apple software (the
 *  "Apple Software"), to use, reproduce, modify and redistribute the Apple
 *  Software, with or without modifications, in source and/or binary forms;
 *  provided that if you redistribute the Apple Software in its entirety and
 *  without modifications, you must retain this notice and the following
 *  text and disclaimers in all such redistributions of the Apple Software. 
 *  Neither the name, trademarks, service marks or logos of Apple Inc. may 
 *  be used to endorse or promote products derived from the Apple Software 
 *  without specific prior written permission from Apple.  Except as 
 *  expressly stated in this notice, no other rights or licenses, express
 *  or implied, are granted by Apple herein, including but not limited to
 *  any patent rights that may be infringed by your derivative works or by
 *  other works in which the Apple Software may be incorporated.
 *  
 *  The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 *  MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 *  THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 *  OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 *  
 *  IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 *  MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 *  AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 *  STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 * 
 *  Copyright (c) 2008 Apple Inc., All rights reserved.
 */

#include <OpenGL/OpenGL.h>

/**
 * GLSL Shader support
 */
class Shader
{
public:
   
   /**
    * Constructor
    */
   Shader();

   /**
    * Destructor
    */
   virtual ~Shader();
   
   /**
    * Initialize shaders
    *
    * @param vertexShaderFileName - Name of the file holding vertex shader
    * @param fragmentShaderFileName - Name of the file holding fragment shader
    *
    * @return Was initialization success
    */
   virtual bool initialize(const char *vertexShaderFileName, const char *fragmentShaderFileName);

   /**
    * Initialize with vertex shader only
    *
    * @param vertexShaderFileName - Name of the file holding vertex shader
    *
    * @return Was initialization success
    */
   virtual bool initializeWithVertexShaderOnly(const char *vertexShaderFileName);

   /**
    * Initialize with fragment shader only
    *
    * @param fragmentShaderFileName - Name of the file holding fragment shader
    *
    * @return Was initialization success
    */
   virtual bool initializeWithFragmentShaderOnly(const char *fragmentShaderFileName);
   
   /**
    * Set value of the particular shader variable
    *
    * @param name - Name of the shader variable
    * @param value - Value of the shader variable
    *
    * @return was setting value success
    */
   virtual bool setShaderVariable(const char *name, double value);

   /**
    * Get value of the particular shader variable
    *
    * @param name - Name of the shader variable
    * @param value - (OUT) Value of the shader variable
    *
    * @return was setting value success
    */
   virtual bool getShaderVariable(const char *name, double *value);
   
   /**
    * Set value of the particular shader variable
    *
    * @param shaderActive - Is shader active and used with OpenGL commands
    * 
    * @return Was setting success
    */
   virtual bool setShaderActive(bool shaderActive);
   
   /**
    * Get is shader variable attached
    *
    * @return Is shader currently attached
    */
   bool getShaderActive() const
   {
      return shaderActive_;
   }
  
private:
   
   // Prevent copy
   Shader(const Shader &rhs);
   Shader & operator=(const Shader &rhs);
   
   /**
    * Create and link program object associated with this two shaders
    *
    * @param vertexShader - Handle to vertex shader
    * @param fragmentShader - Handle to fragment shader
    *
    * @return Was preparation success
    */
   bool prepareShaders(GLhandleARB vertexShader, GLhandleARB fragmentShader);

   /**
    * Create and link program object associated with this vertex shader
    *
    * @param vertexShader - Handle to vertex shader
    *
    * @return Was preparation success
    */
   bool prepareVertexShaderOnly(GLhandleARB vertexShader);

   /**
    * Create and link program object associated with this fragment shader
    *
    * @param vertexShader - Handle to fragment shader
    *
    * @return Was preparation success
    */
   bool prepareFragmentShaderOnly(GLhandleARB vertexShader);
   
   /**
    * the program object
    */
	GLhandleARB programObject_;
   
   /**
    * Was shader initialized
    */
   bool wasInitialized_;
   
   /**
    * Is shader active
    */
   bool shaderActive_;
};


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
 *  Shader.cpp
 *  HoloSim
 *
 * In the case of the conflict of licenses for this file, you could still use other HoloSim code under the terms
 * of GNU Affero General Public License version 3 after the removal of the conflict.
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

#include "Shader.h"
#include "OGLUtils.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <OpenGL/OpenGL.h>
#include <cstring>

using namespace std;
using namespace hdsim;

const char *Shader::TIMESLICE_NAME = "timeSlice";


/**
 * Load shader
 *
 * @param shaderType - Type of the shader
 * @param shader - Name of the shader source
 * @param compiledShader - (OUT) Was shader successfully compiled
 *
 * @return handle on the shader. Valid only if *compiledShader is true
 */
static GLhandleARB loadShader(GLenum shaderType, const char *shader, GLint *compiledShader) 
{
	GLhandleARB shaderObject = 0;
	
	if (shader) 
	{
		GLint infoLogLength = 0;
		
		shaderObject = glCreateShaderObjectARB(shaderType);
      if (getAndResetGLErrorStatus())
      {
         LOG("Creation of shader object failed");
         return false;
      }
		
		glShaderSourceARB(shaderObject, 1, &shader, 0);
      if (getAndResetGLErrorStatus())
      {
         LOG("Reading of shader source failed");
         return false;
      }

		glCompileShaderARB(shaderObject);
      if (getAndResetGLErrorStatus())
      {
         LOG("Compile of shader source failed");
         return false;
      }
		
		glGetObjectParameterivARB(shaderObject, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoLogLength);
		
		if (infoLogLength > 0) 
		{
			char *infoLog = new char[infoLogLength];
			if (infoLog)
			{
				glGetInfoLogARB(shaderObject, infoLogLength, &infoLogLength, reinterpret_cast<GLcharARB *> (infoLog));
            
            stringstream message;
            message << "Shader compile log: " << infoLog;
				LOG(message.str().c_str());
				delete [] infoLog;
			}
		}
		
		glGetObjectParameterivARB(shaderObject, GL_OBJECT_COMPILE_STATUS_ARB, compiledShader);
		if (*compiledShader == 0 )
		{
         stringstream message;
         message << "Failed to compile shader " << shader;
         LOG(message.str().c_str());
		}
	}
	else 
	{
		*compiledShader = 1;
	}
	
	return shaderObject;
} 

/**
 * Link shader program
 *
 * @param programObject - Handle to program object that should be linked
 * @param programLinked - Program object linked
 *
 * @return Was program successfully linked
 */
static bool linkShader(GLhandleARB programObject, GLint *programLinked) 
{
	GLint infoLogLength = 0;
	
	glLinkProgramARB(programObject);
	glGetObjectParameterivARB(programObject, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoLogLength);
	
	if (infoLogLength >  0) 
	{
		char *infoLog = new char[infoLogLength];
		
		if (infoLog)
		{
			glGetInfoLogARB(programObject, infoLogLength, &infoLogLength, reinterpret_cast<GLcharARB *>(infoLog));
         
         stringstream message;
         message << "Program link log:" << infoLog;
         LOG(message.str().c_str());
			delete [] infoLog;
		}
	}
	
	glGetObjectParameterivARB(programObject, GL_OBJECT_LINK_STATUS_ARB, programLinked);
	
	if (!*programLinked)
	{
      stringstream message;
      message << "Failed to link program " << (GLubyte *)&programObject;
      LOG(message.str().c_str());
      return false;
	}
   
   return true;
} 

/**
 * Get shader source from the given filename
 *
 * @param fileName - Name of the file
 *
 * @return Loaded source. Caller owns this pointer and must call delete [] on it. 
 *         Returns empty string if the file can't be opened
 */
char *getShaderSourceFromResourceAdopt(const char *fileName)
{
   stringstream fileContent;
   ifstream inputFile(fileName);
   
   if (!inputFile.is_open())
   {
      stringstream message;
      message << "Unable to open file" << fileName;
      LOG(message.str().c_str());
      return 0;
   }
   
   while (!inputFile.eof())
   {
      string line;
      
      getline(inputFile, line);
		fileContent << line << endl;
   }
   
   inputFile.close();
   
   if (fileContent.str().empty())
   {
      return 0;
   }
   
   // Now we need to convert content to char *
   char *result = new char[fileContent.str().length() + 1];
   strcpy(result, fileContent.str().c_str());
   
   return result;
} 

/**
 * Load shader
 *
 * @param shaderType - Type of the shader
 * @param shaderSourceFileName - Name of the shader source file
 *
 * @return Handle on the shader or 0 if shader was not successfully loaded
 */
static GLhandleARB loadShader(GLenum shaderType, const char *shaderSourceFileName)
{
	GLint shaderCompiled = 0;
   
   char *shaderSource = getShaderSourceFromResourceAdopt(shaderSourceFileName);
   
	GLhandleARB shaderHandle = loadShader(shaderType, shaderSource, &shaderCompiled);
   delete [] shaderSource;
	
	if (!shaderCompiled) 
	{
		if (shaderHandle) 
		{
			glDeleteObjectARB(shaderHandle);
			shaderHandle = 0;
		}
	}
	
	return shaderHandle;
} 

/**
 * Get uniform location
 *
 * @param uniformName - Name of the uniform variable
 * @param programObject - Program object associated with uniform
 * @param uniformLocation - handle to uniform object or undefined if there is no such uniform object
 *
 * @return Was handle retrieval success
 */
static GLint getUniformLocation(const char *uniformName, GLhandleARB programObject, GLint *uniformLocation)
{
	GLint location = glGetUniformLocationARB(programObject, uniformName);

	// GLSL spec allows compiler to optimize away variables that are not used. As a result, not all of the -1 returns on 
   // location are result of the errors. As it is in the spec, it is not subject to discussion is this sane behavior - we 
   // have to live with it.
	if (location == -1) 
	{
      if (getAndResetGLErrorStatus())
      {
         stringstream message;
         message << "No such uniform named " << uniformName;
         LOG(message.str().c_str());
         return false;    
      }
 	}
   
   *uniformLocation = location;
	
	return true;
}

Shader::Shader() : programObject_(0), wasInitialized_(false), shaderActive_(false)
{
}

Shader::~Shader()
{
   if (programObject_)
	   glDeleteObjectARB(programObject_);
}

bool Shader::prepareShaders(GLhandleARB vertexShader, GLhandleARB fragmentShader)
{
	GLint programLinked = 0;
	
	// Create a program object and link both shaders
	programObject_ = glCreateProgramObjectARB();
	
	glAttachObjectARB(programObject_, vertexShader);
	glAttachObjectARB(programObject_, fragmentShader);
	
	if (!linkShader(programObject_, &programLinked)) 
	{
		glDeleteObjectARB(programObject_);
		programObject_ = 0;
		return false;
	}
	
	return true;
}

bool Shader::prepareVertexShaderOnly(GLhandleARB vertexShader)
{
	GLint programLinked = 0;
	
	// Create a program object and link both shaders
	programObject_ = glCreateProgramObjectARB();
	
	glAttachObjectARB(programObject_, vertexShader);
   if (getAndResetGLErrorStatus())
   {
      LOG("Attach of the vertex shader failed");
      return false;
   }
   
	if (!linkShader(programObject_, &programLinked)) 
	{
      LOG("Linking of shader objects failed");
      
		glDeleteObjectARB(programObject_);
      if (getAndResetGLErrorStatus())
      {
         LOG("Delete of program object failed");
         return false;
      }
      
		programObject_ = 0;
		return false;
	}
	
	return true;
}

bool Shader::prepareFragmentShaderOnly(GLhandleARB fragmentShader)
{
	GLint programLinked = 0;
	
	// Create a program object and link both shaders
	programObject_ = glCreateProgramObjectARB();
   if (getAndResetGLErrorStatus())
   {
      LOG("Create of program object failed");
      return false;
   }
	
	glAttachObjectARB(programObject_, fragmentShader);
   if (getAndResetGLErrorStatus())
   {
      LOG("Attach of fragment shader failed");
      return false;
   }
	
	if (!linkShader(programObject_, &programLinked)) 
	{
		glDeleteObjectARB(programObject_);
      if (getAndResetGLErrorStatus())
      {
         LOG("Delete of program object failed");
         return false;
      }
      
		programObject_ = 0;
		return false;
	}
	
	return true;
}

bool Shader::initialize(const char *vertexShaderFileName, const char *fragmentShaderFileName)
{
   PRECONDITION(fragmentShaderFileName  &&  vertexShaderFileName);
   
   GLhandleARB vertexShader, fragmentShader;
   
	// Load and compile both shaders
	vertexShader = loadShader(GL_VERTEX_SHADER_ARB, vertexShaderFileName);
	
	// Ensure vertex shader compiled
	if (!vertexShader)
	{
      return false;
   }
	
	fragmentShader = loadShader(GL_FRAGMENT_SHADER_ARB, fragmentShaderFileName);
		
	// Ensure fragment shader compiled
	if (!fragmentShader)
	{
      glDeleteObjectARB(vertexShader);
      return false;
	}
   
   // Create a program object and link both shaders
   wasInitialized_ = prepareShaders(vertexShader, fragmentShader);
	
   glDeleteObjectARB(vertexShader);
   glDeleteObjectARB(fragmentShader);
   
   return wasInitialized_;
}

bool Shader::initializeWithVertexShaderOnly(const char *vertexShaderFileName)
{
   PRECONDITION(vertexShaderFileName);
   
   GLhandleARB vertexShader;
   
   // Fragment shader only
   vertexShader = loadShader(GL_VERTEX_SHADER_ARB, vertexShaderFileName);
   
   if (!vertexShader)
   {
      return false;
   }
   
   // Create a program object and link both shaders
   wasInitialized_ = prepareVertexShaderOnly(vertexShader);
   glDeleteObjectARB(vertexShader);
   if (getAndResetGLErrorStatus())
   {
      LOG("Delete of vertex shader failed");
      return false;
   }
   
   return wasInitialized_;
}

bool Shader::initializeWithFragmentShaderOnly(const char *fragmentShaderFileName)
{
   PRECONDITION(fragmentShaderFileName);
   
   GLhandleARB fragmentShader;
   
   // Fragment shader only
   fragmentShader = loadShader(GL_FRAGMENT_SHADER_ARB, fragmentShaderFileName);
   
   if (!fragmentShader)
   {
      return false;
   }
   
   // Create a program object and link both shaders
   if (!prepareFragmentShaderOnly(fragmentShader))
   {
      return false;
   }
   
   glDeleteObjectARB(fragmentShader);
   
   wasInitialized_ = true;
   return true;
}

bool Shader::setShaderVariable(const char *uniformName, double value)
{
   PRECONDITION(wasInitialized_);
   
   GLint uniformLocation;
   
   // As compiler is free to optimize any uniform location that doesn't affect program output, the fact that getUniformLocation returned false is 
   // not an error
   if (getUniformLocation(uniformName, programObject_, &uniformLocation))
   {
      glUniform1fARB(uniformLocation, value);
   }
   
   return !getAndResetGLErrorStatus();
}

bool Shader::getShaderVariable(const char *name, double *value)
{
   PRECONDITION(wasInitialized_);
   
   GLint uniformLocation;
   
   // As compiler is free to optimize any uniform location that doesn't affect program output, the fact that getUniformLocation returned false is 
   // not an error
   if (getUniformLocation(name, programObject_, &uniformLocation))
   {
      GLfloat uniformValue;
      glGetUniformfvARB(programObject_, uniformLocation, &uniformValue);

      bool status = getAndResetGLErrorStatus();
      
      if (!status)
      {
         *value = uniformValue;
      }

      return status;
	}

	return true;
}

bool Shader::setShaderActive(bool shaderActive)
{
   PRECONDITION(wasInitialized_);
   
   GLhandleARB handleToUse = shaderActive ? programObject_ : 0;
   
   glUseProgramObjectARB(handleToUse);
   if (getAndResetGLErrorStatus())
   {
      LOG("Error in setShaderActive");
      shaderActive_ = false;
      return false;
   }
   
   shaderActive_ = shaderActive;
   return true;
}

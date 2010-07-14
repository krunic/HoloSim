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

#ifndef SHADER_TEST_H_
#define SHADER_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <OpenGL/OpenGL.h>

namespace hdsim {
   
   class ShaderTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(ShaderTest);
         CPPUNIT_TEST(testVertexAndFragmentShaderCreation);
         CPPUNIT_TEST(testVertexShaderCreation);
         CPPUNIT_TEST(testFragmentShaderCreation);      
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      ShaderTest();
      
      /**
       * Destructor
       */
      virtual ~ShaderTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();
      
      /**
       * Simple test of vertex and fragment shader creation
       */
      void testVertexAndFragmentShaderCreation();
      
      /**
       * Simple test of vertex creation
       */
      void testVertexShaderCreation();

      /**
       * Simple test of fragment creation
       */
      void testFragmentShaderCreation();
      
   private:
      
      // define
      ShaderTest(const ShaderTest &rhs);   
      ShaderTest & operator=(const ShaderTest &rhs);   
      
      CGLContextObj cglContext_;
      GLuint frameBufferID_;
      GLuint colorBufferID_;
      GLuint depthBufferID_;
   };
   
}

#endif
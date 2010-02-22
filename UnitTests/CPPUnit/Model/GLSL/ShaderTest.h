/*
 *  ShaderTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/16/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
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
      GLuint renderBufferID_;
   };
   
}

#endif
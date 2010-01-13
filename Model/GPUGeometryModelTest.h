/*
 *  GPUGeometryModelTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/12/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef CHECKBOARD_TEST_H_
#define CHECKBOARD_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class GPUGeometryModelTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(GPUGeometryModelTest);
      CPPUNIT_TEST(testModelName);
      CPPUNIT_TEST(testCopy);
      CPPUNIT_TEST(testEqual);
      CPPUNIT_TEST(testNonEqual);
      CPPUNIT_TEST(testDiferentDimsAreNonEqual);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      GPUGeometryModelTest();
      
      /**
       * Destructor
       */
      virtual ~GPUGeometryModelTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();
      
      /**
       * Simple test to ensure that name of the model is what we think it is
       */
      void testModelName();
      
      /**
       * Test that there is no aliasing after copy (so that copy constructor and operator= are working correctly
       */
      void testCopy();
      
      /**
       * Test read and write of the checkboard to the file
       */
      void testSerialization();
      
      /**
       * Test that read from the garbage file fails correctly 
       */
      void testReadFromGarbageFile();
      
      /**
       * Test operator==
       */
      void testEqual();
      
      /**
       * Test operator!=
       */
      void testNonEqual();
      
      /**
       * Test that different dimensions are non-equal
       */
      void testDiferentDimsAreNonEqual();
      
   private:
      // define
      GPUGeometryModelTest(const GPUGeometryModelTest &rhs);   
      GPUGeometryModelTest & operator=(const GPUGeometryModelTest &rhs);   
   };
   
}

#endif

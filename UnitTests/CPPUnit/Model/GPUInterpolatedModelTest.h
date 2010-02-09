/*
 *  GPUInterpolatedModelTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 2/4/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef GPU_INTERPOLATED_MODEL_TEST_H_
#define GPU_INTERPOLATED_MODEL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class GPUInterpolatedModelTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(GPUInterpolatedModelTest);
      	CPPUNIT_TEST(testModelName);
         CPPUNIT_TEST(testCopyConstructor);
         CPPUNIT_TEST(testOperatorEqual);
	      CPPUNIT_TEST(testSerialization);
	      CPPUNIT_TEST(testReadFromGarbageFile);
	      CPPUNIT_TEST(testEqual);
	      CPPUNIT_TEST(testNonEqual);
	      CPPUNIT_TEST(testGetFileNameInSameDir);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      GPUInterpolatedModelTest();
      
      /**
       * Destructor
       */
      virtual ~GPUInterpolatedModelTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();

      /**
       * Test that the model name is correctly set
       */
      void testModelName();
      
      /**
       * Test copy constructor
       */
      void testCopyConstructor();
      
      /**
       * Test operator equal
       */
      void testOperatorEqual();
      
      /**
       * Test model serialization
       */
      void testSerialization();
      
      /**
       * Test reading from the garbage file
       */
      void testReadFromGarbageFile();
      
      /**
       * Test operator=
       */
      void testEqual();
      
      /**
       * Test operator !=
       */
      void testNonEqual();
      
      /**
       * Test that file substitution is working correctly
       */
      void testGetFileNameInSameDir();
      
   private:
      
      // define
      GPUInterpolatedModelTest(const GPUInterpolatedModelTest &rhs);   
      GPUInterpolatedModelTest & operator=(const GPUInterpolatedModelTest &rhs);   
   };
   
}

#endif

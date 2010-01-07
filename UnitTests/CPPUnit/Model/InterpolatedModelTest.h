/*
 *  InterpolatedModelTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/24/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef INTERPOLATED_MODEL_TEST_H_
#define INTERPOLATED_MODEL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {

   class InterpolatedModelTest : public CppUnit::TestFixture
	{
      CPPUNIT_TEST_SUITE(InterpolatedModelTest);
         CPPUNIT_TEST(testModelName);
         CPPUNIT_TEST(testCopy);
         CPPUNIT_TEST(testInterpolation);
         CPPUNIT_TEST(testSerialization);
         CPPUNIT_TEST(testReadFromGarbageFile);
         CPPUNIT_TEST(testEqual);
         CPPUNIT_TEST(testNonEqual);
			CPPUNIT_TEST(testTimeSliceBoundaries);
			CPPUNIT_TEST(testMultiObjectInterpolation);
      CPPUNIT_TEST_SUITE_END();
      
   public:
         
      /**
       * Constructor
       */
      InterpolatedModelTest();
      
      /**
       * Destructor
       */
      virtual ~InterpolatedModelTest();
      
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
       * Test that interpolation is working correctly
       */
      void testInterpolation();
      
      /**
       * Test read and write of the checkboard to the file
       */
      void testSerialization();
      
      /**
       * Test that read from the garbage file fails correctly 
       */
      void testReadFromGarbageFile();
      
      /**
       * Test that we are correctly interpolating timeslices
       */
      void testTimeSliceBoundaries();

      /**
       * Test multiobject interpolation (e.g. interpolation when there are three different objects in the interpolated model)
       */
      void testMultiObjectInterpolation();
      
      /**
       * Test operator==
       */
      void testEqual();
      
      /**
       * Test operator!=
       */
      void testNonEqual();

      /**
       * Test operator!= when there is different number of models (e.g. first one three and second one five CheckBoard models in 
       * one interpolated model
       */
      void testNonEqualDifferentModels();
      
   private:
      // don't support copy
      InterpolatedModelTest(const InterpolatedModelTest &rhs);   
      InterpolatedModelTest & operator=(const InterpolatedModelTest &rhs);   
   };

} // namespace

#endif

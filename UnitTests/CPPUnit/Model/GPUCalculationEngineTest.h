/*
 *  GPUCalculationEngineTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/28/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef GPU_CALCULATION_ENGINE_TEST_H_
#define GPU_CALCULATION_ENGINE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class GPUCalculationEngineTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(GPUCalculationEngineTest);
	      CPPUNIT_TEST(testFindWholeWorldSubstring);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      GPUCalculationEngineTest();
      
      /**
       * Destructor
       */
      virtual ~GPUCalculationEngineTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();
      
      /**
       * Simple test that openGL extension processing is done correctly
       */
      void testFindWholeWorldSubstring();
      
   private:
      
      // define
      GPUCalculationEngineTest(const GPUCalculationEngineTest &rhs);   
      GPUCalculationEngineTest & operator=(const GPUCalculationEngineTest &rhs);   
   };
   
}

#endif
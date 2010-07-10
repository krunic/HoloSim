/*
 *  PerformanceTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/6/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef PERFORMANCE_TEST_H_
#define PERFORMANCE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class PerformanceTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(PerformanceTest);
	      CPPUNIT_TEST(testStatedPerforance);
      	CPPUNIT_TEST(testTimerGenerallyWorks);
      	CPPUNIT_TEST(testTimerResolutionBelow1ms);
      	CPPUNIT_TEST(testPreciseDelayTimer);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      PerformanceTest();
      
      /**
       * Destructor
       */
      virtual ~PerformanceTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();
      
      /**
       * Test that we are meeting contract specified in PhD
       */
      void testStatedPerforance();
      
      /**
       * Test that the timer generally works
       */
      void testTimerGenerallyWorks();
      
      /**
       * Test that the timer resolution is below 1ms
       */
      void testTimerResolutionBelow1ms();
      
      /**
       * Test precise delay timer
       */
      void testPreciseDelayTimer();
      
   private:
      // define
      PerformanceTest(const PerformanceTest &rhs);   
      PerformanceTest & operator=(const PerformanceTest &rhs);   
   };
   
}

#endif

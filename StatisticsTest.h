/*
 *  StatisticsTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/9/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef STATISTICS_TEST_H_
#define STATISTICS_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class StatisticsTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(StatisticsTest);
	      CPPUNIT_TEST(testCopyConstructor);
   	   CPPUNIT_TEST(testOperatorEqual);
      	CPPUNIT_TEST(testElapsedTime);
      	CPPUNIT_TEST(testAggregation);
      	CPPUNIT_TEST(testAggregateElapsedTime);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      StatisticsTest();
      
      /**
       * Destructor
       */
      virtual ~StatisticsTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();
      
      /**
       * Test that there copy constructor works correctly
       */
      void testCopyConstructor();
      
      /**
       * Test that operator equal works correctly
       */
      void testOperatorEqual();
      
      /**
       * Test that elapsed time is working correctly
       */
   	void testElapsedTime();
      
      /**
       * Test that aggregation is working correctly
       */
      void testAggregation();
      
      /**
       * Test that when we have multiple starts and stops, we are still getting correct statistics
       */
      void testAggregateElapsedTime();
            
   private:
      // define
      StatisticsTest(const StatisticsTest &rhs);   
      StatisticsTest & operator=(const StatisticsTest &rhs);   
   };
   
}

#endif

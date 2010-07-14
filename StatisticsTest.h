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
      	CPPUNIT_TEST(testRateIsCorrect);
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
         
      /**
       * Test that slower time to finish translates in slower rate
       */
      void testRateIsCorrect();
      
   private:
      // define
      StatisticsTest(const StatisticsTest &rhs);   
      StatisticsTest & operator=(const StatisticsTest &rhs);   
   };
   
}

#endif

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

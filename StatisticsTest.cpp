/*
 *  StatisticsTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/9/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <sstream>

#include "StatisticsTest.h"
#include "PreciseDelay.h"
#include "Statistics.h"
#include "MathHelper.h"

using namespace hdsim;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(StatisticsTest);

StatisticsTest::StatisticsTest()
{
   
}
   
StatisticsTest::~StatisticsTest()
{
   
}
   
void StatisticsTest::setUp()
{
   
}
   
void StatisticsTest::tearDown()
{
   
}

void StatisticsTest::testElapsedTime()
{
   static const long TIME_TO_SLEEP = 100000;
   static const double MAX_RELATIVE_ERROR = 0.01;
   static const int NUM_REPEAT = 4;
   static const int MAX_NUM_VIOLATIONS = NUM_REPEAT / 4;
   
   // Lets first start timer and pause for a while, to make sure that pure creation doesn't start timer
   Statistics testFixture;
   CPPUNIT_ASSERT_MESSAGE("Timer is autostarted", !testFixture.isTimerRunning());

	busyWaitDelay(TIME_TO_SLEEP);
   CPPUNIT_ASSERT_MESSAGE("Timer behaves as if autostarted", testFixture.getElapsedTimeInMicroSeconds() == 0);
   
   int numViolations = 0;
   
   for (int index = 0; index < MAX_NUM_VIOLATIONS; index++)
   {
      testFixture.startTimer();
      busyWaitDelay(TIME_TO_SLEEP);
      long elapsedTime = testFixture.getElapsedTimeInMicroSeconds();

		if (fabs(elapsedTime - TIME_TO_SLEEP)/TIME_TO_SLEEP > MAX_RELATIVE_ERROR)
      {
         numViolations++;
      }
      
      testFixture.resetStatistics();
   }
   
   char message[1024];
   sprintf(message, "Too many violations in measuring timer resolution (%d to be exact)", numViolations);
   CPPUNIT_ASSERT_MESSAGE(message, numViolations <= MAX_NUM_VIOLATIONS);
}

void StatisticsTest::testAggregateElapsedTime()
{
   static const double MICROSECONDS_IN_SECOND = 1000000;
   static const long TEST_INTERVAL = MICROSECONDS_IN_SECOND / 20;
   static const int NUM_INTERVALS_RUNNING = 3;
   static const double STATISTICS_VALUE_IN_PERIOD = 0.1;
   static const double MAX_RELATIVE_ERROR = 0.001;
   
   Statistics testFixture;
   
   testFixture.startTimer();
   busyWaitDelay(TEST_INTERVAL);
   testFixture.addAggregateStatistics(STATISTICS_VALUE_IN_PERIOD);
   testFixture.stopTimer();
   
   busyWaitDelay(3 * TEST_INTERVAL);
   
   for (int index = 0; index < NUM_INTERVALS_RUNNING - 1; index++)
   {
      testFixture.startTimer();
      busyWaitDelay(TEST_INTERVAL);
      testFixture.stopTimer();
   	testFixture.addAggregateStatistics(STATISTICS_VALUE_IN_PERIOD);
   }
   
   double valueInStatistics = testFixture.getTimeAveragedStatistics();
   double expectedValue = STATISTICS_VALUE_IN_PERIOD/(TEST_INTERVAL/MICROSECONDS_IN_SECOND);
   
   double relativeError = fabs(valueInStatistics - expectedValue) / expectedValue;
   
   stringstream message;
   message << "Aggregation is not working correctly, relative error is " << relativeError * 100 << "%";
   CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), relativeError <= MAX_RELATIVE_ERROR);
}

void StatisticsTest::testAggregation()
{
   Statistics testFixture;
   
   testFixture.addAggregateStatistics(1.0);
   testFixture.addAggregateStatistics(1.0);
   
   CPPUNIT_ASSERT_MESSAGE("Aggregation doesn't work properly", areEqual(testFixture.getAggregateStatistics(), 2.0));
}

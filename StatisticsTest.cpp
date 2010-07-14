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

static const double MAX_RELATIVE_ERROR = 0.005;   
static const double MICROSECONDS_IN_SECOND = 1000000;

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

void StatisticsTest::testCopyConstructor()
{
   static const double STATISTICS_VALUE = 0.1;
   
   Statistics original;
   original.addAggregateStatistics(STATISTICS_VALUE);
   
   Statistics constructorCopy(original);
   
   CPPUNIT_ASSERT_MESSAGE("Operator == doesn't work correctly", constructorCopy == original);
   
   // Check that both original and copy are in the same state as prior to copy
   CPPUNIT_ASSERT_MESSAGE("Original statistics value is wrong", areEqual(original.getAggregateStatistics(), STATISTICS_VALUE));
   CPPUNIT_ASSERT_MESSAGE("Original shouldn't have timer started", !original.isTimerRunning());
   
   CPPUNIT_ASSERT_MESSAGE("Constructor copy statistics value is wrong", areEqual(constructorCopy.getAggregateStatistics(), STATISTICS_VALUE));
   CPPUNIT_ASSERT_MESSAGE("Constructor copy shouldn't have timer started", !constructorCopy.isTimerRunning());
   
   long timeValueOfConstructorCopy = constructorCopy.getElapsedTimeInMicroSeconds();

   // Modify original by adding statistics and starting its timer
   original.startTimer();
   busyWaitDelay(100);
   original.addAggregateStatistics(STATISTICS_VALUE);
   
   // Check that original changed as we think it should
   CPPUNIT_ASSERT_MESSAGE("Original statistics value is wrong", areEqual(original.getAggregateStatistics(), 2*STATISTICS_VALUE));
   CPPUNIT_ASSERT_MESSAGE("Original timer shoud be started", original.isTimerRunning());
   
   // Make sure that copy didn't change
   CPPUNIT_ASSERT_MESSAGE("Constructor copy statistics value is wrong", areEqual(constructorCopy.getAggregateStatistics(), STATISTICS_VALUE));
   CPPUNIT_ASSERT_MESSAGE("Constructor copy shouldn't have timer started", !constructorCopy.isTimerRunning());
   CPPUNIT_ASSERT_MESSAGE("Constructor copy aggregate time shouldn't change", constructorCopy.getElapsedTimeInMicroSeconds() == timeValueOfConstructorCopy);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != doesn't work correctly", constructorCopy != original);
}

void StatisticsTest::testOperatorEqual()
{
   static const double STATISTICS_VALUE = 0.1;
   
   Statistics original;
   original.addAggregateStatistics(STATISTICS_VALUE);
   
   Statistics operatorEqualCopy;
   operatorEqualCopy = original;
   
   CPPUNIT_ASSERT_MESSAGE("Operator == doesn't work correctly", operatorEqualCopy == original);
   
   // Check that both original and copy are in the same state as prior to copy
   CPPUNIT_ASSERT_MESSAGE("Original statistics value is wrong", areEqual(original.getAggregateStatistics(), STATISTICS_VALUE));
   CPPUNIT_ASSERT_MESSAGE("Original shouldn't have timer started", !original.isTimerRunning());
   
   CPPUNIT_ASSERT_MESSAGE("Copy statistics value is wrong", areEqual(operatorEqualCopy.getAggregateStatistics(), STATISTICS_VALUE));
   CPPUNIT_ASSERT_MESSAGE("Copy shouldn't have timer started", !operatorEqualCopy.isTimerRunning());
   
   long timeValueOfCopy = operatorEqualCopy.getElapsedTimeInMicroSeconds();
   
   // Modify original by adding statistics and starting its timer
   original.startTimer();
   busyWaitDelay(100);
   original.addAggregateStatistics(STATISTICS_VALUE);
   
   // Check that original changed as we think it should
   CPPUNIT_ASSERT_MESSAGE("Original statistics value is wrong", areEqual(original.getAggregateStatistics(), 2*STATISTICS_VALUE));
   CPPUNIT_ASSERT_MESSAGE("Original timer shoud be started", original.isTimerRunning());
   
   // Make sure that copy didn't change
   CPPUNIT_ASSERT_MESSAGE("Copy statistics value is wrong", areEqual(operatorEqualCopy.getAggregateStatistics(), STATISTICS_VALUE));
   CPPUNIT_ASSERT_MESSAGE("Copy statistics shouldn't have timer started", !operatorEqualCopy.isTimerRunning());
   CPPUNIT_ASSERT_MESSAGE("Copy statistics aggregate time shouldn't change", operatorEqualCopy.getElapsedTimeInMicroSeconds() == timeValueOfCopy);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != doesn't work correctly", operatorEqualCopy != original);
}

void StatisticsTest::testElapsedTime()
{
   static const long TIME_TO_SLEEP = 100000;
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
   static const long TEST_INTERVAL = MICROSECONDS_IN_SECOND / 20;
   static const int NUM_INTERVALS_RUNNING = 3;
   static const double STATISTICS_VALUE_IN_PERIOD = 0.1;
   
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

void StatisticsTest::testRateIsCorrect()
{
   static const long CALC_DURATION = (long) floor(0.1 * MICROSECONDS_IN_SECOND);
   static const long SAMPLES_IN_INTERVAL = 100;
   
   static const double EXPECTED_RESULT = 0.5;
   
   Statistics firstIntervalTimer;
   Statistics secondIntervalTimer;
   
   firstIntervalTimer.startTimer();
   secondIntervalTimer.startTimer();

   	busyWaitDelay(CALC_DURATION);

   firstIntervalTimer.stopTimer();
   secondIntervalTimer.stopTimer();
   
   secondIntervalTimer.startTimer();
   	busyWaitDelay(CALC_DURATION);
   secondIntervalTimer.stopTimer();
   
   firstIntervalTimer.addAggregateStatistics(SAMPLES_IN_INTERVAL);
   secondIntervalTimer.addAggregateStatistics(SAMPLES_IN_INTERVAL);
   
   double firstIntervalResult = firstIntervalTimer.getTimeAveragedStatistics();
   double secondIntervalResult = secondIntervalTimer.getTimeAveragedStatistics();
   
   double ratio = secondIntervalResult / firstIntervalResult;
   
   double relativeError = fabs(ratio - EXPECTED_RESULT) / EXPECTED_RESULT;
   
   stringstream message;
   message << "Timers are not averaging correctly, relative error is " << relativeError * 100 << "%";
   CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), relativeError <= MAX_RELATIVE_ERROR);

   message.clear();
   
   ratio = firstIntervalTimer.getElapsedTimeInMicroSeconds() / (double) secondIntervalTimer.getElapsedTimeInMicroSeconds();
   
   relativeError = fabs(ratio - EXPECTED_RESULT) / EXPECTED_RESULT;

   message << "Timers are not averaging correctly, relative error is " << relativeError * 100 << "%";
   CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), relativeError <= MAX_RELATIVE_ERROR);
}

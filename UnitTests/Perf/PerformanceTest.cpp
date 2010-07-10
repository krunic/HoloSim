/*
 *  PerformanceTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/6/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <sstream>

#include <sys/time.h> 
#include <unistd.h> 

#include "PerformanceTest.h"
#include "GPUGeometryModel.h"
#include "MathHelper.h"
#include "PreciseDelay.h"

using namespace hdsim;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(PerformanceTest);

/**
 * max error for the time measurement
 */
static const long MAX_ABSOLUTE_ERROR_IN_MICROSECONDS = 100;

static struct timeval startTime_;
static bool timerStated_ = false;

/**
 * Start timer. This function is NOT thread-safe
 */
static void startTimer()
{
   struct timezone notUsed;
   
   timerStated_ = true;
   gettimeofday(&startTime_, &notUsed); 
}

/**
 * Stop timer, return time and reset timer. This function is NOT thread-safe
 *
 * @return Elapsed time in microseconds
 */
static long stopTimerAndGetTimeInMicroSeconds()
{
   struct timezone notUsed;
   struct timeval endTime;
   
   gettimeofday(&endTime, &notUsed);
   timerStated_ = false;
   
   // Recalculate time elapsed
   int endSecondsInMicro = endTime.tv_sec * 1000000;
   int startSecondsInMicro = startTime_.tv_sec * 1000000;

   return endSecondsInMicro - startSecondsInMicro + endTime.tv_usec - startTime_.tv_usec;
}

/**
 * Draw triangle covering part of the area, and measure drawing time
 *
 * @param sizeX Size of the grid (number of moxels) in X direction
 * @param sizeY Size of the grid (number of moxels) in Y direction
 *
 * @return Time needed to draw, in microseconds. Note that we can't guaran 
 */
int drawTriangleAndMeasureTimeInMicroSeconds(int sizeX, int sizeY)
{
   static const double Z_OFFSET = 0;
   
   // Expected offset of Z buffer. We would set quad at the half of the viewing frustum so Z buffer should be 1/2
   static const double QUAD_SIZE = 1;

   // Strictly speaking, this would measure initialization time of the fixture as opposed to only rendering time, but
   // we would set a test in the most conservative way possible as we are looking for the minimum of the achieved performance
   // (our PhD thesis proposal stated that we would reach "at least X" moxels per second). As this measure is critical for that
   // goal, we would set it up in such a way that all errors are on the side of underestimating actual performance achieved.
   startTimer(); 
      // Set a quad that covers the whole area
      GPUGeometryModel testFixture(sizeX, sizeY);
      
      // We are not running as bundle in content of unit test, so don't use bundled file
      testFixture.setUseBundledShaders(false);   

      testFixture.setRenderedArea(-2*QUAD_SIZE, -2*QUAD_SIZE, -2*QUAD_SIZE, 2*QUAD_SIZE, 2*QUAD_SIZE, 2*QUAD_SIZE);
      testFixture.addPoint(createPoint(-QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
      testFixture.addPoint(createPoint(-QUAD_SIZE, QUAD_SIZE, Z_OFFSET));
      testFixture.addPoint(createPoint(QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
      
      testFixture.addTriangle(createTriangle(0, 1, 2));
   	testFixture.getAt(0, 0);
   
   long drawTime = stopTimerAndGetTimeInMicroSeconds();

   // Used to make sure that previous test was working correctly and that we were not measuring phantom result
   
   // We would do scanline - there should be two levels in each scanline, and second level should be continious
   // until exit. If we are outside of the polygon, z buffer value should be zero
   const double Z_INFINITY = 1;
   const double Z_EXPECTED = 0.5;
   
   bool quadDetected = false;   
   
   for (int indexY = 0; indexY < sizeY; indexY++)
   {
      bool scanlineEntered = false;
      bool scanlineExited = false;
      
      // We are scanning along X axis
      for (int indexX = 0; indexX < sizeX; indexX++)
      {
         double zValue = testFixture.getAt(indexX, indexY);
         
         stringstream message;
         message << "Error at the coordinates X = " << indexX << " Y = " << indexY << " for value " << zValue;
         
         CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), areEqualInLowPrecision(zValue, Z_INFINITY)  ||  areEqualInLowPrecision(zValue, Z_EXPECTED));
         
         if (areEqualInLowPrecision(zValue, Z_EXPECTED))
         {
            CPPUNIT_ASSERT_MESSAGE("We can encounter Z_OFFSET only in quad or if we didn't entered quad before", !scanlineExited);
            scanlineEntered = true;
            quadDetected = true;
         } 
         else
         {
            // We have Z value equal to 0 if we are here 
            CPPUNIT_ASSERT_MESSAGE("Internal error in the test - zValue should be 1 here", areEqualInLowPrecision(zValue, Z_INFINITY));
            
            // If we were already inside the quad and we encountered infinity, we now should be outside of quad
            scanlineExited = scanlineEntered;
         }
      }
   }

   CPPUNIT_ASSERT_MESSAGE("Quad was not detected", quadDetected);
   CPPUNIT_ASSERT_MESSAGE("Quad was never entered or detected", quadDetected);
   
   return drawTime; 
}

PerformanceTest::PerformanceTest()
{

}

PerformanceTest::~PerformanceTest()
{
   
}

void PerformanceTest::setUp()
{
   
}

void PerformanceTest::tearDown()
{
   
}

void PerformanceTest::testStatedPerforance()
{
   // Lets do single triangle on square
   static const int X_DIMENSION = 500;
   static const int Y_DIMENSION = 500;
   static const int NUM_MOXELS = X_DIMENSION * Y_DIMENSION;
   
   long timeElapsed = drawTriangleAndMeasureTimeInMicroSeconds(X_DIMENSION, Y_DIMENSION);
   
   double achievedMoxelsPerSecond = 1000000 * ((double)NUM_MOXELS)/timeElapsed;
   
   // This was agreed as min performance for my PhD thesis. Actually, we are using 10M as opposed to 100,000 that were negotiated, to show that we have much faster computation
   // then min required
   static const double STATED_MOXELS_PER_SECOND = 10000000.0;
   
   char message[1024];
   sprintf(message, "We are achieving only %lf instead of the stated %lf moxels per second", achievedMoxelsPerSecond, STATED_MOXELS_PER_SECOND);
   CPPUNIT_ASSERT_MESSAGE(message, achievedMoxelsPerSecond >= STATED_MOXELS_PER_SECOND);
}

void PerformanceTest::testTimerGenerallyWorks()
{
   static const long TIME_TO_WAIT_IN_MICROSECONDS = 500000;
   
   // max relative error is 1% for timer resolution at TIME_TO_WAIT to be considered generally acceptable
   static const double MAX_RELATIVE_ERROR = 0.001;
   
   startTimer();
   busyWaitDelay(TIME_TO_WAIT_IN_MICROSECONDS);
   long timeElapsed = stopTimerAndGetTimeInMicroSeconds();
   
   double relativeError = fabs(timeElapsed - TIME_TO_WAIT_IN_MICROSECONDS) / TIME_TO_WAIT_IN_MICROSECONDS;  
   
   CPPUNIT_ASSERT_MESSAGE("Timer is not even close to real elapsed time", relativeError <= MAX_RELATIVE_ERROR);
}

void PerformanceTest::testTimerResolutionBelow1ms()
{
   static const long TIME_TO_WAIT_IN_MICROSECONDS = 50000;
      
   startTimer();
   busyWaitDelay(TIME_TO_WAIT_IN_MICROSECONDS);
      
   double absoluteError = fabs(stopTimerAndGetTimeInMicroSeconds() - TIME_TO_WAIT_IN_MICROSECONDS);  
   
   stringstream message;
   message << "Absolute error " << absoluteError << " is too large";
   CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), absoluteError <= MAX_ABSOLUTE_ERROR_IN_MICROSECONDS);
}

void PerformanceTest::testPreciseDelayTimer()
{
   static const long TIME_TO_WAIT_MICROSECONDS = 50000;
   
   startTimer();
	busyWaitDelay(TIME_TO_WAIT_MICROSECONDS);
   long timeElapsed = stopTimerAndGetTimeInMicroSeconds();
   
   double absoluteError = fabs(TIME_TO_WAIT_MICROSECONDS - timeElapsed);
   
   stringstream message;
   message << "Error too large, absolute error is " << absoluteError;
   CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), absoluteError <= MAX_ABSOLUTE_ERROR_IN_MICROSECONDS); 
}

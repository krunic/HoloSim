/*
 *  Statistics.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/9/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include "Statistics.h"
#include "SimpleDesignByContract.h"

using namespace hdsim;

Statistics::Statistics() : timerActive_(false), aggregateTimeElapsed_(0), aggregateStatistics_(0)
{

}
      
Statistics::~Statistics()
{
   
}
      
void Statistics::addAggregateStatistics(double value)
{
   aggregateStatistics_ += value;
}
      
double Statistics::getAggregateStatistics() const
{
   return aggregateStatistics_;
}
      
double Statistics::getTimeAveragedStatistics() const
{
	double timeInSeconds = getElapsedTimeInMicroSeconds()/1000000.0;
   
   return getAggregateStatistics()/timeInSeconds;
}
      
long int Statistics::getElapsedTimeSinceLastTimerStartInMicroSeconds() const
{
   CHECK(isTimerRunning(), "Timer should be running to invoke this method");
   
   struct timeval endTime;
   struct timezone notUsed;
   
   gettimeofday(&endTime, &notUsed);
   
   // Recalculate time elapsed
   int endSecondsInMicro = endTime.tv_sec * 1000000;
   int startSecondsInMicro = startTime_.tv_sec * 1000000;
   
   return endSecondsInMicro - startSecondsInMicro + endTime.tv_usec - startTime_.tv_usec;
}

long int Statistics::getElapsedTimeInMicroSeconds() const
{
   long currentPart = isTimerRunning() ? getElapsedTimeSinceLastTimerStartInMicroSeconds() : 0;
   
   return currentPart + aggregateTimeElapsed_;
}
      
void Statistics::resetStatistics()
{
   struct timezone notUsed;
   
   gettimeofday(&startTime_, &notUsed);
	aggregateTimeElapsed_ = 0;
   timerActive_ = false;
   aggregateStatistics_ = 0.0;
}

double Statistics::isTimerRunning() const
{
   return timerActive_;
}

void Statistics::startTimer()
{
   CHECK(!isTimerRunning(), "Timer shouldn't be running when this method is called");
   
   struct timezone notUsed;
   gettimeofday(&startTime_, &notUsed);
   timerActive_ = true;
}

void Statistics::stopTimer()
{
   CHECK(isTimerRunning(), "Timer should be running when this method is called");
   
   long elapsedInCurrentTimerRun = getElapsedTimeSinceLastTimerStartInMicroSeconds();
   
   aggregateTimeElapsed_ += elapsedInCurrentTimerRun;
   timerActive_ = false;
}

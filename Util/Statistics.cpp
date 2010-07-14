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

#include "Statistics.h"
#include "SimpleDesignByContract.h"

using namespace hdsim;

Statistics::Statistics() : timerActive_(false), aggregateTimeElapsed_(0), aggregateStatistics_(0)
{

}

Statistics::Statistics(const Statistics &rhs) : timerActive_(false), aggregateTimeElapsed_(0), aggregateStatistics_(0)
{
	copyFrom(rhs);
}

Statistics & Statistics::operator=(const Statistics &rhs) 
{
   if (&rhs == this)
      return *this;
   
   copyFrom(rhs);
   return *this;
}
      
Statistics::~Statistics()
{
   
}

void Statistics::copyFrom(const Statistics &rhs) 
{
   aggregateStatistics_ = rhs.getAggregateStatistics();
   timerActive_ = rhs.isTimerRunning();
   aggregateTimeElapsed_ = rhs.aggregateTimeElapsed_;
   startTime_ = rhs.startTime_;
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

double Statistics::getElapsedTimeInMicroSeconds() const
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

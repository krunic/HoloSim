/*
 *  Statistics.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/9/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <sys/time.h>
#include <unistd.h>

namespace hdsim {
   
   /**
    * This class is responsible for collecting statistics for the running system. It would track generic statistics
    */
   class Statistics
	{
   public:
      /**
       * Constructor
       */
      Statistics();
      
      /**
       * Destructor
       */
      virtual ~Statistics();
      
      /**
       * Add statistics. This is aggregate statistics (e.g. sum of all statistics) and would be added to the existing sum. 
       * If timer is not running, it would start it
       *
       * @param value Value of the statistics
       */
      virtual void addAggregateStatistics(double value);
      
      /**
       * Get aggregate statistics value
       *
       * @return Aggregate statistics value
       */
      virtual double getAggregateStatistics() const;
      
      /**
       * Check is timer active
       *
       * @return Is timer currently running
       */
      virtual double isTimerRunning() const;
      
      /**
       * Activate timer. All statistics that would be added would be added based on the time interval that starts here
       *
       * PRECONDITION: Timer must not be running
       */
      virtual void startTimer();
      
      /**
       * Stop timer. Will not add any subsequent time to collection interval for statistics
       *
       * PRECONDITION: Timer must be running
       */
      virtual void stopTimer();
      
      /**
       * Get time averaged value of the statistics
       *
       * @return time averaged value of the statistics
       */
      virtual double getTimeAveragedStatistics() const;
         
      /**
       * Get time that elapsed during all times that timer was started until now
       *
       * @return time elapsed in microseconds
       */
      virtual long int getElapsedTimeInMicroSeconds() const;
      
      /**
       * Reset the timer to zero. Aggregate statistics are zeroed too
       */
      virtual void resetStatistics();
      
   protected:
      /**
       * Get time elapsed since last time timer was started. Doesn't count accumulated time.
       *
       * @return Time elapsed since last timer start. 
       */
      virtual long getElapsedTimeSinceLastTimerStartInMicroSeconds() const;
      
   private:
      
      /**
       * Is timer currently active
       */
      bool timerActive_;
      
      /**
       * Aggregate time we were running up until now
       */
      long aggregateTimeElapsed_;
      
      /**
       * Value of the aggregate statistics
       */
      double aggregateStatistics_;
      
      /**
       * Time at which statistics collection was started
       */
      struct timeval startTime_;
	};
}

#endif
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

#include "MathHelper.h"

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
       * Copy constructor
       *
       * @param rhs Object to use to create copy
       */
      Statistics(const Statistics &rhs);
      
      /**
       * Operator =
       *
       * @param rhs Object to use to create copy
       */
      Statistics & operator=(const Statistics &rhs);
      
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
       * Copy value from rhs to this object
       *
       * @param rhs Value to copy
       */
      void copyFrom(const Statistics &rhs);

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
      
      // friend with its operators
      friend bool operator==(const Statistics &lhs, const Statistics &rhs);
      friend bool operator!=(const Statistics &lhs, const Statistics &rhs);
	};
   
   /** 
    * Compare two statistics. 
    * 
    * @param lhs Left operand
    * @param rhs right operand
    * 
    * @return Are statistics equal (equal being that timeslice and Z values are close enough that difference could be attributed to floating point error)
    */
   inline bool operator==(const Statistics &lhs, const Statistics &rhs)
   {
      if (lhs.timerActive_ != rhs.timerActive_)
         return false;
      
      if (lhs.aggregateTimeElapsed_ != rhs.aggregateTimeElapsed_)
         return false;
      
      if (!areEqual(lhs.aggregateStatistics_, rhs.aggregateStatistics_))
         return false;
      
      if (lhs.timerActive_)
      {
         if (lhs.startTime_.tv_sec != rhs.startTime_.tv_sec)
            return false;

         if (lhs.startTime_.tv_usec != rhs.startTime_.tv_usec)
            return false;
      }
      
      return true;
   }
   
   /** 
    * Compare two interpolated models. They are the same if they are at the "close" timeslice (so close that it could be attributed to floating point rounding) and
    * their start and stop model are "close" too
    * 
    * @param lhs Left operand
    * @param rhs right operand
    * 
    * @return Are models different (equal being that timeslice and Z values are close enough that difference could be attributed to floating point error)
    */
   inline bool operator!=(const Statistics &lhs, const Statistics &rhs)
   {
      return !(rhs == lhs);
   }
   
}

#endif
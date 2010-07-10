/*
 *  PreciseTimer.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/9/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <sys/time.h> 
#include <unistd.h> 

#include "PreciseDelay.h"

void hdsim::busyWaitDelay(long microsecondsToWait)
{
   struct timeval startTime, endTime;
   struct timezone notUsed;
   
   gettimeofday(&startTime, &notUsed);
   long startTimeInMicroSeconds = startTime.tv_sec * 1000000 + startTime.tv_usec;
   
   while (true)
   {
      gettimeofday(&endTime, &notUsed);
      
      // Recalculate time elapsed
      long endTimeInMicroSeconds = endTime.tv_sec * 1000000 + endTime.tv_usec;

      long timeElapsedInMicroSeconds = endTimeInMicroSeconds - startTimeInMicroSeconds;   

      if (timeElapsedInMicroSeconds > microsecondsToWait)
      {
         return;
      }
   }
}
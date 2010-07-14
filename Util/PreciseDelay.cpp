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
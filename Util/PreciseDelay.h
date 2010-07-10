/*
 *  PreciseTimer.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/9/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef PRECISE_DELAY_H_
#define PRECISE_DELAY_H_

namespace hdsim {
   
   /**
    * Problem with the sleep() and usleep() is that OS is not guaranteed to wake you up on time, making them not such a great way to measure time
    *
    * To address this, we would use busy wait sleep that is subject only to the limited timer resolution. This method spins CPU in the loop and as such
    * results in the high CPU usage
    *
    * NOTE - This method is limited on resolution of the undelined hardware timer and as such is not guaranteed (and on OS X is not) precise to 
    * within single microsecond
    *
    * @param microsecondsToWait Wait in microseconds. 
    */
   void busyWaitDelay(long microsecondsToWait);
}

#endif

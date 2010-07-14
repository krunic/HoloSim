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

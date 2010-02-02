/*
 *  MathHelper.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/22/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

#include <cmath>

namespace hdsim {

   /**
    * Maximum distance between floating point numbers for them to be considered equal
    */
   static const double FLOATING_POINTS_EQUAL_DELTA = 0.000000000001l;
   
   /**
    * Check that two floating point numbers are sufficiently close
    * 
    * @param x First floating point
    * @param y Second floating point
    */
   static const bool areEqual(double x, double y)
   {
      return fabs(x - y) <= FLOATING_POINTS_EQUAL_DELTA;
   }
   
   /**
    * Get max value
    *
    * @return maxValue
    */
   static const double max(double x, double y)
   {
      return x > y ? x : y;
   }

   /**
    * Get min value
    *
    * @return minValue
    */
   static const double min(double x, double y)
   {
      return x < y ? x : y;
   }
   
} // namespace

#endif

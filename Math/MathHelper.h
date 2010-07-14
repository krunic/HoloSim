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

#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

namespace hdsim {

   /**
    * Maximum distance between floating point numbers for them to be considered equal
    */
   static const double FLOATING_POINTS_EQUAL_DELTA = 0.000000000001l;
   
   /**
    * Maximum distance between floating point numbers for them to be considered equal when in low precision
    */
   static const double FLOATING_POINTS_LOW_PRECISION_EQUAL_DELTA = 0.000001l;
   
   /**
    * Check that two floating point numbers are sufficiently close
    * 
    * @param x First floating point
    * @param y Second floating point
    */
   static bool areEqual(double x, double y)
   {
      return fabs(x - y) <= FLOATING_POINTS_EQUAL_DELTA;
   }
   
   /**
    * Some operations (e.g. Z buffer) have significantly less precision then the double aritmetic, so we need to account for that
    *
    * @param x First floating point
    * @param y Second floating point
    */
   static bool areEqualInLowPrecision(double x, double y)
   {
      return fabs(x - y) <= FLOATING_POINTS_LOW_PRECISION_EQUAL_DELTA;
   }
   
   /**
    * Get max value
    *
    * @return maxValue
    */
   static double max(double x, double y)
   {
      return x > y ? x : y;
   }

   /**
    * Get min value
    *
    * @return minValue
    */
   static double min(double x, double y)
   {
      return x < y ? x : y;
   }

   /**
    * Convert string to number
    *
    * @param str Input string
    * @param result (OUT) result
    */
   template<class T> inline T stringToNumber(const std::string &str, T *result)
   {
      std::istringstream inputStream(str);
      char extraCharactersAfterNumber;
      
      // Conversion to string has to succeed first, and then there should be no extra spaces after the number
      return (inputStream >> *result)  &&  !inputStream.get(extraCharactersAfterNumber);
   }
   
} // namespace

#endif

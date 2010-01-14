/*
 *  Point.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef POINT_H_
#define POINT_H_

#include "MathHelper.h"

namespace hdsim {
   
	// This can have performance impact - consider using flighweight
	class Point {
   
	public: 
   	~Point() {}
   
	   double getX() const {
   	   return x_;
	   }
   
	   double getY() const {
	      return y_;
   	}
   
	   double getZ() const {
   	   return z_;
	   }
   
	   void setX(double x) {
   	   x_ = x;
	   }
   
	   void setY(double y) {
   	   y_ = y;
	   }
   
	   void setZ(double z) {
   	   z_ = z;
	   }
   
	   void setXYZ(double x, double y, double z) {
   	   x_ = x;
	      y_ = y;
   	   z_ = z;
	   }
    
	   // In this case, bitwise copy works well, which is why we are not defining copy constructor and operator=
		friend Point createPoint(double x, double y, double z);
      friend bool operator==(const Point &lhs, const Point &rhs);
      friend bool operator!=(const Point &lhs, const Point &rhs);
      
      Point(const Point &rhs) : x_(rhs.x_), y_(rhs.y_), z_(rhs.z_) {}
      
      Point &operator=(const Point &rhs) {
         
         if (&rhs == this)
            return *this;
         
         x_ = rhs.x_;
         y_ = rhs.y_;
         z_ = rhs.z_;         
         
         return *this;
      }

	private:

	   // Prevent inheritance, as this class doesn't have virtual members. Done for performance reasons
   	Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}
	   double x_, y_, z_;
   
	};
   
   inline bool operator==(const Point &lhs, const Point &rhs) {
      return areEqual(lhs.x_, rhs.x_)  &&  areEqual(lhs.y_, rhs.y_)  &&  areEqual(lhs.z_, rhs.z_);
   }
   
   inline bool operator!=(const Point &lhs, const Point &rhs) {
      return !(lhs == rhs);
   }
   
   inline Point createPoint(double x, double y, double z) {
      return Point(x, y, z);
   }
   
}

#endif

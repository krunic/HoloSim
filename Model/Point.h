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
		static Point createPoint(double x, double y, double z) {
   	   return Point(x, y, z);
	   }      
   
	private:

	   // Prevent inheritance, as this class doesn't have virtual members. Done for performance reasons
   	Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}
   
	   double x_, y_, z_;
   
	};
}

#endif

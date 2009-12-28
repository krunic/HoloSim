/*
 *  AbstractDrawingCode.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/27/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#include "AbstractDrawingCode.h"

#include <cmath>

namespace hdsim {
   
   double normalizeAngle(double angle)
	{
      double result = angle;
      
      if (result >= 360)
         result = fmod(result, 360);
      
      if (result < 0)
         result = fmod(result, 360) + 360;
      
      return result;
	}
};

using namespace hdsim;

AbstractDrawingCode::AbstractDrawingCode() : rotationX_(0), rotationY_(0), rotationZ_(0)
{
   
}

AbstractDrawingCode::~AbstractDrawingCode()
{
   
}

void AbstractDrawingCode::setRotationAngles(double x, double y, double z)
{
   rotationX_ = normalizeAngle(x);
   rotationY_ = normalizeAngle(y);
   rotationZ_ = normalizeAngle(z);
}

double AbstractDrawingCode::getRotationAngleX() const
{
   return rotationX_;
}

double AbstractDrawingCode::getRotationAngleY() const
{
   return rotationY_;
}

double AbstractDrawingCode::getRotationAngleZ() const
{
   return rotationZ_;
}

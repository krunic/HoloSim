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

/*
 *  MouseAdapter.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 5/15/07.
 *  Copyright � 2007-2010 Veljko Krunic. All rights reserved.
 *
 */

#include "MouseAdapter.h"

#include "SimpleDesignByContract.h"

using namespace hdsim;

/**
 * What is default ratio between angle of rotation (in degrees) and mouse move (in pixels). 
 */
const double DEFAULT_MOUSE_MOVE_RATIO = 1.0;

/**
 * What is default ratio between angle of FOV changes (in degrees) and mouse move (in pixels). 
 */
const double DEFAULT_MOUSE_FOV_RATIO = 1.0;

/**
 * What is default min FOV in degrees
 */
const double DEFAULT_MIN_FOV = 1.0;

/**
 * What is default max FOV in degrees
 */
const double DEFAULT_MAX_FOV = 1.0;

MouseAdapter::MouseAdapter() : ControllerAdapter(), lastX_(0), lastY_(0), azimuth_(0), height_(0), distance_(0), centerX_(0), 
	centerY_(0), centerZ_(0), mouseMoveRatio_(DEFAULT_MOUSE_MOVE_RATIO), mouseFOVRatio_(DEFAULT_MOUSE_FOV_RATIO), 
	minFOV_(DEFAULT_MIN_FOV), maxFOV_(DEFAULT_MAX_FOV)
{
   
}
   
MouseAdapter::MouseAdapter(
   const MouseAdapter &rhs
) : ControllerAdapter(rhs), 
    lastX_(rhs.lastX_), lastY_(rhs.lastY_), azimuth_(rhs.azimuth_), height_(rhs.height_), distance_(rhs.distance_), 
    centerX_(rhs.centerX_), centerY_(rhs.centerY_), centerZ_(rhs.centerZ_), mouseMoveRatio_(DEFAULT_MOUSE_MOVE_RATIO), 
	 mouseFOVRatio_(DEFAULT_MOUSE_FOV_RATIO), minFOV_(DEFAULT_MIN_FOV), maxFOV_(DEFAULT_MAX_FOV)
{
   
}
   
MouseAdapter & MouseAdapter::operator=(const MouseAdapter &rhs)
{
   if (this == &rhs)
      return *this;
   
   // Call super
   ControllerAdapter::operator=(rhs);
   
   lastX_ = rhs.lastX_;
   lastY_ = rhs.lastY_;
   azimuth_ = rhs.azimuth_;
   height_ = rhs.height_;
   distance_ = rhs.distance_;
   centerX_ = rhs.centerX_;
   centerY_ = rhs.centerY_;
   centerZ_ = rhs.centerZ_;
   
   return *this;
}
   
MouseAdapter::~MouseAdapter()
{
   // Don't delete anything as we don't own the object we are pointing too
}
   
void MouseAdapter::controlForMouseAt(int x, int y, bool lmb, bool rmb)
{
   PRECONDITION(lmb  &&  !rmb);
   
   // This order is INTENTIONAL due to the current setup of the model
   getDrawer()->setRotationAngles(getDrawer()->getRotationAngleX() + y*getMouseMoveRatio(), 
                                  getDrawer()->getRotationAngleY(),
                                  getDrawer()->getRotationAngleZ() + x*getMouseMoveRatio());
}

void MouseAdapter::controlForMouseScrollChange(int x, int y, int z)
{
   // Only change FOV if we are within limits
   double newFOV = getDrawer()->getFOV() + y*getMouseFOVRatio();
                                       
	if (newFOV >= getMinFOV()  &&  newFOV <= getMaxFOV())
      getDrawer()->setFOV(newFOV);                                       
}

   
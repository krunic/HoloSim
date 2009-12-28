/*
 *  ControllerAdapter.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 5/15/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#include <cstdlib>

#include "ControllerAdapter.h"

using namespace hdsim;

ControllerAdapter::ControllerAdapter() : drawingCode_(0)
{
}
   
ControllerAdapter::ControllerAdapter(const ControllerAdapter &rhs) : drawingCode_(rhs.drawingCode_)
{
}
   
ControllerAdapter & ControllerAdapter::operator=(const ControllerAdapter &rhs)
{
   if (this == &rhs)
      return *this;
   
   // Share same object pointer as we would be adapters around the same object
   drawingCode_ = rhs.drawingCode_;
   
   return *this;
}
   
ControllerAdapter::~ControllerAdapter()
{
   
}

void ControllerAdapter::setDrawer(AbstractDrawingCode *code)
{
   drawingCode_ = code;
}

AbstractDrawingCode *ControllerAdapter::getDrawer() const
{
   return drawingCode_;
}

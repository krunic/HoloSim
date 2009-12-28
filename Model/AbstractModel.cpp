/*
 *  AbstractModel.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/20/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#include "AbstractModel.h"

using namespace hdsim;

AbstractModel::~AbstractModel()
{
   
}

AbstractModel *AbstractModel::cloneOrphan() const
{
	// We can't get clone of abstract model
   return 0;
}
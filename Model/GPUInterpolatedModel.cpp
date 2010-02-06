/*
 *  GPUInterpolatedModel.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 2/4/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>

#include "GPUInterpolatedModel.h"
#include "SimpleDesignByContract.h"

using namespace hdsim;

GPUInterpolatedModel::GPUInterpolatedModel() : model_(), timeSlice_(0)
{
}

GPUInterpolatedModel::GPUInterpolatedModel(const GPUInterpolatedModel &rhs) : model_(rhs.model_), timeSlice_(rhs.timeSlice_)
{
}

GPUInterpolatedModel & GPUInterpolatedModel::operator=(const GPUInterpolatedModel &rhs)
{
   if (this == &rhs)
      return *this;

   model_ = rhs.model_;
   timeSlice_ = rhs.timeSlice_;
   return *this;
}

GPUInterpolatedModel::~GPUInterpolatedModel()
{
}

double GPUInterpolatedModel::getAt(int x, int y) const
{
   // For a time being, we don't support timeslice, so we should use this
   return model_.getAt(x, y);
}

AbstractModel * GPUInterpolatedModel::cloneOrphan() const
{
   return new GPUInterpolatedModel(*this);
}

void GPUInterpolatedModel::initializeToCleanState()
{
   model_.initializeToCleanState();
   timeSlice_ = 0;
}

bool GPUInterpolatedModel::readFromFile(FILE *fp)
{
   // First line is model name
   char readThis[1024];
   sprintf(readThis, "%s\n", getModelName());
   fscanf(fp, readThis, NULL);
   
   // Next line is timeslice
	int itemsRead = fscanf(fp, " %lf\n", &timeSlice_);
   if (itemsRead != 1)
   {
      initializeToCleanState();
      return false;
   }
   
   // Next line is model, so just read it
   return model_.readFromFile(fp);
}

bool GPUInterpolatedModel::saveToFile(FILE *fp) const
{
   FAIL("Implement this");
   return false;
}

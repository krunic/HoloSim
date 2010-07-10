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
#include <string>
#include <sstream>

#include "MathHelper.h"
#include "GPUInterpolatedModel.h"
#include "SimpleDesignByContract.h"

using namespace hdsim;
using namespace std;

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

bool GPUInterpolatedModel::isModelCalculated() const
{
   return model_.isModelCalculated();
}

void GPUInterpolatedModel::forceModelCalculation() const
{
   model_.forceModelCalculation();
}


bool GPUInterpolatedModel::readFromFile(const std::string &fileName)
{
   string line; 
   
   ifstream openedFile(fileName.c_str());
   
   // First line is model name
	if (!getline(openedFile, line))
   {
      return false;
   }
   
   if (line != getModelName())
   {
      return false;
   }
     
   // Next line is timeslice
   if (!getline(openedFile, line))
   {
   	return false;
   }
   
   if (!stringToNumber(line, &timeSlice_))
   {
      return false;
   }
   
   // Get name of the model from the file
   string modelName; 
   
   if (!getline(openedFile, modelName))
   {
      return false;
   }

   // Next line is model, so just read it
   return model_.readFromFile(getFileNameInSameDirAsOriginalFile(fileName, modelName));
}

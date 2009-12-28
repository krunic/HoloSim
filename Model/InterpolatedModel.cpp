/*
 *  InterpolatedModel.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/24/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#include "InterpolatedModel.h"

#include <cstdio>

#include "MathHelper.h"
#include "SimpleDesignByContract.h"

using namespace hdsim;

const double InterpolatedModel::MIN_TIME_SLICE;
const double InterpolatedModel::MAX_TIME_SLICE;

InterpolatedModel::InterpolatedModel() : timeSlice_(0)
{
   
}

InterpolatedModel::InterpolatedModel(int numModels, int size) : timeSlice_(0)
{
   for (int indexModel = 0; indexModel < numModels; indexModel++)
   {
      models_.push_back(new CheckBoard(size, size));
   }
}

InterpolatedModel::InterpolatedModel(const InterpolatedModel &rhs) : timeSlice_(0)
{
   copyFrom(rhs);   
}

InterpolatedModel & InterpolatedModel::operator=(const InterpolatedModel &rhs)
{
	if (&rhs == this)
      return *this;
   
   AbstractModel::operator=(rhs);
   copyFrom(rhs);
   
   POSTCONDITION(models_.size() == rhs.models_.size());
   POSTCONDITION(areEqual(getTimeSlice(), rhs.getTimeSlice()));
   return *this;
}

/**
 * Delete all elements of the array
 *
 * @param vec Vector containing array of pointers
 */
void deleteAllElements(vector <CheckBoard *> &vec)
{
   // We own all the models in us so delete them
   for (int indexModel = 0; indexModel < vec.size(); indexModel++)
      delete vec[indexModel];
   
   // And remove all dead pointers from the vector
   vec.clear();   
   POSTCONDITION(vec.size() == 0);
}

InterpolatedModel::~InterpolatedModel()
{
   deleteAllElements(models_);
}

void InterpolatedModel::copyFrom(const AbstractModel &rhs)
{
   // First delete all models 
   deleteAllElements(models_);
   CHECK(models_.size() == 0, "Models size is 0");
   
   const InterpolatedModel &copyFrom = dynamic_cast<const InterpolatedModel &>(rhs);
   
   // And clone and adopt all from rhs
   for (int indexModel = 0; indexModel < copyFrom.models_.size(); indexModel++)
   {
	   const AbstractModel *copy = copyFrom.getModelAtIndex(indexModel);
      addModelAdopt(copy->cloneOrphan());
   }
   
   timeSlice_ = copyFrom.timeSlice_;
   POSTCONDITION(areEqual(getTimeSlice(), copyFrom.getTimeSlice()));
   POSTCONDITION(getNumberOfModels() == copyFrom.getNumberOfModels());
}

void InterpolatedModel::addModelAdopt(AbstractModel *model)
{
   PRECONDITION(model  &&  dynamic_cast<CheckBoard *>(model));
	models_.push_back(dynamic_cast<CheckBoard *>(model));   
}

int InterpolatedModel::getNumberOfModels() const
{
   return models_.size();
}

const AbstractModel *InterpolatedModel::getModelAtIndex(int index) const
{
   PRECONDITION(index >= 0  &&  index < models_.size());
	return models_[index];   
}

void InterpolatedModel::setTimeSlice(double ts)
{
   PRECONDITION(ts >= MIN_TIME_SLICE);
   PRECONDITION(ts <= MAX_TIME_SLICE);
   PRECONDITION(models_.size() > 1);
   timeSlice_ = ts;
}

double InterpolatedModel::getTimeSlice() const
{
   return timeSlice_;
}

const char *InterpolatedModel::getModelName() const 
{
   return INTERPOLATED_MODEL_NAME;
}

double InterpolatedModel::getAt(int x, int y) const
{
   // Execute linear interpolation between begin model (at min value of timeslice) and end value (at max value of time slice)
   
   // First renormalize timeslice between 0 and 1
   double tNormalized = (getTimeSlice() - getInterpolationBeginTimeSlice())/(getInterpolationEndTimeSlice() - getInterpolationBeginTimeSlice());
   
   // And do classical linear interpolation between begin and end for [0, 1]
   return getEndModel()->getAt(x, y) * tNormalized + getBeginModel()->getAt(x, y) * (1 - tNormalized);
}

int InterpolatedModel::getSizeX() const
{
   PRECONDITION(models_.size() > 0);
   return models_[0]->getSizeX();
}

int InterpolatedModel::getSizeY() const
{
   PRECONDITION(models_.size() > 0);
   return models_[0]->getSizeY();
}

void InterpolatedModel::setValueInModelWithIndex(int index, int x, int y, double value)
{
	models_[index]->setAt(x, y, value);   
}

AbstractModel *InterpolatedModel::cloneOrphan() const 
{
   // Create copy of us and return it
   return new InterpolatedModel(*this);
}

void InterpolatedModel::initializeToCleanState()
{
   deleteAllElements(models_);
   timeSlice_ = MIN_TIME_SLICE;
}

bool InterpolatedModel::readFromFile(FILE *fp)
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
   
   // Next line is number of models
   int numModels;
	itemsRead = fscanf(fp, " %d\n", &numModels);
   if (itemsRead != 1)
   {
      initializeToCleanState();
      return false;
   }

   deleteAllElements(models_);
   
   for (int indexModel = 0; indexModel < numModels; indexModel++)
   {
      CheckBoard *model = new CheckBoard();
      if (!model->readFromFile(fp))
      {
         delete model;
         initializeToCleanState();
         return false;
      }
      
      models_.push_back(model);
   }
     
   return true;
}

bool InterpolatedModel::saveToFile(FILE *fp) const
{
   if (!fprintf(fp, "%s\n", getModelName()))
		return false;
   
	if (!fprintf(fp, "%lf\n", getTimeSlice()))
		return false;

   if (!fprintf(fp, "%d\n", getNumberOfModels()))
       return false;

   for (int modelIndex = 0; modelIndex < getNumberOfModels(); modelIndex++)
	   if (!getModelAtIndex(modelIndex)->saveToFile(fp))
   	   return false;
   
   return true;
}


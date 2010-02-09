/*
 *  GPUGeometryModel.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <string>
#include <fstream>

#include "GPUGeometryModel.h"
#include "Collada.h"
#include "SimpleDesignByContract.h"
#include "GPUCalculationEngine.h"

using namespace hdsim;
using namespace std;

GPUGeometryModel::GPUGeometryModel() : sizeX_(0), sizeY_(0), boundMinX_(0), boundMaxX_(0), 
													boundMinY_(0), boundMaxY_(0), boundMinZ_(0), boundMaxZ_(0),
												   renderedAreaMinX_(0), renderedAreaMinY_(0), renderedAreaMaxX_(0), 
													renderedAreaMaxY_(0), renderedAreaMinZ_(0), renderedAreaMaxZ_(0), 
													calculationEngine_(0), changedSinceLastRecalc_(true)
{
   calculationEngine_ = new GPUCalculationEngine();
}
      
GPUGeometryModel::GPUGeometryModel(int sizeX, int sizeY) : sizeX_(sizeX), sizeY_(sizeY), 
                                                           boundMinX_(0), boundMaxX_(0), 
                                                           boundMinY_(0), boundMaxY_(0), 
                                                           boundMinZ_(0), boundMaxZ_(0),
                                                           renderedAreaMinX_(0), renderedAreaMinY_(0),
                                                           renderedAreaMaxX_(0), renderedAreaMaxY_(0), 
																			  renderedAreaMinZ_(0), renderedAreaMaxZ_(0), 
																			  calculationEngine_(0), changedSinceLastRecalc_(true)
{
   calculationEngine_ = new GPUCalculationEngine();
}
      
GPUGeometryModel::GPUGeometryModel(const GPUGeometryModel &rhs) : sizeX_(0), sizeY_(0), 
																						boundMinX_(0), boundMaxX_(0), 
																						boundMinY_(0), boundMaxY_(0), 
																						boundMinZ_(0), boundMaxZ_(0),
																						renderedAreaMinX_(0), renderedAreaMinY_(0),
																						renderedAreaMaxX_(0), renderedAreaMaxY_(0), 
																						renderedAreaMinZ_(0), renderedAreaMaxZ_(0), 
																						calculationEngine_(0), changedSinceLastRecalc_(true)
{
	copyFrom(rhs);
}
      
GPUGeometryModel & GPUGeometryModel::operator=(const GPUGeometryModel &rhs) 
{
   if (&rhs == this)
      return *this;
   
   copyFrom(rhs);
   return *this;
}
      
GPUGeometryModel::~GPUGeometryModel() 
{
   delete calculationEngine_;
}
      
void GPUGeometryModel::initializeToCleanState() 
{
   sizeX_ = sizeY_ = 0;
   changedSinceLastRecalc_ = true;
	renderedAreaMinX_ = renderedAreaMinY_ = renderedAreaMaxX_ = renderedAreaMaxY_ = renderedAreaMinZ_ = renderedAreaMaxZ_ = 0;
   clearGeometry();
}

void GPUGeometryModel::clearGeometry()
{
	points_.clear();
   triangles_.clear();
   
   boundMinX_ = boundMinY_ = boundMaxX_ = boundMaxY_ = 0;
}

void GPUGeometryModel::copyFrom(const GPUGeometryModel &rhs) 
{
   changedSinceLastRecalc_ = true;   
   
   sizeX_ = rhs.getSizeX();
   sizeY_ = rhs.getSizeY();
   
   boundMinX_ = rhs.getBoundMinX();
   boundMaxX_ = rhs.getBoundMaxX();
   
   boundMinY_ = rhs.getBoundMinY();
   boundMaxY_ = rhs.getBoundMaxY();
   
   boundMinZ_ = rhs.getBoundMinZ();
   boundMaxZ_ = rhs.getBoundMaxZ();

   setRenderedArea(rhs.getRenderedAreaMinX(), rhs.getRenderedAreaMinY(), rhs.getRenderedAreaMinZ(), rhs.getRenderedAreaMaxX(), rhs.getRenderedAreaMaxY(), rhs.getRenderedAreaMaxZ());
   calculationEngine_ = new GPUCalculationEngine();
   
   points_ = rhs.points_;
   triangles_ = rhs.triangles_;
}
     
AbstractModel *GPUGeometryModel::cloneOrphan() const 
{
   // Create copy of us and return it
   return new GPUGeometryModel(*this);
}

double GPUGeometryModel::getAt(int x, int y) const
{
   if (changedSinceLastRecalc_)
   {
		calculationEngine_->calculateEngine(this);
      changedSinceLastRecalc_ = false;
   }
   
   return calculationEngine_->getAt(x, y);
}

bool GPUGeometryModel::readFromFile(const string &fileName) 
{
   changedSinceLastRecalc_ = true;
   
   // First line is model name
   string line;
   ifstream inputStream(fileName.c_str());
   
   if (!getline(inputStream, line))
   {
      return false;
   }
   
   if (line != getModelName())
   {
      return false;
   }
   
   // Get line for x size
   int x, y;
   
   if (!getline(inputStream, line))
   {
      return false;
   }
   
   if (!stringToNumber(line, &x))
   {
      return false;
   }
   
   if (!getline(inputStream, line))
   {
      return false;
   }
   
   if (!stringToNumber(line, &y))
   {
      return false;
   }

   setSizeX(x);
   setSizeY(y);

   // Next line is collada filename
   if (!getline(inputStream, line))
   {
      return false;
   }
   
   // That filename is relative to the file we are reading from
   return loadCollada(getFileNameInSameDirAsOriginalFile(fileName, line).c_str(), *this);
}

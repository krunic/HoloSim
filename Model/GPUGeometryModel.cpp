/*
 *  GPUGeometryModel.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include "GPUGeometryModel.h"
#include "Collada.h"

using namespace hdsim;

GPUGeometryModel::GPUGeometryModel() : sizeX_(0), sizeY_(0), boundMinX_(0), boundMaxX_(0), 
													boundMinY_(0), boundMaxY_(0), boundMinZ_(0), boundMaxZ_(0)
{

}
      
GPUGeometryModel::GPUGeometryModel(int sizeX, int sizeY) : sizeX_(sizeX), sizeY_(sizeY), 
                                                           boundMinX_(0), boundMaxX_(0), 
                                                           boundMinY_(0), boundMaxY_(0), 
                                                           boundMinZ_(0), boundMaxZ_(0) 
{
   
}
      
GPUGeometryModel::GPUGeometryModel(const GPUGeometryModel &rhs) : sizeX_(rhs.sizeX_), sizeY_(rhs.sizeY_),
																						boundMinX_(rhs.boundMinX_), boundMaxX_(boundMaxX_), 
																						boundMinY_(boundMinY_), boundMaxY_(boundMaxY_), 
																						boundMinZ_(boundMinZ_), boundMaxZ_(boundMaxZ_)
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
}
      
void GPUGeometryModel::initializeToCleanState() 
{
   sizeX_ = sizeY_ = 0;
   clearGeometry();
}

void GPUGeometryModel::clearGeometry()
{
	points_.clear();
   triangles_.clear();
}

void GPUGeometryModel::copyFrom(const GPUGeometryModel &rhs) {
   sizeX_ = rhs.getSizeX();
   sizeY_ = rhs.getSizeY();
   
   boundMinX_ = rhs.getBoundMinX();
   boundMaxX_ = rhs.getBoundMaxX();
   boundMinY_ = rhs.getBoundMinY();
   boundMaxY_ = rhs.getBoundMaxY();
   boundMinZ_ = rhs.getBoundMinZ();
   boundMaxZ_ = rhs.getBoundMaxZ();
   
   points_ = rhs.points_;
   triangles_ = rhs.triangles_;
}
     
AbstractModel *GPUGeometryModel::cloneOrphan() const {
   // Create copy of us and return it
   return new GPUGeometryModel(*this);
}

double GPUGeometryModel::getAt(int x, int y) const {
   assert(0);
}

bool GPUGeometryModel::readFromFile(FILE *fp) {
   
   // First line is model name
   char readThis[1024];
   sprintf(readThis, "%s\n", getModelName());
   fscanf(fp, readThis, NULL);
   
   // Second line is x y sizes
   int x, y;
   int sizeRead = fscanf(fp, "%d %d\n", &x, &y);
   if (sizeRead != 2)
   {
      return false;
   }
   
   setSizeX(x);
   setSizeY(y);

   // Next line is collada filename
   // Line from the file we are getting would point us to the actual filename for the collada model
   char fileName[1024];
   int charsRead = fscanf(fp, "%s\n", fileName);
   if (charsRead != 1)
   {
      return false;
   }

   return loadCollada(fileName, *this);
}

bool GPUGeometryModel::saveToFile(FILE *fp) const {
   assert(0);
}


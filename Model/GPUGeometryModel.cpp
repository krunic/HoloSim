/*
 *  GPUGeometryModel.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include "GPUGeometryModel.h"

using namespace hdsim;

GPUGeometryModel::GPUGeometryModel() : sizeX_(0), sizeY_(0) {
}
      
GPUGeometryModel::GPUGeometryModel(int sizeX, int sizeY) : sizeX_(sizeX), sizeY_(sizeY) {
}
      
GPUGeometryModel::GPUGeometryModel(const GPUGeometryModel &rhs) : sizeX_(rhs.sizeX_), sizeY_(rhs.sizeY_) {
	copyFrom(rhs);
}
      
GPUGeometryModel & GPUGeometryModel::operator=(const GPUGeometryModel &rhs) {
   
   if (&rhs == this)
      return *this;
   
   copyFrom(rhs);
   return *this;
}
      
GPUGeometryModel::~GPUGeometryModel() {
}
      
void GPUGeometryModel::initializeToCleanState() {
   sizeX_ = sizeY_ = 0;
	points_.clear();
   triangles_.clear();
}

void GPUGeometryModel::copyFrom(const GPUGeometryModel &rhs) {
   sizeX_ = rhs.getSizeX();
   sizeY_ = rhs.getSizeY();
   
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
   assert(0);
}

bool GPUGeometryModel::saveToFile(FILE *fp) const {
   assert(0);
}


/*
 *  GPUCalculationEngine.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include "GPUCalculationEngine.h"
#include "GPUGeometryModel.h"
#include "SimpleDesignByContract.h"

#include <assert.h>

using namespace hdsim;

GPUCalculationEngine::GPUCalculationEngine() {
   assert(0);
}

GPUCalculationEngine::GPUCalculationEngine(const GPUCalculationEngine &rhs) {
   assert(0);  
}

GPUCalculationEngine &GPUCalculationEngine::operator=(const GPUCalculationEngine &rhs) {
   assert(0);  
}
   
GPUCalculationEngine::~GPUCalculationEngine() {
   assert(0);
}

void GPUCalculationEngine::positionCamera(int sizeX, int sizeY, double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
{
   assert(0);
}

void GPUCalculationEngine::calculate(GPUGeometryModel *model)
{
   assert(0);
}

void GPUCalculationEngine::calculateEngine(AbstractModel *model) {
   GPUGeometryModel *geometryModel = dynamic_cast<GPUGeometryModel *>(model);
   CHECK(geometryModel, "This calculation engine operates only with the geometry model");
   
   // Now, we need to do appropriate setup
   positionCamera(geometryModel->getSizeX(), geometryModel->getSizeY(),
                  geometryModel->getBoundMinX(), geometryModel->getBoundMaxX(),
                  geometryModel->getBoundMinY(), geometryModel->getBoundMaxY(),
                  geometryModel->getBoundMinZ(), geometryModel->getBoundMaxZ());
   
   calculate(geometryModel);
}

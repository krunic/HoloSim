/*
 * HoloSim, visualization and control of the moxel based environment.
 *
 * Copyright (C) 2010 Veljko Krunic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "MathHelper.h"
#include "GPUInterpolatedModel.h"
#include "SimpleDesignByContract.h"

using namespace hdsim;
using namespace std;

GPUInterpolatedModel::GPUInterpolatedModel() : model_(), timeSlice_(0), optimizeDrawing_(false), 
															  optimizeDrawingThreshold_(0), optimizedModelSizeX_(0), optimizedModelSizeY_(0), decimatedModel_(0)
{
}

GPUInterpolatedModel::GPUInterpolatedModel(const GPUInterpolatedModel &rhs) : model_(rhs.model_), timeSlice_(rhs.timeSlice_),
																										optimizeDrawing_(rhs.optimizeDrawing_), 
                                                                              optimizeDrawingThreshold_(rhs.optimizeDrawingThreshold_), 
                                                                              optimizedModelSizeX_(0), optimizedModelSizeY_(0), decimatedModel_(0)
{
   *this = rhs;
}

GPUInterpolatedModel & GPUInterpolatedModel::operator=(const GPUInterpolatedModel &rhs)
{
   if (this == &rhs)
      return *this;

   model_ = rhs.model_;
   timeSlice_ = rhs.timeSlice_;
   fileName_ = rhs.fileName_;
 	optimizeDrawing_ = rhs.optimizeDrawing_;
	optimizeDrawingThreshold_ = rhs.optimizeDrawingThreshold_;
   optimizedModelSizeX_ = rhs.optimizedModelSizeX_;
   optimizedModelSizeY_ = rhs.optimizedModelSizeY_;
   
  	int optimizedModelSize = optimizedModelSizeX_ * optimizedModelSizeY_;
   
   if (optimizedModelSize > 0)
   {
      decimatedModel_ = new double[optimizedModelSize];
      CHECK(decimatedModel_, "Copying of the model failed");
      
   	memcpy(decimatedModel_, rhs.decimatedModel_, optimizedModelSize * sizeof(double));   
   }
	else
   {
      decimatedModel_ = 0;
   }
   
   return *this;
}

bool GPUInterpolatedModel::isDrawingOptimizationActive() const
{
   int modelSize = model_.getSizeX() * model_.getSizeY();
   
   return getOptimizeDrawing()  &&  modelSize > getMoxelThreshold(); 
}

GPUInterpolatedModel::~GPUInterpolatedModel()
{
   delete [] decimatedModel_;
}

int GPUInterpolatedModel::getSizeX() const
{
   if (isDrawingOptimizationActive())
   {
      if (!model_.isModelCalculated())
         forceModelCalculation();

      return optimizedModelSizeX_;
   }
   else
   {
      return model_.getSizeX();
   }
}

int GPUInterpolatedModel::getSizeY() const
{
   if (isDrawingOptimizationActive())
   {
      if (!model_.isModelCalculated())
         forceModelCalculation();
      
		return optimizedModelSizeY_;
   }
   else
   {
    	return model_.getSizeY();  
   }
}

double GPUInterpolatedModel::getAt(int x, int y) const
{
   if (isDrawingOptimizationActive())
   {
      if (!model_.isModelCalculated())
         forceModelCalculation();

      CHECK(x < optimizedModelSizeX_, "X coordinate too large");
      CHECK(y < optimizedModelSizeY_, "Y coordinate too large");
            
      return decimatedModel_[y * optimizedModelSizeY_ + x];
   }
   else
   {
      if (!model_.isModelCalculated())
      {
         forceModelCalculation();
      }
      
   	return model_.getAt(x, y);
   }
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
   moxelCalculationStatistics_.startTimer();
   
   	lastMoxelRenderingStatistics_.resetStatistics();
   	lastMoxelRenderingStatistics_.startTimer();
   	
			model_.forceModelCalculation();
   	
   	lastMoxelRenderingStatistics_.stopTimer();
   
   moxelCalculationStatistics_.stopTimer();
   
   lastMoxelRenderingStatistics_.addAggregateStatistics(getTotalNumMoxels());
   moxelCalculationStatistics_.addAggregateStatistics(getTotalNumMoxels());
   
   if (isDrawingOptimizationActive())
   {
      optimizedModelSizeX_ = getModelSizeForOptimizedDrawingX();
      optimizedModelSizeY_ = getModelSizeForOptimizedDrawingY();
      
      delete [] decimatedModel_;
      decimatedModel_ = getDecimatedModelAdopt(&model_, optimizedModelSizeX_, optimizedModelSizeY_);
   }
}

int GPUInterpolatedModel::getModelSizeForOptimizedDrawingX() const
{
   CHECK(getOptimizeDrawing(), "Optimized drawing must be enabled to invoke this method");
   
   double correctionFactor = sqrt(getMoxelThreshold() / (double)(model_.getSizeX() * model_.getSizeY()));
   
   return floor(correctionFactor * model_.getSizeX());
}

int GPUInterpolatedModel::getModelSizeForOptimizedDrawingY() const
{
   CHECK(getOptimizeDrawing(), "Optimized drawing must be enabled to invoke this method");   

   double correctionFactor = sqrt(getMoxelThreshold() / (double)(model_.getSizeX() * model_.getSizeY()));
   
   return floor(correctionFactor * model_.getSizeY());
}

double *GPUInterpolatedModel::getDecimatedModelAdopt(const AbstractModel *m, int xSize, int ySize)
{
   // We would assume fourth quadrant in comments (x grows to right, y goes down)
   
   // Algorithm is based on decimated grid covering gpuPixelGrid, with each pixel in decimated grid having at least one moxel mapped to it
   // Unless we can exactly divide dimensions, we would have decimated pixels on right and bottom edge having more GPU pixels then pixel at (0, 0).
   // We would call this pixels "Edge pixels"
   
   // Algorithm would add values in all GPU pixels to the decimated pixel, and then would divide value in decimated pixel with the number of GPU pixels mapped to it
   
   const int gpuSizeX = m->getSizeX();
   const int gpuSizeY = m->getSizeY();
   const int numMoxels = gpuSizeX * gpuSizeY;
   
   CHECK(gpuSizeX >= xSize  &&  gpuSizeY >= ySize, "Decimated grid can't be larger then original grid");
   
   // How many gpu pixels are mapped to the single decimated pixel
   int xGPUPixelsInDecimatedPixel = gpuSizeX / xSize;
   
   // In general, there is reminder on the right edge 
   int rightEdgePixelsInDecimatedPixel = xGPUPixelsInDecimatedPixel + m->getSizeX() % xSize;
   
   // Same for Y
   int yGPUPixelsInDecimatedPixel = m->getSizeY() / ySize;
   int bottomEdgePixelsInDecimatedPixel = yGPUPixelsInDecimatedPixel + m->getSizeY() % ySize;
   
   int normalQuadMoxels = xGPUPixelsInDecimatedPixel * yGPUPixelsInDecimatedPixel;
   
   // Zero-init result
   double *result = new double[numMoxels];
   CHECK(result, "Memory allocation failure");
   
   memset(result, 0, numMoxels * sizeof(double));
   
   // Sum values in all decimated points
   for (int indexY = 0; indexY < gpuSizeY; indexY++)
      for (int indexX = 0; indexX < gpuSizeX; indexX++)
      {
         int posX = indexX / xGPUPixelsInDecimatedPixel;
         posX = posX < xSize ? posX : xSize - 1;
         
         int posY = indexY / yGPUPixelsInDecimatedPixel;
         posY = posY < ySize ? posY : ySize - 1;
         
         result[posY * ySize + posX] += m->getAt(indexX, indexY);
      }
   
   // Fix evenly sized decimated areas
   for (int indexY = 0; indexY < ySize - 1; indexY++)
   {
      int startOfScanLine = indexY * xSize;
      
      for (int indexX = 0; indexX < xSize - 1; indexX++)
      {
         result[startOfScanLine + indexX] /= normalQuadMoxels;
      }      
   }
   
   // Size of the quad on the right edge
   int gpuPixelsInDecimatedPixelOnRightEdge = rightEdgePixelsInDecimatedPixel * yGPUPixelsInDecimatedPixel;
   
   // Right edge
   for (int indexY = 0; indexY < ySize - 1; indexY++)
   {
      result[(indexY + 1) * xSize - 1] /= gpuPixelsInDecimatedPixelOnRightEdge;
   }
   
   int gpuPixelsInDecimatedPixelOnBottomEdge = xGPUPixelsInDecimatedPixel * bottomEdgePixelsInDecimatedPixel;
   
   // Bottom edge
   for (int indexX = 0; indexX < xSize - 1; indexX++)
   {
      result[(ySize - 1) * xSize + indexX] /= gpuPixelsInDecimatedPixelOnBottomEdge;
   }
   
   // Bottom right corner
   int gpuPixelsInDecimatedPixelInLowerRightQuad = rightEdgePixelsInDecimatedPixel * bottomEdgePixelsInDecimatedPixel;
   result[xSize * ySize - 1] /= gpuPixelsInDecimatedPixelInLowerRightQuad;
   
   return result;
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
   
   fileName_ = fileName;

   // Next line is model, so just read it
   return model_.readFromFile(getFileNameInSameDirAsOriginalFile(fileName, modelName));
}

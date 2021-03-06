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

#include <string>
#include <fstream>

#include "GPUGeometryModel.h"
#include "Collada.h"
#include "SimpleDesignByContract.h"
#include "GPUCalculationEngine.h"

using namespace hdsim;
using namespace std;

static void printErorrMessage(const string &fileName, const string &line)
{
   stringstream message;
   message << "Error encountered in file " << fileName << " in line " << line;
   LOG(message.str().c_str());
}

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

bool GPUGeometryModel::isModelCalculated() const
{
   return !changedSinceLastRecalc_;
}

void GPUGeometryModel::setNeedsRecalc()
{
   changedSinceLastRecalc_ = true;
}

void GPUGeometryModel::forceModelCalculation() const
{
   calculationEngine_->calculateEngine(this);
   changedSinceLastRecalc_ = false;
}

double GPUGeometryModel::getAt(int x, int y) const
{
   if (!isModelCalculated())
   {
      forceModelCalculation();
   }
   
   double value = calculationEngine_->getAt(x, y);
   return value;
}

bool GPUGeometryModel::readFromFile(const std::string &fileName) 
{
   changedSinceLastRecalc_ = true;
   
   // First line is model name
   string line;
   ifstream inputStream(fileName.c_str());
   
   if (!getline(inputStream, line))
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   if (line != getModelName())
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   // Get line for x size
   int x, y;
   
   if (!getline(inputStream, line))
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   if (!stringToNumber(line, &x))
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   if (!getline(inputStream, line))
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   if (!stringToNumber(line, &y))
   {
      printErorrMessage(fileName, line);
      return false;
   }

   setSizeX(x);
   setSizeY(y);

   // Next line is collada filename
   if (!getline(inputStream, line))
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   // That filename is relative to the file we are reading from
   if (!loadCollada(getFileNameInSameDirAsOriginalFile(fileName, line).c_str(), *this))
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   // Next line is name of the shader to use
   if (!getline(inputStream, line))
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   string pathToShader;
   
   setPathToShaderSource(getFileNameInSameDirAsOriginalFile(pathToShader, line).c_str());
   
   // And this one is name of the 1D texture to use
   if (!getline(inputStream, line))
   {
      printErorrMessage(fileName, line);
      return false;
   }
   
   string pathTo1DTexture;

   setPathTo1DTexture(getFileNameInSameDirAsOriginalFile(pathTo1DTexture, line).c_str());
   
   fileName_ = fileName;
   
   return true;
}

void GPUGeometryModel::setTimeSlice(double timeSlice)
{
   PRECONDITION(calculationEngine_);
   
   calculationEngine_->setTimeSlice(timeSlice);
   changedSinceLastRecalc_ = true;
}

double GPUGeometryModel::getTimeSlice() const
{
   PRECONDITION(calculationEngine_);
   return calculationEngine_->getTimeSlice();
}

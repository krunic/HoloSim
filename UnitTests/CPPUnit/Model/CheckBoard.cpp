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

#include "CheckBoard.h"

#include "MathHelper.h"
#include "SimpleDesignByContract.h"

// Default array value
static const double DEFAULT_ARRAY_VALUE = 0.0;

using namespace hdsim;

CheckBoard::CheckBoard() : sizeX_(0), sizeY_(0), positions_(0)
{
   
}

CheckBoard::CheckBoard(int sizeX, int sizeY) : sizeX_(sizeX), sizeY_(sizeY), positions_(new double[sizeX * sizeY])
{
   for (int index = 0; index < sizeX_ * sizeY_; index++)
      positions_[index] = DEFAULT_ARRAY_VALUE;
}

CheckBoard::CheckBoard(const CheckBoard &rhs) : sizeX_(rhs.sizeX_), sizeY_(rhs.sizeY_), positions_(new double[rhs.sizeX_ * rhs.sizeY_])
{
   // Implement over common copy code
   copyFrom(rhs);
}

CheckBoard & CheckBoard::operator=(const CheckBoard &rhs)
{
   if (&rhs == this)
      return *this;
   
   // Call super
   AbstractModel::operator=(rhs);
   copyFrom(rhs);
   return *this;
}

void CheckBoard::copyFrom(const CheckBoard &rhs)
{
	PRECONDITION(sizeX_ == rhs.sizeX_);
   PRECONDITION(sizeY_ == rhs.sizeY_);
   PRECONDITION(positions_  &&  rhs.positions_);
   
   for (int indexX = 0; indexX < sizeX_; indexX++)
      for (int indexY = 0; indexY < sizeY_; indexY++)
	      positions_[get1DIndex(indexX, indexY)] = rhs.positions_[indexX, indexY];
   
   sizeX_ == rhs.sizeX_;
   sizeY_ == rhs.sizeY_;
   
   POSTCONDITION(positions_ != rhs.positions_);
}

CheckBoard::~CheckBoard()
{
	delete [] positions_;   
}

const char * CheckBoard::getModelName() const
{
   return CHECKBOARD_MODEL_NAME;
}

void CheckBoard::setAt(int x, int y, double value)
{
   PRECONDITION(x >= 0  &&  x < sizeX_);
   PRECONDITION(y >= 0  &&  y < sizeY_);
   
   positions_[get1DIndex(x, y)] = value;
   POSTCONDITION(y * sizeX_ + x < sizeX_ * sizeY_);
}

double CheckBoard::getAt(int x, int y) const
{
   PRECONDITION(x >= 0  &&  x < sizeX_);
   PRECONDITION(y >= 0  &&  y < sizeY_);
   PRECONDITION(y * sizeX_ + x < sizeX_ * sizeY_);
   
   return positions_[get1DIndex(x, y)];
}

AbstractModel *CheckBoard::cloneOrphan() const 
{
   // Create copy of us and return it
   return new CheckBoard(*this);
}

void CheckBoard::initializeToCleanState()
{
   delete positions_;
   positions_ = 0;
   sizeX_ = sizeY_ = 0;
}

bool CheckBoard::readFromFile(const std::string &fileName)
{
	CHECK(false, "File I/O is not implemented with this model");   
   return true;
}

const char *CheckBoard::getFileName() const
{
	CHECK(false, "File I/O is not implemented with this model");   
   return 0;
}


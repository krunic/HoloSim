/*
 *  CheckBoard.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/20/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
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

bool CheckBoard::readFromFile(FILE *fp)
{
   // First line is model name
   char readThis[1024];
   sprintf(readThis, "%s\n", getModelName());
   fscanf(fp, readThis, NULL);

   // Next line is sizeX, sizeY
   int itemsRead = fscanf(fp, " %d %d\n", &sizeX_, &sizeY_);
   if (itemsRead != 2)
   {
      initializeToCleanState();
      return false;
  }
   
   delete positions_;
   positions_ = new double[sizeX_ * sizeY_];
   
   for (int indexY = 0; indexY < sizeY_; indexY++)
   {
      for (int indexX = 0; indexX < sizeX_; indexX++)
      {
         itemsRead = fscanf(fp, " %lf", positions_ + get1DIndex(indexX, indexY));
			if (itemsRead != 1)
         {
            initializeToCleanState();
            return false;
         }
      }

		// Skip newline at the end of file
		fscanf(fp, "\n");
   }
   
   return true;
}

bool CheckBoard::saveToFile(FILE *fp) const
{
   int itemsWritten = fprintf(fp, "%s\n", getModelName());
   if (itemsWritten <= 0)
		return false;
   
   itemsWritten = fprintf(fp, "%d %d\n", getSizeX(), getSizeY());
   if (itemsWritten <= 0)
		return false;
   
   for (int indexY = 0; indexY < sizeY_; indexY++)
   {
      for (int indexX = 0; indexX < sizeX_; indexX++)
      {
         itemsWritten = fprintf(fp, " %lf", positions_[get1DIndex(indexX, indexY)]);
			if (itemsWritten <= 0)
            return false;
      }

		// Write newline at the end of line
		itemsWritten = fprintf(fp, "\n");
		if (itemsWritten <= 0)
         return false;
   }
   
   return true;
}

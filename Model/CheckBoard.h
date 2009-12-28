/*
 *  CheckBoard.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/20/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#ifndef CHECKBOARD_H_
#define CHECKBOARD_H_

#include "AbstractModel.h"
#include "MathHelper.h"

namespace hdsim {

   /**
    * Name of the checkboard model
    */
   static const char * const CHECKBOARD_MODEL_NAME = "CheckboardModel";

   /**
    * Checkboard used for remembering rod position at the particular moment in time. Note that it can't do 
    */
   class CheckBoard : public AbstractModel {
     
   public:
      
      /**
       * Default constructor
       */
      CheckBoard();
      
      /**
       * Constructor with arguments
       *
       * @param sizeX - Size of the board in X direction
       * @param sizeY - Size of the board in Y direction
       */
      CheckBoard(int sizeX, int sizeY);
      
      /**
         * Copy constructor
       *
       * @param rhs Copy to take
       */
      CheckBoard(const CheckBoard &rhs);
      
      /**
         * Constructor with arguments
       *
       * @param rhs Copy to take
       */
      CheckBoard & operator=(const CheckBoard &rhs);

      /**
       * Destructor
       */
      virtual ~CheckBoard();

      // Overriden methods
      virtual const char * getModelName() const;   
      virtual void setAt(int x, int y, double value);
      virtual double getAt(int x, int y) const;
      virtual AbstractModel *cloneOrphan() const;
      
      /**
       * Get size in X direction
       *
       * @return Size in X direction
       */
      virtual int getSizeX() const
      {
         return sizeX_;
      }
      
      /**
       * Get size in Y direction
       *
       * @return Size in Y direction
       */
      virtual int getSizeY() const
      {
         return sizeY_;
      }

      /**
       * Read model from file. Format of the file is:
       *
       * TypeName
       * sizeX, sizeY\n
       *
       * (for each size Y one row) Zvalue(1) Zvalue(2) ... Zvalue(SizeX)\n
       *
       * @param fp File to read from
       *
       * @return Was read success
       */
      virtual bool readFromFile(FILE *fp);
      
      /**
       * Write model to file. Format of the file is:
       *
       * TypeName
       * sizeX, sizeY\n
       *
       * (for each size Y one row) Zvalue(1) Zvalue(2) ... Zvalue(SizeX)\n
       *
       * @param fp File to write to
       *
       * @return Was write success
       */
      virtual bool saveToFile(FILE *fp) const;
      
   private:

      /**
       * Initilize model to clean state
       */
      void initializeToCleanState();

      /**
       * Copy value from rhs to this object
       *
       * @param rhs Value to copy
       */
      void copyFrom(const CheckBoard &rhs);
      
      /**
       * Calculate linear index position in 1D array corresponding to given indexes
       *
       * Example - in 2D array with sizeX == 1 and sizeY == 2, linear index corresponding to [0, 1] is 1
       *
       * @param x X position
       * @param y Y position    
       *
       * @return Linear index in 1D array
       */
      int get1DIndex(int x, int y) const
      {
         return y * sizeX_ + x;
      }
      
      // Friend with its operators
      friend bool operator==(const CheckBoard &lhs, const CheckBoard &rhs);
      friend bool operator!=(const CheckBoard &lhs, const CheckBoard &rhs);

      /*
       * Allows unit test to access private parts of this class. Problem is that in the future unit test class might not be part of the release
       * executable. GCC allows for this, but to the best of my knowledge standard doesn't define that unknown and unused friend classes are ignored
       */
      friend class CheckBoardTest;

      /**
       * Size in x direction
       */
      int sizeX_;
      
      /**
       * Size in y direction
       */
      int sizeY_;
      
      /**
       * Positions
       */
      double *positions_;
   };

   /** 
    * Compare two checkboards. Note that exact match is not required in Z values - it is enough for the Z values to be "close" for checkboards to be declared equal
    * 
    * @param lhs Left operand
    * @param rhs right operand
    * 
    * @return Are models equal (equal being that Z values are close enough that difference could be attributed to floating point error)
    */
   inline bool operator==(const CheckBoard &lhs, const CheckBoard &rhs)
   {
      if (lhs.sizeX_ != rhs.sizeX_  ||  lhs.sizeY_ != rhs.sizeY_)
         return false;
      
      for (int index = 0; index < lhs.sizeX_ * lhs.sizeY_; index++)
         if (!areEqual(lhs.positions_[index], rhs.positions_[index]))
            return false;
      
      return true;
   }

   /** 
   * Compare two checkboards. Note that exact match is not required in Z values - it is enough for the Z values to be "close" for checkboards to be declared equal
   * 
   * @param lhs Left operand
   * @param rhs right operand
   * 
   * @return Are models different (equal being that Z values are close enough that difference could be attributed to floating point error)
   */
   inline bool operator!=(const CheckBoard &lhs, const CheckBoard &rhs)
   {
      return !(lhs == rhs);
   }
   
} // namespace

#endif

/*
 *  GPUInterpolatedModel.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 2/4/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef GPU_INTERPOLATED_MODEL_H_
#define GPU_INTERPOLATED_MODLE_H_

#include "GPUGeometryModel.h"
#include "SimpleDesignByContract.h"

static const char *const GPU_INTERPOLATED_MODEL_NAME = "GPUInterpolatedModel";

using namespace std;

namespace hdsim {
   
   /**
    * This model performs linear interpolation between inital position in Z buffer and calculaed position in Z buffer
    */
   class GPUInterpolatedModel : public AbstractModel
   {
   public:
      
      /**
       * Min value of the time slice
       */
      static const double MIN_TIME_SLICE = 0;
      
      /**
       * Max value of the time slice
       */
      static const double MAX_TIME_SLICE = 1;
      
      /**
       * Default constructor
       */
      GPUInterpolatedModel();
      
      /**
       * Copy constructor
       *
       * @param rhs Object to use to create copy
       */
      GPUInterpolatedModel(const GPUInterpolatedModel &rhs);
      
      /**
       * Operator =
       *
       * @param rhs Object to use to create copy
       */
      GPUInterpolatedModel & operator=(const GPUInterpolatedModel &rhs);
      
      /**
       * Destructor
       */
      virtual ~GPUInterpolatedModel();
      
      // Overriden methods
      virtual const char *getModelName() const
      {
         return GPU_INTERPOLATED_MODEL_NAME;
      }
      
      virtual double getAt(int x, int y) const;
      virtual AbstractModel *cloneOrphan() const;
      
      /**
       * Get model size in X direction
       *
       * @return Model size at X direction
       */
      virtual int getSizeX() const
      {
         return model_.getSizeX();
      }
      
      /**
       * Get model size in Y direction
       *
       * @return Model size at Y direction
       */
      virtual int getSizeY() const
      {
         return model_.getSizeY();
      }
      
      /**
       * Set current value of timeslice. This is opportunity to do internal caching, if subclass wants to do it
       *
       * PRECONDITION: Begin and end model have to be set before making this call
       * 
       * @param ts value of the timeslice
       */
      virtual void setTimeSlice(double ts)
      {
         timeSlice_ = ts;
      }
      
      /**
       * Get current value of timeslice. This is opportunity to do internal caching, if subclass wants to do it
       *
       * @return Value of the timeslice
       */
      virtual double getTimeSlice() const
      {
         return timeSlice_;
      }
      
      /**
       * Read model from file. Format of the file is:
       *
       * Model name\n
       * Number of models\n       
       * timeslice\n
       * Number of models
       * model1\n
       * model2\n
       *
       * @param fp File to read from
       *
       * @return Was read success
       */
      virtual bool readFromFile(FILE *fp);
      
      /**
       * Write model to file. Format of the file is:
       *
       * Model name\n    
       * Number of models\n       
       * timeslice\n      
       * model1\n
       * model2\n
       *
       * @param fp File to read from
       *
       * @return Was save success
       */
      virtual bool saveToFile(FILE *fp) const;
      
   private:
      
      /**
       * Initilize model to clean state
       */
      void initializeToCleanState();
      
      /**
       * Copy from the abstract model to this model
       *
       * @param rhs Model to copy from
       */
      void copyFrom(const AbstractModel &rhs);
      
      // friend with its operators
      friend bool operator==(const GPUInterpolatedModel &lhs, const GPUInterpolatedModel &rhs);
      friend bool operator!=(const GPUInterpolatedModel &lhs, const GPUInterpolatedModel &rhs);
      
      /**
       * Model that is encapsulated
       */
		GPUGeometryModel model_;
      
      /**
       * Timeslice at which we should be
       */
      double timeSlice_;
   };
   
   /** 
    * Compare two interpolated models. They are the same if they are at the "close" timeslice (so close that it could be attributed to floating point rounding) and
    * their start and stop model are "close" too
    * 
    * @param lhs Left operand
    * @param rhs right operand
    * 
    * @return Are models equal (equal being that timeslice and Z values are close enough that difference could be attributed to floating point error)
    */
   inline bool operator==(const GPUInterpolatedModel &lhs, const GPUInterpolatedModel &rhs)
   {
      if (!areEqual(lhs.getTimeSlice(), rhs.getTimeSlice()))
         return false;

      return lhs.model_ == rhs.model_  &&  lhs.timeSlice_ == rhs.timeSlice_;
      
      return true;
   }
   
   /** 
    * Compare two interpolated models. They are the same if they are at the "close" timeslice (so close that it could be attributed to floating point rounding) and
    * their start and stop model are "close" too
    * 
    * @param lhs Left operand
    * @param rhs right operand
    * 
    * @return Are models different (equal being that timeslice and Z values are close enough that difference could be attributed to floating point error)
    */
   inline bool operator!=(const GPUInterpolatedModel &lhs, const GPUInterpolatedModel &rhs)
   {
      return !(rhs == lhs);
   }
   
} // namespace

#endif
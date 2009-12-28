/*
 *  InterpolatedModel.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/24/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#ifndef INTERPOLATED_MODEL_H_
#define INTERPOLATED_MODLE_H_

#include <vector>

#include "CheckBoard.h"
#include "SimpleDesignByContract.h"

static const char *const INTERPOLATED_MODEL_NAME = "InterpolatedModel";

using namespace std;

namespace hdsim {

   /**
    * This model performs linear interpolation between models in it.
    *
    * Class is capable of holding N models in it, and does linear interpolation between begin and end model in segment. Example: suppose there are 
    * four models in interpolated model. Suppose that we have timeslice at position 0.5. We would want to interpolate between models 2 and 3.
    *
    * Formally, if we have N models and current value of timeslice is tc, then:
    *
    * ts (TimeSlice Size) = 1/(N-1) <br>
    * Begin Model Index = floor(tc/ts) <br>
    * End Model Index = Begin Model Index + 1 <br>
    * Initial Timeslice Value in Interval = floor(tc/ts) * ts  <br>
    * Final Timeslice Value in Interval =  Initial Timeslice Value in Interval + ts <br>
    */
   class InterpolatedModel : public AbstractModel
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
      InterpolatedModel();
      
      /**
       * Constructor
       *
       * @param numModels number of (initially zero-filled) models
       * @param size Size of those models in both x and y directions
       */
      InterpolatedModel(int numModels, int size);
      
      /**
       * Copy constructor
       *
       * @param rhs Object to use to create copy
       */
      InterpolatedModel(const InterpolatedModel &rhs);
      
      /**
       * Operator =
       *
       * @param rhs Object to use to create copy
       */
      InterpolatedModel & operator=(const InterpolatedModel &rhs);
      
      /**
       * Destructor
       */
      virtual ~InterpolatedModel();
      
      // Overriden methods
      virtual const char * getModelName() const;   
      virtual double getAt(int x, int y) const;
      virtual AbstractModel *cloneOrphan() const;
      
      /**
       * Set value at the currently active model. That gives you quick way to change value in the begin or end model without incuring price of copy in resetting the model.
       *
       * @param index Index in the model we should set value
       * @param x X value
       * @param y Y value
       * @param value Value to use
       */
      virtual void setValueInModelWithIndex(int index, int x, int y, double value);
      
      /**
       * Add model for interpolation at the end of the model queue. 
       *
       * @param model Model to set. This has to be a heap pointer and this class would take ownership of it. It is always added at the end of the list of models.
       */
      virtual void addModelAdopt(AbstractModel *model);
      
      /**
       * Get number of models
       *
       * @return Number of models
       */
      virtual int getNumberOfModels() const;
      
      /**
       * Get model at the given index
       *
       * PRECONDITION index >= 0 and index < getNumberOfModels
       *
       * Get model at the particular index
       */
      virtual const AbstractModel *getModelAtIndex(int index) const;
      
      /**
       * Get model size in X direction
       *
       * @return Model size at X direction
       */
      virtual int getSizeX() const;

      /**
       * Get model size in Y direction
       *
       * @return Model size at Y direction
       */
      virtual int getSizeY() const;
      
      /**
       * Set current value of timeslice. This is opportunity to do internal caching, if subclass wants to do it
       *
       * PRECONDITION: Begin and end model have to be set before making this call
       * 
       * @param ts value of the timeslice
       */
      virtual void setTimeSlice(double ts);
      
      /**
       * Get current value of timeslice. This is opportunity to do internal caching, if subclass wants to do it
       *
       * @return Value of the timeslice
       */
      virtual double getTimeSlice() const;
      
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
      
	protected:
         
      /**
       * Get modifiable internal model from which we should begin any interpolation
       *
       * PRECONDITION There should be at least two models added to interpolated model       
       *
       * @return Model associated with beggining of this timeslice
       */
      virtual const CheckBoard *getBeginModel() const
      {
         PRECONDITION(getNumberOfModels() > 1);
         // ts (from class comment) is 1/(getNumberOfModels - 1)
         int index = (int)floor(getTimeSlice() * (getNumberOfModels() - 1));
         CheckBoard *result = models_[index];
         POSTCONDITION(result);
         return result;
      }
      
      /**
       * Get modifiable internal model in which we should end any interpolation
       *
       * PRECONDITION There should be at least two models added to interpolated model       
       *
       * @return Model associated with end of this timeslice
       */
      virtual const CheckBoard *getEndModel() const
      {
         PRECONDITION(getNumberOfModels() > 1);
         // ts (from class comment) is 1/(getNumberOfModels - 1)         
         int index = (int)floor(getTimeSlice() * (getNumberOfModels() - 1)) + 1;
         
         // Now, there is one catch here, and that is that when timeslice reaches end value, you want begin and end model to 
         // be the same model. So we would renormalize index in that situation, causing begin and end to be the same
         index = index < models_.size() - 1 ? index : models_.size() - 1; 
         
         CheckBoard *result = models_[index];
         POSTCONDITION(result  &&  index < models_.size());
         return result;
      }

      /**
       * Get beggining value of timeslice for this current interval. At that time, we would return 
       * exact begin model for this timeslice without interpolation.
       *
       * PRECONDITION There should be at least two models added to interpolated model       
       *
       * @return Timeslice associated with the beggining of this segment
       */
      virtual double getInterpolationBeginTimeSlice() const
      {
         PRECONDITION(getNumberOfModels() > 1);
         // ts (from class comment) is 1/(getNumberOfModels - 1)
			return floor(getTimeSlice() * (getNumberOfModels() - 1))/(getNumberOfModels() - 1);
      }
      
      /**
       * Get end value of timeslice for this current interval. At that time, we would return 
       * exact begin model for this timeslice without interpolation.
       *
       * PRECONDITION There should be at least two models added to interpolated model       
       *
       * @return Timeslice associated with the endbeggining of this segment
       */
      virtual double getInterpolationEndTimeSlice() const
      {
         PRECONDITION(getNumberOfModels() > 1);
         // ts (from class comment) is 1/(getNumberOfModels - 1)         
			return getInterpolationBeginTimeSlice() + 1/(static_cast<float>(getNumberOfModels()) - 1);
      }
      
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
      friend bool operator==(const InterpolatedModel &lhs, const InterpolatedModel &rhs);
      friend bool operator!=(const InterpolatedModel &lhs, const InterpolatedModel &rhs);
      
      friend class InterpolatedModelTest;
      
      /**
       * Vector of models
       */
      vector<CheckBoard *>models_;
      
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
   inline bool operator==(const InterpolatedModel &lhs, const InterpolatedModel &rhs)
   {
      if (!areEqual(lhs.getTimeSlice(), rhs.getTimeSlice()))
         return false;
      
      for (int modelIndex; modelIndex < rhs.models_.size(); modelIndex++)
         if (*lhs.models_[modelIndex] != *rhs.models_[modelIndex])
            return false;
      
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
   inline bool operator!=(const InterpolatedModel &lhs, const InterpolatedModel &rhs)
   {
      return !(rhs == lhs);
   }
   
} // namespace

#endif

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

#ifndef GPU_INTERPOLATED_MODEL_H_
#define GPU_INTERPOLATED_MODEL_H_

#include "GPUGeometryModel.h"
#include "SimpleDesignByContract.h"
#include "Statistics.h"

#include <string>

static const char *const GPU_INTERPOLATED_MODEL_NAME = "GPUInterpolatedModel";

using namespace std;

namespace hdsim {
   
   /**
    * This model was once intended to performs linear interpolation between inital position in Z buffer and calculaed position in Z buffer. At this point, it is 
    * simply a proxy for the final model and is left in in the case that we later decide to work with some morphing from one model to another
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
      virtual int getSizeX() const;
      virtual int getSizeY() const;
      
      /**
       * Get model that is reduced on xSize, ySize dimensions. It would apply box filter on all the cells in the model, with the rightmost and bottom part holding 
       * any "extra" cells if m->getSizeX() is not exactly divisible with xSize (and respective for ySize
       *
       * @param m Model to use
       * @param xSize X size of the decimated model
       * @param ySize Y size of the decimated model       
       * 
       * @return Pointer to the 1D array of doubles holding decimated size, stored so that [x][y] corresponds to [y * ySize + x]. Caller is responsible for invoking delete [] on this pointer
       */
      static double *getDecimatedModelAdopt(const AbstractModel *m, int xSize, int ySize);
      
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
         model_.setTimeSlice(ts);
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
       * Get minX of the bound
       */
      virtual double getBoundMinX() const
      {
         return model_.getBoundMinX();
      }
      
      /**
       * Get maxX of the bound
       */
      virtual double getBoundMaxX() const
      {
         return model_.getBoundMaxX();
      }
      
      /**
       * Get minY of the bound
       */
      virtual double getBoundMinY() const
      {
         return model_.getBoundMinY();
      }
      
		/**
       * Get maxY of the bound
       */
      virtual double getBoundMaxY() const
      {
         return model_.getBoundMaxX();
      }
      
      /**
       * Get minZ of the bound
       */
      virtual double getBoundMinZ() const
      {
         return model_.getBoundMinZ();
      }
      
      /**
       * Get maxZ of the bound
       */
      virtual double getBoundMaxZ() const
      {
         return model_.getBoundMaxZ();
      }

      /**
       * Get minX of the rendered area
       */
      virtual double getRenderedAreaMinX() const
      {
         return model_.getRenderedAreaMinX();
      }
      
      /**
       * Get maxX of the rendered area
       */
      virtual double getRenderedAreaMaxX() const
      {
         return model_.getRenderedAreaMaxX();
      }
      
      /**
       * Get minY of the rendered area
       */
      virtual double getRenderedAreaMinY() const
      {
         return model_.getRenderedAreaMinY();
      }
      
		/**
       * Get maxY of the bound
       */
      virtual double getRenderedAreaMaxY() const
      {
         return model_.getRenderedAreaMaxY();
      }
      
      /**
       * Get minZ of the rendered area
       */
      virtual double getRenderedAreaMinZ() const
      {
         return model_.getRenderedAreaMinZ();
      }
      
		/**
       * Get maxZ of the bound
       */
      virtual double getRenderedAreaMaxZ() const
      {
         return model_.getRenderedAreaMaxZ();
      }
      
      /**
       * Set rendered area bounds
       */
      virtual void setRenderedArea(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
      {
         model_.setRenderedArea(minX, minY, minZ, maxX, maxY, maxZ);
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
       * @param fileName File to read from
       *
       * @return Was read success
       */
      virtual bool readFromFile(const std::string &fileName);
      
      /**
       * Get is model calculated or there are still calculations to perform that are pending
       *
       * @return Is model calculated
       */
      virtual bool isModelCalculated() const;
      
      /**
       * Perform all calculations on the model
       */
      virtual void forceModelCalculation() const;
      
      /**
       * Get fileName from which model was loaded
       *
       * @return file name from which model was loaded
       */
      virtual const char *getFileName() const 
      {
         return fileName_.c_str();
      }
      
      /**
       * Set should we optimize drawing if the number of moxels exceeds threshold
       */
      virtual void setOptimizeDrawing(bool optimize)
      {
         if (optimizeDrawing_ != optimize)
         {
         	model_.setNeedsRecalc();
            optimizeDrawing_ = optimize;   
         }
      }
      
      /**
       * Get should we optimize drawing if the number of moxels exceeds threshold
       *
       * @return Should we optimize drawing
       */
      virtual bool getOptimizeDrawing() const
      {
         return optimizeDrawing_;
      }
      
      /**
       * Set moxel optimization threshold. If we optimize drawing, resulting model drawn would have this many moxels
       *
       * @param threshold Max number of moxels in optimized model
       */
      virtual void setMoxelThreshold(int threshold)
      {
         if (threshold != optimizeDrawingThreshold_)
         {
            model_.setNeedsRecalc();
            optimizeDrawingThreshold_ = threshold;
         }
      }
      
      /**
       * Get moxel optimization threshold. If we optimize drawing, resulting model drawn would have this many moxels
       *
       * @return Max number of moxels in optimized model
       */
      virtual int getMoxelThreshold() const
      {
         return optimizeDrawingThreshold_;
      }
      
      /**
       * Get recommended model size for optimized drawing
       *
       * PRECONDITION Optimized drawing must be enabled
       *
       * @return Recommended model size for drawing in X direction
       */
      virtual int getModelSizeForOptimizedDrawingX() const;
      
      /**
       * Get recommended model size for optimized drawing
       *
       * PRECONDITION Optimized drawing must be enabled
       *
       * @return Recommended model size for drawing in Y direction
       */
      virtual int getModelSizeForOptimizedDrawingY() const;
   
      /**
       * Is this model currently optimizing content
       */
      virtual bool isDrawingOptimizationActive() const;
      
      /**
       * Get total num moxels in the model. This doesn't depend on is model optimizing drawing or not
       * 
       * @return Total number of moxels
       */
      virtual int getTotalNumMoxels() const
      {
         return model_.getSizeX() * model_.getSizeY();
      }
      
      /**
       * Get moxel rendering statistics for the last frame in which moxels were rendered
       */
      virtual Statistics getLastRenderedFrameMoxelStatistics() const
      {
         return lastMoxelRenderingStatistics_;
   	}
      
      /**
       * Get statistics related to moxel calculation
       *
       * @return Statistics related to moxel calculation
       */
      virtual Statistics getMoxelCalculationStatistics() const 
      {
         return moxelCalculationStatistics_;
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
      
      /**
       * Filename from which model was loaded
       */
      std::string fileName_;
      
      /**
       * Should we optimize drawing
       */
      bool optimizeDrawing_;
      
      /**
       * Threshold for number of moxels in model that, if exceeded, cuases us to optimize drawing
       */
      int optimizeDrawingThreshold_;
      
      /**
       * Statistics for rendering last frame of moxels
       */
      mutable Statistics lastMoxelRenderingStatistics_;
      
      /**
       * Statistics related to moxel calculation
       */
      mutable Statistics moxelCalculationStatistics_;
      
      /**
       * Optimized model size in X
       */
      mutable int optimizedModelSizeX_;
      
      /**
       * Optimized model size in Y
       */
      mutable int optimizedModelSizeY_;
      
      /**
       * Simplified model
       */
      mutable double *decimatedModel_;
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
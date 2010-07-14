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

#ifndef GPU_CALCULATION_ENGINE_H_
#define GPU_CALCULATION_ENGINE_H_

#include <string>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/OpenGL.h>

#include "AbstractModel.h"
#include "GPUGeometryModel.h"
#include "Shader.h"

namespace hdsim {

   /**
    * Uses frame buffer object to perform GPU based calculation
    */ 
	class GPUCalculationEngine {
   
		public:

      	GPUCalculationEngine();
   
		   virtual ~GPUCalculationEngine();
      
      	/**
          * Calculate positions for the given model and store it so that it could be subsequently obtained
          *
          * This function is not thread safe
          *
          * @param model Calculate picture for engine position
          */
		   virtual void calculateEngine(const AbstractModel *model);
      
      	/**
          * Initialization
          *
          * @param model - model to use
          */
	      virtual void initialize(const AbstractModel *model);
      
      	/**
          * Notifies calculation engine that associated OpenGL context was destroyed and can't be used
          */
	      virtual void deInitialize();
      
      	/**
          * Get previously calculated position at x, y
          *
          * @param x - X position
          * @param y - Y position
          *
          * @return Previously calculated value at that position
          */
      	virtual double getAt(int x, int y) const 
         {
            return renderedDepth_[y*width_ + x];
         }
      
         /**
          * Should we load shader from bundle
          *
          * @param timeSlice - Timeslice to use
          */
         virtual void setTimeSlice(double timeSlice)
         {
				timeSlice_ = timeSlice;
         }
         
         /**
          * Get value of the timeslice
          *
          * @return timeSlice 
          */
         virtual double getTimeSlice() const
         {
            return timeSlice_;
         }
      
         /**
          * Get path to shader file
			 * 
          * @param model model to use for getting the path
          * @param path (OUT) Path to the filename using UNIX file convention
          *
          * @return Was conversion success
          */
      virtual bool getPathToShaderFileAdopt(const GPUGeometryModel *model, std::string *path) const;

   	private:
               
      	// copying is not supported for now
	      GPUCalculationEngine(const GPUCalculationEngine &rhs);
   	   GPUCalculationEngine &operator=(const GPUCalculationEngine &rhs);
      
      	/**
          * Initialize OpenGL frame buffer
          */
	      bool initFrameBuffer(int width, int height);
      
      	/**
          * Destroy OpenGL frame buffer
          */
	      bool destroyFrameBuffer();
      
         /**
          * Calculate positions for the given model.
          *
          * This function is not thread safe
          *
          * @param model Calculate picture for engine position
          */
         void calculate(const GPUGeometryModel *model);
      
         /**
          * IDs of the current render buffer and frame buffer objects used
          */
         GLuint frameBufferID_, colorBufferID_, depthBufferID_;
      
      	/**
          * Were we succesfully initialized
          */
	      bool wasInitialized_;
      
      	/**
          * Dimensions
          */
	      int width_, height_;
            
      	/**
          * Off screen context used for drawing
          */
         CGLContextObj cglContext_;
      
      	/**
          * Rendered depth buffer (from the graphic card)
          */
	      GLfloat *renderedDepth_;
      
      	/**
          * Shader to use
          */
	      Shader shader_;
      
      	/**
          * Timeslice value
          */
	      double timeSlice_;
	};
   
}

#endif

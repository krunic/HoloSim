/*
 *  GPUCalculationEngine.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef GPU_CALCULATION_ENGINE_
#define GPU_CALCULATION_ENGINE_

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/OpenGL.h>

#include "AbstractModel.h"
#include "GPUGeometryModel.h"

namespace hdsim {

   /**
    * Check is this whole world substring
    *
    * @param searchFor - string to search for
    * @param searchIn - String to search in for searchFor
    */
   bool isWholeWorldSubstring(const char *searchIn, const char *searchFor);
   
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
         GLuint frameBufferID_, renderBufferID_, colorBufferID_;
      
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
          * Memory for the offscreen drawning
          */
	      char *offScreenMemory_;
      
      	/**
          * Rendered depth buffer (from the graphic card)
          */
	      double *renderedDepth_;

	};
   
}

#endif

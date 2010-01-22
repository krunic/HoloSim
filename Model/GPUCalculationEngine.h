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

#include "AbstractModel.h"
#include "GPUGeometryModel.h"

namespace hdsim {

	class GPUCalculationEngine {
   
		public:

      	GPUCalculationEngine();
		   GPUCalculationEngine(const GPUCalculationEngine &rhs);
		   GPUCalculationEngine &operator=(const GPUCalculationEngine &rhs);
   
		   virtual ~GPUCalculationEngine();
      
      	/**
          * Calculate positions for the given model.
          *
          * This function is not thread safe
          *
          * @param model Calculate picture for engine position
          */
		   virtual void calculateEngine(AbstractModel *model);
      
   	private:

         /**
          * Calculate positions for the given model.
          *
          * This function is not thread safe
          *
          * @param sizeX Number of moxels in the X axis of the model
          * @param sizeY Number of moxels in the Y axis of the model
          * @param minX Min frustum on X axis
          * @param maxX Max frustum on X axis
          * @param minY Min frustum on Y axis
          * @param maxY Max frustum on Y axis
          * @param minZ Min frustum on Z axis
          * @param maxZ Max frustum on Z axis
          */      
      	virtual void positionCamera(int sizeX, int sizeY, double minX, double maxX, double minY, double maxY, double minZ, double maxZ);

         /**
          * Calculate positions for the given model.
          *
          * This function is not thread safe
          *
          * @param model Calculate picture for engine position
          */
         virtual void calculate(GPUGeometryModel *model);
	};
   
}

#endif

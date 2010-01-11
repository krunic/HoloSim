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

namespace hdsim {

	class GPUCalculationEngine {
   
		public:

      	GPUCalculationEngine();
		   GPUCalculationEngine(const GPUCalculationEngine &rhs);
		   GPUCalculationEngine &operator=(const GPUCalculationEngine &rhs);
   
		   virtual ~GPUCalculationEngine();
		   virtual void calculateEngine(const AbstractModel *model);
	};
   
}

#endif

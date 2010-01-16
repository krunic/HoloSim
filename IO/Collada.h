/*
 * Copyright © Veljko Krunic 2007-2010. All rights reserved.
 *
 * Modification of the original code, which license is below:
 * 
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the MIT Open Source License, for details please see license.txt or the website
 * http://www.opensource.org/licenses/mit-license.php
 *
 */ 
#ifndef COLLADA_H_
#define COLLADA_H_

#include <list>
#include <vector>
#include <iostream>

#include "GPUGeometryModel.h"

namespace hdsim {
   
   bool loadCollada(const char *name, GPUGeometryModel &loadToThisModel);
}


#endif

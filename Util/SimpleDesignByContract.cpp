/*
 *  SimpleDesignByContract.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/29/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#include "SimpleDesignByContract.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>

using namespace hdsim;

void hdsim::checkAssertion(bool condition, const char *message, const char *type, const char *fileName, int sourceLine)
{
   if (!condition)
   {
      if (message)
      {
	      fprintf(stderr, "%s failed at file %s, line %d with message: %s\n", type, fileName, sourceLine, message);         
      }
      else
      {
	      fprintf(stderr, "%s failed at file %s, line %d\n", type, fileName, sourceLine);         
      }

      exit(1);
   }
}
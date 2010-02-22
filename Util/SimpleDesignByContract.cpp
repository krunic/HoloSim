/*
 *  SimpleDesignByContract.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/29/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */

#include "SimpleDesignByContract.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <iostream>

using namespace hdsim;
using namespace std;

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

void hdsim::log(const char *message, const char *fileName, int sourceLine)
{
   if (message)
   {
      cerr << message << endl << "File " << fileName << ", line " << sourceLine << endl;         
   }
   else
   {
      cerr << "Empty log message at file " << fileName << ", line " << sourceLine << endl;         
   }
}
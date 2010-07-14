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
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

#include "AbstractModel.h"

using namespace hdsim;
using namespace std;

string hdsim::getFileNameInSameDirAsOriginalFile(const string &fileName, const string &newFile)
{
   string result;
   
   int foundAtPosition = fileName.rfind('/');
   
   if (foundAtPosition == string::npos)
   {
      // We have a file in the current directory 
      result = newFile;
   }
   else
   {
      // We need to add filename to string name
      result = fileName.substr(0, foundAtPosition); 
      result += "/";
      result += newFile;
   }
   
   return result;
}

AbstractModel::~AbstractModel()
{
   
}

AbstractModel *AbstractModel::cloneOrphan() const
{
	// We can't get clone of abstract model
   return 0;
}


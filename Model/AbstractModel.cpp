/*
 *  AbstractModel.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/20/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
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


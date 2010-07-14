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

#ifndef ABSTRACT_MODEL_H_
#define ABSTRACT_MODEL_H_

#include <cstdio>
#include <string>

namespace hdsim {
   
   /**
    * Used when we need to open file in the same directory as original file was
    *
    * @param fileName Name of the file
    * @param newFile Name of the new file
    *
    * @return New filename using same path as original file
    */
   std::string getFileNameInSameDirAsOriginalFile(const std::string &fileName, const std::string &newFile);

   /**
    * This is definition of the abstract model that should be used for the storing of data about the board. There is contract between model and view controller that makes it possible 
    * for the view to render appropriate model. 
    */
   class AbstractModel {
      
   public:

      /**
       * Destructor 
       */
      virtual ~AbstractModel() = 0;
      
      /**
       * Get name of the model
       * 
       * @return Name of the model
       */
      virtual const char * getModelName() const = 0;   
      
      /**
       * Get value at the particular location
       * @param x X position
       * @param y Y position
       *
       * @return Value to set at the particular place
       */
      virtual double getAt(int x, int y) const = 0;
      
      /**
       * Get model size in X direction
       *
       * @return Model size at X direction
       */
      virtual int getSizeX() const = 0;
      
      /**
       * Get model size in Y direction
       *
       * @return Model size at Y direction
       */
      virtual int getSizeY() const = 0;
      
      /**
       * Provide clone of this model that is deep copy
       *
       * @return Clone of the deep copy or 0 if there is no clone. Caller owns that pointer and needs to deallocate it
       */
      virtual AbstractModel *cloneOrphan() const = 0;
      
      /**
       * Read model from file
       *
       * @param fileName file to read from
       *
       * @return Was read success
       */
      virtual bool readFromFile(const std::string &fileName) = 0;
      
      /**
       * Get filename from which model was loaded
       *
       * @return Get filename from which model was loaded
       */
      virtual const char *getFileName() const = 0;
   };
   
} // namespace

#endif

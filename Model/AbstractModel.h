/*
 *  AbstractModel.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/20/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#ifndef ABSTRACT_MODEL_H_
#define ABSTRACT_MODEL_H_

#include <cstdio>

namespace hdsim {

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
       * Provide clone of this model that is deep copy
       *
       * @return Clone of the deep copy or 0 if there is no clone. Caller owns that pointer and needs to deallocate it
       */
      virtual AbstractModel *cloneOrphan() const = 0;
      
      /**
       * Read model from file
       *
       * @param fp file to read from
       *
       * @return Was read success
       */
      virtual bool readFromFile(FILE *fp) = 0;
      
      /**
       * Save model to file
       *
       * @param fp file to save model to
       *
       * @return Was write success
       */
      virtual bool saveToFile(FILE *fp) const = 0;
   };
   
} // namespace

#endif

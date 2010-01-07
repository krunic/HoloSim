/*
 *  ControllerAdapter.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 5/15/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef CONTROLLER_ADAPTER_H_
#define CONTROLLER_ADAPTER_H_

#include "AbstractDrawingCode.h"

namespace hdsim {

   /**
    * ControllerAdapter is responsible for controlling view over the 3D view so that 3D view is changed based on the controller input. Note that 
    * ControllerAdapter doesn't own a view positioning - drawer owns it. ControllerAdaoter just translates changes in device orientation to drawer.
    */
   class ControllerAdapter
   {
   public:
      /**
       * Constructor
       */
      ControllerAdapter();
      
      /**
       * Copy constructor
       *
       * @param rhs Copy to make from
       */
      ControllerAdapter(const ControllerAdapter &rhs);
      
      /**
       * Operator =
       */
      ControllerAdapter & operator=(const ControllerAdapter &rhs);
      
      /**
       * Destructor
       */
      virtual ~ControllerAdapter() = 0;
      
      /**
       * Set adaptee
       *
       * @param code adaptee
       */
      virtual void setDrawer(AbstractDrawingCode *code);
      
      /**
       * Get adaptee
       *
       * @return Drawing code adaptee
       */
      virtual AbstractDrawingCode *getDrawer() const;
      
   private:
      AbstractDrawingCode *drawingCode_;
   };

} // namespace

#endif

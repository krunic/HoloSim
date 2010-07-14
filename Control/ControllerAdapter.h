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

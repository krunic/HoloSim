/*
 *  MouseAdapter.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 5/15/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#ifndef MOUSE_ADAPTER_H_
#define MOUSE_ADAPTER_H_

#include "ControllerAdapter.h"

namespace hdsim {

   /**
    * Mouse based control of the model. This class is responsible for controlling view over the 3D view so that 3D view is changed based on the controller 
    * input. Note that MouseAdapter doesn't own a view positioning - drawer owns it. It just translates changes in device orientation to drawer.
    */
   class MouseAdapter : public ControllerAdapter
   {
   public:
      /**
      * Constructor
       */
      MouseAdapter();
      
      /**
      * Copy constructor
       */
      MouseAdapter(const MouseAdapter &rhs);
      
      /**
         * Operator =
       * 
       * @param rhs Make copy from this
       */
      MouseAdapter & operator=(const MouseAdapter &rhs);
      
      /**
       * Destructor
       */
      virtual ~MouseAdapter();   
      
      /**
       * Move model based on the new mouse position
       *
       * PRECONDITIONS: At the moment, it must be that lmb == true and rmb == false as this is only supported control
       *
       * @param x X position
       * @param y Y position
       * @param lmb Left button down?
       * @param rmb Right button down?
       */
      virtual void controlForMouseAt(int x, int y, bool lmb, bool rmb);
      
      /**
       * Move model based on the mouse scroll change. At the moment, we would react only on position change in Y axis
       *
       * @param x X position change
       * @param y Y position change 
       * @param z Z position change
       */
      virtual void controlForMouseScrollChange(int x, int y, int z);
      
      /**
       * Get how many degrees model would move for every pixel moved
       *
       * @return Ratio between degrees model rotated and pixels moved
       */
      virtual double getMouseMoveRatio() const 
      {
         return mouseMoveRatio_;
      }

      /**
       * Set how many degrees model would move for every pixel moved
       *
       * @param ratio ratio between degrees model rotated and pixels moved
       */
      virtual void setMouseMoveRatio(const double ratio) 
      {
         mouseMoveRatio_ = ratio;
      }   
      
      /**
       * Get how many degrees FOV would change for every pixel moved
       *
       * @return Ratio between degrees fov changed and pixels moved
       */
      virtual double getMouseFOVRatio() const 
      {
         return mouseFOVRatio_;
      }
      
      /**
       * Set how many degrees model would move for every pixel moved
       *
       * @param ratio ratio between degrees model rotated and pixels moved
       */
      virtual void setMouseFOVRatio(double ratio) 
      {
         mouseFOVRatio_ = ratio;
      }   
      
      /**
       * Set minimum value allowed for FOV
       *
       * @param minFOV min allowed FOV
       */
      virtual void setMinFOV(double minFOV)
      {
         minFOV_ = minFOV;
      }

      /**
       * Get minimum value allowed for FOV
       *
       * @return min allowed FOV
       */
      virtual double getMinFOV() const
      {
         return minFOV_;
      }

      /**
       * Set maximum value allowed for FOV
       *
       * @param maxFOV max allowed FOV
       */
      virtual void setMaxFOV(double maxFOV)
      {
         maxFOV_ = maxFOV;
      }

      /**
       * Get maximum value allowed for FOV
       *
       * @return max allowed FOV
       */
      virtual double getMaxFOV() const
      {
         return maxFOV_;
      }
      
private:
         
     /**
       * Last X position of the mouse
       */
     int lastX_;
      
      /**
       * Last Y position of the mouse
       */
      int lastY_;
      
      /**
         * Azimuth of the point from which we are looking from
       */
      double azimuth_;
      
      /**
         * Height of the point from which we are looking from
       */
      double height_;
      
      /**
         * Distance of the point from which we are looking from
       */
      double distance_;
      
      /**
       * Center where we are looking
       */
      double centerX_, centerY_, centerZ_;
      
      /**
       * Ratio between number of pixels moved and model move
       */
      double mouseMoveRatio_;

      /**
       * Ratio between number of pixels moved and FOV change
       */
      double mouseFOVRatio_;
      
      /**
         * Min FOV value allowed on this controller
       */
      double minFOV_;
      
      /**
         * Max FOV allowed on this controller
       */
      double maxFOV_;
   };
   
} // namespace

#endif
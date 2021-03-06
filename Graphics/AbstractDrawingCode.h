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

#ifndef ABSRACT_DRAWING_CODE_H_
#define ABSRACT_DRAWING_CODE_H_

#include "AbstractModel.h"
#include "Statistics.h"

namespace hdsim {

   /**
    * Interface defining contract for drawing code
    */
   class AbstractDrawingCode
   {
   public:

      /**
       * Constructor
       */
      AbstractDrawingCode();
      
      /**
       * Destructor
       */
      virtual ~AbstractDrawingCode();
      
      /**
       * Set bounds in which drawing should occure
       *
       * @param width Width of the window
       * @param height Height of the window
       */
      virtual void setBounds(int width, int height) = 0;
      
      /**
       * Initialize drawing mode with any specific parameters
       */
      virtual void initializeDrawingMode() = 0;
      
      /**
       * Perform actual drawing
       *
       * @param m model to draw
       */
      virtual void draw(const AbstractModel *m) = 0;
      
      /**
       * Set aspect ratio
       *
       * @param ratio aspect ratio
       */
      virtual void setAspectRatio(double ratio) = 0;
      
      /**
       * Get aspect ratio
       *
       * @return Aspect ratio
       */
      virtual double getAspectRatio() const = 0;
      
      /**
       * Set field of view
       *
       * @param fov Field of view
       */
      virtual void setFOV(double fov) = 0;
      
      /**
       * Get field of view
       *
       * @return Field of view
       */
      virtual double getFOV() const = 0;
      
      /**
       * Get statistics related to frame rendering for all frames rendered so far
       *
       * @return Statistics related to frame rendering
       */
      virtual Statistics getAllFrameRenderingStatistics() const = 0;
      
      /**
       * Set what are initial rotation angles for the view. Angles are given in degrees, and view is rotated around center of the view for that many degrees before drawing.
       *
       * @param x rotate this many degrees around X axis
       * @param y rotate this many degrees around Y axis
       * @param z rotate this many degrees around Z axis    
       */
      virtual void setRotationAngles(double x, double y, double z);
      
      /**
       * Get initial rotation angle around X axis 
       *
       * @return Rotation angle around X axis
       */
      virtual double getRotationAngleX() const;
      
      /**
       * Get initial rotation angle around Y axis 
       *
       * @return Rotation angle around Y axis
       */
      virtual double getRotationAngleY() const;
      
      /**
       * Get initial rotation angle around Z axis 
       *
       * @return Rotation angle around Z axis
       */
      virtual double getRotationAngleZ() const;
      
   private:
      
      /**
       * Initial rotation around X axis
       */
      double rotationX_;
      
      /**
         * Initial rotation around Y axis
       */
      double rotationY_;
      
      /**
         * Initial rotation around Z axis
       */
      double rotationZ_;  
   };

   
   /**
    * Normalize angle so that it is within [0, 360)
    *
    * @param angle Angle to normalize
    * 
    * @return Normalized angle
    */
   double normalizeAngle(double angle);

} // namespace

#endif

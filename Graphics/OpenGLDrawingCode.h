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

#ifndef OPENGL_DRAWING_CODE_H_ 
#define OPENGL_DRAWING_CODE_H_

#ifndef __APPLE__
#error "This was tested only on OS X"
#endif

#include "AbstractDrawingCode.h"


namespace hdsim {

   class GPUGeometryModel;

   /**
    * This is OpenGL based renderer. See DrawingCode for the contract of its methods
    */
   class OpenGLDrawingCode : public AbstractDrawingCode
   {
   public:
      /**
       * Constructor
       */
      OpenGLDrawingCode();
      
      /**
       * Desctructor
       */
      virtual ~OpenGLDrawingCode();
      
      // Redefined methods
      virtual void setBounds(int width, int height);
      virtual void initializeDrawingMode();
      virtual void draw(const AbstractModel *m);
      virtual void setAspectRatio(double);
      virtual double getAspectRatio() const;
      virtual void setFOV(double);
      virtual double getFOV() const;
      virtual Statistics getAllFrameRenderingStatistics() const;
            
   protected:

      /**
       * Swap back and front buffer
       */
      virtual void swapBuffers();

   private:
      // Make sure that implicits are not defined
      OpenGLDrawingCode(const OpenGLDrawingCode &);
      OpenGLDrawingCode &operator=(const OpenGLDrawingCode &);

      /**
       * Setup projection and coordinate system
       */
      void setupProjectionAndCoordinateSystem();
      
      /**
       * Init frame buffer
       */
      
      /**
       * Current Field of View in 3D view
       */
      double fov_;
      
      /**
       * Current aspect ration of the window
       */
      double aspectRatio_;   
      
      /**
       * Was model drawn at least once
       */
      bool wasModelDrawn_;
            
      /**
       * Statistics related to frame rendering calculation
       */
      Statistics allFrameRenderingStatistics_;
   };
   
} // namespace

#endif

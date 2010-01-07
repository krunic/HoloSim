/*
 *  drawingCode.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 4/14/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef OPENGL_DRAWING_CODE_H_ 
#define OPENGL_DRAWING_CODE_H_

#ifndef __APPLE__
#error "This was tested only on OS X"
#endif

#include "AbstractDrawingCode.h"

namespace hdsim {

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
       * Current Field of View in 3D view
       */
      double fov_;
      
      /**
       * Current aspect ration of the window
       */
      double aspectRatio_;   
   };
   
} // namespace

#endif

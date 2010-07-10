/*
 *  OpenGLDrawingCode.cpp
 *  HoloSim
 *
 *  Create portability layer, so that we isolate actual drawing code from the GUI code
 *
 *  Created by Veljko Krunic on 4/14/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 */

#include "OpenGLDrawingCode.h"

#include <cstdio>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "GPUInterpolatedModel.h"
#include "SimpleDesignByContract.h"

using namespace hdsim;

static const double INITIAL_FOV = 45;
static const double INITIAL_ASPECT_RATIO = 1;

static const double MIN_Z = -40;
static const double MAX_Z = 10;

static const double BASE_SIZE = 4;
static const int ROD_COVERAGE_PERCENTAGE = 80;

// Offset to avoid Z buffer fighting
static const double Z_OFFSET = 0.01;

// Max rod height in comparasion with base
static const double MAX_ROD_HEIGHT = 10;

OpenGLDrawingCode::OpenGLDrawingCode() : AbstractDrawingCode(), aspectRatio_(0), fov_(0)
{
   fov_ = INITIAL_FOV;
   aspectRatio_ = INITIAL_ASPECT_RATIO;   
}

OpenGLDrawingCode::~OpenGLDrawingCode()
{
   
}

void OpenGLDrawingCode::setAspectRatio(double ratio)
{
   aspectRatio_ = ratio;
}

double OpenGLDrawingCode::getAspectRatio() const
{
   return aspectRatio_;
}

void OpenGLDrawingCode::setFOV(double fov)
{
   fov_ = fov;
}

double OpenGLDrawingCode::getFOV() const
{
   return fov_;
}

void OpenGLDrawingCode::initializeDrawingMode()
{
   GLfloat lightPosition[] = {0, 1, 3, 0.0};
   GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0}; 
   GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};

   char message[4096];
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   // Configure the view
   glEnable(GL_CULL_FACE);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
   
	glCullFace(GL_BACK);   
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
   
   glFrontFace(GL_CCW);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
   
   glShadeModel(GL_SMOOTH);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);

   glEnable(GL_LIGHTING);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);

   glEnable(GL_DEPTH_TEST);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
   
   // Add some ambient lighting.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
   
   // Initialize the light
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
   
   // Position it
   glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
   
   // and switch it on.
   glEnable(GL_LIGHT0);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
}

void OpenGLDrawingCode::setBounds(int width, int height)
{
   glViewport(0, 0, width, height);
   char message[4096];
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
}

void OpenGLDrawingCode::swapBuffers()
{
#ifdef __APPLE__
   glSwapAPPLE();
#else
   glSwapBuffers();
#endif
   char message[4096];
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
}

/**
 * Draw box of specified sizes
 * 
 * @param xSize size in X direction
 * @param ySize size in Y direction
 * @param zSize size in Z direction
 */
static void drawBox(double xSize, double ySize, double zSize)
{
   double halfX = xSize/2;
   double halfY = ySize/2;
   double halfZ = zSize/2;   
   
   glBegin(GL_QUADS);
   	// XY plane, closer
	   glNormal3f(0, 0, 1);
	   glVertex3d(halfX, halfY, halfZ);
	   glVertex3d(-halfX, halfY, halfZ);
	   glVertex3d(-halfX, -halfY, halfZ);
	   glVertex3d(halfX, -halfY, halfZ);   
      
      // XY plane, further
	   glNormal3f(0, 0, -1);
	   glVertex3d(halfX, halfY, -halfZ);
	   glVertex3d(halfX, -halfY, -halfZ);
	   glVertex3d(-halfX, -halfY, -halfZ);
	   glVertex3d(-halfX, halfY, -halfZ);   
      
      // XZ plane, top
	   glNormal3f(0, 1, 0);
	   glVertex3d(halfX, halfY, -halfZ);
	   glVertex3d(-halfX, halfY, -halfZ);
	   glVertex3d(-halfX, halfY, halfZ);
	   glVertex3d(halfX, halfY, halfZ);   

      // XZ plane, bottom
	   glNormal3f(0, -1, 0);
	   glVertex3d(halfX, -halfY, halfZ);
	   glVertex3d(-halfX, -halfY, halfZ);
	   glVertex3d(-halfX, -halfY, -halfZ);
	   glVertex3d(halfX, -halfY, -halfZ);   

      // YZ plane, left
	   glNormal3f(-1, 0, 0);
	   glVertex3d(-halfX, halfY, -halfZ);
	   glVertex3d(-halfX, -halfY, -halfZ);
	   glVertex3d(-halfX, -halfY, halfZ);
	   glVertex3d(-halfX, halfY, halfZ);   

      // YZ plane, right
	   glNormal3f(1, 0, 0);
	   glVertex3d(halfX, halfY, halfZ);
	   glVertex3d(halfX, -halfY, halfZ);
	   glVertex3d(halfX, -halfY, -halfZ);
	   glVertex3d(halfX, halfY, -halfZ);   
      
   glEnd();
}

/**
 * Draw box of specified sizes at specified position in space.
 * 
 * @param centerX x position of the center of the box
 * @param centerY y position of the center of the box
 * @param centerZ z position of the center of the box
 * @param xSize box size in X direction
 * @param ySize box size in Y direction
 * @param zSize size in Z direction
 */
static void drawBoxAt(double centerX, double centerY, double centerZ, double xSize, double ySize, double zSize)
{
   glPushMatrix();
	   glTranslated(-centerX, -centerY, -centerZ);
   	drawBox(xSize, ySize, zSize);
   glPopMatrix();
}

/**
 * Draw base of the model for the given size of the base
 *
 * @param baseSize
 */
void drawModelBase(double baseSize)
{
   glPushMatrix();
      // It is 1/4th of the size in Z direction, totally arbitrary. Upper face is at Z = -0.01
   	double baseHeight = baseSize/8;
      glTranslated(0, 0, baseHeight/2 - 0.01);
      drawBox(baseSize, baseSize, baseHeight);
   glPopMatrix();
}

/**
 * Draw box of specified sizes at specified position in matrix. Used for drawing rods.
 * 
 * @param boxSize Size of the box that is base of the model
 * @param numRods Number of rods in single direction (equal in X and Y directions)
 * @param boxOccupancy What portion of the box is occupied by the rods
 * @param indexX x position of the ceneter of the rod in XY coordinate system of the box
 * @param indexY y position of the ceneter of the rod in XY coordinate system of the box
 * @param rodZSize size of rod in Z direction
 */
static void drawRodAt(double boxSize, int numRods, int boxOccupancy, int indexX, int indexY, double rodZSize)
{
   // For occupancy of 100%, we have that rod size is:
   // boxSizeX/numRodsX
   // 
   // Center of the rod position in respect to base's upper left orgin is
	//   indexX * sizeX + sizeX/2
   //   indexY * sizeY + sizeY/2
   //
   // As big box is at centered at 0, 0, 0 that means that center is at
   //   indexX * sizeX + sizeX/2 - boxSizeX/2
   //   indexY * sizeY + sizeY/2 - boxSizeY/2
   //
   // For occupancy of boxOccupancy%, we have that rod size is:
   //   boxSizeX*boxOccupancy/(100*numRodsX)
   //   In all cases, center is in Yglobal = Z/2 
   
   double sizeX = boxSize/numRods;
   double sizeY = boxSize/numRods;
   
   double rodCenterX = indexX*sizeX + sizeX/2 - boxSize/2;
   double rodCenterY = indexY*sizeY + sizeY/2 - boxSize/2;   
   
   double rodSizeXY = boxSize*boxOccupancy / (100*numRods);
   
   // Now, draw rod in given center that is in Z from 0 to rodZSize (so his Z center is rodZSize/2)
   drawBoxAt(rodCenterX, rodCenterY, rodZSize/2 + Z_OFFSET, rodSizeXY, rodSizeXY, rodZSize);
}

//#define RICKS_PICTURE

/**
 * Clear screen
 */
static inline void clearScreen()
{
#ifdef RICKS_PICTURE   
   glClearColor(0.5, 0.5, 0.5, 1.0);
#else
   glClearColor(0, 0, 0, 1.0);
#endif
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   char message[4096];
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
}

void OpenGLDrawingCode::setupProjectionAndCoordinateSystem()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(getFOV(), getAspectRatio(), 0.001, MAX_Z - MIN_Z);
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt(// Eye position
             0, 0, MAX_Z,
             // Center position
             0, 0, 0, 
             // Up position
             0, 1, 0); 
   
   // Do initial rotation
   glRotated(getRotationAngleX(), 1, 0, 0);
   glRotated(getRotationAngleY(), 0, 1, 0);
   glRotated(getRotationAngleZ(), 0, 0, 1);
}

Statistics OpenGLDrawingCode::getMoxelCalculationStatistics() const
{
	return moxelCalculationStatistics_;   
}

Statistics OpenGLDrawingCode::getFrameRenderingStatistics() const
{
   return frameRenderingStatistics_;
}

void OpenGLDrawingCode::draw(const AbstractModel *m)
{
   // This service knows at the moment only how to draw interpolated models
   const GPUInterpolatedModel *model = dynamic_cast<const GPUInterpolatedModel*>(m);
   
   model->getAt(0, 0);
   
   CHECK(model, "Model must be of the type GPUInterpolatedModel");
   CHECK(model->getSizeX() == model->getSizeY(), "Model sizes must be equal in X and Y for this drawer to work");
   CHECK(model->getRenderedAreaMaxZ() >= 0, "Model must have at least some part above the ground plane");
   
   // Setup screen and projection
   initializeDrawingMode();   
   setupProjectionAndCoordinateSystem();
   clearScreen();
     
   // And draw models 
   // Set the properties of the material under ambient light
   float mat[4];
   char message[4096];

   // Set the properties of the material under ambient light
   mat[0] = 0.1;
   mat[1] = 0.1;
   mat[2] = 0.7;
   mat[3] = 1.0;
   glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
   
   // Set the properties of the material under diffuse light
   mat[0] = 0.2;
   mat[1] = 0.6;
   mat[2] = 0.1;
   mat[3] = 1.0;   
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);	   
   sprintf(message, "OpenGL returned an error %d", (int)glGetError());
   CHECK(glGetError() == GL_NO_ERROR, message);
     
   // Holodeck can't show negative Z offset but we would rescale that minZ iz Z=1. And we would rescale so 
   // that the Z coordinates are proportional to the X and Y axis
   double xRenderSize = model->getRenderedAreaMaxX() - model->getRenderedAreaMinX();
   double yRenderSize = model->getRenderedAreaMaxY() - model->getRenderedAreaMinY();
   
   double maxRodSize = BASE_SIZE * (model->getRenderedAreaMaxZ() - model->getRenderedAreaMinZ())/max(xRenderSize, yRenderSize);
   
   drawModelBase(BASE_SIZE);
   
   // Make sure that model is not precalculated at this point (otherwise, our measurement is not correct)
   CHECK(!model->isModelCalculated(), "Model must not be calculated at this point");
   
   // Force calculation. This is where moxel related operations would happen
   model->forceModelCalculation();
   
   // and draw all the rods
   for (int indexX = 0; indexX < model->getSizeX(); indexX++)
      for (int indexY = 0; indexY < model->getSizeY(); indexY++)
      {
         // Transform from [0, 1] in Z buffer to the maxZ coordinate
         
         //THIS IS A PROBLEM - A LOT OF DRAWING HAPPENED ON INITIALIZED BUFFER THAT GETS UNINITIALIZED. HOW TO FIX? E.G. ENSURE PRECALC?
         
         double zValue = maxRodSize*(1-model->getAt(indexX, indexY));
         drawRodAt(BASE_SIZE, model->getSizeX(), ROD_COVERAGE_PERCENTAGE, indexX, indexY, zValue);
      }
   
   swapBuffers();
}

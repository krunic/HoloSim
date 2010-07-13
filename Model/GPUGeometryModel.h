/*
 *  GPUGeometryModel.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/11/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef GPU_GEOMETRY_MODEL_H_
#define GPU_GEOMETRY_MODEL_H_

#include <vector>
#include <string>

#include "AbstractModel.h"
#include "MathHelper.h"
#include "TriangleByPointIndex.h"
#include "Point.h"
#include "SimpleDesignByContract.h"

namespace hdsim {
     
   /**
    * Name of the geometry model
    */
   static const char * const GPU_GEOMETRY_MODEL_NAME = "GPUGeometryModel";
   
   // Forward declaration to resolve circular dependency
   class GPUCalculationEngine;

   /**
    * GPU based checkboard model used for remembering rod position at the particular moment in time. It is fed 3D geometry (points, triangles) and then will calculate 
    * checkboard based on that geometry
    *
    * Some concept:
    * Bounds - bounding box of geometry
    * Size - size of the checkboard
    * RenderedArea - complete area represented by the board, exists only in the X and Y area
    */
   class GPUGeometryModel : public AbstractModel {
      
   public:
      
      /**
       * Default constructor, setting dimensions to 0, 0
       */
      GPUGeometryModel();
      
      /**
       * Constructor with arguments
       *
       * @param sizeX - Size of the board in X direction
       * @param sizeY - Size of the board in Y direction
       */
      GPUGeometryModel(int sizeX, int sizeY);
      
      /**
       * Copy constructor
       *
       * @param rhs Copy to take
       */
      GPUGeometryModel(const GPUGeometryModel &rhs);
      
      /**
       * Operator =
       *
       * @param rhs Copy to take
       */
      GPUGeometryModel & operator=(const GPUGeometryModel &rhs);
      
      /**
       * Destructor
       */
      virtual ~GPUGeometryModel();
      
      // Overriden methods
      virtual const char * getModelName() const 
      {
         return GPU_GEOMETRY_MODEL_NAME;
      }   
      
      virtual double getAt(int x, int y) const;
      virtual AbstractModel *cloneOrphan() const;

      /**
       * Set size in X direction
       *
       * @param sizeX Size in X direction
       */
      virtual void setSizeX(int sizeX) 
      {
		   changedSinceLastRecalc_ = true;         
         sizeX_ = sizeX;
      }

      /**
       * Set size in Y direction
       *
       * @param sizeY Size in Y direction
       */
      virtual void setSizeY(int sizeY) 
      {
		   changedSinceLastRecalc_ = true;	         
         sizeY_ = sizeY;
      }
      
      /**
       * Get size in X direction
       *
       * @return Size in X direction
       */
      virtual int getSizeX() const
      {
         return sizeX_;
      }
      
      /**
       * Get size in Y direction
       *
       * @return Size in Y direction
       */
      virtual int getSizeY() const
      {
         return sizeY_;
      }
      
      /**
       * Read model from file. File uses Collada format, but as Collada is complex format and we don't need all its features, only geometry extraction from Collada (without any transformation) 
       * is supported
       *
       * @param fileName File to read from
       *
       * @return Was read success
       */
      virtual bool readFromFile(const std::string &fileName);
      
      /**
       * Add new point to the model
       *
       * @param point Point to add
       */
      virtual void addPoint(const Point &point) 
      {
		   changedSinceLastRecalc_ = true;
         
         points_.push_back(point);
         
         // And recalculate bounds
         boundMinX_ = min(point.getX(), getBoundMinX());
         boundMinY_ = min(point.getY(), getBoundMinY());
         boundMinZ_ = min(point.getZ(), getBoundMinZ());
         boundMaxX_ = max(point.getX(), getBoundMaxX());
         boundMaxY_ = max(point.getY(), getBoundMaxY());
         boundMaxZ_ = max(point.getZ(), getBoundMaxZ());
      }
      
      /**
       * Get number of points
       *
       * @return Number of points
       */
      virtual int getNumPoints() const 
      {
         return points_.size();
      }
      
      /**
       * Get Point
       *
       * @param index Index of the point
       *
       * @return Point
       */
      virtual const Point &getPoint(int index) const 
      {
         return points_[index];
      }
      
      /**
       * Replace point at the index.
       * 
       * @param index Must be smaller then getNumPoints()
       * @param point New value of the point
       */
      virtual void replacePointAt(int index, const Point &point) 
      {
         CHECK(index >= 0  &&  index < getNumPoints(), "Index out of bound");
         
         changedSinceLastRecalc_ = true;
      	points_[index] = point;
      }

      /**
       * Add new trianble to the model
       *
       * @param triangle Triangle to add
       */
      virtual void addTriangle(const TriangleByPointIndexes &triangle) 
      {
		   changedSinceLastRecalc_ = true;         
         triangles_.push_back(triangle);
      }
      
      /**
       * Get number of triangles
       *
       * @return Number of triangles
       */
      virtual int getNumTriangles() const 
      {
         return triangles_.size();
      }
      
      /**
       * Get triangle
       *
       * @param index Index of the triangle
       *
       * @return Point
       */
      virtual const TriangleByPointIndexes &getTriangle(int index) const 
      {
         return triangles_[index];
      }
      
      /**
       * Replace triangles at the index
       * 
       * @param triangle New value of the triangle
       * @param index Must be smaller then getNumTriangles()
       */
      virtual void replaceTriangleAt(int index, const TriangleByPointIndexes &triangle) 
      {
		   changedSinceLastRecalc_ = true;
         
         CHECK(index >= 0  &&  index < getNumPoints(), "Index out of bound");
      	triangles_[index] = triangle;
      }
      
      /**
       * Set value of the timeslice
       * 
       * @param timeSlice
       */
      virtual void setTimeSlice(double timeSlice);
      
      /**
       * Get value of the timeslice
       * 
       * @return Value of the timeslice
       */
      virtual double getTimeSlice() const;

      /**
       * Initilize model to clean state, including forgetting dimensions
       */
      void initializeToCleanState();
      
      /**
       * Clear geometry from the model
       */
      virtual void clearGeometry();
      
		/**
       * Get minX of the bound
       */
      virtual double getBoundMinX() const
      {
         return boundMinX_;
      }

      /**
       * Get maxX of the bound
       */
      virtual double getBoundMaxX() const
      {
         return boundMaxX_;
      }

      /**
       * Get minY of the bound
       */
      virtual double getBoundMinY() const
      {
         return boundMinY_;
      }
      
		/**
       * Get maxY of the bound
       */
      virtual double getBoundMaxY() const
      {
         return boundMaxY_;
      }

      /**
       * Get minZ of the bound
       */
      virtual double getBoundMinZ() const
      {
         return boundMinZ_;
      }

      /**
       * Get maxZ of the bound
       */
      virtual double getBoundMaxZ() const
      {
         return boundMaxZ_;
      }

      /**
       * Get minX of the rendered area
       */
      virtual double getRenderedAreaMinX() const
      {
         return renderedAreaMinX_;
      }
      
      /**
       * Get maxX of the rendered area
       */
      virtual double getRenderedAreaMaxX() const
      {
         return renderedAreaMaxX_;
      }
      
      /**
       * Get minY of the rendered area
       */
      virtual double getRenderedAreaMinY() const
      {
         return renderedAreaMinY_;
      }
      
		/**
       * Get maxY of the bound
       */
      virtual double getRenderedAreaMaxY() const
      {
         return renderedAreaMaxY_;
      }
      
      /**
       * Get minZ of the rendered area
       */
      virtual double getRenderedAreaMinZ() const
      {
         return renderedAreaMinZ_;
      }
      
		/**
       * Get maxZ of the bound
       */
      virtual double getRenderedAreaMaxZ() const
      {
         return renderedAreaMaxZ_;
      }
      
      /**
       * Set rendered area bounds
       */
      virtual void setRenderedArea(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
      {
         // Check is it really needed to invalidate before you do it
		   bool boundsChanged = !areEqual(minX, getRenderedAreaMinX())  ||  !areEqual(maxX, getRenderedAreaMaxX())  ||
                              !areEqual(minY, getRenderedAreaMinY())  ||  !areEqual(maxY, getRenderedAreaMaxY())  ||
									   !areEqual(minZ, getRenderedAreaMinZ())  ||  !areEqual(maxZ, getRenderedAreaMaxZ());
         
         changedSinceLastRecalc_ = changedSinceLastRecalc_  ||  boundsChanged;
         
			// It is quite possible that if test would take more time due to the possible pipeline stall then all assignments, but we 
         // should measure before we optimize
         if (changedSinceLastRecalc_)
         {
            renderedAreaMinX_ = minX;
            renderedAreaMinY_ = minY;
            renderedAreaMaxX_ = maxX;
            renderedAreaMaxY_ = maxY;
            renderedAreaMinZ_ = minZ;
            renderedAreaMaxZ_ = maxZ;         
         }
      }
      
      /**
       * Get is model calculated or there are still calculations to perform that are pending
       *
       * @return Is model calculated
       */
      virtual bool isModelCalculated() const;
      
      /**
       * Perform all calculations on the model
       */
      virtual void forceModelCalculation() const;
      
      /**
       * Get the path to the shader source
       *
       * @return Path to shader source
       */
      virtual const char *getPathToShaderSource() const
      {
         return pathToShaderSource_.c_str();
      }
      
      /**
       * Set the path to the shader source
       *
       * @param path Path to the source
       */
      virtual void setPathToShaderSource(const char *path)
      {
         pathToShaderSource_ = path;
      }
      
      /**
       * Get the path to 1D texture
       *
       * @return Path to shader source
       */
      virtual const char *getPathTo1DTexture() const
      {
         return pathTo1DTexture_.c_str();
      }
      
      /**
       * Set the path to the 1D texture
       *
       * @param path Path to the source
       */
      virtual void setPathTo1DTexture(const char *path)
      {
         pathTo1DTexture_ = path;
      }
      
   private:
      
      /**
       * Bounds of the view frustum
       */
      double boundMinX_, boundMinY_, boundMinZ_, boundMaxX_, boundMaxY_, boundMaxZ_;

      /**
       * Rendered area 
       */
      double renderedAreaMinX_, renderedAreaMinY_, renderedAreaMaxX_, renderedAreaMaxY_, renderedAreaMinZ_, renderedAreaMaxZ_;
      
      /**
       * Associated calculation engine
       */
      GPUCalculationEngine *calculationEngine_;
      
      /**
       * Did we change after last recalc. Note that this variable is not considered part of the const of the object because it is related to the
       * caching, not to the model state (model state for cached and non-cached object is considered the same)
       */
      mutable bool changedSinceLastRecalc_;
      
      /**
       * Path to the shader source
       */
      std::string pathToShaderSource_;
      
      /**
       * Path to 1D texture
       */
      std::string pathTo1DTexture_;
      
      /**
       * Copy value from rhs to this object
       *
       * @param rhs Value to copy
       */
      void copyFrom(const GPUGeometryModel &rhs);
           
      // Friend with its operators
      friend bool operator==(const GPUGeometryModel &lhs, const GPUGeometryModel &rhs);
      friend bool operator!=(const GPUGeometryModel &lhs, const GPUGeometryModel &rhs);
      
      /**
       * Size in x direction
       */
      int sizeX_;
      
      /**
       * Size in y direction
       */
      int sizeY_;
      
      /**
       * Points
       */
      std::vector<Point> points_;
      
      /**
       * Triangles, defined by the point index
       */
      std::vector<TriangleByPointIndexes> triangles_;
   };
   
   /** 
    * Compare two models. Note that exact match is not required in Z values - it is enough for the Z values to be "close" for checkboards to be declared equal
    * 
    * @param lhs Left operand
    * @param rhs right operand
    * 
    * @return Are models equal (equal being that Z values are close enough that difference could be attributed to floating point error)
    */
   inline bool operator==(const GPUGeometryModel &lhs, const GPUGeometryModel &rhs)
   {
      if (lhs.points_.size() != rhs.points_.size()  ||  lhs.triangles_.size() != rhs.triangles_.size())
         return false;
      
      if (!areEqual(lhs.getRenderedAreaMinX(), rhs.getRenderedAreaMinX())  ||  !areEqual(lhs.getRenderedAreaMaxX(), rhs.getRenderedAreaMaxX()))
         return false;

      if (!areEqual(lhs.getRenderedAreaMinY(), rhs.getRenderedAreaMinY())  ||  !areEqual(lhs.getRenderedAreaMaxY(), rhs.getRenderedAreaMaxY()))
         return false;
      
      if (lhs.getSizeX() != rhs.getSizeX()  ||  lhs.getSizeY() != rhs.getSizeY())
         return false;
      
      for (int i = 0; i < lhs.points_.size(); i++)
         if (lhs.points_[i] != rhs.points_[i])
            return false;
   
      for (int i = 0; i < lhs.triangles_.size(); i++)
         if (lhs.triangles_[i] != rhs.triangles_[i])
            return false;
      
      return true;
   }
   
   /** 
    * Compare two models. Note that exact match is not required in Z values - it is enough for the Z values to be "close" for checkboards to be declared equal
    * 
    * @param lhs Left operand
    * @param rhs right operand
    * 
    * @return Are models different (equal being that Z values are close enough that difference could be attributed to floating point error)
    */
   inline bool operator!=(const GPUGeometryModel &lhs, const GPUGeometryModel &rhs)
   {
      return !(lhs == rhs);
   }
   
} // namespace

#endif

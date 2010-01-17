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

#include "AbstractModel.h"
#include "MathHelper.h"
#include "TriangleByPointIndex.h"
#include "Point.h"

namespace hdsim {
   
   /**
    * Name of the checkboard model
    */
   static const char * const GPU_GEOMETRY_MODEL_NAME = "GPUGeometryModel";
   
   /**
    * Checkboard used for remembering rod position at the particular moment in time. It is fed 3D geometry (points, triangles) and then will calculate checkboard based on that geometry
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
      virtual const char * getModelName() const {
         return GPU_GEOMETRY_MODEL_NAME;
      }   
      
      virtual double getAt(int x, int y) const;
      virtual AbstractModel *cloneOrphan() const;

      /**
       * Set size in X direction
       *
       * @param sizeX Size in X direction
       */
      virtual int setSizeX(int sizeX) 
      {
         sizeX_ = sizeX;
      }

      /**
       * Set size in Y direction
       *
       * @param sizeY Size in Y direction
       */
      virtual int setSizeY(int sizeY) 
      {
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
       * @param fp File to read from
       *
       * @return Was read success
       */
      virtual bool readFromFile(FILE *fp);
      
      /**
       * Write model to file. File uses Collada format, but as Collada is complex format and we don't need all its features, only geometry (without any transformation) is supported
       *
       * @param fp File to write to
       *
       * @return Was write success
       */
      virtual bool saveToFile(FILE *fp) const;
      
      /**
       * Add new point to the model
       *
       * @param point Point to add
       */
      virtual void addPoint(const Point &point) {
         points_.push_back(point);
      }
      
      /**
       * Get number of points
       *
       * @return Number of points
       */
      virtual int getNumPoints() const {
         return points_.size();
      }
      
      /**
       * Get Point
       *
       * @param index Index of the point
       *
       * @return Point
       */
      virtual const Point &getPoint(int index) const {
         return points_[index];
      }
      
      /**
       * Replace point at the index.
       * 
       * @param Point New value of the point
       * @param index Must be smaller then getNumPoints()
       */
      virtual void replacePointAt(int index, const Point &point) {
         assert(index >= 0  &&  index < getNumPoints());
      	points_[index] = point;
      }

      /**
       * Add new trianble to the model
       *
       * @param triangle Triangle to add
       */
      virtual void addTriangle(const TriangleByPointIndexes &triangle) {
         triangles_.push_back(triangle);
      }
      
      /**
       * Get number of triangles
       *
       * @return Number of triangles
       */
      virtual int getNumTriangles() const {
         return triangles_.size();
      }
      
      /**
       * Get triangle
       *
       * @param index Index of the triangle
       *
       * @return Point
       */
      virtual const TriangleByPointIndexes &getTriangle(int index) const {
         return triangles_[index];
      }
      
      /**
       * Replace triangles at the index
       * 
       * @param triangle New value of the triangle
       * @param index Must be smaller then getNumTriangles()
       */
      virtual void replaceTriangleAt(int index, const TriangleByPointIndexes &triangle) {
         assert(index >= 0  &&  index < getNumPoints());
      	triangles_[index] = triangle;
      }

      /**
       * Initilize model to clean state, including forgetting dimensions
       */
      void initializeToCleanState();
      
      /**
       * Clear geometry from the model
       */
      virtual void clearGeometry();
      
   private:
      
      /**
       * Copy value from rhs to this object
       *
       * @param rhs Value to copy
       */
      void copyFrom(const GPUGeometryModel &rhs);
      
      /**
       * Calculate linear index position in 1D array corresponding to given indexes
       *
       * Example - in 2D array with sizeX == 1 and sizeY == 2, linear index corresponding to [0, 1] is 1
       *
       * @param x X position
       * @param y Y position    
       *
       * @return Linear index in 1D array
       */
      int get1DIndex(int x, int y) const
      {
         return y * sizeX_ + x;
      }
      
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

/*
 * Copyright © Veljko Krunic 2007-2010. All rights reserved.
 *
 * Modification of the original code and examples on collada.org site for Collada DOM, which license is below:
 * 
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the MIT Open Source License, for details please see license.txt or the website
 * http://www.opensource.org/licenses/mit-license.php
 *
 */ 

#include <list>
#include <vector>
#include <iostream>

#include <dae.h>
#include <dom/domSource.h>
#include <dom/domTriangles.h>
#include <dom/domPolygons.h>
#include <dom/domGeometry.h>
#include <dom/domFloat_array.h>
#include <dom/domNode.h>
#include <dom/domCOLLADA.h>
#include <dom/domP.h>
#include <dom/domInt_array.h>

#include "Collada.h"
#include "GPUGeometryModel.h"

using namespace std;
using namespace hdsim;

static bool loadPointsToModel(domMesh *meshElement, GPUGeometryModel &loadToThisModel)
{
   // Now, load all source points in the points array
   
   // Copy the vertices we are going to use into myGeometry. To keep things simple,
   // we will assume there is only one domSource and domFloatArray in the domMesh,
   // that it is the array of vertices, and that it is in X, Y, Z format. A real
   // app would find the vertices by starting with domPolygons and following
   // the links through the domInput, domVertices, domSource, domFloat_array,
   // and domTechnique.
   if (meshElement->getSource_array().getCount() != 1)
   {
      return false;
   }

   domSource *source = meshElement->getSource_array()[0];
   
   if (source->getFloat_array()->getCount() != 1)
   {
		return false;      
   }
   
   domFloat_array &floatArray = source->getFloat_array()[0];
   
   // Assume there are 3 values per vertex with a stride of 3.
   int numPoints = floatArray.getCount()/3;
   
   // Copy the vertices into my structure one-by-one
   int indexInArray = 0;
   for (int indexPoint = 0; indexPoint < numPoints; indexPoint++ ) 
   {
      double x = floatArray.getValue()[indexInArray++];
      double y = floatArray.getValue()[indexInArray++];         
      double z = floatArray.getValue()[indexInArray++];
      
      loadToThisModel.addPoint(createPoint(x, y, z));
   }         
   
   return true;
}

static bool loadTrianglesToModel(domMesh *meshElement, GPUGeometryModel &loadToThisModel) 
{
	int numTriangleGroups = meshElement->getTriangles_array().getCount();
	for (int indexTriangleGroup = 0; indexTriangleGroup < numTriangleGroups; indexTriangleGroup++)
	{
   	domTriangles *triangles = meshElement->getTriangles_array().get(indexTriangleGroup);
   
	   int numTriangles = triangles->getCount();
   	for (int indexTriangles = 0; indexTriangles < numTriangles; indexTriangles++)
	   {
   	   // For each triangle, we need to find its vertexes
	      domP *triangleIndexes = triangles->getP();
      
   	   // Now we need to add triangle vertexes:
      	int numTriangleIndexes = triangleIndexes->getValue().getCount();
	      for (int indexInTriangleIndexes = 0; indexInTriangleIndexes < numTriangleIndexes; indexInTriangleIndexes += 3)
   	   {
      	   int index1 = triangleIndexes->getValue()[indexInTriangleIndexes];
         	int index2 = triangleIndexes->getValue()[indexInTriangleIndexes + 1];
	         int index3 = triangleIndexes->getValue()[indexInTriangleIndexes + 2];
         
   	      loadToThisModel.addTriangle(createTriangle(index1, index2, index3));
	       }
      }
   }
                                        
   return true;                                        
}

bool loadCollada(const char *name, GPUGeometryModel &loadToThisModel) 
{
   DAE dae;
   daeElement *root = dae.open(name);
   if (!root) 
   {
      return false;
   }
   
   int numGeometries = dae.getDatabase()->typeLookup<domGeometry>().size();
   for (int indexGeometries = 0; indexGeometries < numGeometries; indexGeometries++)
   {
      domGeometry *geometry = dae.getDatabase()->typeLookup<domGeometry>().at(indexGeometries);
      if (!geometry)
      {
         return false;
      }
      
      domMesh *meshElement = daeSafeCast<domMesh>(root->getDescendant("mesh"));
      if (!meshElement)
      {
         return false;
      }

      if (!loadPointsToModel(meshElement, loadToThisModel))
      {
         return false;
      }
                 
      // We support only triangles
      if (meshElement->getPolygons_array().getCount() != 0)
      {
         return false;
      }
      
      if (!loadTrianglesToModel(meshElement, loadToThisModel))
      {
         return false;
      }
   }
   
   return true;
}

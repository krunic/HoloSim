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
#include <string>

#include <dae.h>   
#include <dom/domSource.h> 
#include <dom/domVertices.h>
#include <dom/domInputLocal.h> 
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

/**
 * Read ID of the source array in vertecies. That is an array holding POSITION vertexes.
 *
 * @param vertecies DOM element holding vertecies
 * @param idOfSourceArray (OUT) value of the id
 *
 * @return Was ID found
 */
static bool readSourceArrayID(const domVertices *vertecies, string &idOfSourceArray)
{
   int numArrays = vertecies->getInput_array().getCount();
   
   // Iterate through arrays and find the one that is POSITION. There is probably faster way to do this, but I am 
   // really fedup with COLLADA DOM documentation and this is not time critical
   for (int indexInputs = 0; indexInputs < numArrays; indexInputs++)
   {
      domInputLocal *current = vertecies->getInput_array()[indexInputs];
      if (!strcmp(current->getSemantic(), "POSITION")) 
      {
      	// This is the position array, holding our vertex array id. Find the id of source
         idOfSourceArray = current->getAttribute("source");
         return true;
      }
   }

   return false;
}

/**
 * Get domSource element corresponding to idOfSourceArray
 *
 * @param meshElement Mesh element from the COLLADA file
 * @param idOfSourceArray id that given domElement should have
 *
 * @return Element or NULL if nothing is found
 */
static domSource *getDomSource(const domMesh *meshElement, const string &idOfSourceArray)
{
   int numSources = meshElement->getSource_array().getCount();  
   
   for (int indexSources = 0; indexSources < numSources; indexSources++)
   {
      string elementID = meshElement->getSource_array()[indexSources]->getID();
      
      // source ID is actually missing leading hash, so we need to add it
      elementID = "#" + elementID;
      
      if (elementID == idOfSourceArray)
      {
         return meshElement->getSource_array()[indexSources];
         
      }
   }
   
   return 0;
}

/**
 * Loads all points from the mesh element to the model
 *
 * @param meshElement Mesh element from the COLLADA file
 * @param loadToThisModel (OUT) model with points from the mesh added. Undefined in the case of error
 *
 * @return Was model succesfully loaded
 */
static bool loadPointsToModel(const domMesh *meshElement, GPUGeometryModel &loadToThisModel)
{
   // Now, load all source points in the points array
   domVertices *vertecies = meshElement->getVertices();
   
   string idOfSourceArray;
   
   if (!readSourceArrayID(vertecies, idOfSourceArray))
   {
      return false;
   }
   
   // Copy the vertices we are going to use into myGeometry. To keep things simple,
   // we will assume there is only one domSource and domFloatArray in the domMesh,
   // that it is the array of vertices, and that it is in X, Y, Z format. A real
   // app would find the vertices by starting with domPolygons and following
   // the links through the domInput, domVertices, domSource, domFloat_array,
   // and domTechnique.
   
   domSource *source = getDomSource(meshElement, idOfSourceArray);
	if (!source)
   {
      return false;
   }
   
   domFloat_array *floatArray = source->getFloat_array();
   
   // Assume there are 3 values per vertex with a stride of 3.
   int numPoints = floatArray->getCount()/3;
   
   // Copy the vertices into my structure one-by-one
   int indexInArray = 0;
   for (int indexPoint = 0; indexPoint < numPoints; indexPoint++ ) 
   {
      double x = floatArray->getValue()[indexInArray++];
      double y = floatArray->getValue()[indexInArray++];         
      double z = floatArray->getValue()[indexInArray++];
      
      loadToThisModel.addPoint(createPoint(x, y, z));
   }         
   
   return true;
}

/**
 * Loads all triangles from the mesh element to the model
 *
 * @param meshElement Mesh element from the COLLADA file
 * @param loadToThisModel (OUT) model with triangles from the mesh added. Undefined in the case of error
 *
 * @return Was model succesfully loaded
 */
static bool loadTrianglesToModel(domMesh *meshElement, GPUGeometryModel &loadToThisModel) 
{
	int numTriangleGroups = meshElement->getTriangles_array().getCount();
	for (int indexTriangleGroup = 0; indexTriangleGroup < numTriangleGroups; indexTriangleGroup++)
	{
   	domTriangles *triangles = meshElement->getTriangles_array().get(indexTriangleGroup);
      int numTriangles = triangles->getCount();
   	domP *triangleIndexes = triangles->getP();
      
      // For each triangle, we need to find its vertexes
      int indexInTriangleIndexes = 0;
	   for (int indexTriangles = 0; indexTriangles < numTriangles; indexTriangles++)
	   {
         int index1 = triangleIndexes->getValue()[indexInTriangleIndexes];
         int index2 = triangleIndexes->getValue()[indexInTriangleIndexes + 1];
         int index3 = triangleIndexes->getValue()[indexInTriangleIndexes + 2];
      
         loadToThisModel.addTriangle(createTriangle(index1, index2, index3));
      }
   }
                                        
   return true;                                        
}

/**
 * Loads geometry from the file to the model
 *
 * @param meshElement Mesh element from the COLLADA file
 * @param loadToThisModel (OUT) model with triangles from the mesh added. Will have cleared geometry and preserved dimensions in the case of error
 *
 * @return Was model succesfully loaded
 */
bool loadCollada(const char *name, GPUGeometryModel &loadToThisModel) 
{
   loadToThisModel.clearGeometry();
   
   DAE dae;
   daeElement *root = dae.open(name);
   if (!root) 
   {
      loadToThisModel.clearGeometry();
      return false;
   }
   
   int numGeometries = dae.getDatabase()->typeLookup<domGeometry>().size();
   for (int indexGeometries = 0; indexGeometries < numGeometries; indexGeometries++)
   {
      domGeometry *geometry = dae.getDatabase()->typeLookup<domGeometry>().at(indexGeometries);
      if (!geometry)
      {
         loadToThisModel.clearGeometry();
         return false;
      }
      
      domMesh *meshElement = daeSafeCast<domMesh>(root->getDescendant("mesh"));
      if (!meshElement)
      {
         loadToThisModel.clearGeometry();
         return false;
      }

      if (!loadPointsToModel(meshElement, loadToThisModel))
      {
         loadToThisModel.clearGeometry();
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

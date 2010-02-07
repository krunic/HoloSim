/*
 *  ColladaTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/16/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include "ColladaTest.h"

#include <cppunit/extensions/HelperMacros.h>

#include "Collada.h"
#include "GPUGeometryModel.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(ColladaTest);

ColladaTest::ColladaTest() 
{

}

ColladaTest::~ColladaTest()
{

}

void ColladaTest::setUp()
{

}

void ColladaTest::tearDown()
{

}

void ColladaTest::testLoadQuad()
{
   GPUGeometryModel model;
   
   CPPUNIT_ASSERT_MESSAGE("Can't load simple collada file", loadCollada("TestQuad.dae", model));

   // This is triangulated quad, so 4 points and 2 triangles
   CPPUNIT_ASSERT_MESSAGE("Wrong number of points", model.getNumPoints() == 4);
   CPPUNIT_ASSERT_MESSAGE("Wrong number of triangles", model.getNumTriangles() == 2);
   
   // This are point indexes from the file for triangles - check them  <p>0 1 2 1 0 3</p>
   TriangleByPointIndexes triangle = model.getTriangle(0);
   int index1 = triangle.getIndex1();
   int index2 = triangle.getIndex2();
   int index3 = triangle.getIndex3();
   
   CPPUNIT_ASSERT_MESSAGE("Triangle 0 has wrong indexes", index1 == 0  &&  index2 == 1  &&  index3 == 2);
   
   triangle = model.getTriangle(1);
   index1 = triangle.getIndex1();
   index2 = triangle.getIndex2();
   index3 = triangle.getIndex3();
   
   CPPUNIT_ASSERT_MESSAGE("Triangle 1 has wrong indexes", index1 == 1  &&  index2 == 0  &&  index3 == 3);
   
   
   // This are point coordinated from the file for points - check them 
   // 104.5792364 121.9253048 0.0000000 61.1417364 67.5503048 0.0000000 61.1417364 121.9253048 0.0000000 104.5792364 67.5503048 0.0000000
   
   Point point = model.getPoint(0);
   CPPUNIT_ASSERT_MESSAGE("Point 0 has wrong coordinates", areEqual(point.getX(), 104.5792364)  &&  areEqual(point.getY(), 121.9253048)  &&  areEqual(point.getZ(), 0.0000000));
   
   point = model.getPoint(1);
   CPPUNIT_ASSERT_MESSAGE("Point 1 has wrong coordinates", areEqual(point.getX(), 61.1417364)  &&  areEqual(point.getY(), 67.5503048)  &&  areEqual(point.getZ(), 0.0000000));

   point = model.getPoint(2);
   CPPUNIT_ASSERT_MESSAGE("Point 2 has wrong coordinates", areEqual(point.getX(), 61.1417364)  &&  areEqual(point.getY(), 121.9253048)  &&  areEqual(point.getZ(), 0.0000000));

   point = model.getPoint(3);
   CPPUNIT_ASSERT_MESSAGE("Point 3 has wrong coordinates", areEqual(point.getX(), 104.5792364)  &&  areEqual(point.getY(), 67.5503048)  &&  areEqual(point.getZ(), 0.0000000));
}

void ColladaTest::testLoadChair()
{
   GPUGeometryModel model;
   
   CPPUNIT_ASSERT_MESSAGE("Can't load moderately complex collada file", loadCollada("Chair.dae", model));
}

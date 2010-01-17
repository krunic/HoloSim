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
}

void ColladaTest::testLoadChair()
{
   GPUGeometryModel model;
   
   CPPUNIT_ASSERT_MESSAGE("Can't load moderately complex collada file", loadCollada("Chair.dae", model));
}

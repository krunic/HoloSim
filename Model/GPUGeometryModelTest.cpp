/*
 *  GPUGeometryModelTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/12/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include "GPUGeometryModelTest.h"
#include "GPUGeometryModel.h"
#include "MathHelper.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(GPUGeometryModelTest);

GPUGeometryModelTest::GPUGeometryModelTest()
{

}

GPUGeometryModelTest::~GPUGeometryModelTest()
{
   
}

void GPUGeometryModelTest::setUp()
{
   
}

void GPUGeometryModelTest::tearDown()
{
   
}

void GPUGeometryModelTest::testModelName()
{
   GPUGeometryModel gpuGeometryModel;
   CPPUNIT_ASSERT_MESSAGE("Name of the checkboard is not correct", !strcmp(gpuGeometryModel.getModelName(), GPU_GEOMETRY_MODEL_NAME));
}

void GPUGeometryModelTest::testCopy()
{
   GPUGeometryModel original(1, 1);
   GPUGeometryModel constructorCopy(original);
   
	original.addPoint(createPoint(0, 0, 0));
	original.addPoint(createPoint(0, 0, 0));
	original.addPoint(createPoint(0, 0, 0));
   original.addTriangle(createTriangle(0, 1, 2));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", constructorCopy.getNumPoints() == 0  &&  constructorCopy.getNumTriangles() == 0);
   
   // Clear original
   original.initializeToCleanState();
   
   // Same story for the operator =
   constructorCopy = original; 
	original.addPoint(createPoint(0, 0, 0));
	original.addPoint(createPoint(0, 0, 0));
	original.addPoint(createPoint(0, 0, 0));
   original.addTriangle(createTriangle(0, 1, 2));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", constructorCopy.getNumPoints() == 0  &&  constructorCopy.getNumTriangles() == 0);
}

void GPUGeometryModelTest::testEqual()
{
   GPUGeometryModel lhs(1, 1);
   GPUGeometryModel rhs(1, 1);

   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly", lhs == rhs);
   
   lhs.addPoint(createPoint(0, 0, 0));
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly after adding point", lhs != rhs);
   
   rhs.addPoint(createPoint(0, 0, 0));
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly comparing points", lhs == rhs);
   
   // We would need two more points to add triangle
   lhs.addPoint(createPoint(0, 1, 0));
   lhs.addPoint(createPoint(1, 0, 0));
   
   rhs.addPoint(createPoint(0, 1, 0));
   rhs.addPoint(createPoint(1, 0, 0));
   
   // Now, add triangle - should make lhs and rhs different as one doesn't have triangles
   lhs.addTriangle(createTriangle(0, 1, 2));  
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly after adding triangles", lhs != rhs);
   
   // And one to rhs for comparasion purposes
   rhs.addTriangle(createTriangle(0, 1, 2));
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly after adding triangles", lhs == rhs);  
}

void GPUGeometryModelTest::testDiferentDimsAreNonEqual()
{
   GPUGeometryModel lhs(1, 1);
   GPUGeometryModel rhs(2, 1);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != not working correctly", lhs != rhs);
}

void GPUGeometryModelTest::testLoadModel()
{
   GPUGeometryModel testFixture;
   
   FILE *fp = fopen("singleQuad.gpuGeometryModel", "r");
   CPPUNIT_ASSERT_MESSAGE("Opening file failed", fp);
   CPPUNIT_ASSERT_MESSAGE("Model load failed", testFixture.readFromFile(fp));
   
   // 30 times 30 model of quad with 4 points and 2 triangles
   CPPUNIT_ASSERT_MESSAGE("Invalid dimensions read", testFixture.getSizeX() == 30  &&  testFixture.getSizeY() == 30);
   CPPUNIT_ASSERT_MESSAGE("Invalid number of points in the model", testFixture.getNumPoints() == 4);
   CPPUNIT_ASSERT_MESSAGE("Invalid number of triangles in the model", testFixture.getNumTriangles() == 2);
}

void GPUGeometryModelTest::testModelCleaning()
{
   GPUGeometryModel testFixture(1, 1);
   
   // Add 3 points and triangle to the model
   testFixture.addPoint(createPoint(1, 1, 0));
   testFixture.addPoint(createPoint(1, 2, 0));
   testFixture.addPoint(createPoint(2, 3, 0));
   
   testFixture.addTriangle(createTriangle(0, 1, 2));
   
   CPPUNIT_ASSERT_MESSAGE("Points not correctly added", testFixture.getNumPoints() == 3);
   CPPUNIT_ASSERT_MESSAGE("Triangles not correctly added", testFixture.getNumTriangles() == 1);
   
   testFixture.initializeToCleanState();
   
   // Now points should all be empty
   CPPUNIT_ASSERT_MESSAGE("Points not correctly cleared", !testFixture.getNumPoints());
   CPPUNIT_ASSERT_MESSAGE("Triangles not correctly added", !testFixture.getNumTriangles());
   
   // And geometry should be cleaned too (sizes reset to 0)
   CPPUNIT_ASSERT_MESSAGE("Geometry should be cleaned too", !testFixture.getSizeX()  &&  !testFixture.getSizeY());
}

void GPUGeometryModelTest::testGeometryCleaning()
{
   int initialSizeX = 1;
   int initialSizeY = 2;
   GPUGeometryModel testFixture(1, 2);
   
   // Add 3 points and triangle to the model
   testFixture.addPoint(createPoint(1, 1, 0));
   testFixture.addPoint(createPoint(1, 2, 0));
   testFixture.addPoint(createPoint(2, 3, 0));
   
   testFixture.addTriangle(createTriangle(0, 1, 2));
   
   CPPUNIT_ASSERT_MESSAGE("Points not correctly added", testFixture.getNumPoints() == 3);
   CPPUNIT_ASSERT_MESSAGE("Triangles not correctly added", testFixture.getNumTriangles() == 1);
   
   testFixture.clearGeometry();
   
   // Now points should all be empty
   CPPUNIT_ASSERT_MESSAGE("Points not correctly cleared", !testFixture.getNumPoints());
   CPPUNIT_ASSERT_MESSAGE("Triangles not correctly added", !testFixture.getNumTriangles());
   
   // And geometry should be intact
   CPPUNIT_ASSERT_MESSAGE("Geometry in X should not be changed", testFixture.getSizeX() == initialSizeX);
   CPPUNIT_ASSERT_MESSAGE("Geometry in Y should not be changed", testFixture.getSizeY() == initialSizeY);
}

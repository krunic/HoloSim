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

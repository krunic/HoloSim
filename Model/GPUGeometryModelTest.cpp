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
   const double valueToSet = 0;
   const double valueToChange = valueToSet + 1;
   
   GPUGeometryModel original(1, 1);
   GPUGeometryModel constructorCopy(original);
   
	original.setAt(0, 0, valueToSet);
   constructorCopy.setAt(0, 0, valueToChange);
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", original.getAt(0, 0) == valueToSet);
   
   // Same story for the operator =
   constructorCopy = original; 
	original.setAt(0, 0, valueToSet);
   constructorCopy.setAt(0, 0, valueToChange);
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", original.getAt(0, 0) == valueToSet);
}

void GPUGeometryModelTest::testEqual()
{
   const double valueToSet = 0;
   
   GPUGeometryModel lhs(1, 1);
   lhs.setAt(0, 0, valueToSet);
   
   GPUGeometryModel rhs(1, 1);
   rhs.setAt(0, 0, valueToSet);
   
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly", lhs == rhs);
}

void GPUGeometryModelTest::testNonEqual()
{
   const double valueToSet = 0;
   
   GPUGeometryModel lhs(1, 1);
   lhs.setAt(0, 0, valueToSet);
   
   GPUGeometryModel rhs(1, 1);
   rhs.setAt(0, 0, valueToSet + 1);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != not working correctly", lhs != rhs);
}

void GPUGeometryModelTest::testDiferentDimsAreNonEqual()
{
   GPUGeometryModel lhs(1, 1);
   GPUGeometryModel rhs(2, 1);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != not working correctly", lhs != rhs);
}

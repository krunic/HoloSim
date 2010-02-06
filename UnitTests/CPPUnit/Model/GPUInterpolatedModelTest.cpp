/*
 *  GPUInterpolatedModelTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 2/4/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include "GPUInterpolatedModel.h"
#include "GPUInterpolatedModelTest.h"
#include "MathHelper.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(GPUInterpolatedModelTest);

GPUInterpolatedModelTest::GPUInterpolatedModelTest()
{
   
}

GPUInterpolatedModelTest::~GPUInterpolatedModelTest()
{
   
}

void GPUInterpolatedModelTest::setUp()
{
   
}

void GPUInterpolatedModelTest::tearDown()
{
   
}

void GPUInterpolatedModelTest::testModelName()
{
   GPUInterpolatedModel model;
   
   CPPUNIT_ASSERT_MESSAGE("Name of the interpolated model is not correct", !strcmp(model.getModelName(), GPU_INTERPOLATED_MODEL_NAME));
}

void GPUInterpolatedModelTest::testCopyConstructor()
{
   const double valueToSet = 0;
   const double valueToChange = valueToSet + 1;
   
   GPUInterpolatedModel original;
   original.setTimeSlice(valueToSet);
   
	GPUInterpolatedModel constructorCopy(original);
   
   original.setTimeSlice(valueToChange);
   CPPUNIT_ASSERT_MESSAGE("Setter failed", areEqual(constructorCopy.getTimeSlice(), valueToSet));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", areEqual(original.getTimeSlice(), valueToChange));
}

void GPUInterpolatedModelTest::testOperatorEqual()
{
   const double valueToSet = 0;
   const double valueToChange = valueToSet + 1;
   
   GPUInterpolatedModel original;
   original.setTimeSlice(valueToSet);
   
	GPUInterpolatedModel constructorCopy = original;
   
   original.setTimeSlice(valueToChange);
   CPPUNIT_ASSERT_MESSAGE("Setter failed", areEqual(constructorCopy.getTimeSlice(), valueToSet));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", areEqual(original.getTimeSlice(), valueToChange));
}

void GPUInterpolatedModelTest::testSerialization()
{
	FILE *fp = fopen("singleQuad.GPUHoloSim", "r");
   CPPUNIT_ASSERT_MESSAGE("Test files not correctly setup and copied", fp);
   
	GPUInterpolatedModel testFixture;
   
   CPPUNIT_ASSERT_MESSAGE("Reading from file failed", testFixture.readFromFile(fp));
   fclose(fp);
   
   // And assert that values in the file were correctly read. This coordinates are from file
   CPPUNIT_ASSERT_MESSAGE("Incorrect dimensions in X", testFixture.getSizeX() == 30);
   CPPUNIT_ASSERT_MESSAGE("Incorrect dimensions in Y", testFixture.getSizeX() == 30);
   CPPUNIT_ASSERT_MESSAGE("Timeslice was not correctly read", areEqual(testFixture.getTimeSlice(), 0.31415927));
}

void GPUInterpolatedModelTest::testReadFromGarbageFile()
{
   const char *testFileName = "testCheckBoardReadGarbage.tmp";
   
   // Print some garbage in the file
   FILE *fp = fopen(testFileName, "w");
   fprintf(fp, "Some garbage\n");
   fclose(fp);
   
   // Now read from the file and compare models
   GPUInterpolatedModel toLoad;
   fp = fopen(testFileName, "r");
   CPPUNIT_ASSERT_MESSAGE("Reading from file should fail but it didn't", !toLoad.readFromFile(fp));
   fclose(fp);
	
   unlink(testFileName);
}

void GPUInterpolatedModelTest::testEqual()
{
   GPUInterpolatedModel lhs;
   lhs.setTimeSlice((GPUInterpolatedModel::MIN_TIME_SLICE + GPUInterpolatedModel::MAX_TIME_SLICE)/2);
   
   GPUInterpolatedModel rhs;
   rhs.setTimeSlice((GPUInterpolatedModel::MIN_TIME_SLICE + GPUInterpolatedModel::MAX_TIME_SLICE)/2);
   
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly", lhs == rhs);
}

void GPUInterpolatedModelTest::testNonEqual()
{
   GPUInterpolatedModel lhs;
   lhs.setTimeSlice(GPUInterpolatedModel::MIN_TIME_SLICE);
   
   GPUInterpolatedModel rhs;
   rhs.setTimeSlice((GPUInterpolatedModel::MIN_TIME_SLICE + GPUInterpolatedModel::MAX_TIME_SLICE)/2);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != not working correctly", lhs != rhs);
}

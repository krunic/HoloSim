/*
 * HoloSim, visualization and control of the moxel based environment.
 *
 * Copyright (C) 2010 Veljko Krunic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cppunit/extensions/HelperMacros.h>

#include "CheckBoard.h"
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
   original.setMoxelThreshold(valueToChange);
   
   CPPUNIT_ASSERT_MESSAGE("Time setter failed", areEqual(constructorCopy.getTimeSlice(), valueToSet));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened on time", areEqual(original.getTimeSlice(), valueToChange));
   
   CPPUNIT_ASSERT_MESSAGE("Moxel setter failed", areEqual(constructorCopy.getMoxelThreshold(), valueToSet));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened on moxels", areEqual(original.getMoxelThreshold(), valueToChange));
}

void GPUInterpolatedModelTest::testOperatorEqual()
{
   const double valueToSet = 0;
   const double valueToChange = valueToSet + 1;
   
   GPUInterpolatedModel original;
   original.setTimeSlice(valueToSet);
   original.setMoxelThreshold(valueToSet);
   
	GPUInterpolatedModel operatorEqualCopy;
   operatorEqualCopy = original;
   
   original.setTimeSlice(valueToChange);
   original.setMoxelThreshold(valueToChange);
   
   CPPUNIT_ASSERT_MESSAGE("Time setter failed", areEqual(operatorEqualCopy.getTimeSlice(), valueToSet));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened on time", areEqual(original.getTimeSlice(), valueToChange));

   CPPUNIT_ASSERT_MESSAGE("Moxel setter failed", areEqual(operatorEqualCopy.getMoxelThreshold(), valueToSet));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened on moxels", areEqual(original.getMoxelThreshold(), valueToChange));
}

void GPUInterpolatedModelTest::testSerialization()
{
	GPUInterpolatedModel testFixture;
   
   CPPUNIT_ASSERT_MESSAGE("Reading from file failed", testFixture.readFromFile("singleQuad.GPUHoloSim"));
   
   // And assert that values in the file were correctly read. This coordinates are from file
   CPPUNIT_ASSERT_MESSAGE("Incorrect dimensions in X", testFixture.getSizeX() == 30);
   CPPUNIT_ASSERT_MESSAGE("Incorrect dimensions in Y", testFixture.getSizeY() == 30);
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
   CPPUNIT_ASSERT_MESSAGE("Reading from file should fail but it didn't", !toLoad.readFromFile(testFileName));
	
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

void GPUInterpolatedModelTest::testGetFileNameInSameDir()
{
   CPPUNIT_ASSERT_MESSAGE("Simple file substitution fails", getFileNameInSameDirAsOriginalFile("abc.xxx", "de.xxx") == "de.xxx");
   CPPUNIT_ASSERT_MESSAGE("Complex file substitution fails", getFileNameInSameDirAsOriginalFile("/tmp/abc.xxx", "de.xxx") == "/tmp/de.xxx"); 
   CPPUNIT_ASSERT_MESSAGE("URI filename substitution fails", getFileNameInSameDirAsOriginalFile("file://abc.xxx", "de.xxx") == "file://de.xxx"); 
   
   CPPUNIT_ASSERT_MESSAGE("Empty filenames not working correctly", getFileNameInSameDirAsOriginalFile("", "") == "");
   CPPUNIT_ASSERT_MESSAGE("Empty filenames not correctly added", getFileNameInSameDirAsOriginalFile("/abc/d.e", "") == "/abc/");
   CPPUNIT_ASSERT_MESSAGE("Filename were not correctly added to empty filename", getFileNameInSameDirAsOriginalFile("", "addthis") == "addthis");
   
   CPPUNIT_ASSERT_MESSAGE("Relative filenames not working correctly", getFileNameInSameDirAsOriginalFile("/abc/de.xxx", "../fg.xxx") == "/abc/../fg.xxx");
}

void GPUInterpolatedModelTest::testPrecalculationStatus()
{   
   GPUInterpolatedModel testFixture;
   CPPUNIT_ASSERT_MESSAGE("Model shouldn't be calculated at this point", !testFixture.isModelCalculated());
}

void GPUInterpolatedModelTest::testOptimizeDrawing()
{
   GPUInterpolatedModel testFixture;
   CPPUNIT_ASSERT_MESSAGE("Model shouldn't request optimized drawing unless explicitely setup in that way", !testFixture.getOptimizeDrawing());
   
   CPPUNIT_ASSERT_MESSAGE("Reading from file failed", testFixture.readFromFile("singleQuad.GPUHoloSim"));
   
   // And assert that values in the file were correctly read. This coordinates are from file
   CPPUNIT_ASSERT_MESSAGE("Incorrect dimensions in X", testFixture.getSizeX() == 30);
   CPPUNIT_ASSERT_MESSAGE("Incorrect dimensions in Y", testFixture.getSizeY() == 30);
   
   testFixture.setRenderedArea(-10, -10, -10, 10, 10, 10);
   
   testFixture.setOptimizeDrawing(true);
   testFixture.setMoxelThreshold(100);
   
   // Lets do max 100, which should translate in 10 on each side
   CPPUNIT_ASSERT_MESSAGE("Incorrect optimized drawing recommendation in X", testFixture.getSizeX() == 10);
   CPPUNIT_ASSERT_MESSAGE("Incorrect optimized drawing recommendation in Y", testFixture.getSizeY() == 10);   
}


void GPUInterpolatedModelTest::testDecimation()
{
   static const int SIZE_X = 1023;
   static const int SIZE_Y = 1021;
   
   // Expected offset of Z buffer. We would set quad at the half of the viewing frustum so Z buffer should be 1/2
   static const double Z_BUFFER_VALUE = 0.5;
   
   // Set a quad that covers the whole area
   GPUGeometryModel gpuGeometryModel(SIZE_X, SIZE_Y);
   
   static const double QUAD_SIZE = 1;
   static const double Z_OFFSET = 0;
   
   gpuGeometryModel.setRenderedArea(-QUAD_SIZE/2, -QUAD_SIZE/2, -QUAD_SIZE/2, QUAD_SIZE/2, QUAD_SIZE/2, QUAD_SIZE/2);
   
   gpuGeometryModel.addPoint(createPoint(-QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
   gpuGeometryModel.addPoint(createPoint(-QUAD_SIZE, QUAD_SIZE, Z_OFFSET));
   gpuGeometryModel.addPoint(createPoint(QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
   gpuGeometryModel.addPoint(createPoint(QUAD_SIZE, QUAD_SIZE, Z_OFFSET));
   
   gpuGeometryModel.addTriangle(createTriangle(0, 1, 3));
   gpuGeometryModel.addTriangle(createTriangle(0, 2, 3));
   
   for (int indexY = 0; indexY < SIZE_Y; indexY++)
      for (int indexX = 0; indexX < SIZE_X; indexX++)
      {
         double value = gpuGeometryModel.getAt(indexX, indexY);
         if (!areEqualInLowPrecision(value, Z_BUFFER_VALUE))
         {           
            stringstream message;
            message << "Error at the coordinates X = " << indexX << " Y = " << indexY << " got " << value << " instead of " << Z_BUFFER_VALUE;
            CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), false);
         }
      }
   
   // Perform decimation
   static const int DECIMATED_SIZE_X = 99;
   static const int DECIMATED_SIZE_Y = 99;
   
   double *decimated = GPUInterpolatedModel::getDecimatedModelAdopt(&gpuGeometryModel, DECIMATED_SIZE_X, DECIMATED_SIZE_Y);
   CPPUNIT_ASSERT_MESSAGE("Result of decimation is NULL!", decimated);
   
   for (int indexY = 0; indexY < DECIMATED_SIZE_Y; indexY++)
      for (int indexX = 0; indexX < DECIMATED_SIZE_X; indexX++)
      {
         double value = decimated[indexY * DECIMATED_SIZE_Y + indexX];
         
         if (!areEqualInLowPrecision(value, Z_BUFFER_VALUE))
         {           
            stringstream message;
            message << "Decimation error at the coordinates X = " << indexX << " Y = " << indexY << " got " << value << " instead of " << Z_BUFFER_VALUE;
            CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), false);
         }
      }
   
   delete [] decimated;
   
}

void GPUInterpolatedModelTest::testIdentityDecimation()
{
   // Set a quad that covers the whole area
   CheckBoard checkBoard(3, 3);
   
   static const double Z_OFFSET = 1;
   
   // Make middle four items in checkboard offset, and all other 0
   checkBoard.setAt(1, 1, Z_OFFSET);
   
   double *decimated = GPUInterpolatedModel::getDecimatedModelAdopt(&checkBoard, 3, 3);
   CPPUNIT_ASSERT_MESSAGE("Result of decimation is NULL!", decimated);
   
   for (int indexY = 0; indexY < 3; indexY++)
      for (int indexX = 0; indexX < 3; indexX++)
      {
         double value = decimated[indexY * 3 + indexX];
         
         // Only middle point (1, 1) should be > 0
			double expectedValue = (indexX == 1  &&  indexY == 1) ? Z_OFFSET : 0;
         stringstream message;
         message << "Decimation error at the coordinates X = " << indexX << " Y = " << indexY << " got " << value << " instead of " << expectedValue;
         
		   CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), areEqualInLowPrecision(value, expectedValue));
      }
}

void GPUInterpolatedModelTest::testNoShiftsAfterDecimation()
{
   // Set a quad that covers the whole area
   CheckBoard checkBoard(6, 6);
   
   static const double Z_OFFSET = 1;
   
   // Make middle four items in checkboard offset, and all other 0
   checkBoard.setAt(2, 2, Z_OFFSET);
   checkBoard.setAt(2, 3, Z_OFFSET);   
   checkBoard.setAt(3, 2, Z_OFFSET);   
   checkBoard.setAt(3, 3, Z_OFFSET);   
   
   double *decimated = GPUInterpolatedModel::getDecimatedModelAdopt(&checkBoard, 3, 3);
   CPPUNIT_ASSERT_MESSAGE("Result of decimation is NULL!", decimated);
   
   for (int indexY = 0; indexY < 3; indexY++)
      for (int indexX = 0; indexX < 3; indexX++)
      {
         double value = decimated[indexY * 3 + indexX];
         
         // Only middle point (1, 1) should be > 0
			double expectedValue = (indexX == 1  &&  indexY == 1) ? Z_OFFSET : 0;
         stringstream message;
         message << "Decimation error at the coordinates X = " << indexX << " Y = " << indexY << " got " << value << " instead of " << expectedValue;
         
		   CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), areEqualInLowPrecision(value, expectedValue));
      }
}



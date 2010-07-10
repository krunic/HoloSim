/*
 *  GPUGeometryModelTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/12/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <sstream>

#include "GPUGeometryModelTest.h"
#include "GPUGeometryModel.h"
#include "MathHelper.h"

using namespace hdsim;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(GPUGeometryModelTest);

static void writeDepthBufferToCSVFile(const char *fileName, const GPUGeometryModel &model)
{
   // Write debug output
   ofstream myfile;
   myfile.open(fileName);
   
   for (int indexY = 0; indexY < model.getSizeY(); indexY++)
   {
      for (int indexX = 0; indexX < model.getSizeX(); indexX++)
      {
         myfile << model.getAt(indexX, indexY);
         if (indexX < model.getSizeX() - 1)
            myfile << ",";
      }
      
      myfile << endl;
   }
   
   myfile.close();  
}

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
   
   // We are not running as bundle in content of unit test, so don't use bundled file
	gpuGeometryModel.setUseBundledShaders(false);
   
   CPPUNIT_ASSERT_MESSAGE("Name of the checkboard is not correct", !strcmp(gpuGeometryModel.getModelName(), GPU_GEOMETRY_MODEL_NAME));
}

void GPUGeometryModelTest::testCopyConstructor()
{
   const int SIZE_X = 1; 
   const int SIZE_Y = 2;
   GPUGeometryModel original(SIZE_X, SIZE_Y);

   // Test copy of the rendered area, too
   const double MIN_X = 0; 
   const double MIN_Y = -1;
   const double MAX_X = 13324;
   const double MAX_Y = -145;
   const double MIN_Z = -324;
   const double MAX_Z = 32.23;
   original.setRenderedArea(MIN_X, MIN_Y, MIN_Z, MAX_X, MAX_Y, MAX_Z);
   
   GPUGeometryModel constructorCopy(original);
   
   CPPUNIT_ASSERT_MESSAGE("Operator == doesn't work correctly", constructorCopy == original);

   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in min X", areEqual(constructorCopy.getRenderedAreaMinX(), MIN_X));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in max X", areEqual(constructorCopy.getRenderedAreaMaxX(), MAX_X));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in min Y", areEqual(constructorCopy.getRenderedAreaMinY(), MIN_Y));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in max Y", areEqual(constructorCopy.getRenderedAreaMaxY(), MAX_Y));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in min Z", areEqual(constructorCopy.getRenderedAreaMinZ(), MIN_Z));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in max Z", areEqual(constructorCopy.getRenderedAreaMaxZ(), MAX_Z));
   
   // Check that bounds are correctly copied
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in min X", areEqual(original.getBoundMinX(), constructorCopy.getBoundMinX()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in max X", areEqual(original.getBoundMaxX(), constructorCopy.getBoundMaxX()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in min Y", areEqual(original.getBoundMinY(), constructorCopy.getBoundMinY()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in max Y", areEqual(original.getBoundMaxY(), constructorCopy.getBoundMaxY()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in min Z", areEqual(original.getBoundMinZ(), constructorCopy.getBoundMinZ()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in max Z", areEqual(original.getBoundMaxZ(), constructorCopy.getBoundMaxZ()));
   
   // Check dimensions correctly coppied
   CPPUNIT_ASSERT_MESSAGE("Dimensions not correctly copied", constructorCopy.getSizeX() == SIZE_X  &&  constructorCopy.getSizeY() == SIZE_Y);
   
	original.addPoint(createPoint(0, 0, 0));
	original.addPoint(createPoint(0, 0, 0));
	original.addPoint(createPoint(0, 0, 0));
   original.addTriangle(createTriangle(0, 1, 2));
   
   CPPUNIT_ASSERT_MESSAGE("Operator != doesn't work correctly", constructorCopy != original);
   
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", constructorCopy.getNumPoints() == 0  &&  constructorCopy.getNumTriangles() == 0);
}

void GPUGeometryModelTest::testOperatorEqual()
{
   const int SIZE_X = 1; 
   const int SIZE_Y = 2;
   GPUGeometryModel original(SIZE_X, SIZE_Y);
   
   // We are not running as bundle in content of unit test, so don't use bundled file
	original.setUseBundledShaders(false);
   
   // Test copy of the rendered area, too
   const double MIN_X = 0; 
   const double MIN_Y = -1;
   const double MAX_X = 13324;
   const double MAX_Y = -145;
   const double MIN_Z = -23;
   const double MAX_Z = 3435.9;

   original.setRenderedArea(MIN_X, MIN_Y, MIN_Z, MAX_X, MAX_Y, MAX_Z);
   original.setSizeX(SIZE_X);
   original.setSizeY(SIZE_Y);
   
   GPUGeometryModel operatorEqualCopy;
   operatorEqualCopy = original; 
   
   CPPUNIT_ASSERT_MESSAGE("Operator == doesn't work correctly", operatorEqualCopy == original);
   
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in min X for operator =", areEqual(operatorEqualCopy.getRenderedAreaMinX(), MIN_X));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in max X for operator =", areEqual(operatorEqualCopy.getRenderedAreaMaxX(), MAX_X));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in min Y for operator =", areEqual(operatorEqualCopy.getRenderedAreaMinY(), MIN_Y));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in max Y for operator =", areEqual(operatorEqualCopy.getRenderedAreaMaxY(), MAX_Y));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in min Z for operator =", areEqual(operatorEqualCopy.getRenderedAreaMinZ(), MIN_Z));
   CPPUNIT_ASSERT_MESSAGE("Rendered area incorrectly copied in max Z for operator =", areEqual(operatorEqualCopy.getRenderedAreaMaxZ(), MAX_Z));
   
   // Check that bounds are correctly copied
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in min X", areEqual(original.getBoundMinX(), operatorEqualCopy.getBoundMinX()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in max X", areEqual(original.getBoundMaxX(), operatorEqualCopy.getBoundMaxX()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in min Y", areEqual(original.getBoundMinY(), operatorEqualCopy.getBoundMinY()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in max Y", areEqual(original.getBoundMaxY(), operatorEqualCopy.getBoundMaxY()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in min Y", areEqual(original.getBoundMinZ(), operatorEqualCopy.getBoundMinZ()));
   CPPUNIT_ASSERT_MESSAGE("Bounds not correctly copied in max Y", areEqual(original.getBoundMaxZ(), operatorEqualCopy.getBoundMaxZ()));
   
   // Check dimensions correctly coppied
   CPPUNIT_ASSERT_MESSAGE("Dimensions not correctly copied for operator =", operatorEqualCopy.getSizeX() == SIZE_X  &&  operatorEqualCopy.getSizeY() == SIZE_Y);
   
	original.addPoint(createPoint(0, 0, 0));
	original.addPoint(createPoint(0, 0, 0));
	original.addPoint(createPoint(0, 0, 0));
   original.addTriangle(createTriangle(0, 1, 2));
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened for operator =", operatorEqualCopy.getNumPoints() == 0  &&  operatorEqualCopy.getNumTriangles() == 0);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != doesn't work correctly", operatorEqualCopy != original);
}

void GPUGeometryModelTest::testPrecalculationStatus()
{
   GPUGeometryModel testFixture;
   testFixture.setSizeX(100);
   testFixture.setSizeY(100);
   
   testFixture.addPoint(createPoint(0, 0, 0));
   CPPUNIT_ASSERT_MESSAGE("Model shouldn't be calculated at this point", !testFixture.isModelCalculated());
   
	testFixture.forceModelCalculation();
   CPPUNIT_ASSERT_MESSAGE("Model should be calculated at this point", testFixture.isModelCalculated());
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
   
   // We are not running as bundle in content of unit test, so don't use bundled file
   testFixture.setUseBundledShaders(false);

   CPPUNIT_ASSERT_MESSAGE("Model load failed", testFixture.readFromFile("singleQuad.gpuGeometryModel"));
   
   // 30 times 30 model of quad with 4 points and 2 triangles
   CPPUNIT_ASSERT_MESSAGE("Invalid dimensions read", testFixture.getSizeX() == 30  &&  testFixture.getSizeY() == 30);
   CPPUNIT_ASSERT_MESSAGE("Invalid number of points in the model", testFixture.getNumPoints() == 4);
   CPPUNIT_ASSERT_MESSAGE("Invalid number of triangles in the model", testFixture.getNumTriangles() == 2);
}

void GPUGeometryModelTest::testModelCleaning()
{
   GPUGeometryModel testFixture(1, 2);
   
   // We are not running as bundle in content of unit test, so don't use bundled file
   testFixture.setUseBundledShaders(false);
   
   testFixture.setRenderedArea(-1, -2, 3, 4, -5, 6);
   
	testFixture.addPoint(createPoint(0, 0, 0.5));
	testFixture.addPoint(createPoint(0, 1, 0));
	testFixture.addPoint(createPoint(1, 0, 0));
   testFixture.addTriangle(createTriangle(0, 1, 2));
   
   // Clear original
   testFixture.initializeToCleanState();
   
   // Geometry should be cleaned
	CPPUNIT_ASSERT_MESSAGE("Triangles not correctly cleaned", testFixture.getNumTriangles() == 0);
	CPPUNIT_ASSERT_MESSAGE("Points not correctly cleaned", testFixture.getNumPoints() == 0);
   
   // Rendered area - all should be 0 after cleaning
	CPPUNIT_ASSERT_MESSAGE("Rendered area should be 0 after cleaning in min X", areEqual(0, testFixture.getRenderedAreaMinX()));
	CPPUNIT_ASSERT_MESSAGE("Rendered area should be 0 after cleaning in max X", areEqual(0, testFixture.getRenderedAreaMaxX()));
	CPPUNIT_ASSERT_MESSAGE("Rendered area should be 0 after cleaning in min Y", areEqual(0, testFixture.getRenderedAreaMinY()));
	CPPUNIT_ASSERT_MESSAGE("Rendered area should be 0 after cleaning in max Y", areEqual(0, testFixture.getRenderedAreaMaxY()));
   
   // Bounds - should be 0 after cleaning
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in min X", areEqual(0, testFixture.getBoundMinX()));
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in max X", areEqual(0, testFixture.getBoundMaxX()));
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in min Y", areEqual(0, testFixture.getBoundMinY()));
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in max Y", areEqual(0, testFixture.getBoundMaxY()));
   
   // And so should rendered area
   CPPUNIT_ASSERT_MESSAGE("Rendered area should be 0 after cleaning in min X", areEqual(0, testFixture.getRenderedAreaMinX()));
   CPPUNIT_ASSERT_MESSAGE("Rendered area should be 0 after cleaning in max X", areEqual(0, testFixture.getRenderedAreaMaxX()));
   CPPUNIT_ASSERT_MESSAGE("Rendered area should be 0 after cleaning in min Y", areEqual(0, testFixture.getRenderedAreaMinY()));
   CPPUNIT_ASSERT_MESSAGE("Rendered area should be 0 after cleaning in max Y", areEqual(0, testFixture.getRenderedAreaMaxY()));
   
   // And finally, all elements should be reset after cleaning
   CPPUNIT_ASSERT_MESSAGE("Cleaning didn't reset sizes", testFixture.getSizeX() == 0  &&  testFixture.getSizeY() == 0);
}

void GPUGeometryModelTest::testGeometryCleaning()
{
   int initialSizeX = 1;
   int initialSizeY = 2;
   
   const double renderedMinX = 3908.3;
   const double renderedMinY = 49.43;
   const double renderedMaxX = 943422.12;
   const double renderedMaxY = 212321.434;
   const double renderedMinZ = 1233.43;
   const double renderedMaxZ = 4324.34;
   
   GPUGeometryModel testFixture(1, 2);
   
   // We are not running as bundle in content of unit test, so don't use bundled file
   testFixture.setUseBundledShaders(false);
   
   testFixture.setRenderedArea(renderedMinX, renderedMinY, renderedMinZ, renderedMaxX, renderedMaxY, renderedMaxZ);
   
   // Add 3 points and triangle to the model
   testFixture.addPoint(createPoint(1, 1, 0));
   testFixture.addPoint(createPoint(1, 2, 0));
   testFixture.addPoint(createPoint(2, 3, 0));
   
   testFixture.addTriangle(createTriangle(0, 1, 2));
   
   CPPUNIT_ASSERT_MESSAGE("Points not correctly added", testFixture.getNumPoints() == 3);
   CPPUNIT_ASSERT_MESSAGE("Triangles not correctly added", testFixture.getNumTriangles() == 1);
   
   testFixture.clearGeometry();
   
   // Bounds - should be 0 after cleaning
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in min X", areEqual(0, testFixture.getBoundMinX()));
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in max X", areEqual(0, testFixture.getBoundMaxX()));
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in min Y", areEqual(0, testFixture.getBoundMinY()));
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in max Y", areEqual(0, testFixture.getBoundMaxY()));
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in min Z", areEqual(0, testFixture.getBoundMinZ()));
   CPPUNIT_ASSERT_MESSAGE("Bounds should be 0 after cleaning in max Z", areEqual(0, testFixture.getBoundMaxZ()));
   
   // Now points should all be empty
   CPPUNIT_ASSERT_MESSAGE("Points not correctly cleared", !testFixture.getNumPoints());
   CPPUNIT_ASSERT_MESSAGE("Triangles not correctly added", !testFixture.getNumTriangles());
   
   // Sizes should be intact
   CPPUNIT_ASSERT_MESSAGE("Geometry in X should not be changed", testFixture.getSizeX() == initialSizeX);
   CPPUNIT_ASSERT_MESSAGE("Geometry in Y should not be changed", testFixture.getSizeY() == initialSizeY);
   
   // And so should the rendered area
   CPPUNIT_ASSERT_MESSAGE("Rendered area min X shouldn't be reset", areEqual(testFixture.getRenderedAreaMinX(), renderedMinX));
   CPPUNIT_ASSERT_MESSAGE("Rendered area max X shouldn't be reset", areEqual(testFixture.getRenderedAreaMaxX(), renderedMaxX));
   CPPUNIT_ASSERT_MESSAGE("Rendered area min Y shouldn't be reset", areEqual(testFixture.getRenderedAreaMinY(), renderedMinY));
   CPPUNIT_ASSERT_MESSAGE("Rendered area max Y shouldn't be reset", areEqual(testFixture.getRenderedAreaMaxY(), renderedMaxY));
}

void GPUGeometryModelTest::testBounds()
{
   Point upperRight = createPoint(1, 1, 1);
   Point lowerLeft = createPoint(-1, -1, -1);
   
   GPUGeometryModel testFixture;

   // We are not running as bundle in content of unit test, so don't use bundled file
   testFixture.setUseBundledShaders(false);
   
   CPPUNIT_ASSERT_MESSAGE("Model max x bounds are not correct after initialization", testFixture.getBoundMaxX() == 0);
   CPPUNIT_ASSERT_MESSAGE("Model min x bounds are not correct after initialization", testFixture.getBoundMinX() == 0);
   CPPUNIT_ASSERT_MESSAGE("Model max y bounds are not correct after initialization", testFixture.getBoundMaxY() == 0);
   CPPUNIT_ASSERT_MESSAGE("Model min y bounds are not correct after initialization", testFixture.getBoundMinY() == 0);
   CPPUNIT_ASSERT_MESSAGE("Model max z bounds are not correct after initialization", testFixture.getBoundMaxZ() == 0);
   CPPUNIT_ASSERT_MESSAGE("Model min z bounds are not correct after initialization", testFixture.getBoundMinZ() == 0);
   
	testFixture.addPoint(upperRight);
   testFixture.addPoint(lowerLeft);
   
   CPPUNIT_ASSERT_MESSAGE("Model max x bounds are not correct after initialization", areEqual(testFixture.getBoundMaxX(), 1));
   CPPUNIT_ASSERT_MESSAGE("Model min x bounds are not correct after initialization", areEqual(testFixture.getBoundMinX(), -1));
   CPPUNIT_ASSERT_MESSAGE("Model max y bounds are not correct after initialization", areEqual(testFixture.getBoundMaxY(), 1));
   CPPUNIT_ASSERT_MESSAGE("Model min y bounds are not correct after initialization", areEqual(testFixture.getBoundMinY(), -1));
   CPPUNIT_ASSERT_MESSAGE("Model max z bounds are not correct after initialization", areEqual(testFixture.getBoundMaxZ(), 1));
   CPPUNIT_ASSERT_MESSAGE("Model min z bounds are not correct after initialization", areEqual(testFixture.getBoundMinZ(), -1));
}

void GPUGeometryModelTest::testRenderArea()
{
   GPUGeometryModel testFixture;
   
   const double MIN_X = -1, MIN_Y = -2, MAX_X = 324.5, MAX_Y = 435.4, MIN_Z = 12.12, MAX_Z = 123.32;
   
   testFixture.setRenderedArea(MIN_X, MIN_Y, MIN_Z, MAX_X, MAX_Y, MAX_Z);
   
   // We are not running as bundle in content of unit test, so don't use bundled file
   testFixture.setUseBundledShaders(false);
   
   // Make sure that adding geometry doesn't reset the rendered area (it shouldn't because rendered area is not the same thing as bounds

   // Add 3 points and triangle to the model
   testFixture.addPoint(createPoint(1, 1, 0));
   testFixture.addPoint(createPoint(1, 2, 0));
   testFixture.addPoint(createPoint(2, 3, 0));
   
   testFixture.addTriangle(createTriangle(0, 1, 2));
   
   CPPUNIT_ASSERT_MESSAGE("Points not correctly added", testFixture.getNumPoints() == 3);
   CPPUNIT_ASSERT_MESSAGE("Triangles not correctly added", testFixture.getNumTriangles() == 1);
   
   // Check rendered area is stil at the same extent as before
   CPPUNIT_ASSERT_MESSAGE("Min X of rendered area wrong", areEqual(testFixture.getRenderedAreaMinX(), MIN_X));
   CPPUNIT_ASSERT_MESSAGE("Min Y of rendered area wrong", areEqual(testFixture.getRenderedAreaMinY(), MIN_Y));
   CPPUNIT_ASSERT_MESSAGE("Min Z of rendered area wrong", areEqual(testFixture.getRenderedAreaMinZ(), MIN_Z));
   CPPUNIT_ASSERT_MESSAGE("Max X of rendered area wrong", areEqual(testFixture.getRenderedAreaMaxX(), MAX_X));
   CPPUNIT_ASSERT_MESSAGE("Max Y of rendered area wrong", areEqual(testFixture.getRenderedAreaMaxY(), MAX_Y));
   CPPUNIT_ASSERT_MESSAGE("Max Z of rendered area wrong", areEqual(testFixture.getRenderedAreaMaxZ(), MAX_Z));
}

void GPUGeometryModelTest::testQuadCoveringWholeArea()
{
   const int SIZE_X = 32;
   const int SIZE_Y = 32;
    
   // Expected offset of Z buffer. We would set quad at the half of the viewing frustum so Z buffer should be 1/2
   const double Z_BUFFER_VALUE = 0.5;
     
   // Set a quad that covers the whole area
   GPUGeometryModel testFixture(SIZE_X, SIZE_Y);
   
   // We are not running as bundle in content of unit test, so don't use bundled file
   testFixture.setUseBundledShaders(false);
   
   const double QUAD_SIZE = 1;
   const double Z_OFFSET = 0;
   
   testFixture.setRenderedArea(-QUAD_SIZE/2, -QUAD_SIZE/2, -QUAD_SIZE/2, QUAD_SIZE/2, QUAD_SIZE/2, QUAD_SIZE/2);
   
   testFixture.addPoint(createPoint(-QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
   testFixture.addPoint(createPoint(-QUAD_SIZE, QUAD_SIZE, Z_OFFSET));
   testFixture.addPoint(createPoint(QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
   testFixture.addPoint(createPoint(QUAD_SIZE, QUAD_SIZE, Z_OFFSET));
   
   testFixture.addTriangle(createTriangle(0, 1, 3));
   testFixture.addTriangle(createTriangle(0, 2, 3));
   
   for (int indexY = 0; indexY < SIZE_Y; indexY++)
      for (int indexX = 0; indexX < SIZE_X; indexX++)
      {
         double value = testFixture.getAt(indexX, indexY);
         if (!areEqualInLowPrecision(value, Z_BUFFER_VALUE))
         {           
            stringstream message;
            message << "Error at the coordinates X = " << indexX << " Y = " << indexY << " got " << value << " instead of " << Z_BUFFER_VALUE;
            
            writeDepthBufferToCSVFile("testQuadCoveringWholeArea.csv", testFixture);
            
            CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), false);
         }
      }
}

void GPUGeometryModelTest::testCallingTwice()
{
   const int SIZE_X = 32;
   const int SIZE_Y = 32;
   
   // Expected offset of Z buffer. We would set quad at the half of the viewing frustum so Z buffer should be 1/2
   const double Z_BUFFER_VALUE = 0.5;
   
   // Set a quad that covers the whole area
   GPUGeometryModel testFixture(SIZE_X, SIZE_Y);
   
   // We are not running as bundle in content of unit test, so don't use bundled file
   testFixture.setUseBundledShaders(false);

   const double QUAD_SIZE = 1;
   const double Z_OFFSET = 0;
   
   testFixture.setRenderedArea(-QUAD_SIZE/2, -QUAD_SIZE/2, -QUAD_SIZE/2, QUAD_SIZE/2, QUAD_SIZE/2, QUAD_SIZE/2);
   
   testFixture.addPoint(createPoint(-QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
   testFixture.addPoint(createPoint(-QUAD_SIZE, QUAD_SIZE, Z_OFFSET));
   testFixture.addPoint(createPoint(QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
   testFixture.addPoint(createPoint(QUAD_SIZE, QUAD_SIZE, Z_OFFSET));
   
   testFixture.addTriangle(createTriangle(0, 1, 3));
   
   // Draw once, doesn't matter what you get
   testFixture.getAt(0, 0);
   
   // Add new triangle and redraw
   testFixture.addTriangle(createTriangle(0, 2, 3));
   
   for (int indexY = 0; indexY < SIZE_Y; indexY++)
      for (int indexX = 0; indexX < SIZE_X; indexX++)
      {
         double value = testFixture.getAt(indexX, indexY);
         if (!areEqualInLowPrecision(value, Z_BUFFER_VALUE))
         {           
            stringstream message;
            message << "Error at the coordinates X = " << indexX << " Y = " << indexY << " got " << value << " instead of " << Z_BUFFER_VALUE;
            
            writeDepthBufferToCSVFile("testCallingTwice.csv", testFixture);
            
            CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), false);
         }
      }
}


void GPUGeometryModelTest::testTriangleCoveringPartOfTheArea()
{
   const int SIZE_X = 32;
   const int SIZE_Y = 32;
   
   // Set a quad that covers the whole area
   GPUGeometryModel testFixture(SIZE_X, SIZE_Y);
   
   // We are not running as bundle in content of unit test, so don't use bundled file
   testFixture.setUseBundledShaders(false);
   
   const double Z_OFFSET = 0;

   // Expected offset of Z buffer. We would set quad at the half of the viewing frustum so Z buffer should be 1/2
	const double Z_BUFFER_VALUE = 0.5;
   const double QUAD_SIZE = 1;
   
   testFixture.setRenderedArea(-2*QUAD_SIZE, -2*QUAD_SIZE, -2*QUAD_SIZE, 2*QUAD_SIZE, 2*QUAD_SIZE, 2*QUAD_SIZE);
   testFixture.addPoint(createPoint(-QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
   testFixture.addPoint(createPoint(-QUAD_SIZE, QUAD_SIZE, Z_OFFSET));
   testFixture.addPoint(createPoint(QUAD_SIZE, -QUAD_SIZE, Z_OFFSET));
   
   testFixture.addTriangle(createTriangle(0, 1, 2));

   // We would do scanline - there should be two levels in each scanline, and second level should be continious
   // until exit. If we are outside of the polygon, z buffer value should be zero
   const double Z_INFINITY = 1;
   
   bool quadDetected = false;   

   for (int indexY = 0; indexY < SIZE_Y; indexY++)
   {
      bool scanlineEntered = false;
      bool scanlineExited = false;
      
      // We are scanning along X axis
      for (int indexX = 0; indexX < SIZE_X; indexX++)
      {
         double zValue = testFixture.getAt(indexX, indexY);
         
         stringstream message;
         message << "Error at the coordinates X = " << indexX << " Y = " << indexY << " for value " << zValue;
         
         CPPUNIT_ASSERT_MESSAGE(message.str().c_str(), areEqualInLowPrecision(zValue, Z_INFINITY)  ||  areEqualInLowPrecision(zValue, Z_BUFFER_VALUE));
         
         if (areEqualInLowPrecision(zValue, Z_BUFFER_VALUE))
         {
            CPPUNIT_ASSERT_MESSAGE("We can encounter Z_OFFSET only in quad or if we didn't entered quad before", !scanlineExited);
            scanlineEntered = true;
            quadDetected = true;
         } 
         else
         {
            // We have Z value equal to 0 if we are here 
            CPPUNIT_ASSERT_MESSAGE("Internal error in the test - zValue should be 1 here", areEqualInLowPrecision(zValue, Z_INFINITY));
            
            // If we were already inside the quad and we encountered infinity, we now should be outside of quad
            scanlineExited = scanlineEntered;
         }
      }
   }
   
   if (!quadDetected)
   {
      writeDepthBufferToCSVFile("testTriangleCoveringPartOfTheArea.csv", testFixture);
   }
   
   CPPUNIT_ASSERT_MESSAGE("Quad was never entered or detected", quadDetected);
}

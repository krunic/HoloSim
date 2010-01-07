/*
 *  CheckBoardTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/20/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include "CheckBoard.h"
#include "CheckBoardTest.h"
#include "MathHelper.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(CheckBoardTest);

CheckBoardTest::CheckBoardTest()
{
   
}

CheckBoardTest::~CheckBoardTest()
{
   
}

void CheckBoardTest::setUp()
{
   
}

void CheckBoardTest::tearDown()
{
   
}

void CheckBoardTest::testModelName()
{
   CheckBoard checkBoard;
   CPPUNIT_ASSERT_MESSAGE("Name of the checkboard is not correct", !strcmp(checkBoard.getModelName(), CHECKBOARD_MODEL_NAME));
}

void CheckBoardTest::testCopy()
{
   const double valueToSet = 0;
   const double valueToChange = valueToSet + 1;

   CheckBoard original(1, 1);
   CheckBoard constructorCopy(original);
 
	original.setAt(0, 0, valueToSet);
   constructorCopy.setAt(0, 0, valueToChange);
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", original.getAt(0, 0) == valueToSet);
                          
   // Same story for the operator =
   constructorCopy = original; 
	original.setAt(0, 0, valueToSet);
   constructorCopy.setAt(0, 0, valueToChange);
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", original.getAt(0, 0) == valueToSet);
}

void CheckBoardTest::testSerialization()
{
   const char *testFileName = "testCheckBoardSerialization.tmp";
   const int sizeX = 2;
   const int sizeY = 3;
   
   // Create larger board and set its values
   CheckBoard toSave(sizeX, sizeY);
   
   int value = 0;
   for (int indexX = 0; indexX < sizeX; indexX++)
      for (int indexY = 0; indexY < sizeY; indexY++)
         toSave.setAt(indexX, indexY, value++);

   FILE *fp = fopen(testFileName, "w");
   CPPUNIT_ASSERT_MESSAGE("Saving to file failed", toSave.saveToFile(fp));
   fclose(fp);
   
   // Now read from the file and compare models
   CheckBoard toLoad;
   fp = fopen(testFileName, "r");
   CPPUNIT_ASSERT_MESSAGE("Reading from file failed", toLoad.readFromFile(fp));
   fclose(fp);
   
   // Make sure that models are the same
   for (int indexX = 0; indexX < sizeX; indexX++)
      for (int indexY = 0; indexY < sizeY; indexY++)
      {
         char message[1024];
         sprintf(message, "File reading failed, values are different at x = %d and y = %d", indexX, indexY);
         CPPUNIT_ASSERT_MESSAGE(message, areEqual(toSave.getAt(indexX, indexY), toLoad.getAt(indexX, indexY)));
      }
         
   // And check that operator== gives same result
   CPPUNIT_ASSERT_MESSAGE("Models are the same but == thinks different", toSave == toLoad);
   
   unlink(testFileName);
}

void CheckBoardTest::testReadFromGarbageFile()
{
   const char *testFileName = "testCheckBoardReadGarbage.tmp";

   // Print some garbage in the file
   FILE *fp = fopen(testFileName, "w");
   fprintf(fp, "Some garbage\n");
   fclose(fp);
   
   // Now read from the file and compare models
   CheckBoard toLoad;
   fp = fopen(testFileName, "r");
   CPPUNIT_ASSERT_MESSAGE("Reading from file should fail but it didn't", !toLoad.readFromFile(fp));
   fclose(fp);
	
   unlink(testFileName);
}

void CheckBoardTest::testEqual()
{
   const double valueToSet = 0;
   
   CheckBoard lhs(1, 1);
   lhs.setAt(0, 0, valueToSet);
   
   CheckBoard rhs(1, 1);
   rhs.setAt(0, 0, valueToSet);
   
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly", lhs == rhs);
}

void CheckBoardTest::testNonEqual()
{
   const double valueToSet = 0;
   
   CheckBoard lhs(1, 1);
   lhs.setAt(0, 0, valueToSet);
   
   CheckBoard rhs(1, 1);
   rhs.setAt(0, 0, valueToSet + 1);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != not working correctly", lhs != rhs);
}

void CheckBoardTest::testDiferentDimsAreNonEqual()
{
   CheckBoard lhs(1, 1);
   CheckBoard rhs(2, 1);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != not working correctly", lhs != rhs);
}

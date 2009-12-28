/*
 *  InterpolatedModelTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/24/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#include "InterpolatedModelTest.h"

#include "InterpolatedModel.h"
#include "MathHelper.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(InterpolatedModelTest);

InterpolatedModelTest::InterpolatedModelTest()
{
   
}

InterpolatedModelTest::~InterpolatedModelTest()
{
   
}

void InterpolatedModelTest::setUp()
{
   
}

void InterpolatedModelTest::tearDown()
{
   
}

void InterpolatedModelTest::testModelName()
{
   InterpolatedModel model;
   
   CPPUNIT_ASSERT_MESSAGE("Name of the interpolated model is not correct", !strcmp(model.getModelName(), INTERPOLATED_MODEL_NAME));
}

void InterpolatedModelTest::testCopy()
{
   const double valueToSet = 0;
   const double valueToChange = valueToSet + 1;
   
   CheckBoard *begin = new CheckBoard(1, 1);
   begin->setAt(0, 0, valueToSet);
   
   CheckBoard *end = new CheckBoard(1, 1);
   end->setAt(0, 0, valueToSet);

   InterpolatedModel original;
   original.addModelAdopt(begin);
   original.addModelAdopt(end);

	InterpolatedModel constructorCopy(original);
   
   constructorCopy.setValueInModelWithIndex(0, 0, 0, valueToChange);
   constructorCopy.setValueInModelWithIndex(1, 0, 0, valueToChange);
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", original.getBeginModel()->getAt(0, 0) == valueToSet);
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", original.getEndModel()->getAt(0, 0) == valueToSet);
   
   // Same story for the operator =
   constructorCopy = original; 
   constructorCopy.setValueInModelWithIndex(0, 0, 0, valueToChange);
   constructorCopy.setValueInModelWithIndex(1, 0, 0, valueToChange);
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", original.getBeginModel()->getAt(0, 0) == valueToSet);
   CPPUNIT_ASSERT_MESSAGE("Aliasing happened", original.getEndModel()->getAt(0, 0) == valueToSet);
}

void InterpolatedModelTest::testInterpolation()
{
   const double initialValue = 0;
   const double endValue = 1;
   
   CheckBoard *begin = new CheckBoard(1, 1);
   begin->setAt(0, 0, initialValue);
   
   CheckBoard *end = new CheckBoard(1, 1);
   end->setAt(0, 0, endValue);
   
   InterpolatedModel model;
	model.addModelAdopt(begin);
   model.addModelAdopt(end);
   
   // At 0 we are in the beggining stage
   model.setTimeSlice(InterpolatedModel::MIN_TIME_SLICE);
   CPPUNIT_ASSERT_MESSAGE("Interpolation doesn't work correct for beginning", areEqual(model.getAt(0, 0), initialValue));
                          
   model.setTimeSlice(InterpolatedModel::MAX_TIME_SLICE);
   CPPUNIT_ASSERT_MESSAGE("Interpolation doesn't work correct for end", areEqual(model.getAt(0, 0), endValue));
                          
   model.setTimeSlice((InterpolatedModel::MIN_TIME_SLICE + InterpolatedModel::MAX_TIME_SLICE) / 2);
   CPPUNIT_ASSERT_MESSAGE("Interpolation doesn't work correct for end", areEqual(model.getAt(0, 0), (initialValue + endValue)/2));
}

void InterpolatedModelTest::testSerialization()
{
   const char *testFileName = "testInterpolatedModelSerialization.tmp";
   const int sizeX = 2;
   const int sizeY = 3;
   
   // Create larger board and set its values
   CheckBoard *begin = new CheckBoard(sizeX, sizeY);
   
   int value = 0;
   for (int indexX = 0; indexX < sizeX; indexX++)
      for (int indexY = 0; indexY < sizeY; indexY++)
         begin->setAt(indexX, indexY, value++);
   
   CheckBoard *end = new CheckBoard(sizeX, sizeY);
   
   value = 0;
   for (int indexX = 0; indexX < sizeX; indexX++)
      for (int indexY = 0; indexY < sizeY; indexY++)
         end->setAt(indexX, indexY, value++);
   
   InterpolatedModel toSave;
   toSave.addModelAdopt(begin);
   toSave.addModelAdopt(end);
   toSave.setTimeSlice((InterpolatedModel::MIN_TIME_SLICE + InterpolatedModel::MAX_TIME_SLICE)/2);
   
   FILE *fp = fopen(testFileName, "w");
   CPPUNIT_ASSERT_MESSAGE("Saving to file failed", toSave.saveToFile(fp));
   fclose(fp);
   
   // Now read from the file and compare models
   InterpolatedModel toLoad;
   fp = fopen(testFileName, "r");
   CPPUNIT_ASSERT_MESSAGE("Reading from file failed", toLoad.readFromFile(fp));
   fclose(fp);
   
   // Make sure that models are the same
	CPPUNIT_ASSERT_MESSAGE("Load/Save not working correctly", toSave == toLoad);
	unlink(testFileName);
}

void InterpolatedModelTest::testReadFromGarbageFile()
{
   const char *testFileName = "testCheckBoardReadGarbage.tmp";
   
   // Print some garbage in the file
   FILE *fp = fopen(testFileName, "w");
   fprintf(fp, "Some garbage\n");
   fclose(fp);
   
   // Now read from the file and compare models
   InterpolatedModel toLoad;
   fp = fopen(testFileName, "r");
   CPPUNIT_ASSERT_MESSAGE("Reading from file should fail but it didn't", !toLoad.readFromFile(fp));
   fclose(fp);
	
   unlink(testFileName);
}

void InterpolatedModelTest::testEqual()
{
   const double valueToSet = 0;
   
   InterpolatedModel lhs;
	
   CheckBoard *begin = new CheckBoard(1, 1);
   begin->setAt(0, 0, valueToSet);
   
   CheckBoard *end = new CheckBoard(1, 1);
   end->setAt(0, 0, valueToSet);
   
   lhs.addModelAdopt(begin);
   lhs.addModelAdopt(end);
   lhs.setTimeSlice(InterpolatedModel::MIN_TIME_SLICE);
   
   InterpolatedModel rhs;
   
   begin = new CheckBoard(1, 1);
   begin->setAt(0, 0, valueToSet);
   
   end = new CheckBoard(1, 1);
   end->setAt(0, 0, valueToSet);
   
   rhs.addModelAdopt(begin);
   rhs.addModelAdopt(end);
   rhs.setTimeSlice(InterpolatedModel::MIN_TIME_SLICE);
   
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly", lhs == rhs);
}

void InterpolatedModelTest::testNonEqual()
{
   const double valueToSet = 0;
   
   InterpolatedModel lhs;
	
   CheckBoard *begin = new CheckBoard(1, 1);
   begin->setAt(0, 0, valueToSet);

   CheckBoard *end = new CheckBoard(1, 1);
   end->setAt(0, 0, valueToSet);
   
   lhs.addModelAdopt(begin);
   lhs.addModelAdopt(end);
   lhs.setTimeSlice(InterpolatedModel::MIN_TIME_SLICE);
   
   InterpolatedModel rhs;

   begin = new CheckBoard(1, 1);
   begin->setAt(0, 0, valueToSet);
   
   end = new CheckBoard(1, 1);
   end->setAt(0, 0, valueToSet);
   
   rhs.addModelAdopt(begin);
   rhs.addModelAdopt(end);
   rhs.setTimeSlice(InterpolatedModel::MAX_TIME_SLICE);
   
   CPPUNIT_ASSERT_MESSAGE("Operator != not working correctly", lhs != rhs);
}

void InterpolatedModelTest::testTimeSliceBoundaries()
{
   // Add five different models to the object under test, and check that slices for 0.3 are 0.25 and 0.5
   InterpolatedModel *interpolatedModel = new InterpolatedModel();
   
   CheckBoard *model1 = new CheckBoard();
   interpolatedModel->addModelAdopt(model1);
   
   CheckBoard *model2 = new CheckBoard();
   interpolatedModel->addModelAdopt(model2);
   
   CheckBoard *model3 = new CheckBoard();
   interpolatedModel->addModelAdopt(model3);
   
   CheckBoard *model4 = new CheckBoard();
   interpolatedModel->addModelAdopt(model4);
   
   CheckBoard *model5 = new CheckBoard();
   interpolatedModel->addModelAdopt(model5);
   
   interpolatedModel->setTimeSlice(0.3);
   
	CPPUNIT_ASSERT_MESSAGE("Begin interolation boundaries for timeslices are not correct", areEqual(interpolatedModel->getInterpolationBeginTimeSlice(), 0.25));
	CPPUNIT_ASSERT_MESSAGE("End interolation boundaries for timeslices are not correct", areEqual(interpolatedModel->getInterpolationEndTimeSlice(), 0.5));
   CPPUNIT_ASSERT_MESSAGE("Begin model for the timeslice is not correct", interpolatedModel->getBeginModel() == model2);
   CPPUNIT_ASSERT_MESSAGE("End model for the timeslice is not correct", interpolatedModel->getEndModel() == model3);   
   
   // Similarly, for the 0.9 we are talking about last segment
   interpolatedModel->setTimeSlice(0.9);
   
	CPPUNIT_ASSERT_MESSAGE("Begin interolation boundaries for timeslices are not correct", areEqual(interpolatedModel->getInterpolationBeginTimeSlice(), 0.75));
	CPPUNIT_ASSERT_MESSAGE("End interolation boundaries for timeslices are not correct", areEqual(interpolatedModel->getInterpolationEndTimeSlice(), 1.0));
   CPPUNIT_ASSERT_MESSAGE("Begin model for the timeslice is not correct", interpolatedModel->getBeginModel() == model4);
   CPPUNIT_ASSERT_MESSAGE("End model for the timeslice is not correct", interpolatedModel->getEndModel() == model5);   
}

void InterpolatedModelTest::testMultiObjectInterpolation()
{
   // Create three models, with sizes in them 0, 1, 3. Check values exactly on model boundaries and halfway
   InterpolatedModel *interpolatedModel = new InterpolatedModel();
	
   CheckBoard *model1 = new CheckBoard(1, 1);
   model1->setAt(0, 0, 0);
	interpolatedModel->addModelAdopt(model1);

   CheckBoard *model2 = new CheckBoard(1, 1);
   model2->setAt(0, 0, 1);
	interpolatedModel->addModelAdopt(model2);

   CheckBoard *model3 = new CheckBoard(1, 1);
   model3->setAt(0, 0, 2);
	interpolatedModel->addModelAdopt(model3);
   
   // At timeslice 0, it should be 0
   interpolatedModel->setTimeSlice(0);
   CPPUNIT_ASSERT_MESSAGE("At position 0, we should be equal to the first model", areEqual(interpolatedModel->getAt(0, 0), model1->getAt(0, 0)));
   
   // At timeslice 0.25 we should be halfway between first and second model
   interpolatedModel->setTimeSlice(0.25);
   CPPUNIT_ASSERT_MESSAGE("At position 0.25, we should be equal to the halfway between first and second model", 
                          areEqual(interpolatedModel->getAt(0, 0), (model1->getAt(0, 0) + model2->getAt(0, 0))/2));

   // At timeslice 0.5 we should be equal to second model
   interpolatedModel->setTimeSlice(0.5);
   CPPUNIT_ASSERT_MESSAGE("At position 0.5, we should be equal to the second model", areEqual(interpolatedModel->getAt(0, 0), model2->getAt(0, 0)));

	// At timeslice 0.75 we should be equal to second model
   interpolatedModel->setTimeSlice(0.75);
   CPPUNIT_ASSERT_MESSAGE("At position 0.75, we should be equal to the halfway between second and third model", 
                          areEqual(interpolatedModel->getAt(0, 0), (model2->getAt(0, 0) + model3->getAt(0, 0))/2));

	// At timeslice 0.75 we should be equal to second model
  	interpolatedModel->setTimeSlice(1);
  	CPPUNIT_ASSERT_MESSAGE("At position 1, we should be equal to the third model", areEqual(interpolatedModel->getAt(0, 0), model3->getAt(0, 0)));
}

void InterpolatedModelTest::testNonEqualDifferentModels()
{
   const double valueToSet = 0;
   
   InterpolatedModel lhs;
	
   CheckBoard *begin = new CheckBoard(1, 1);
   begin->setAt(0, 0, valueToSet);
   
   CheckBoard *end = new CheckBoard(1, 1);
   end->setAt(0, 0, valueToSet);
   
   lhs.addModelAdopt(begin);
   lhs.addModelAdopt(end);
   lhs.setTimeSlice(InterpolatedModel::MIN_TIME_SLICE);
   
   InterpolatedModel rhs;
   
   begin = new CheckBoard(1, 1);
   begin->setAt(0, 0, valueToSet);
   
   end = new CheckBoard(1, 1);
   end->setAt(0, 0, valueToSet);
   
   // Add one extra model in rhs
   CheckBoard *extraModel = new CheckBoard(1, 1);
   extraModel->setAt(0, 0, valueToSet);
   
   rhs.addModelAdopt(begin);
   rhs.addModelAdopt(end);
   rhs.addModelAdopt(extraModel);
   rhs.setTimeSlice(InterpolatedModel::MIN_TIME_SLICE);
   
   CPPUNIT_ASSERT_MESSAGE("Operator == not working correctly", lhs != rhs);
}

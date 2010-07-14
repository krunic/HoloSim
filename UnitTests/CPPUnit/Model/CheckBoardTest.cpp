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

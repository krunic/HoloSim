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

#include "AbstractDrawingCode.h"
#include "AbstractDrawingCodeTest.h"
#include "MathHelper.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(AbstractDrawingCodeTest);

AbstractDrawingCodeTest::AbstractDrawingCodeTest()
{
   
}
      
AbstractDrawingCodeTest::~AbstractDrawingCodeTest()
{
   
}
      
void AbstractDrawingCodeTest::setUp()
{
   
}
      
void AbstractDrawingCodeTest::tearDown()
{
   
}
      
void AbstractDrawingCodeTest::testNormalizeAngle()
{
   CPPUNIT_ASSERT_MESSAGE("Angles in [0, 360) should remain unchanged", areEqual(0, normalizeAngle(0)));
   CPPUNIT_ASSERT_MESSAGE("Not normalized correctly", areEqual(0, normalizeAngle(360)));
   CPPUNIT_ASSERT_MESSAGE("Not normalized correctly", areEqual(10, normalizeAngle(370)));
   CPPUNIT_ASSERT_MESSAGE("Not normalized correctly", areEqual(20, normalizeAngle(740)));
   CPPUNIT_ASSERT_MESSAGE("Not normalized correctly", areEqual(0, normalizeAngle(3600)));
   CPPUNIT_ASSERT_MESSAGE("Not normalized correctly", areEqual(350, normalizeAngle(-10)));
   CPPUNIT_ASSERT_MESSAGE("Not normalized correctly", areEqual(355, normalizeAngle(-365)));  
}
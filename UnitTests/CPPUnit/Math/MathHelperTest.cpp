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

#include "MathHelperTest.h"

#include "MathHelper.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(MathHelperTest);

MathHelperTest::MathHelperTest()
{
   
}
   
MathHelperTest::~MathHelperTest()
{
   
}
   
void MathHelperTest::setUp()
{
   
}
   
void MathHelperTest::tearDown()
{
   
}
   
void MathHelperTest::testEqual()
{
   CPPUNIT_ASSERT_MESSAGE("Floating point inequality not working for close numbers", areEqual(0, FLOATING_POINTS_EQUAL_DELTA));
   CPPUNIT_ASSERT_MESSAGE("Floating point inequality not working for far numbers", !areEqual(0, 2*FLOATING_POINTS_EQUAL_DELTA));
}

void MathHelperTest::testEqualInLowPrecision()
{
   CPPUNIT_ASSERT_MESSAGE("Floating points are equal in double precision when they shouldn't be", !areEqual(0, FLOATING_POINTS_LOW_PRECISION_EQUAL_DELTA));
   CPPUNIT_ASSERT_MESSAGE("Floating points are not equal in single precision when they should be", areEqualInLowPrecision(0, FLOATING_POINTS_LOW_PRECISION_EQUAL_DELTA));
   CPPUNIT_ASSERT_MESSAGE("Single precision floating point inequality not working", !areEqualInLowPrecision(0, 2*FLOATING_POINTS_LOW_PRECISION_EQUAL_DELTA));
}

void MathHelperTest::testMin()
{
   CPPUNIT_ASSERT_MESSAGE("Min not correctly calculated for two numbers", areEqual(min(1.0, 2.0), 1.0));
   CPPUNIT_ASSERT_MESSAGE("Min not correctly calculated for equal numbers", areEqual(min(1.0, 1.0), 1.0));
   CPPUNIT_ASSERT_MESSAGE("Min not correctly calculated for negative numbers", areEqual(min(-1.0, 1.0), -1.0));
}

void MathHelperTest::testMax()
{
   CPPUNIT_ASSERT_MESSAGE("Max not correctly calculated for two numbers", areEqual(max(1.0, 2.0), 2.0));
   CPPUNIT_ASSERT_MESSAGE("Max not correctly calculated for equal numbers", areEqual(max(1.0, 1.0), 1.0));
   CPPUNIT_ASSERT_MESSAGE("Max not correctly calculated for negative numbers", areEqual(max(-1.0, 1.0), 1.0));
}

void MathHelperTest::testStringToNumber()
{
   int intNumber;
   CPPUNIT_ASSERT_MESSAGE("Conversion of integer failed", stringToNumber("5", &intNumber));
   CPPUNIT_ASSERT_MESSAGE("Result of string conversion incorrect", intNumber == 5);
   
   double doubleNumber;
   CPPUNIT_ASSERT_MESSAGE("Conversion of the double number failed", stringToNumber("5.01", &doubleNumber));
   CPPUNIT_ASSERT_MESSAGE("Conversion of the double number was not correct", areEqual(5.01, doubleNumber));
   
	CPPUNIT_ASSERT_MESSAGE("Empty string not correctly detected", !stringToNumber("", &intNumber));
	CPPUNIT_ASSERT_MESSAGE("Non-number string not correctly detected", !stringToNumber("abc", &intNumber));
	CPPUNIT_ASSERT_MESSAGE("Trailing string not correctly detected", !stringToNumber("4abc", &intNumber));
	CPPUNIT_ASSERT_MESSAGE("Trailing space not correctly detected", !stringToNumber("4 abc", &intNumber));   
	CPPUNIT_ASSERT_MESSAGE("Leading string not correctly detected", !stringToNumber("abc4", &intNumber));   
	CPPUNIT_ASSERT_MESSAGE("Leading space not correctly detected", !stringToNumber("abc 4", &intNumber));
	CPPUNIT_ASSERT_MESSAGE("Spaces are not correctly trimmed", !stringToNumber(" 4 ", &intNumber));   
}

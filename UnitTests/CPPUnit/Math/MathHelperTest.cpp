/*
 *  MathHelperTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/22/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
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

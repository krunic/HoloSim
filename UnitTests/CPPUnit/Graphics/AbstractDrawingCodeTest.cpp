/*
 *  AbstractDrawingCodeTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 8/18/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
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
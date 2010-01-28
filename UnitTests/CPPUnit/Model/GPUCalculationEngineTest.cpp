/*
 *  GPUCalculationEngineTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/28/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include "GPUCalculationEngine.h"
#include "GPUCalculationEngineTest.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(GPUCalculationEngineTest);

GPUCalculationEngineTest::GPUCalculationEngineTest() 
{
   
}

GPUCalculationEngineTest::~GPUCalculationEngineTest()
{
   
}

void GPUCalculationEngineTest::setUp()
{
   
}

void GPUCalculationEngineTest::tearDown()
{
   
}

void GPUCalculationEngineTest::testFindWholeWorldSubstring()
{
   CPPUNIT_ASSERT_MESSAGE("SIMPLE MATCH NOT FOUND", isWholeWorldSubstring("abc 123 de", "123"));
   CPPUNIT_ASSERT_MESSAGE("BEGIN MATCH NOT FOUND", isWholeWorldSubstring("123 de", "123"));
   CPPUNIT_ASSERT_MESSAGE("END MATCH NOT FOUND", isWholeWorldSubstring("abc 123", "123"));
   
	CPPUNIT_ASSERT_MESSAGE("MATCHED WHAT IS NOT WHOLE WORLD", !isWholeWorldSubstring("abc123de", "123"));
   CPPUNIT_ASSERT_MESSAGE("MATCHED INCORRECTLY WITH PREFIX", !isWholeWorldSubstring("abc123 de", "123"));
   CPPUNIT_ASSERT_MESSAGE("MATCHED INCORRECTLY WITH SUFFIX", !isWholeWorldSubstring("abc 123de", "123"));
}

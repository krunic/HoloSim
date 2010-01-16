/*
 *  ColladaTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/16/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include "ColladaTest.h"

#include <cppunit/extensions/HelperMacros.h>

#include "Collada.h"
#include "GPUGeometryModel.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(ColladaTest);

ColladaTest::ColladaTest() 
{

}

ColladaTest::~ColladaTest()
{

}

void ColladaTest::setUp()
{

}

void ColladaTest::tearDown()
{

}

void ColladaTest::testLoadQuad()
{
   GPUGeometryModel model;
   
   CPPUNIT_ASSERT_MESSAGE("Can't load simple collada file", loadCollada("TestQuad.dae", model));
}

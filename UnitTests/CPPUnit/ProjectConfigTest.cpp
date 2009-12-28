/*
 *  ProjectConfigTestCase.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 5/19/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include "ProjectConfigTest.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(ProjectConfigTest);

ProjectConfigTest::ProjectConfigTest()
{
   
}

ProjectConfigTest::~ProjectConfigTest()
{
   
}

void ProjectConfigTest::setUp()
{
   
}

void ProjectConfigTest::tearDown()
{
   
}

void ProjectConfigTest::testCPPUnitWorks()
{
   CPPUNIT_ASSERT_MESSAGE("Project is not configured correctly, this CPPUnit test should be passing", true);
}

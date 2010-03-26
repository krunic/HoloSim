/*
 *  ShaderTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/16/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include "ShaderTest.h"

#include "OGLUtils.h"
#include "Shader.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(ShaderTest);

ShaderTest::ShaderTest() 
{
   
}

ShaderTest::~ShaderTest()
{
   
}

void ShaderTest::setUp()
{
}

void ShaderTest::tearDown()
{
}

void ShaderTest::testVertexAndFragmentShaderCreation()
{
   Shader testFixture;
   
   CPPUNIT_ASSERT_MESSAGE("Initialization of the offscreen renderer failed", initOpenGLOffScreenRender(100, 100, &cglContext_, &frameBufferID_, &colorBufferID_, &depthBufferID_));
   CPPUNIT_ASSERT_MESSAGE("Shader creation and compilation failed", testFixture.initialize("Plasma.vs", "Plasma.fs"));
   CPPUNIT_ASSERT_MESSAGE("Teardown of the offscreen renderer failed", destroyOpenGLOffScreenRender(cglContext_, frameBufferID_, colorBufferID_, depthBufferID_));   
}

void ShaderTest::testVertexShaderCreation()
{
   Shader testFixture;
   
   CPPUNIT_ASSERT_MESSAGE("Initialization of the offscreen renderer failed", initOpenGLOffScreenRender(100, 100, &cglContext_, &frameBufferID_, &colorBufferID_, &depthBufferID_));
   CPPUNIT_ASSERT_MESSAGE("Shader creation and compilation failed", testFixture.initializeWithVertexShaderOnly("PlasmaVSOnly.vs"));
   CPPUNIT_ASSERT_MESSAGE("Teardown of the offscreen renderer failed", destroyOpenGLOffScreenRender(cglContext_, frameBufferID_, colorBufferID_, depthBufferID_));   
}

void ShaderTest::testFragmentShaderCreation()
{
   Shader testFixture;
   
   CPPUNIT_ASSERT_MESSAGE("Initialization of the offscreen renderer failed", initOpenGLOffScreenRender(100, 100, &cglContext_, &frameBufferID_, &colorBufferID_, &depthBufferID_));
   CPPUNIT_ASSERT_MESSAGE("Shader creation and compilation failed", testFixture.initializeWithFragmentShaderOnly("PlasmaFSOnly.fs"));
   CPPUNIT_ASSERT_MESSAGE("Teardown of the offscreen renderer failed", destroyOpenGLOffScreenRender(cglContext_, frameBufferID_, colorBufferID_, depthBufferID_));   
}

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

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

#ifndef GPU_GEOMETRY_MODEL_TEST_H_
#define GPU_GEOMETRY_MODEL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class GPUGeometryModelTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(GPUGeometryModelTest);
         CPPUNIT_TEST(testModelName);
         CPPUNIT_TEST(testEqual);
         CPPUNIT_TEST(testDiferentDimsAreNonEqual);
         CPPUNIT_TEST(testModelCleaning);
         CPPUNIT_TEST(testLoadModel);
         CPPUNIT_TEST(testGeometryCleaning);
         CPPUNIT_TEST(testBounds);
	      CPPUNIT_TEST(testRenderArea);
	      CPPUNIT_TEST(testQuadCoveringWholeArea);
	      CPPUNIT_TEST(testTriangleCoveringPartOfTheArea);
      	CPPUNIT_TEST(testCopyConstructor);
	      CPPUNIT_TEST(testCallingTwice);
      	CPPUNIT_TEST(testOperatorEqual);
	      CPPUNIT_TEST(testPrecalculationStatus);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      GPUGeometryModelTest();
      
      /**
       * Destructor
       */
      virtual ~GPUGeometryModelTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();
      
      /**
       * Simple test to ensure that name of the model is what we think it is
       */
      void testModelName();
      
      /**
       * Test that there copy constructor works correctly
       */
      void testCopyConstructor();
      
      /**
       * Test that operator equal works correctly
       */
      void testOperatorEqual();
      
      /**
       * Test read and write of the checkboard to the file
       */
      void testSerialization();
      
      /**
       * Test that read from the garbage file fails correctly 
       */
      void testReadFromGarbageFile();
      
      /**
       * Test operator==
       */
      void testEqual();
      
      /**
       * Test operator!=
       */
      void testNonEqual();
      
      /**
       * Test that different dimensions are non-equal
       */
      void testDiferentDimsAreNonEqual();
      
      /**
       * Test loading of the model
       */
      void testLoadModel();
      
      /**
       * Test cleaning of the model (geometry and size should be reset)
       */
      void testModelCleaning();
      
      /**
       * Test cleaning of the geometry (only geometry should be reset)
       */
      void testGeometryCleaning();     
      
      /**
       * Test model bounds
       */
      void testBounds();
      
      /**
       * Test of the proper setup of rendering area
       */
      void testRenderArea();
      
      /**
       * Test of the quad parallel to Z plane, which is larger then rendering area
       */
      void testQuadCoveringWholeArea();

      /**
       * Test calling twice (we had a problem with that
       */
      void testCallingTwice();
      
      /**
       * Test of the quad parallel to Z plane, which is smaller then rendering area
       */
      void testTriangleCoveringPartOfTheArea();

      /**
       * Test status of precalculation
       */
      void testPrecalculationStatus();
      
   private:
      // define
      GPUGeometryModelTest(const GPUGeometryModelTest &rhs);   
      GPUGeometryModelTest & operator=(const GPUGeometryModelTest &rhs);   
   };
   
}

#endif

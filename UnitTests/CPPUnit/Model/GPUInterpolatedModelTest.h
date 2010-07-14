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

#ifndef GPU_INTERPOLATED_MODEL_TEST_H_
#define GPU_INTERPOLATED_MODEL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class GPUInterpolatedModelTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(GPUInterpolatedModelTest);
      	CPPUNIT_TEST(testModelName);
         CPPUNIT_TEST(testCopyConstructor);
         CPPUNIT_TEST(testOperatorEqual);
	      CPPUNIT_TEST(testSerialization);
	      CPPUNIT_TEST(testReadFromGarbageFile);
	      CPPUNIT_TEST(testEqual);
	      CPPUNIT_TEST(testNonEqual);
	      CPPUNIT_TEST(testGetFileNameInSameDir);
	      CPPUNIT_TEST(testPrecalculationStatus);
	      CPPUNIT_TEST(testOptimizeDrawing);
         CPPUNIT_TEST(testDecimation);
         CPPUNIT_TEST(testNoShiftsAfterDecimation);      
         CPPUNIT_TEST(testIdentityDecimation);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      GPUInterpolatedModelTest();
      
      /**
       * Destructor
       */
      virtual ~GPUInterpolatedModelTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();

      /**
       * Test that the model name is correctly set
       */
      void testModelName();
      
      /**
       * Test copy constructor
       */
      void testCopyConstructor();
      
      /**
       * Test operator equal
       */
      void testOperatorEqual();
      
      /**
       * Test model serialization
       */
      void testSerialization();
      
      /**
       * Test reading from the garbage file
       */
      void testReadFromGarbageFile();
      
      /**
       * Test operator=
       */
      void testEqual();
      
      /**
       * Test operator !=
       */
      void testNonEqual();
      
      /**
       * Test that file substitution is working correctly
       */
      void testGetFileNameInSameDir();
      
      /**
       * Test status of precalculation
       */
      void testPrecalculationStatus();
      
      /**
       * Test that request for drawing optimization works correctly
       */
      void testOptimizeDrawing();

      /**
       * Test that decimation is working correctly
       */
      void testDecimation();
      
      /**
       * Test that drawing optimization venters properly and doesn't cause shift in drawed picture
       */
      void testNoShiftsAfterDecimation();
      
      /**
       * Test that decimation supposed to result in the same size of the model as original model does
       */
   	void testIdentityDecimation();
      
   private:
      
      // define
      GPUInterpolatedModelTest(const GPUInterpolatedModelTest &rhs);   
      GPUInterpolatedModelTest & operator=(const GPUInterpolatedModelTest &rhs);   
   };
   
}

#endif

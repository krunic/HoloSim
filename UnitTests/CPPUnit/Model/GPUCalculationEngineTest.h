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

#ifndef GPU_CALCULATION_ENGINE_TEST_H_
#define GPU_CALCULATION_ENGINE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class GPUCalculationEngineTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(GPUCalculationEngineTest);
	      CPPUNIT_TEST(testFindWholeWorldSubstring);
	      CPPUNIT_TEST(testOpenGLExtensions);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      GPUCalculationEngineTest();
      
      /**
       * Destructor
       */
      virtual ~GPUCalculationEngineTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();
      
      /**
       * Simple test that openGL extension processing is done correctly
       */
      void testFindWholeWorldSubstring();
      
      /**
       * This is test with real OpenGL extensions 
       */
      void testOpenGLExtensions();
      
   private:
      
      // define
      GPUCalculationEngineTest(const GPUCalculationEngineTest &rhs);   
      GPUCalculationEngineTest & operator=(const GPUCalculationEngineTest &rhs);   
   };
   
}

#endif
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

#ifndef ABSTRACT_DRAWING_CODE_TEST_H_
#define ABSTRACT_DRAWING_CODE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   /**
   * Test math utility functions
    */
   class AbstractDrawingCodeTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(AbstractDrawingCodeTest);
	      CPPUNIT_TEST(testNormalizeAngle);
      CPPUNIT_TEST_SUITE_END();
      
	public:
         
      /**
       * Constructor
       */
      AbstractDrawingCodeTest();
      
      /**
       * Destructor
       */
      virtual ~AbstractDrawingCodeTest();
      
      /**
         * Prepare test for running
       */
      void setUp();
      
      /**
         * Cleanup after test
       */
      void tearDown();
      
      /**
         * Simple test to ensure that equality works correctly
       */
      void testNormalizeAngle();
      
	private:
      AbstractDrawingCodeTest(const AbstractDrawingCodeTest &rhs);   
      AbstractDrawingCodeTest & operator=(const AbstractDrawingCodeTest &rhs);
   };
   
} // namespace

#endif

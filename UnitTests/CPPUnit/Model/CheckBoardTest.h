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

#ifndef CHECKBOARD_TEST_H_
#define CHECKBOARD_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {

   class CheckBoardTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(CheckBoardTest);
         CPPUNIT_TEST(testModelName);
         CPPUNIT_TEST(testCopy);
         CPPUNIT_TEST(testEqual);
         CPPUNIT_TEST(testNonEqual);
         CPPUNIT_TEST(testDiferentDimsAreNonEqual);
      CPPUNIT_TEST_SUITE_END();
      
   public:
         
      /**
       * Constructor
       */
      CheckBoardTest();
      
      /**
       * Destructor
       */
      virtual ~CheckBoardTest();
      
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
       * Test that there is no aliasing after copy (so that copy constructor and operator= are working correctly
       */
      void testCopy();
      
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
      
   private:
      // define
      CheckBoardTest(const CheckBoardTest &rhs);   
      CheckBoardTest & operator=(const CheckBoardTest &rhs);   
   };

}
   
#endif

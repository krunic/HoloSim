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

#ifndef COLLADA_TEST_H_
#define COLLADA_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   class ColladaTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(ColladaTest);
         CPPUNIT_TEST(testLoadQuad);
         CPPUNIT_TEST(testLoadChair);
      CPPUNIT_TEST_SUITE_END();
      
   public:
      
      /**
       * Constructor
       */
      ColladaTest();
      
      /**
       * Destructor
       */
      virtual ~ColladaTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();
      
      /**
       * Simple test to ensure that quad created in Google Sketchup could be correctly loaded
       */
      void testLoadQuad();
      
      /**
       * More complex test of loading chair
       */
      void testLoadChair();
      
   private:
      
      // define
      ColladaTest(const ColladaTest &rhs);   
      ColladaTest & operator=(const ColladaTest &rhs);   
   };
   
}

#endif
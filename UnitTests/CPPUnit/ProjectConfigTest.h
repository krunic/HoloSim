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

#ifndef PROJECT_CONFIG_TEST_H_
#define PROJECT_CONFIG_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {

   /**
    * Test that CPPUnit is correctly configured.
    */
   class ProjectConfigTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(ProjectConfigTest);
        CPPUNIT_TEST(testCPPUnitWorks);
      CPPUNIT_TEST_SUITE_END();
      
   public:

      /**
       * Constructor
       */
      ProjectConfigTest();
      
      /**
       * Destructor
       */
      virtual ~ProjectConfigTest();
      
      /**
       * Prepare test for running
       */
      void setUp();
      
      /**
       * Cleanup after test
       */
      void tearDown();

      /**
       * Simple test to ensure that CPPUnit framework is working correctly
       */
      void testCPPUnitWorks();
      
   private:
      // define
      ProjectConfigTest(const ProjectConfigTest &rhs);   
      ProjectConfigTest & operator=(const ProjectConfigTest &rhs);
   };

} // namespace   
   
#endif

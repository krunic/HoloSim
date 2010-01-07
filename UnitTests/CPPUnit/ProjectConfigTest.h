/*
 *  ProjectConfigTestCase.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 5/19/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
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

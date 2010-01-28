/*
 *  ColladaTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/16/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
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
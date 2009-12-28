/*
 *  CheckBoardTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/20/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
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
         CPPUNIT_TEST(testSerialization);
         CPPUNIT_TEST(testReadFromGarbageFile);
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
      
   private:
      // define
      CheckBoardTest(const CheckBoardTest &rhs);   
      CheckBoardTest & operator=(const CheckBoardTest &rhs);   
   };

}
   
#endif

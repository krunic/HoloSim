/*
 *  MathHelperTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/22/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */

#ifndef MATH_HELPER_TEST_H_
#define MATH_HELPER_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace hdsim {
   
   /**
    * Test math utility functions
    */
   class MathHelperTest : public CppUnit::TestFixture  {
      CPPUNIT_TEST_SUITE(MathHelperTest);
         CPPUNIT_TEST(testEqual);
      CPPUNIT_TEST_SUITE_END();
      
   public:
         
      /**
       * Constructor
       */
      MathHelperTest();
      
      /**
         * Destructor
       */
      virtual ~MathHelperTest();
      
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
      void testEqual();
      
   private:
         MathHelperTest(const MathHelperTest &rhs);   
         MathHelperTest & operator=(const MathHelperTest &rhs);
   };

} // namespace

#endif

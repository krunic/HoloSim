/*
 *  AbstractDrawingCodeTest.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 8/18/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
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

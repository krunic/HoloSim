/*
 *  UnitTests.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 5/19/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/TestFactoryRegistry.h>

#include <cppunit/ui/text/TestRunner.h>

int main( int argc, char **argv)
{
   CppUnit::TextUi::TestRunner runner;
   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
   runner.addTest(registry.makeTest());
   bool wasSuccessful = runner.run("", false);
   return !wasSuccessful;
}

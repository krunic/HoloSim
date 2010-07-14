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

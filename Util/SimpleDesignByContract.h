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

#ifndef SIMPLE_DESIGN_BY_CONTRACT_H_
#define SIMPLE_DESIGN_BY_CONTRACT_H_

#ifndef NDEBUG

#define PRECONDITION(condition) hdsim::checkAssertion((condition), 0, "PRECONDITION", __FILE__, __LINE__)
#define POSTCONDITION(condition) hdsim::checkAssertion((condition), 0, "POSTCONDITION", __FILE__, __LINE__)
#define CHECK(condition, message) hdsim::checkAssertion((condition), message, "CHECK", __FILE__, __LINE__);
#define FAIL(message) hdsim::checkAssertion(false, message, "CHECK", __FILE__, __LINE__);
#define LOG(message) hdsim::log(message, __FILE__, __LINE__);


#else

// We must ensure that condition gets executed, so that semantic of the program doesn't change when contract checking is disabled

#define PRECONDITION(condition, message) condition
#define POSTCONDITION(condition, message) condition
#define CHECK(condition, message) condition
#define FAIL(message)

#endif

namespace hdsim {

   /**
    * Check that given condition holds. Used in the light Design By Contract framework that is made for Holodeck, as there are no good DoC
    * frameworks for C++ that are easy to setup on Mac.
    *
    * Note that this is missing many elements that real DoC framework should have like invariants, awareness of inheritance and integration with documentation system
    *
    * @param condition condition that must be true
    * @param message to issue if condition fails
    * @param type what was type of this assertion (e.g. precondition, postcondition, etc...)
    * @param fileName file where this originated from
    * @param sourceLine line in file
    */
   void checkAssertion(bool condition, const char *message, const char *type, const char *fileName, int sourceLine);
   
   /**
    * Log message to stderr
    *
    * @param message to issue if condition fails
    * @param fileName file where this originated from
    * @param sourceLine line in file
    */
   void log(const char *message, const char *fileName, int sourceLine);
   
} // namespace

#endif
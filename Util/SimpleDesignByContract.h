/*
 *  SimpleDesignByContract.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/29/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#ifndef SIMPLE_DESIGN_BY_CONTRACT_H_
#define SIMPLE_DESIGN_BY_CONTRACT_H_

#ifndef NDEBUG

#define PRECONDITION(condition) hdsim::checkAssertion((condition), 0, "PRECONDITION", __FILE__, __LINE__)
#define POSTCONDITION(condition) hdsim::checkAssertion((condition), 0, "POSTCONDITION", __FILE__, __LINE__)
#define CHECK(condition, message) hdsim::checkAssertion((condition), message, "CHECK", __FILE__, __LINE__);

#else

#define PRECONDITION(condition, message)
#define POSTCONDITION(condition, message)
#define CHECK(condition, message)

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
   
} // namespace

#endif
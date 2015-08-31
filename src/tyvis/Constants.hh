#ifndef CONSTANTS_HH
#define CONSTANTS_HH

// Copyright (c) Ohio Board of Regents and the University of Cincinnati.  
// All Rights Reserved.

// UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
// LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR
// DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "tyvis/tyvis-config.h"

/// SPARSE error constants.
enum sparseErrors {E_INTERN = 1,E_SINGULAR = 102,E_ITERLIM = 103,E_ORDER = 104};

/// Solver state constants similar to those defined in SPICE.
enum solverModeConstants {
  MODE = 0x3, MODETRAN = 0x1, MODEAC = 0x2, MODEDC = 0x70,MODEDCOP = 0x10, MODETRANOP = 0x20, 
  INITF = 0x3f00, MODEINITFLOAT = 0x100, MODEINITJCT = 0x200, MODEINITFIX = 0x400,
  MODEINITSMSIG = 0x800, MODEINITTRAN = 0x1000, MODEINITPRED = 0x2000, MODEUIC = 0x10000l 
};

/// Kernel state constants similar to those defined in SPICE.
enum iterationModeConstants { 
  NISHOULDREORDER = 0x1, NIREORDERED = 0x2, NIUNINITIALIZED = 0x4, NIACSHOULDREORDER = 0x10,
  NIACREORDERED = 0x20, NIACUNINITIALIZED = 0x40, NIDIDPREORDER = 0x100, NIPZSHOULDREORDER = 0x200
};

/// Used by the nodelist to determine the type and number of nodes associated with a quantity.
enum nodeConstants {EFFORT=3, FLOW=4};

/// Defines the type of the equation.
enum eqnType {BRANCH_EQN, FREE_EQN, DIFFERENTIAL_EQN};

enum nodeError {NONODE = -1};

#endif

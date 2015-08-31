#ifndef STDTYPES_HH
#define STDTYPES_HH

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
#include <iostream>
#include <sstream>
#include <string.h>

#ifdef HAVE_STDINT_H
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>
#endif

using std::ostream;
using std::ostringstream;
using std::cout;
using std::cerr;
using std::endl;

#include "tyvis/vhdl.hh"
#include "tyvis/VHDLData.hh"
#include "tyvis/Types.hh"
#include "tyvis/std_standardPkg.hh"

class AccessObject;
class AccessObject;


#include "tyvis/UniversalInteger.hh"

struct UniversalReal;
struct PhysicalLiteral;

#include "tyvis/UniversalInteger.hh"
#include "tyvis/PhysicalLiteral.hh"
#include "tyvis/UniversalReal.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/KernelTypes.hh"

#endif

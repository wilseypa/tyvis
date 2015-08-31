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

#ifndef TYPES_HH
#define TYPES_HH

#include "tyvis/tyvis-config.h"
#include <iostream>
using std::ostream;
#include <vector>
using std::vector;

#ifndef ASSERT
#ifndef NO_DEVELOPER_ASSERTIONS
#include <cassert>
#define ASSERT( x ) assert( x )
#else
#define ASSERT( x )
#endif
#endif

#include "Attribute.hh"

class RValue;
class SignalBase;
class VHDLProcess;

enum SigType { EXPLICIT, NULL_TRANSACTION, G_BUS, IMPLICIT };

enum TransactionType { NORMALTRANSACTION, CANCELTRANSACTION};

enum PortType { IN, OUT, INOUT, BUFFER, LINKAGE };
enum VarType { NORMAL, ACCESS };
// LAST_EVENT is first because it must be updated first in 
// VHDLKernel::updateSignal.  The other attributes depend on this value.
// DO NOT CHANGE THIS.

// This enumeration type tells us the state of the kernel regarding wait
// statements: it is not waiting, waiting inside a procedure, or waitin
// inside a process.
enum WaitiState { NOT_WAITING = -5, WAITING_IN_PROC };

// This enumeration type is used to implement wait statements within a
// procedure.  Three conditions arise:
// 1. The procedure hits a wait statement, and returns.
// 2. The procedure is called to evaluate if the wait has resumed.
// 3. The procedure returns normally (after a VHDL "return" statement).
// These three states are represened by the following enumerations
// reapectively.
enum ProcedureWaitReturn_t {EXECUTE_WAIT_RETURN, RESUME_WAIT_RETURN, 
			    NORMAL_RETURN};


ostream& operator<<(ostream& os, const SigType& st);
ostream& operator<<(ostream& os, const PortType& pt);
ostream& operator<<(ostream& os, const VarType& vt);

#ifdef __DECCXX
typedef long int LONG;  // 8 byte long
#else
typedef long long int LONG; // 4 byte long, 8 byte long long
#endif

#define PHYSICAL_MAX(typeInfo) PhysicalTypeInfo::getMax(typeInfo)
#define PHYSICAL_MIN(typeInfo) PhysicalTypeInfo::getMin(typeInfo)
#define INTEGER_MAX IntegerTypeInfo::getMax()
#define INTEGER_MIN IntegerTypeInfo::getMin()
#define VHDL_STRING(stringPart) StringLiteral( std_standard_stringTypeInfo(), stringPart )
#define VHDL_TIME(timePart) PhysicalLiteral( std_standard_timeTypeInfo(), timePart )

/** Type for up and down type conversion functions, and resolution
    function ids.  These functions are accessed from a global function
    array.  This is necessary for static elaboration.  We must be able
    to write these function "references" to a file and read it back
    before execution.  This can not be done if write pointers to
    file. */
typedef int TypeConversionFnId_t;

typedef int ResolutionFnId_t;

class Value;
/**
   Defines the signature of a resolution function.  The parameter passed in
   must be array typed.
*/
typedef Value (*ResolutionFnPtr)(VHDLProcess *, const RValue &);
typedef Value (*TypeConversionFnPtr)(VHDLProcess *, const RValue &);

#endif

#ifndef VHDL_CC
#define VHDL_CC

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

#include "vhdl.hh"
#include "VHDLProcess.hh"
#include "ArrayInfo.hh"
#include "SigEvent.hh"
#include "UniversalInteger.hh"
#include "PhysicalLiteral.hh"
#include "UniversalReal.hh"
#include "VHDLVTime.hh"
#include "WaitEvent.hh"

void registerTyvisDeserializers(){
  ArrayInfo::registerDeserializer();
  SigEvent::registerDeserializer();
  UniversalInteger::registerDeserializer();
  PhysicalLiteral::registerDeserializer();
  UniversalReal::registerDeserializer();
  VHDLVTime::registerDeserializer();
  WaitEvent::registerDeserializer();
}

#endif

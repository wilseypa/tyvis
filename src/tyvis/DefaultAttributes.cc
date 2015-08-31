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

#include "DefaultAttributes.hh"
#include "STDTypes.hh"
#include "std_standardPkg.hh"

DefaultAttributes::DefaultAttributes() :
  event( false ),
  active( false ),
  last_event(VHDLVTime::getVHDLVTimePositiveInfinity()),
  last_active(VHDLVTime::getVHDLVTimePositiveInfinity()),
  sigtype( INT ){
  last_value.inttype = 0;  
}

DefaultAttributes::DefaultAttributes(const DefaultAttributes& val):
  last_event(val.last_event),
  last_active(val.last_active) {

  event = val.event;
  active = val.active;
  sigtype = val.sigtype;
  switch(sigtype) {
  case INT:
    last_value.inttype = val.last_value.inttype;
    break;
  case DOUBLE:
    last_value.floattype = val.last_value.floattype;
    break;
  case INT64:
    last_value.physicaltype = val.last_value.physicaltype;
    break;
  default:
    break;
  }
}


DefaultAttributes&
DefaultAttributes::operator=(const DefaultAttributes& val) {
  event = val.event;
  active = val.active;
  last_event = val.last_event;
  sigtype = val.sigtype;
  last_active = val.last_active;
  switch(sigtype) {
  case INT:
    last_value.inttype = val.last_value.inttype;
    break;
  case DOUBLE:
    last_value.floattype = val.last_value.floattype;
    break;
  case INT64:
    last_value.physicaltype = val.last_value.physicaltype;
    break;
  default:
    break;
  }
  return *this;
}

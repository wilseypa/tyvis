#ifndef ATTRIBUTE_HH
#define ATTRIBUTE_HH

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

class SignalBase;

enum AttribType { LAST_EVENT, EVENT, ACTIVE, LAST_ACTIVE, LAST_VALUE, 
		  DRIVING, DRIVING_VALUE, QUIET, STABLE, TRANSACTION,
		  INVALID};

class Attribute {
public:
  Attribute( AttribType initType, SignalBase &initSignal ) :
    myAttrib( initType ), mySignal( initSignal ){}

  SignalBase &getSignal() const { return mySignal; }
  AttribType getType() const { return myAttrib; }

private:
  AttribType myAttrib;
  SignalBase &mySignal;
};

inline ostream & 
operator << (ostream &os, const Attribute &at) {
  switch( at.getType() ) {
  case EVENT:
  case ACTIVE:
  case LAST_VALUE:
  case DRIVING:
  case DRIVING_VALUE:
    os << "Driving_Value Attribute";
    break;
  case LAST_ACTIVE:
  case LAST_EVENT:
    os << "Last_Event attribute";;
    break;
  case STABLE:
  case QUIET:
  case TRANSACTION:
    break;
  default:
    os << "(invalid attribute)";
    break;
  }
  return os;
}

inline ostream &
operator << (ostream &os, const AttribType &at) {
  switch(at) {
  case EVENT:
    os << "EVENT";
    break;
  case ACTIVE:
    os << "ACTIVE";
    break;
  case LAST_EVENT:
    os << "LAST_EVENT";
    break;
  case LAST_ACTIVE:
    os << "LAST_ACTIVE";
    break;
  case LAST_VALUE:
    os << "LAST_VALUE";
    break;
  case DRIVING:
    os << "DRIVING";
    break;
  case DRIVING_VALUE:
    os << "DRIVING_VALUE";
    break;
  case STABLE:
    os << "STABLE";
    break;
  case QUIET:
    os << "QUIET";
    break;
  case TRANSACTION:
    os << "TRANSACTION";
    break;
  default:
    os << "(invalid attribute type)";
    break;
  }
  return os;
}

#endif

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

#include "Types.hh"

ostream&
operator<<(ostream& os, const SigType& st) {
  switch (st) {
  case EXPLICIT:
    os << "EXPLICIT";
    break;
  case G_BUS:
    os << "G_BUS";
    break;
  case NULL_TRANSACTION:
    os << "NULL_TRANSACTION";
    break;
  default:
    os << "(invalid SigType)";
    break;
  }
  return os;
}


ostream&
operator<<(ostream& os, const PortType& pt) {
  switch (pt) {
  case IN:
    os << "IN";
    break;
  case OUT:
    os << "OUT";
    break;
  case INOUT:
    os << "INOUT";
    break;
  case BUFFER:
    os << "BUFFER";
    break;
  case LINKAGE:
    os << "LINKAGE";
    break;
  default:
    os << "(invalid PortType)";
    break;
  }
  return os;
}


ostream&
operator<<(ostream& os, const VarType& vt) {
  switch (vt) {
  case NORMAL:
    os << "NORMAL";
    break;
  case ACCESS:
    os << "ACCESS";
    break;
  default:
    os << "(invalid VarType)";
    break;
  }
  return os;
}


    
LONG
str_to_long_long(const char *str) {
  int i = 0;
  long long int retval = 0;

  while(str[i] != '\0') {
    retval += 10 * retval + (str[i++] - '0');
  }

  return retval;
}

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

#include "IntegerLiteral.hh"
#include "UniversalInteger.hh"
#include "AccessObject.hh"
#include "std_standardPkg.hh"
#include <clutils/StringUtilities.h>
#include <sstream>
using std::stringstream;
using std::ostringstream;

IntegerLiteral::IntegerLiteral() :
  val(std_standard_integerTypeInfo().left().getIntValue()){}

IntegerLiteral::IntegerLiteral(const ScalarTypeInfo &initTypeInfo) :
  val(initTypeInfo.left().getIntValue()){}

bool
IntegerLiteral::operator==( const RValue &compareTo ) const {
  return (getIntValue() == compareTo.getIntValue());
}

bool
IntegerLiteral::operator!=( const RValue &compareTo ) const {
  return (getIntValue() != compareTo.getIntValue());
}

bool
IntegerLiteral::operator > ( const RValue &compareTo ) const{
  return (getIntValue() > compareTo.getIntValue());
}

bool
IntegerLiteral::operator >= ( const RValue &compareTo ) const {
  return (getIntValue() >= compareTo.getIntValue());
}

bool
IntegerLiteral::operator < ( const RValue &compareTo ) const {
  return (getIntValue() < compareTo.getIntValue());
}

bool
IntegerLiteral::operator <= ( const RValue &compareTo ) const {
  return (getIntValue() <= compareTo.getIntValue());
}

void
IntegerLiteral::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "Value = " << val;

  return;
}

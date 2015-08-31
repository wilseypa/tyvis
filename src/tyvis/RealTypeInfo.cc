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

#include "RealTypeInfo.hh"
#include <clutils/StringUtilities.h>

//The constructor for RealTypeInfo
RealTypeInfo::RealTypeInfo() :
  ScalarTypeInfo(){}

RealTypeInfo::RealTypeInfo(const RealTypeInfo &that) :
  ScalarTypeInfo( that ){}

RealTypeInfo&
RealTypeInfo::operator=(const RealTypeInfo &that) {
  ScalarTypeInfo::operator=( that );
  return *this;
}

TypeInfo *
RealTypeInfo::clone() const {
  return new RealTypeInfo( *this );
}

RealTypeInfo::RealTypeInfo( double l,
			    ArrayInfo::ArrayDirn_t d,
			    double r ) :
  ScalarTypeInfo( UniversalReal( l ), d, UniversalReal( r ) ){}

RealTypeInfo::RealTypeInfo( const RValue &lbound,
			    ArrayInfo::ArrayDirn_t d,
			    const RValue &rbound) :
  ScalarTypeInfo( lbound, d, rbound ){}

Value
RealTypeInfo::vhdlPlus( const RValue &left, const RValue &right ) const {
  const UniversalReal &leftReal = dynamic_cast<const UniversalReal &>(left.readVal());
  return Value(leftReal.vhdlPlus(right));
}

Value
RealTypeInfo::vhdlMinus( const RValue &left, const RValue &right ) const {
  const UniversalReal &leftReal = dynamic_cast<const UniversalReal &>(left.readVal());
  return Value(leftReal.vhdlMinus(right));
}

Value
RealTypeInfo::vhdlMultiply( const RValue &left, const RValue &right ) const {
  const UniversalReal &leftReal = dynamic_cast<const UniversalReal &>(left.readVal());
  return Value(leftReal.vhdlMultiply(right));
}

Value
RealTypeInfo::vhdlDivide( const RValue &left, const RValue &right ) const {
  const UniversalReal &leftReal = dynamic_cast<const UniversalReal &>(left.readVal());
  return Value(leftReal.vhdlDivide(right));
}

Value
RealTypeInfo::vhdlMod( const RValue &left, const RValue &right ) const {
  abort();
}
Value
RealTypeInfo::vhdlRem( const RValue &left, const RValue &right ) const {
  abort();
}
Value
RealTypeInfo::vhdlPow( const RValue &left, const RValue &right ) const {
  const UniversalReal &leftReal = dynamic_cast<const UniversalReal &>(left.readVal());
  return Value(leftReal.vhdlPow(right));
}

RValue *
RealTypeInfo::create( const SubElementFactory * ) const {
  return new UniversalReal( left() );
}

Value
RealTypeInfo::value( const string &initValue ) const {
  int num = 0;
  return Value( UniversalReal( stringToDouble( initValue, num ) ) );
}

void
RealTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  ScalarTypeInfo::debugPrintDetails(os, indent);
  return;
}

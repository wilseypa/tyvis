// Copyright (c) Clifton Labs, Inc.  All Rights Reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "std_standardPkg.hh"
#include "UniversalInteger.hh"
#include "UniversalReal.hh"
#include "PhysicalLiteral.hh"
#include "AccessObject.hh"
#include <clutils/StringUtilities.h>
#include <sstream>
#include <cmath>
#include <typeinfo>

using std::stringstream;
using std::ostringstream;

UniversalInteger::UniversalInteger( const TypeInfo &, const RValue &i) :
  IntegerLiteral(i){}

UniversalInteger::UniversalInteger(const RValue &i) :
  IntegerLiteral(i){}


Value
UniversalInteger::vhdlUnaryPlus() const  {
  return *this;
}

Value
UniversalInteger::vhdlUnaryMinus() const {
  return UniversalInteger( -1*getIntValue() );
}


UniversalInteger
UniversalInteger::vhdlAbs() const {
  return UniversalInteger( ::abs(getIntValue()));
}

Value
UniversalInteger::vhdlPlus( const RValue &rhs ) const {
  return UniversalInteger( getIntValue() + rhs.getIntValue() );
}

Value
UniversalInteger::vhdlMinus( const RValue &rhs ) const {
  return UniversalInteger( getIntValue() - rhs.getIntValue() );
}

Value
UniversalInteger::vhdlMultiply( const RValue &rhs ) const {
  if (typeid(rhs.readVal()) == typeid(PhysicalLiteral)) {
    return rhs.readVal().vhdlMultiply(*this);
  }
  return UniversalInteger( getIntValue() * rhs.getIntValue() );
}

Value
UniversalInteger::vhdlDivide( const RValue &rhs ) const {
  return UniversalInteger( getIntValue() / rhs.getIntValue() );
}
// VHDL LRM Section 7.2.6: 
// The result of the modulus operation is such that (A mod B) has the sign
// of B and an absolute value less than the absolute vaue of B; in
// addition, for some integer value N, this result must satisfy the
// relation:
//     A = B*N + (A mod B)
Value
UniversalInteger::vhdlMod( const RValue &rhs ) const {
  int lval = getIntValue();
  int rval = rhs.getIntValue();
  return UniversalInteger((lval*rval < 0) ? (rval + lval%rval) : (lval%rval));
}
// VHDL LRM Section 7.2.6: 
// Integer division and remainder are defined by the following relation:
//     A = (A/B)*B + (A rem B)
// where (A rem B) has the sign of A and an absolute value less than the
// absolute value of B.
Value
UniversalInteger::vhdlRem( const RValue &rhs ) const {
  return UniversalInteger( getIntValue() % rhs.getIntValue() );
}

Value
UniversalInteger::vhdlPow( const RValue &rhs ) const {
  return UniversalInteger( ::pow(getDoubleValue(), 
				 rhs.getDoubleValue() ));
}

// The following functions are necessary since bit is an enum type and all
// enums are implemented as integers within the kernel.
//The following functons are only for bit types

Value
UniversalInteger::vhdlAnd( const RValue &rhs ) const {
  return UniversalInteger( getIntValue() & rhs.getIntValue() );
}

Value
UniversalInteger::vhdlOr( const RValue &rhs ) const {
  return UniversalInteger( getIntValue() | rhs.getIntValue() );
}

Value
UniversalInteger::vhdlNand( const RValue &rhs ) const {
  return UniversalInteger( 1 - ( getIntValue() & rhs.getIntValue() ) );
}

Value
UniversalInteger::vhdlNor( const RValue &rhs ) const {
  return UniversalInteger( 1 - ( getIntValue() | rhs.getIntValue() ) );
}

Value
UniversalInteger::vhdlXor( const RValue &rhs ) const {
  return UniversalInteger( getIntValue() != rhs.getIntValue() );
}

Value
UniversalInteger::vhdlXnor( const RValue &rhs ) const {
  return UniversalInteger( getIntValue() == rhs.getIntValue() );
}

Value
UniversalInteger::vhdlNot() const {
  return UniversalInteger(!getIntValue());
}

const TypeInfo &
UniversalInteger::getTypeInfo() const {
  return std_standard_integerTypeInfo();
}

UniversalInteger &
UniversalInteger::operator++(int){
  val++;
  return *this;
}

UniversalInteger &
UniversalInteger::operator=( const UniversalInteger &that ){
  val = that.getIntValue();
  return *this;
}

RValue &
UniversalInteger::operator=( const RValue &that ){
  val = that.getIntValue();
  return *this;
}

void
UniversalInteger::print( ostream &os ) const {
  os << getIntValue();
}

void
UniversalInteger::vcdPrint( ostream &os ) const {
  unsigned int i = getIntValue();
  os << "b";
  if (i == 0) {
    os << "0";
  }
  else {
    string bin;
    while (i != 0) {
      if ( i & 1 ) {
        bin.append("1");
      }
      else {
        bin.append("0");
      }
      i >>= 1;
    }
    for ( int j=bin.size()-1 ; j>=0 ; --j ) {
      os << bin[j];
    }
  }
  os << " ";
}

string
UniversalInteger::toString() const {
  return intToString( getIntValue() );
}

// This is strange for this to be here, but it's to prevent circular
// dependencies in the headers.

const RValue &
RValue::defaultIndex(){
  static UniversalInteger retval(1);
  return retval;
}


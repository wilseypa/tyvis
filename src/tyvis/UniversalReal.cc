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

#include "UniversalReal.hh"
#include "AccessObject.hh"
#include "std_standardPkg.hh"
#include <clutils/StringUtilities.h>
#include <cmath>
#include <sstream>
#include <typeinfo>

using std::ostringstream;
using std::stringstream;

UniversalReal::UniversalReal() :
  myValue( getMin() ){}

UniversalReal::UniversalReal(double d) :
  myValue( d ){}

UniversalReal::UniversalReal(int value) :
  myValue( (double)value ){}

UniversalReal::UniversalReal( const RValue &d ) :
  VHDLData(),
  myValue( d.getDoubleValue() ){}

UniversalReal::UniversalReal( const TypeInfo &,
			      const RValue &initValue ) :
  VHDLData(),
  myValue( initValue.getDoubleValue() ){}


// Here, (DBL_MAX > 1.0E38) has to hold, Required by LRM.
const UniversalReal &
UniversalReal::getMax(){
  static UniversalReal max(1.0E38);
  return max;
}

const UniversalReal &
UniversalReal::getMin(){
  static UniversalReal min(-1.0E38);
  return min;
}

Value
UniversalReal::vhdlUnaryPlus() const {
  return *this;
}

Value
UniversalReal::vhdlUnaryMinus() const {
  return UniversalReal( -1.0 * getDoubleValue() );
}


UniversalReal
UniversalReal::vhdlAbs() const {
  return UniversalReal( fabs( getDoubleValue() ));
}

Value
UniversalReal::vhdlPlus( const RValue &rhs ) const {
  return UniversalReal( getDoubleValue() + rhs.getDoubleValue() );
}

Value
UniversalReal::vhdlMinus( const RValue &rhs ) const {
  return UniversalReal( getDoubleValue() - rhs.getDoubleValue() );
}

Value
UniversalReal::vhdlMultiply( const RValue &rhs) const {
  if (typeid(rhs.readVal()) == typeid(PhysicalLiteral)) {
    return rhs.readVal().vhdlMultiply(*this);
  }
  return UniversalReal( getDoubleValue() * rhs.getDoubleValue() );
}

Value
UniversalReal::vhdlDivide(const RValue &rhs ) const {
  return UniversalReal(getDoubleValue() / rhs.getDoubleValue());
}

Value
UniversalReal::vhdlPow( const RValue &rhs ) const {
  return UniversalReal( ::pow(getDoubleValue(), rhs.getDoubleValue()));
}

Value
UniversalReal::vhdlMod( const RValue & ) const {
  abort();
}

Value
UniversalReal::vhdlRem( const RValue & ) const {
  abort();
}


const TypeInfo &
UniversalReal::getTypeInfo() const {
  return std_standard_realTypeInfo();
}

string
UniversalReal::toString() const {
  return doubleToString( getDoubleValue() );
}

void
UniversalReal::print( ostream &os ) const {
  os << getDoubleValue();
}

void
UniversalReal::vcdPrint( ostream &os ) const {
  os << "r" << getDoubleValue() << " ";
}

void
UniversalReal::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "Value = " << myValue;
 
  return;
}

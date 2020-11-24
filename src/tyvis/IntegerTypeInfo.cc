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

#include "IntegerTypeInfo.hh"
#include "UniversalInteger.hh"
#include <StringUtilities.h>
#include <climits>

//The constructor for IntegerTypeInfo
IntegerTypeInfo::IntegerTypeInfo() :
  ScalarTypeInfo(){}

IntegerTypeInfo::IntegerTypeInfo(const IntegerTypeInfo &that ) :
  ScalarTypeInfo( that ){}

IntegerTypeInfo::IntegerTypeInfo( int initLeft,
				  ArrayInfo::ArrayDirn_t initDirection,
				  int initRight ) :
  ScalarTypeInfo( new UniversalInteger( initLeft ),
		  initDirection,
		  new UniversalInteger( initRight ) ){}

IntegerTypeInfo::IntegerTypeInfo( const RValue &lbound,
				  ArrayInfo::ArrayDirn_t d,
				  const RValue &rbound ) :
  ScalarTypeInfo( lbound, d, rbound ){
}

IntegerTypeInfo &
IntegerTypeInfo::operator=( const IntegerTypeInfo &ri ){
  ScalarTypeInfo::operator=( ri );
  
  return *this;
}

TypeInfo *
IntegerTypeInfo::clone() const {
  return new IntegerTypeInfo( *this );
}

RValue *
IntegerTypeInfo::create( const SubElementFactory * ) const {
  return new UniversalInteger( left() );
}

const IntegerLiteral &
IntegerTypeInfo::getMax(){
  static UniversalInteger max( INT_MAX );
  return max;
}

const IntegerLiteral &
IntegerTypeInfo::getMin(){
  // Putting INT_MIN in here causes some problems with default settings
  // on the compiler.  Also note that this should be the same number as
  // found in 
  static UniversalInteger min( -INT_MAX );
  return min;
}

Value 
IntegerTypeInfo::vhdlPlus( const RValue &left, const RValue &right ) const {
  const UniversalInteger &leftInt = dynamic_cast<const UniversalInteger &>(left.readVal());
  return Value(leftInt.vhdlPlus(right));
}

Value 
IntegerTypeInfo::vhdlMinus( const RValue &left, const RValue &right ) const {
  const UniversalInteger &leftInt = dynamic_cast<const UniversalInteger &>(left.readVal());
  return Value(leftInt.vhdlMinus(right));
}

Value 
IntegerTypeInfo::vhdlMultiply( const RValue &left, const RValue &right ) const {
  const UniversalInteger &leftInt = dynamic_cast<const UniversalInteger &>(left.readVal());
  return Value(leftInt.vhdlMultiply(right));
}

Value 
IntegerTypeInfo::vhdlDivide( const RValue &left, const RValue &right ) const {
  const UniversalInteger &leftInt = dynamic_cast<const UniversalInteger &>(left.readVal());
  return Value(leftInt.vhdlDivide(right));
}

Value 
IntegerTypeInfo::vhdlMod( const RValue &left, const RValue &right ) const {
  const UniversalInteger &leftInt = dynamic_cast<const UniversalInteger &>(left.readVal());
  return Value(leftInt.vhdlMod(right));

}

Value 
IntegerTypeInfo::vhdlRem( const RValue &left, const RValue &right ) const {
  const UniversalInteger &leftInt = dynamic_cast<const UniversalInteger &>(left.readVal());
  return Value(leftInt.vhdlRem(right));
}

Value 
IntegerTypeInfo::vhdlPow( const RValue &left, const RValue &right ) const {
  const UniversalInteger &leftInt = dynamic_cast<const UniversalInteger &>(left.readVal());
  return Value(leftInt.vhdlPow(right));
}

Value
IntegerTypeInfo::val( const RValue &expression ) const{
  return Value(UniversalInteger( expression ));
}

Value
IntegerTypeInfo::succ( const RValue &expression ) const {
  return Value( UniversalInteger( expression.getIntValue() + 1 ) );
}

Value
IntegerTypeInfo::pred( const RValue &expression ) const {
  return Value( UniversalInteger( expression.getIntValue() - 1 ) );
}

Value
IntegerTypeInfo::value( const string &initValue ) const {
  int len = 0;
  return Value( UniversalInteger( (int)stringToLong( initValue, len ) ) );
}

void
IntegerTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  ScalarTypeInfo::debugPrintDetails(os, indent);
  return;
}

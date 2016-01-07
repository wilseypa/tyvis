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

#include "FileObject.hh"
#include "PhysicalTypeInfo.hh"
#include "PhysicalLiteral.hh"
#include "std_standardPkg.hh"
#include <warped.h>
#include <StringUtilities.h>
#include <cmath>

//The constructors for PhysicalTypeInfo
PhysicalTypeInfo::PhysicalTypeInfo() :
  ScalarTypeInfo(){}

PhysicalTypeInfo::PhysicalTypeInfo( const IntegerTypeInfo &range,
				    const vector<UnitDeclaration> &units ) : 
  ScalarTypeInfo( range ),
  myUnits( units ){}
// DRH - Need to separately addUnits() to avoid recursion problem in g++-4.0
PhysicalTypeInfo::PhysicalTypeInfo( const IntegerTypeInfo &range ) :
  ScalarTypeInfo( range ){}

PhysicalTypeInfo::PhysicalTypeInfo( const vector<UnitDeclaration> &units ) : 
  ScalarTypeInfo(),
  myUnits( units ){
  // Can't use an initializer list since "this" is not yet constructed
  setLeft( getMin(*this) );
  setDirection( ArrayInfo::to );
  setRight( getMax(*this) );
}

PhysicalTypeInfo::PhysicalTypeInfo( const PhysicalTypeInfo &that ) :
  ScalarTypeInfo( that ),
  myUnits( that.getUnits() ){}

TypeInfo *
PhysicalTypeInfo::clone() const {
  return new PhysicalTypeInfo( *this );
}

PhysicalTypeInfo&
PhysicalTypeInfo::operator=(const PhysicalTypeInfo &that ){
  ScalarTypeInfo::operator=( that );
  myUnits          = that.getUnits();

  return *this;
}

const vector<UnitDeclaration> &
PhysicalTypeInfo::getUnits() const {
  return myUnits;
}

const string &
PhysicalTypeInfo::getPrimaryUnit() const {
  if( myUnits.size() > 0 ){
    ASSERT( myUnits[0].getIdentifier() != "" );
    ASSERT( myUnits[0].getIdentifier() == myUnits[0].getMultiplier().getUnit() );
    return myUnits[0].getIdentifier();
  }
  else{
    static string empty("");
    return empty;
  }
}

void
PhysicalTypeInfo::addUnits( const vector<UnitDeclaration> &unitList ){
  myUnits = unitList;
}


RValue *
PhysicalTypeInfo::create( const SubElementFactory * ) const {
  return new PhysicalLiteral( *this );
}

const PhysicalLiteral
PhysicalTypeInfo::getMax( const PhysicalTypeInfo &typeInfo ){
  return PhysicalLiteral( typeInfo, getWarped64Max() );
}

const PhysicalLiteral
PhysicalTypeInfo::getMin( const PhysicalTypeInfo &typeInfo ){
  return PhysicalLiteral( typeInfo, getWarped64Min() );
}

const PhysicalLiteral &
PhysicalTypeInfo::getZeroTime(){
  static PhysicalLiteral zeroTime( std_standard_timeTypeInfo(), UniversalInteger(0) );
  return zeroTime;
}

const PhysicalLiteral &
PhysicalTypeInfo::getInfinityTime(){
  static PhysicalLiteral infinityTime(PhysicalTypeInfo::getMax(std_standard_timeTypeInfo()));
  return infinityTime;
}

LONG
PhysicalTypeInfo::getBaseMultiplier( const string &unitName ) const {
  LONG retval = 1LL;
  for( vector<UnitDeclaration>::const_iterator i = myUnits.begin();
       i < myUnits.end();
       i++ ){
    retval = retval * (*i).getMultiplier().getValue();
    if( stringCaseCompare( (*i).getIdentifier(), unitName ) ){
      break;
    }
  }
  return retval;
}

Value
PhysicalTypeInfo::value( FileObject &file ) const {
  int endPos;
  string valueToken = file.nextToken();
  string unitToken  = file.nextToken();
  LONG value = stringToLong( valueToken, endPos );
  return PhysicalLiteral( *this, value, unitToken );
}

Value
PhysicalTypeInfo::value( const string &initValue ) const {
  cerr << "PhysicalTypeInfo::value( " << initValue << " ) unimplemented!" << endl;
  abort();
  return UniversalInteger(0);
}

Value
PhysicalTypeInfo::vhdlPlus( const RValue &left, const RValue &right ) const {
  return PhysicalLiteral( *this,
                          left.getInt64Value() + right.getInt64Value() );
}

Value
PhysicalTypeInfo::vhdlMinus( const RValue &left, const RValue &right ) const {
  return PhysicalLiteral( *this,
                          left.getInt64Value() - right.getInt64Value() );
}

Value
PhysicalTypeInfo::vhdlUnaryPlus( const RValue &that ) const {
  return PhysicalLiteral( *this,
                          that.getInt64Value() );
}

Value
PhysicalTypeInfo::vhdlUnaryMinus( const RValue &that ) const {
  return PhysicalLiteral( *this,
                          -1 * that.getInt64Value() );
}

Value
PhysicalTypeInfo::vhdlMultiply( const RValue &left, const RValue &right ) const {
  return PhysicalLiteral( *this,
                          left.getInt64Value() * right.getInt64Value() );
}

Value
PhysicalTypeInfo::vhdlDivide( const RValue &left, const RValue &right ) const {
  return PhysicalLiteral( *this,
                          left.getInt64Value() / right.getInt64Value() );
}

Value
PhysicalTypeInfo::vhdlMod( const RValue &left, const RValue &right ) const {
  int lval = left.getIntValue();
  int rval = right.getIntValue();
  return PhysicalLiteral( *this,
                          (lval*rval < 0 ) ? (rval + lval%rval) : (lval%rval));
}

Value
PhysicalTypeInfo::vhdlRem( const RValue &left, const RValue &right ) const {
  return PhysicalLiteral( *this,
                          left.getIntValue() % right.getIntValue() );
}

Value
PhysicalTypeInfo::vhdlPow( const RValue &left, const RValue &right ) const {
  return PhysicalLiteral( *this,
  			  (LONG)(::pow(left.getDoubleValue(), right.getDoubleValue())));
}

Value
PhysicalTypeInfo::val( const RValue &expression ) const {
  return Value( PhysicalLiteral( *this, expression ) );
}

Value
PhysicalTypeInfo::succ( const RValue &expression ) const {
  return Value( PhysicalLiteral( *this, expression.getIntValue() + 1 ) );
}

Value
PhysicalTypeInfo::pred( const RValue &expression ) const {
  return Value( PhysicalLiteral( *this, expression.getIntValue() - 1 ) );
}

void
PhysicalTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  ScalarTypeInfo::debugPrintDetails(os, indent);
  os << NL(indent) << "vector<UnitDeclaration> myUnits (size = ";
  os << myUnits.size() << "):";
  for (int i=0 ; i<myUnits.size() ; ++i)
    os << NL(indent) << myUnits[i].getIdentifier();
  return;
}

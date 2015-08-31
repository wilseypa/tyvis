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

#include "Alias.hh"
#include "Value.hh"
#include "TypeInfo.hh"
#include "std_standardPkg.hh"

#include <iostream>
using std::cerr;
using std::endl;

Alias::Alias( RValue &initObject ) :
  realObject( initObject ),
  a(1),
  b(0),
  myInd(0) {
}

Alias::Alias( const ArrayInfo &info, RValue &initObject ) :
  realObject( const_cast<RValue &>(initObject) ){
  if (initObject.isCompositeType()) {
    ASSERT (info.length() == initObject.length().getIntValue());
    if (info.isAscending()) {
      calculateAB(info.left(), info.left() +
            abs(realObject.left().getIntValue()-realObject.right().getIntValue()),
            realObject.left().getIntValue(), realObject.right().getIntValue());
    }
    else {
      calculateAB(info.left(), info.left() -
            abs(realObject.left().getIntValue()-realObject.right().getIntValue()),
            realObject.left().getIntValue(), realObject.right().getIntValue());
    }
  }
  else {
    a = 1;
    b = 0;
    myInd = 0;
  }
}

Alias::Alias( const ArrayInfo &info, Value initObject ) :
  realObject( *(new Value(&initObject.readVal())) ){
  initObject.saveMyData();
  if (initObject.isCompositeType()) {
    ASSERT (info.length() == initObject.length().getIntValue());
    if (info.isAscending()) {
      calculateAB(info.left(), info.left() +
            abs(realObject.left().getIntValue()-realObject.right().getIntValue()),
            realObject.left().getIntValue(), realObject.right().getIntValue());
    }
    else {
      calculateAB(info.left(), info.left() -
            abs(realObject.left().getIntValue()-realObject.right().getIntValue()),
            realObject.left().getIntValue(), realObject.right().getIntValue());
    }
  }
  else {
    a = 1;
    b = 0;
    myInd = 0;
  }
}

Alias::Alias( const ArrayTypeInfo &info, RValue &initObject ) :
  realObject( const_cast<RValue &>(initObject) ){
  if (initObject.isCompositeType()) {
    ASSERT (info.length() == initObject.length().getIntValue());
    if (info.isConstrained()) {
      calculateAB(info.left().getIntValue(), info.right().getIntValue(),
                realObject.left().getIntValue(), realObject.right().getIntValue());
    }
    else {
      calculateAB(info.left().getIntValue(), info.left().getIntValue() +
            abs(realObject.left().getIntValue()-realObject.right().getIntValue()),
            realObject.left().getIntValue(), realObject.right().getIntValue());
    }
  }
  else {
    a = 1;
    b = 0;
    myInd = 0;
  }
}

Alias::Alias( const Alias &that ) :
  LValue( that ),
  realObject( that.realObject ){
}

Alias::~Alias(){
  if (myInd)
    delete myInd;
}

const RValue &
Alias::readVal() const {
  return realObject.readVal();
}

RValue &
Alias::readVal(){
  ASSERT( dynamic_cast<const LValue *>( &realObject ) != 0 );
  return const_cast<RValue &>((realObject).readVal());
}

void
Alias::updateVal( const RValue &newVal ){
  ASSERT( dynamic_cast<const LValue *>( &realObject ) != 0 );
  dynamic_cast<LValue &>(realObject).updateVal( newVal );
}


int
Alias::getIntValue() const {
  return realObject.getIntValue();
}

LONG
Alias::getInt64Value() const {
  return realObject.getInt64Value();
}

double
Alias::getDoubleValue() const {
  return realObject.getDoubleValue();
}

RValue &
Alias::operator=( const RValue &that ) {
  realObject.operator=( that );
  return *this;
}

Alias &
Alias::operator=( const Alias &that ){
  operator=( that.readVal() );
  return *this;
}

bool
Alias::operator==( const RValue &compareTo ) const {
  return realObject.operator==( compareTo );
}

bool
Alias::operator!=( const RValue &compareTo ) const {
  return realObject.operator!=( compareTo );
}

bool
Alias::operator>( const RValue &compareTo ) const {
  return realObject.operator>( compareTo );
}

bool
Alias::operator>=( const RValue &compareTo ) const {
  return realObject.operator>=( compareTo );
}

bool
Alias::operator<( const RValue &compareTo ) const {
  return realObject.operator<( compareTo );
}

bool
Alias::operator<=( const RValue &compareTo ) const {
  return realObject.operator<=( compareTo );
}

void
Alias::print( ostream &os ) const {
  realObject.print(os);
}

string 
Alias::toString() const {
  return realObject.toString();
}

bool
Alias::isCompositeType() const {
  return realObject.isCompositeType();
}

bool
Alias::isScalarType() const {
  return realObject.isScalarType();
}

bool
Alias::isPhysicalType() const {
  return realObject.isPhysicalType();
}

Value
Alias::left( const RValue &param ) const {
  if (myInd)
    return myInd->left( param );
  return realObject.left( param );
}

Value
Alias::right( const RValue &param ) const {
  if (myInd)
    return myInd->right( param );
  return realObject.right( param );
}

Value
Alias::high( const RValue &param ) const {
  if (myInd)
    return myInd->high( param );
  return realObject.high( param );
}

Value
Alias::low( const RValue &param ) const {
  if (myInd)
    return myInd->low( param );
  return realObject.low( param );
}

const ScalarTypeInfo &
Alias::range( const RValue &index ) const {
  if (myInd)
    return myInd->range( index );
  return realObject.range( index );
}

const ScalarTypeInfo &
Alias::reverse_range( const RValue &index ) const {
  if (myInd)
    return myInd->reverse_range( index );
  return realObject.reverse_range( index );
}

Value
Alias::length( const RValue &index ) const {
  return realObject.length( index );
}

Value
Alias::ascending( const RValue &index ) const {
  if (myInd)
    return myInd->ascending( index );
  return realObject.ascending( index );
}

const RValue &
Alias::operator[]( int index ) const {
  return realObject.operator[](ind(index));
}

RValue &
Alias::operator[]( int index ) {
  return realObject.operator[](ind(index));
}

const RValue &
Alias::operator[]( const RValue &index ) const {
  return realObject.operator[](ind(index));
}

RValue &
Alias::operator[]( const RValue &index ){
  return realObject.operator[](ind(index));
}

const RValue &
Alias::getField( int index ) const {
  return realObject.getField(index);
}

RValue &
Alias::getField( int index ){
  return realObject.getField(index);
}

const RValue &
Alias::getField( const RValue &index ) const {
  return realObject.getField(index);
}

RValue &
Alias::getField( const RValue &index ){
  return realObject.getField(index);
}

const string &
Alias::getUnit() const {
  
  return realObject.getUnit();
}

RValue *
Alias::clone() const {
  return new Alias( *this );
}

LONG
Alias::getValue() const {
  return realObject.getValue();
}

int
Alias::getUnits() const {
  return realObject.getUnits();
}

const TypeInfo &
Alias::getTypeInfo() const {
  return realObject.getTypeInfo();
}

const EnumerationLiteral &
Alias::vhdlEqual( const RValue &that ) const {
  return realObject.vhdlEqual( that );
}

const EnumerationLiteral &
Alias::vhdlNotEqual( const RValue &that ) const {
  return realObject.vhdlNotEqual( that );
}

const EnumerationLiteral &
Alias::vhdlGreaterThan( const RValue &that ) const {
  return realObject.vhdlGreaterThan( that );
}

const EnumerationLiteral &
Alias::vhdlGreaterThanEqual( const RValue &that ) const{
  return realObject.vhdlGreaterThanEqual( that );
}

const EnumerationLiteral &
Alias::vhdlLessThan( const RValue &that ) const{
  return realObject.vhdlLessThan( that );
}

const EnumerationLiteral &
Alias::vhdlLessThanEqual( const RValue &that ) const{
  return realObject.vhdlLessThanEqual( that );
}

Value
Alias::vhdlConcatenate( const RValue &that ) const {
  return realObject.vhdlConcatenate( that );
}

Value
Alias::vhdlUnaryPlus( ) const {
  return realObject.vhdlUnaryPlus( );
}

Value
Alias::vhdlUnaryMinus( ) const {
  return realObject.vhdlUnaryMinus( );
}

Value
Alias::vhdlPlus( const RValue &that ) const {
  return realObject.vhdlPlus( that );
}

Value
Alias::vhdlMinus( const RValue &that ) const {
  return realObject.vhdlMinus( that );
}

Value
Alias::vhdlMultiply( const RValue &that ) const {
  return realObject.vhdlMultiply( that );
}

Value
Alias::vhdlDivide( const RValue &that ) const {
  return realObject.vhdlDivide( that );
}

Value
Alias::vhdlMod( const RValue &that ) const {
  return realObject.vhdlMod( that );
}

Value
Alias::vhdlRem( const RValue &that ) const {
  return realObject.vhdlRem( that );
}

Value
Alias::vhdlPow( const RValue &that ) const {
  return realObject.vhdlPow( that );
}


RValue &
Alias::all(){
  abort();
}

const RValue &
Alias::all() const {
  abort();
}


int
Alias::ind (const int index) const {
  return a * index + b;
}

int
Alias::ind (const RValue & index) const {
  return a * index.getIntValue() + b;
}

int
Alias::ind (const int index) {
  return a * index + b;
}

int
Alias::ind (const RValue & index) {
  return a * index.getIntValue() + b;
}

void
Alias::calculateAB( int left, int right, int leftR, int rightR ) {
  if (left > right) {
    if (leftR > rightR)
      a = 1;
    else
      a = -1;
  }
  else {
    if (leftR > rightR)
      a = -1;
    else
      a = 1;
  }
  b = leftR - a*left;
  ArrayTypeInfo *ati;
  if (left > right)
    ati = new ArrayTypeInfo(std_standard_stringTypeInfo(), IntegerTypeInfo (UniversalInteger(left),
                ArrayInfo::downto, UniversalInteger(right)));
  else
    ati = new ArrayTypeInfo(std_standard_stringTypeInfo(), IntegerTypeInfo (UniversalInteger(left),
                ArrayInfo::to, UniversalInteger(right)));
  myInd = new StringLiteral(*ati);
  delete ati;
}

void
Alias::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "a = " << a << "  b = " << b;
  os << NL(indent) << "RValue &realObject: ";
  realObject.debugPrint(os, indent+5);

  os << NL(indent) << "StringLiteral *myInd: ";
  if (myInd)
    myInd->debugPrint(os, indent+5);
  else
    os << " is null";
 
  return;
}

SignalNetinfo &
Alias::addToFanOut( VHDLKernel *process ){
  return dynamic_cast<LValue &>(realObject).addToFanOut( process );
}

SignalNetinfo &
Alias::addChild( SignalNetinfo &, VHDLKernel * ){
  cerr << "Alias::addChild( SignalNetinfo &, VHDLKernel * ) called - aborting!" << endl;
  abort();
}

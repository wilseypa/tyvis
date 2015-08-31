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

#include "Value.hh"
#include "tyvis/Aggregate.hh"
#include "tyvis/Element.hh"
#include "tyvis/StringLiteral.hh"
#include "tyvis/Variable.hh"
#include <iostream>
#include <typeinfo>

using std::cerr;
using std::endl;

Value::Value( const RValue &initData ) :
  myData( initData.clone() ),
  saveData(false) {}

Value::Value( RValue *initData ) :
  myData( initData ),
  saveData(false) {}

Value::Value( const Value &that ) :
  LValue( that ),
  saveData(false) {
  ASSERT( that.myData != 0 );
  myData = that.myData->clone();
}

Value::Value( RValue &parent, const ScalarTypeInfo &sliceSpecifier ) :
  saveData(false) {
  if (typeid(parent.readVal()) == typeid(Aggregate))
    myData = new Aggregate(dynamic_cast<Aggregate &>(const_cast<RValue &>
                 (parent.readVal())), sliceSpecifier);
  else if (typeid(parent.readVal()) == typeid(StringLiteral)) {
    myData = new StringLiteral(dynamic_cast<StringLiteral &>(const_cast<RValue &>
               (parent.readVal())), sliceSpecifier);
  }
  else if (typeid(parent.readVal()) == typeid(Variable) ||
           typeid(parent.readVal()) == typeid(Element)){
    if (typeid(parent.readVal().readVal()) == typeid(Aggregate))
      myData = new Aggregate(dynamic_cast<Aggregate &>(const_cast<RValue &>
                 (parent.readVal().readVal())), sliceSpecifier);
    else if (typeid(parent.readVal().readVal()) == typeid(StringLiteral))
      myData = new StringLiteral(dynamic_cast<StringLiteral &>(const_cast<RValue &>
                 (parent.readVal().readVal())), sliceSpecifier);
    else {
      cerr << "Value slice constructor for Element called with " <<
              typeid(parent.readVal().readVal()).name() << endl;
      ASSERT(false);
    }
  }
  else {
    cerr << "Value slice constructor called with " << typeid(parent.readVal()).name() << endl;
    ASSERT(false);
  }
}

Value::Value( Value parent, const ScalarTypeInfo &sliceSpecifier ) :
  saveData(false) {
  if (typeid(parent.readVal()) == typeid(Aggregate))
    myData = new Aggregate(dynamic_cast<Aggregate &>(const_cast<RValue &>
                 (parent.readVal())), sliceSpecifier);
  else if (typeid(parent.readVal()) == typeid(StringLiteral)) {
    myData = new StringLiteral(dynamic_cast<StringLiteral &>(const_cast<RValue &>
               (parent.readVal())), sliceSpecifier);
  }
  else if (typeid(parent.readVal()) == typeid(Variable) ||
           typeid(parent.readVal()) == typeid(Element)){
    if (typeid(parent.readVal().readVal()) == typeid(Aggregate))
      myData = new Aggregate(dynamic_cast<Aggregate &>(const_cast<RValue &>
                   (parent.readVal().readVal())), sliceSpecifier);
    else if (typeid(parent.readVal().readVal()) == typeid(StringLiteral)) {
      myData = new StringLiteral(dynamic_cast<StringLiteral &>(const_cast<RValue &>
                 (parent.readVal().readVal())), sliceSpecifier);
    }
    else {
      cerr << "Value slice constructor for Element called with " <<
              typeid(parent.readVal().readVal()).name() << endl;
      ASSERT(false);
    }
  }
  else {
    cerr << "Value slice constructor called with " << typeid(parent.readVal()).name() << endl;
    ASSERT(false);
  }
}

Value::Value() :
  myData(0),
  saveData(false) {}

Value::~Value(){
  if (!saveData)
    delete myData;
}

const RValue &
Value::readVal() const {
  ASSERT( myData != 0 );
  return myData->readVal();
}

RValue &
Value::readVal(){
  ASSERT( myData != 0 );
  return const_cast<RValue &>(myData->readVal());
}

void
Value::updateVal( const RValue &newValue ){
  if (myData)
    delete myData;
  myData = newValue.clone();
  return;
}

int
Value::getIntValue() const {
  ASSERT( myData != 0 );
  return myData->getIntValue();
}

LONG
Value::getInt64Value() const {
  ASSERT( myData != 0 );
  return myData->getInt64Value();
}

double
Value::getDoubleValue() const {
  ASSERT( myData != 0 );
  return myData->getDoubleValue();
}

RValue &
Value::operator=( const RValue &that ) {
  myData = that.clone();
  return *this;
}

Value &
Value::operator=( const Value &that ){
  operator=( that.readVal() );
  return *this;
}

bool
Value::operator==( const RValue &compareTo ) const {
  ASSERT( myData != 0 );
  return myData->operator==( compareTo );
}

bool
Value::operator!=( const RValue &compareTo ) const {
  ASSERT( myData != 0 );
  return myData->operator!=( compareTo );
}

bool
Value::operator>( const RValue &compareTo ) const {
  ASSERT( myData != 0 );
  return myData->operator>( compareTo );
}

bool
Value::operator>=( const RValue &compareTo ) const {
  ASSERT( myData != 0 );
  return myData->operator>=( compareTo );
}

bool
Value::operator<( const RValue &compareTo ) const {
  ASSERT( myData != 0 );
  return myData->operator<( compareTo );
}

bool
Value::operator<=( const RValue &compareTo ) const {
  ASSERT( myData != 0 );
  return myData->operator<=( compareTo );
}

void
Value::print( ostream &os ) const {
  ASSERT( myData != 0 );
  myData->print(os);
}

void
Value::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "RValue *myData: ";
  if (myData)
    myData->debugPrint(os, indent+5);
  else
    os << " is null";
  return;
}

string 
Value::toString() const {
  ASSERT( myData != 0 );
  return myData->toString();
}

void
Value::vcdPrint( ostream &os ) const {
  ASSERT( myData != 0 );
  myData->vcdPrint(os);
}

bool
Value::isCompositeType() const {
  ASSERT( myData != 0 );
  return myData->isCompositeType();
}

bool
Value::isScalarType() const {
  ASSERT( myData != 0 );
  return myData->isScalarType();
}

bool
Value::isPhysicalType() const {
  ASSERT( myData != 0 );
  return myData->isPhysicalType();
}

Value
Value::length( const RValue &index ) const {
  ASSERT( myData != 0 );
  return myData->length( index );
}

SignalNetinfo &
Value::addToFanOut( VHDLKernel *process ){
  ASSERT( myData != 0 );
  return dynamic_cast<LValue *>(myData)->addToFanOut( process );
}
//DRH
SignalNetinfo &
Value::addChild( SignalNetinfo &, VHDLKernel * ){
  cerr << "Value::addChild called - aborting!" << endl;
  abort();
}

const RValue &
Value::operator[]( int index ) const {
  ASSERT( myData != 0 );
  return myData->operator[](index);
}

RValue &
Value::operator[]( int index ) {
  ASSERT( myData != 0 );
  return myData->operator[](index);
}

const RValue &
Value::operator[]( const RValue &index ) const {
  ASSERT( myData != 0 );
  return myData->operator[](index);
}

RValue &
Value::operator[]( const RValue &index ){
  ASSERT( myData != 0 );
  return myData->operator[](index);
}

const RValue &
Value::getField( int index ) const {
  ASSERT( myData != 0 );
  return myData->getField(index);
}

RValue &
Value::getField( int index ){
  ASSERT( myData != 0 );
  return myData->getField(index);
}

const RValue &
Value::getField( const RValue &index ) const {
  ASSERT( myData != 0 );
  return myData->getField(index);
}

RValue &
Value::getField( const RValue &index ){
  ASSERT( myData != 0 );
  return myData->getField(index);
}

const string &
Value::getUnit() const {
  ASSERT( myData != 0 );
  return myData->getUnit();
}

RValue *
Value::clone() const {
  return new Value( *this );
}

LONG
Value::getValue() const {
  ASSERT( myData != 0 );
  return myData->getValue();
}

int
Value::getUnits() const {
  ASSERT( myData != 0 );
  return myData->getUnits();
}

const TypeInfo &
Value::getTypeInfo() const {
  ASSERT( myData != 0 );
  return myData->getTypeInfo();
}

const EnumerationLiteral &
Value::vhdlEqual( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlEqual( that );
}

const EnumerationLiteral &
Value::vhdlNotEqual( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlNotEqual( that );
}

const EnumerationLiteral &
Value::vhdlGreaterThan( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlGreaterThan( that );
}

const EnumerationLiteral &
Value::vhdlGreaterThanEqual( const RValue &that ) const{
  ASSERT( myData != 0 );
  return myData->vhdlGreaterThanEqual( that );
}

const EnumerationLiteral &
Value::vhdlLessThan( const RValue &that ) const{
  ASSERT( myData != 0 );
  return myData->vhdlLessThan( that );
}

const EnumerationLiteral &
Value::vhdlLessThanEqual( const RValue &that ) const{
  ASSERT( myData != 0 );
  return myData->vhdlLessThanEqual( that );
}

Value
Value::vhdlConcatenate( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlConcatenate( that );
}

Value
Value::vhdlUnaryPlus( ) const {
  ASSERT( myData != 0 );
  return myData->vhdlUnaryPlus( );
}

Value
Value::vhdlUnaryMinus( ) const {
  ASSERT( myData != 0 );
  return myData->vhdlUnaryMinus( );
}

Value
Value::vhdlPlus( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlPlus( that );
}

Value
Value::vhdlMinus( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlMinus( that );
}

Value
Value::vhdlMultiply( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlMultiply( that );
}

Value
Value::vhdlDivide( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlDivide( that );
}

Value
Value::vhdlMod( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlMod( that );
}

Value
Value::vhdlRem( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlRem( that );
}

Value
Value::vhdlPow( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlPow( that );
}

Value
Value::vhdlNot() const {
  ASSERT( myData != 0 );
  return myData->vhdlNot();
}

Value
Value::vhdlAnd( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlAnd( that );
}

Value
Value::vhdlOr( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlOr( that );
}

Value
Value::vhdlNand( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlNand( that );
}

Value
Value::vhdlNor( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlNor( that );
}

Value
Value::vhdlXor( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlXor( that );
}

Value
Value::vhdlXnor( const RValue &that ) const {
  ASSERT( myData != 0 );
  return myData->vhdlXnor( that );
}

Value
Value::vhdlSll( const RValue &rhs ) const {
  ASSERT( myData != 0 );
  return myData->vhdlSll( rhs );
}

Value
Value::vhdlSrl( const RValue &rhs ) const {
  ASSERT( myData != 0 );
  return myData->vhdlSrl( rhs );
}

Value
Value::vhdlSla( const RValue &rhs ) const {
  ASSERT( myData != 0 );
  return myData->vhdlSla( rhs );
}

Value
Value::vhdlSra( const RValue &rhs ) const {
  ASSERT( myData != 0 );
  return myData->vhdlSra( rhs );
}

Value
Value::vhdlRol( const RValue &rhs ) const {
  ASSERT( myData != 0 );
  return myData->vhdlRol( rhs );
}

Value
Value::vhdlRor( const RValue &rhs ) const {
  ASSERT( myData != 0 );
  return myData->vhdlRor( rhs );
}


RValue &
Value::all(){
  abort();
}

const RValue &
Value::all() const {
  abort();
}


Value
Value::left( const RValue &param ) const {
  ASSERT( myData != 0 );
  return myData->left( param );
}

Value
Value::right( const RValue &param ) const {
  ASSERT( myData != 0 );
  return myData->right( param );
}

Value
Value::high( const RValue &param ) const {
  ASSERT( myData != 0 );
  return myData->high( param );
}

Value
Value::low( const RValue &param ) const {
  ASSERT( myData != 0 );
  return myData->high( param );
}

const ScalarTypeInfo &
Value::range( const RValue &index ) const {
  ASSERT( myData != 0 );
  return myData->range( index );
}

const ScalarTypeInfo &
Value::reverse_range( const RValue &index ) const {
  ASSERT( myData != 0 );
  return myData->reverse_range( index );
}

Value
Value::ascending( const RValue &index ) const {
  ASSERT( myData != 0 );
  return myData->ascending( index );
}

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

#include <iostream>
#include "ArrayInfo.hh"
#include "VHDLData.hh"
#include "Types.hh"
#include "STDTypes.hh"
#include "AccessObject.hh"
#include "TyvisVarArgs.hh"
#include "Aggregate.hh"

using std::cerr;
using std::endl;

void
VHDLData::print( ostream& os ) const {
  os << toString();
}

void 
VHDLData::assignSlice(const RValue *that, const ArrayInfo* a1, const ArrayInfo *a2 ){
  cerr << "Error: Default assignSlice called! Arguments: ";
  that->print(cerr);
  cerr << " " << a1 << endl;
  cerr << " " << a2 << endl;
  abort();
} 

void 
VHDLData::assignSlice(const RValue *that, const ArrayInfo* a1) {
  cerr << "Error: Default assignSlice called! Arguments: ";
  that->print(cerr);
  cerr << " " << a1 << endl;
  abort();
} 

string
VHDLData::toString() const {
  cerr << "VHDLData::toString called" << endl;
  abort();
}

bool
VHDLData::isCompositeType() const {
  return false;
}

const RValue &
VHDLData::operator[]( int index ) const {
  return *this;
}

RValue &
VHDLData::operator[]( int index ){
  return *this;
}

const RValue &
VHDLData::operator[]( const RValue &index ) const {
  return *this;
}

RValue &
VHDLData::operator[]( const RValue &index ){
  return *this;
}

const RValue &
VHDLData::getField( int index ) const {
  return *this;
}

RValue &
VHDLData::getField( int index ){
  return *this;
}

const RValue &
VHDLData::getField( const RValue &index ) const {
  return getField( index.getIntValue() );
}

RValue &
VHDLData::getField( const RValue &index ){
  return getField( index.getIntValue() );
}

const string &
VHDLData::getUnit() const {
  static const string empty("");
  return empty;
}

bool
VHDLData::isPhysicalType() const {
  return false;
}

LONG
VHDLData::getValue() const {
  cerr << "VHDLData::getValue() called!  Should only be applied to physical types!" << endl;
  abort();
  return (LONG)0;
}

int
VHDLData::getUnits() const {
  cerr << "VHDLData::getUnits() called!  Should only be applied to physical types!" << endl;
  abort();
  return 0;
}

ostream &
operator << (ostream &os, const VHDLData &data) {
  data.print(os);
  return os;
}

const EnumerationLiteral &
VHDLData::vhdlEqual( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this == rhs );
}

const EnumerationLiteral &
VHDLData::vhdlNotEqual( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this != rhs );
}

const EnumerationLiteral &
VHDLData::vhdlGreaterThan( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this > rhs );
}

const EnumerationLiteral &
VHDLData::vhdlGreaterThanEqual( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this >= rhs );
}

const EnumerationLiteral &
VHDLData::vhdlLessThan( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this < rhs );
}

const EnumerationLiteral &
VHDLData::vhdlLessThanEqual( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this <= rhs );
}

Value
VHDLData::vhdlConcatenate( const RValue &rhs ) const {
  TyvisVarArgs args(RVALUE);
  args << *this << rhs;
  return Aggregate( args );
}

RValue &
VHDLData::all(){
  abort();
}

const RValue &
VHDLData::all() const {
  abort();
}

Value
VHDLData::length( const RValue &index ) const {
  ASSERT( index.getIntValue() == 1 );
  return UniversalInteger(1);
}

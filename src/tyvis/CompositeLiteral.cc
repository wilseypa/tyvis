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

#include "ArrayTypeInfo.hh"
#include "CompositeLiteral.hh"
#include "CompositeTypeInfo.hh"
#include "EnumerationLiteral.hh"
#include "EnumerationTypeInfo.hh"
#include "IntegerTypeInfo.hh"
#include "ObjectBase.hh"
#include "RangeIterator.hh"
#include "SignalNetinfo.hh"
#include "SubElementFactory.hh"
#include "TyvisVarArgs.hh"

#include <Iterator.h>
#include <iostream>
#include <set>
using std::cerr;
using std::endl;

CompositeLiteral::CompositeLiteral() :
  myTypeInfo(0),
  indexTypeInfo(0),
  deleteMembers(true),
  myFactory(0){}

CompositeLiteral::CompositeLiteral( const TypeInfo &initTypeInfo,
				    const RValue &initValue ) :
  myTypeInfo( dynamic_cast<CompositeTypeInfo *>(initTypeInfo.clone()) ),
  indexTypeInfo( 0 ),
  deleteMembers( true ),
  myFactory(0){
  for( int i = 0; i < initValue.length().getIntValue(); i++ ){
    RValue *current = initValue.getField(i).clone();
    append( current );
  }
}

CompositeLiteral::CompositeLiteral( const CompositeTypeInfo &initTypeInfo,
				    TyvisVarArgs &initValue ) :
  myTypeInfo( dynamic_cast<CompositeTypeInfo *>(initTypeInfo.clone()) ),
  indexTypeInfo( 0 ),
  deleteMembers( !initValue.isLValueArgs() ),
  myFactory(0){
  bool positional = false;
  if (initValue.hasNext()){
    if (!dynamic_cast<const ArrayTypeInfo *>(myTypeInfo) || !initTypeInfo.isConstrained() ||
        initValue.choice() == nullInfo()) {
      positional = true;
      while( initValue.hasNext() && initValue.choice() != others() ){
        RValue *current = initValue.next();
        append( current );
        myCleanUps.push_back(current);
      }
      if (initValue.hasNext() && initValue.choice() == others() )
        ASSERT (dynamic_cast<const ArrayTypeInfo *>(myTypeInfo) && initTypeInfo.isConstrained());
    }
    if ( dynamic_cast<const ArrayTypeInfo *>(myTypeInfo) && initTypeInfo.isConstrained() &&
         ( !positional || (positional && initValue.hasNext() && initValue.choice() == others())) ){
      int left = initTypeInfo.left().getIntValue();
      int right = initTypeInfo.right().getIntValue();
      int hi = left>right?left:right;
      int lo = left<right?left:right;
      int size = hi-lo+1;
      int start;
      bool filled[size];
      int i, curr;
      curr = 0;
      if (positional)
        start = myElements.size();
      else
        start = 0;
      myElements.resize(size);
      for (i=0 ; i<start ; ++i) {
        filled[i] = true;
      }
      for (i=start ; i<size ; ++i) {
        filled[i] = false;
        myElements[i] = 0;
      }
      while( initValue.hasNext() && initValue.choice() != others() ){
        ASSERT (initValue.choice() != nullInfo());
        int itLow = initValue.choice().left();
        int itHigh = initValue.choice().right();
        if (itLow > itHigh) {
          i = itHigh;
          itHigh = itLow;
          itLow = i;
        }
        ASSERT (itLow >= lo && itHigh <=hi);
        RValue *current = initValue.next();
        bool copied = false;
        for (i=itLow; i<=itHigh ; ++i) {
          curr = abs(i-left);
          copied = true;
          myElements[curr] = current;
          filled[curr] = true;
        }
        if (!copied && deleteMembers)
          delete current;
      }
      if ( initValue.hasNext() && initValue.choice() == others() ){
        RValue *current = initValue.next();
        bool copied = false;
        for (i=0 ; i<size ; ++i){
          if (!filled[i]){
            copied = true;
            myElements[i] = current;
          }
        }
        if (!copied && deleteMembers)
          delete current;
      }
    }
  }
}

CompositeLiteral::CompositeLiteral( TyvisVarArgs &initValue ) :
  myTypeInfo(0),
  indexTypeInfo( 0 ),
  deleteMembers( !initValue.isLValueArgs() ),
  myFactory(0){
  while( initValue.hasNext() ){
    RValue *current = initValue.next();
    append( current );
    myCleanUps.push_back(current);
  }
}

CompositeLiteral::CompositeLiteral( const CompositeTypeInfo &initTypeInfo ) :
  myTypeInfo( dynamic_cast<CompositeTypeInfo *>(initTypeInfo.clone()) ),
  indexTypeInfo( 0 ),
  deleteMembers(true),
  myFactory(0){
  if( initTypeInfo.isConstrained() ){
    buildElements( initTypeInfo, myElements, 0 );
  }
}

CompositeLiteral::CompositeLiteral( const SubElementFactory &subElementFactory ) :
  myTypeInfo( dynamic_cast<const CompositeTypeInfo *>( subElementFactory.getParentTypeInfo().clone()) ),
  indexTypeInfo( 0 ),
  deleteMembers(true),
  myFactory( subElementFactory.clone() ){
  if( myTypeInfo->isConstrained() ){
    buildElements( *myTypeInfo, myElements, &subElementFactory );
  }
}

CompositeLiteral::CompositeLiteral( const ArrayTypeInfo &typeInfo,
				    const string &initValue ) :
  myTypeInfo( dynamic_cast<CompositeTypeInfo *>(typeInfo.clone()) ),
  indexTypeInfo( 0 ),
  deleteMembers(true),
  myFactory(0){
  insertElements( initValue );
}

CompositeLiteral::CompositeLiteral( const ArrayTypeInfo &typeInfo,
				    const RValue &initValue ) :
  myTypeInfo( dynamic_cast<CompositeTypeInfo *>(typeInfo.clone()) ),
  indexTypeInfo( 0 ),
  deleteMembers(true),
  myFactory( 0 ){
  insertElements( initValue );
}

CompositeLiteral::CompositeLiteral( const CompositeLiteral &that ) :
  LValue( that ),
  myTypeInfo( that.myTypeInfo ? dynamic_cast<CompositeTypeInfo *>(that.myTypeInfo->clone()) : 0 ),
  indexTypeInfo( 0 ),
  deleteMembers(true),
  myFactory( that.myFactory ? that.myFactory->clone() : 0 ){
  cloneElements( that.myElements );
}

CompositeLiteral::CompositeLiteral( CompositeLiteral &that,
				    const ScalarTypeInfo &sliceSpecifier ) :
  LValue( that ),
  myTypeInfo( buildSliceTypeInfo( that, sliceSpecifier ) ),
  indexTypeInfo( 0 ),
  deleteMembers(false),
  myFactory( 0 ){
  sliceElements( that );
}

void
CompositeLiteral::destroyElements(){
  if( deleteMembers ){
    std::set<RValue *> deleted;
    deleted.clear();
    for( vector<RValue *>::const_iterator i = myElements.begin(); i != myElements.end(); ++i ){
      if (*i) {
        if (deleted.find(*i) == deleted.end()) {
          deleted.insert(*i);
          delete (*i);
        }
      }
    }
    myElements.clear();
    for( vector<RValue *>::const_iterator i = myCleanUps.begin(); i != myCleanUps.end(); ++i ){
      if (*i) {
        if (deleted.find(*i) == deleted.end()) {
          deleted.insert(*i);
          delete (*i);
        }
      }
    }
    myCleanUps.clear();
  }
}

CompositeLiteral::~CompositeLiteral(){
  destroyElements();
  if (myTypeInfo) {
    delete myTypeInfo;
    myTypeInfo = 0;
  }
  if (indexTypeInfo) {
    delete indexTypeInfo;
    indexTypeInfo = 0;
  }
  if (myFactory) {
    delete myFactory;
    myFactory = 0;
  }
}

const TypeInfo &
CompositeLiteral::getTypeInfo() const {
  ASSERT( myTypeInfo != 0 );
  return *myTypeInfo;
}

const RValue &
CompositeLiteral::operator[]( int index ) const {
  return (*const_cast<CompositeLiteral *>(this))[index];
}

RValue &
CompositeLiteral::operator[]( int index ){
  return getField(storageIndex( index ));
}

const RValue &
CompositeLiteral::operator[]( const RValue &index ) const {
  return (*this)[index.getIntValue()];
}

RValue &
CompositeLiteral::operator[]( const RValue &index ){
  return (*this)[index.getIntValue()];
}


RValue &
CompositeLiteral::getField( int index ){
  RValue *retval = 0;
  if( index > length().getIntValue() - 1 ){
    cerr << "Error: - trying to access index " << index << " on slice with " 
	 << length().getIntValue() << " elements." << endl;
    abort();
  }
  else{
    retval = myElements[index];
  }
  ASSERT( retval != 0 );
  return *retval;
}

const RValue &
CompositeLiteral::getField( int index ) const {
  return const_cast<CompositeLiteral *>(this)->getField( index );
}

const RValue &
CompositeLiteral::getField( const RValue &index ) const {
  return getField( index.getIntValue() );
}

RValue &
CompositeLiteral::getField( const RValue &index ){
  return getField( index.getIntValue() );
}


Value
CompositeLiteral::left( const RValue &param ) const {
  if( param.getIntValue() > 1 ){
    return getElementTypeInfo().left( UniversalInteger(param.getIntValue() - 1) );
  }
  else {
    return getIndexTypeInfo().left();
  }
}

Value
CompositeLiteral::right( const RValue &param ) const {
  if( param.getIntValue() > 1 ){
    return getElementTypeInfo().right( UniversalInteger(param.getIntValue() - 1) );
  }
  else {
    return getIndexTypeInfo().right();
  }
}

Value
CompositeLiteral::high( const RValue &param ) const {
  if( param.getIntValue() > 1 ){
    return getElementTypeInfo().high( UniversalInteger(param.getIntValue() - 1) );
  }
  else {
    return getIndexTypeInfo().high();
  }
}

Value
CompositeLiteral::low( const RValue &param ) const {
  if( param.getIntValue() > 1 ){
    return getElementTypeInfo().low( UniversalInteger(param.getIntValue() - 1) );
  }
  else {
    return getIndexTypeInfo().low();
  }
}

const ScalarTypeInfo &
CompositeLiteral::range( const RValue &index ) const {
  if( index == defaultIndex() ){
    return divineIndexTypeInfo();
  }
  else{
    abort();
  }
}

const ScalarTypeInfo &
CompositeLiteral::reverse_range( const RValue & ) const {
  abort();
}

Value
CompositeLiteral::length( const RValue &index ) const { 
  ASSERT( index.getIntValue() == 1 );
  return UniversalInteger((int)myElements.size());
}

Value
CompositeLiteral::ascending( const RValue &index ) const {
  ASSERT( index.getIntValue() == 1 );
  return getIndexTypeInfo().ascending();
}

void
CompositeLiteral::debugPrintDetails( ostream &os, int indent ) const {
  if (indexTypeInfo) {
    os << NL(indent) << "My local ScalarTypeInfo *indexTypeInfo: ";
    indexTypeInfo->debugPrint(os, indent+2);
  }
  os << NL(indent) << "TypeInfo *myTypeInfo: ";
  if (myTypeInfo)
    myTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";

  os << NL(indent) << "vector<RValue *> myElements:";
  if (myElements.size() > 0) {
    for (unsigned int i=0 ; i<myElements.size() ; ++i) {
      os << NL(indent) << "myElements[" << i << "] (at " << myElements[i] << "):";
      if (myElements[i]) {
        myElements[i]->debugPrint(os, indent+5);
      }
      else {
        os << "is null";
      }
    }
  }
  else {
    os << " is empty";
  }

  return;
}

SignalNetinfo &
CompositeLiteral::addToFanOut( VHDLKernel * ){
  cerr << "CompositeLiteral::addToFanOut( VHDLKernel * ) called - aborting!" << endl;
  abort();
  return *new SignalNetinfo;
}

SignalNetinfo &
CompositeLiteral::addChild( SignalNetinfo &, VHDLKernel * ){
  cerr << "CompositeLiteral::addChild( SignalNetinfo &, VHDLKernel * ) called - aborting!" << endl;
  abort();
  return *new SignalNetinfo;
}

void
CompositeLiteral::clearElements ( ){
  int len = myElements.size();
  for (int i = 0 ; i < len ; ++i ){
    if (myElements[i])
      delete myElements[i];
  }
  myElements.clear();
}

void
CompositeLiteral::voidElements ( ){
  int len = myElements.size();
  for (int i = 0 ; i < len ; ++i ){
    myElements[i] = 0;
  }
}

void
CompositeLiteral::append( const RValue &newElement ){
  if (newElement.isCompositeType() && (!myTypeInfo ||
      myTypeInfo->getElementTypeInfo() == newElement.getField(0).getTypeInfo())){
    for (int i=0 ; i < newElement.length().getIntValue() ; ++i){
      append(newElement.getField(i));
    }
  }
  else {
    myElements.push_back( newElement.clone() );
  }
}

void
CompositeLiteral::append( RValue *newElement ){
  if (newElement->isCompositeType() && (!myTypeInfo ||
      myTypeInfo->getElementTypeInfo() == newElement->getField(0).getTypeInfo())){
    for (int i=0 ; i < newElement->length().getIntValue() ; ++i){
      append(newElement->getField(i));
    }
  }
  else {
    myElements.push_back( newElement );
  }
}

void
CompositeLiteral::shiftLogical( int ){
  abort();
}

const TypeInfo &
CompositeLiteral::getElementTypeInfo( const RValue &index ) const {
  if( index == defaultIndex() ){
    if( myTypeInfo != 0 ){
      return myTypeInfo->getElementTypeInfo();
    }
    else if( !myElements.empty() ){
      return myElements[0]->getTypeInfo();
    }
    else{
      cerr << "CompositeLiteral::getElementTypeInfo() called on empty CompositeLiteral with no type!" << endl;
      abort();
    }
  }
  else{
    abort();
  }
  return myElements[0]->getTypeInfo();
}

const ScalarTypeInfo &
CompositeLiteral::divineIndexTypeInfo( ) const {
  int left = 1, right = 1;
  ArrayInfo::ArrayDirn_t direction = ArrayInfo::to;

  delete indexTypeInfo;
  indexTypeInfo = 0;

  if( myTypeInfo != 0 ){
    if( myTypeInfo->isUnconstrained() ){
      direction = myTypeInfo->getIndexTypeInfo().getDirection();
      if( myTypeInfo->getIndexTypeInfo().left() == IntegerTypeInfo::getMax() ){
	if( direction == ArrayInfo::downto ){
	  left = length().getIntValue() - 1 + right;
	}
      }
      else{
	left = myTypeInfo->getIndexTypeInfo().left().getIntValue();
      }
      if( myTypeInfo->getIndexTypeInfo().right() == IntegerTypeInfo::getMax() ){
	if( direction == ArrayInfo::to ){
	  right = length().getIntValue() - 1 + left;
	}
      }
      else{
	right = myTypeInfo->getIndexTypeInfo().right().getIntValue();
      }
      indexTypeInfo = new IntegerTypeInfo( left, direction, right );
    }
    else{
      // We have a myTypeInfo, it's constrained.
      indexTypeInfo = 
	dynamic_cast<const ScalarTypeInfo *>(myTypeInfo->getIndexTypeInfo(defaultIndex()).clone());
      ASSERT( indexTypeInfo != 0 );     
    }
  }
  else{
    // We have no myTypeInfo.  We're going to synthesize left, to, right
    right = length( defaultIndex() ).getIntValue();
    indexTypeInfo = new IntegerTypeInfo( left, direction, right );
  }
  return *indexTypeInfo;
}

const ScalarTypeInfo &
CompositeLiteral::getIndexTypeInfo( const RValue &index ) const {
  if( index == defaultIndex() ){
    return divineIndexTypeInfo();
  }
  else{
    abort();
  }
}

int
CompositeLiteral::getIntValue() const { 
  if (length().getIntValue() > 8*sizeof(int)) {
    cerr << "CompositeLiteral::getIntValue() called with length of " << length().getIntValue() <<
      " vs number of bits in int of " << 8*sizeof(int) << endl;
    abort();
  }
  int rVal = 0;
  string digit;
  for( int i = 0 ; i<length().getIntValue(); ++i ){
    rVal <<= 1;
    digit = getField(i).toString();
    if (digit == "'0'") {
    }
    else if (digit == "'1'") {
      rVal += 1;
    }
    else {
      cerr << "CompositeLiteral::getIntValue() called with unknown digit: " << digit << endl;
      return 0;
    }
  }
  return rVal;
}

LONG
CompositeLiteral::getInt64Value() const { 
  if (length().getInt64Value() > 8*sizeof(LONG)) {
    cerr << "CompositeLiteral::getInt64Value() called with length of " << length().getIntValue() <<
      " vs number of bits in LONG of " << 8*sizeof(LONG) << endl;
    abort();
  }
  LONG rVal = 0;
  string digit;
  for( int i = 0 ; i<length().getIntValue(); ++i ){
    rVal <<= 1;
    digit = getField(i).toString();
    if (digit == "'0'") {
    }
    else if (digit == "'1'") {
      rVal += 1;
    }
    else {
      cerr << "CompositeLiteral::getInt64Value() called with unknown digit: " << digit << endl;
      return 0;
    }
  }
  return rVal;
}

double
CompositeLiteral::getDoubleValue() const { 
  cerr << "CompositeLiteral::getDoubleValue() called - aborting!" << endl;
  abort();
  return 0.0;
}

CompositeLiteral &
CompositeLiteral::operator=( const CompositeLiteral &that ){
  CompositeLiteral::operator=( static_cast<const RValue &>(that) );
  return *this;
}

RValue &
CompositeLiteral::operator=( const RValue &that ){
  int thatLength = that.length(defaultIndex()).getIntValue();
  if( length().getIntValue() < thatLength ){
    deleteMembers = true; 
    destroyElements();
    ASSERT( myTypeInfo != 0 );
    buildElements( *myTypeInfo, myElements, 0, thatLength );
    deleteMembers = true;
  }
  for( int i = 0; i < thatLength; i++ ){
    getField(i) = that.getField(i);
  }
  return *this;
}

bool
CompositeLiteral::operator==( const RValue &that ) const {
  bool retval = true;
  
  if( length().getIntValue() != that.length(defaultIndex()).getIntValue() ){
    retval = false;
  }
  else {
    for( int i = 0; i < length().getIntValue(); i++ ){
      if( getField(i) != that.getField(i) ){
        retval = false;
	break;
      }
    }
  }

  return retval;
}

bool
CompositeLiteral::operator!=( const RValue &that ) const {
  return !(operator==( that ));
}

bool
CompositeLiteral::operator>( const RValue &that ) const {
  return (!(operator<(that)) && !(operator==(that)));
}

bool
CompositeLiteral::operator>=( const RValue &that ) const {
  return !(operator<(that));
}

bool
CompositeLiteral::operator<( const RValue &that ) const {
  bool retval = true;
  if( length().getIntValue() < that.length(defaultIndex()).getIntValue() ){
    retval = false;
  }
  else {
    ASSERT(dynamic_cast<const ArrayTypeInfo *>(myTypeInfo) != 0 );
    ASSERT(dynamic_cast<const ArrayTypeInfo *>(&that.getTypeInfo()) != 0 );
    const ArrayTypeInfo &ltypeinfo = *dynamic_cast<const ArrayTypeInfo *>(myTypeInfo);
    const ArrayTypeInfo &rtypeinfo = *dynamic_cast<const ArrayTypeInfo *>(&that.getTypeInfo());
    if( ltypeinfo.range(defaultIndex()).getDirection() == ArrayInfo::to ){
      if( rtypeinfo.range(defaultIndex()).getDirection() == ArrayInfo::to ){ //TO:TO
        for( int i = length().getIntValue()-1; i >= 0; i--){
	  if( getField(i) > that.getField(i) ){
	    retval = false;
	    break;
	  }
        }
      }
      else { //TO:DOWNTO
        int index = length().getIntValue()-1;
	for( int i = 0; i < length().getIntValue(); i++ ){
          if( getField(index) > that.getField(i) ){
            retval = false;
	    break;
	  }
	  index--;
	}
      }
    }
    else { 
      if( rtypeinfo.range(defaultIndex()).getDirection() == ArrayInfo::to ){ //DOWNTO:TO
        int index = length().getIntValue()-1;
        for( int i = 0; i < length().getIntValue(); i++ ){
          if( getField(i) > that.getField(index) ){
	    retval = false;
	    break;
	  }
          index--;
	}
      }
      else { //DOWNTO:DOWNTO
        for( int i = 0; i < length().getIntValue(); i++ ){
	  if( getField(i) > that.getField(i) ){
	    retval = false;
	    break;
	  }
	}
      }
    }
  }
}

bool
CompositeLiteral::operator<=( const RValue &that ) const {
  return !(operator>(that));
}

Value
CompositeLiteral::vhdlUnaryPlus( ) const {
  abort();
}

Value
CompositeLiteral::vhdlUnaryMinus( ) const {
  abort();
}

Value
CompositeLiteral::vhdlPlus( const RValue & ) const {
  abort();
}

Value
CompositeLiteral::vhdlMinus( const RValue & ) const {
  abort();
}

Value
CompositeLiteral::vhdlMultiply( const RValue & ) const {
  abort();
}

Value
CompositeLiteral::vhdlDivide( const RValue & ) const {
  abort();
}

Value
CompositeLiteral::vhdlMod( const RValue & ) const {
  abort();
}

Value
CompositeLiteral::vhdlRem( const RValue & ) const {
  abort();
}

Value
CompositeLiteral::vhdlPow( const RValue & ) const {
  abort();
}

//ASSUMES the Aggregate is a single Dimensional Aggregate of EnumerationLiteral Type
Value
CompositeLiteral::vhdlAnd( const RValue &rhs ) const {
  return bitOperation( rhs, &EnumerationLiteral::vhdlAnd );
}

//ASSUMES the Aggregate is a single Dimensional Aggregate of Boolean or Bit Type
Value
CompositeLiteral::vhdlNand( const RValue &rhs ) const {
  return bitOperation( rhs, &EnumerationLiteral::vhdlNand );
}

//ASSUMES the Aggregate is a single Dimensional Aggregate of Boolean or Bit Type
Value
CompositeLiteral::vhdlOr( const RValue &rhs ) const {
  return bitOperation( rhs, &EnumerationLiteral::vhdlOr );
}

//ASSUMES the Aggregate is a single Dimensional Aggregate of Boolean or Bit Type
Value
CompositeLiteral::vhdlNor( const RValue &rhs ) const {
  return bitOperation( rhs, &EnumerationLiteral::vhdlNor );
}

//ASSUMES the Aggregate is a single Dimensional Value of Boolean or Bit Type
Value
CompositeLiteral::vhdlXnor( const RValue &rhs ) const {
  return bitOperation( rhs, &EnumerationLiteral::vhdlXnor );
}

//ASSUMES the Aggregate is a single Dimensional Aggregate of Boolean or Bit Type
Value
CompositeLiteral::vhdlXor( const RValue &rhs ) const {
  return bitOperation( rhs, &EnumerationLiteral::vhdlXor );
}

Value
CompositeLiteral::vhdlNot() const {
  RValue *retval = clone();
  for(int i =0; i < length().getIntValue(); i++) {
    const EnumerationLiteral &lhs = dynamic_cast<const EnumerationLiteral&>(getField(i));
    const_cast<RValue &>(retval->getField(i)) = lhs.vhdlNot();
  }
  
  return retval;
}

Value
CompositeLiteral::vhdlSll( const RValue &rhs ) const {
  RValue *retval = clone();
  dynamic_cast<CompositeLiteral *>(retval)->shiftLogical( rhs.getIntValue() );
  return retval;
}

//ASSUMES the Aggregate is a single Dimensional Aggregate of Boolean or Bit Type
Value
CompositeLiteral::vhdlSrl( const RValue &rhs ) const {
  RValue *retval = clone();
  dynamic_cast<CompositeLiteral *>(retval)->shiftLogical( -rhs.getIntValue() );
  return retval;
}

Value
CompositeLiteral::vhdlSla( const RValue &rhs ) const {
  int lhsSize = length().getIntValue();
  int numberOfShifts = rhs.getIntValue();
  int i = 0;
  
  if (numberOfShifts < 0) {
    return vhdlSll( UniversalInteger(numberOfShifts).vhdlAbs() );
  }
  
  RValue *retval = clone();
  if ((numberOfShifts == 0) || (lhsSize == 0)) {
    return retval;
  }
  
  EnumerationLiteral last_element = (EnumerationLiteral &) getField(lhsSize - 1);
  
  for(i = numberOfShifts; i < lhsSize; i++) {
    EnumerationLiteral& enumerationPtr = (EnumerationLiteral&)retval->getField(i - numberOfShifts);
    EnumerationLiteral& enumerationLhs = (EnumerationLiteral&) getField(i);
    enumerationPtr = enumerationLhs;
  }
  
  for(i = lhsSize - numberOfShifts; i < lhsSize; i++) {
    const_cast<RValue &>(retval->getField(i)) = last_element;
  }
      
  return retval;
}

//ASSUMES the Aggregate is a single Dimensional Aggregate of Boolean or Bit Type
Value
CompositeLiteral::vhdlSra( const RValue &rhs ) const {
  int lhsSize = length().getIntValue();
  int numberOfShifts = rhs.getIntValue();
  int i = 0;
  
  if (numberOfShifts < 0) {
    return vhdlSla( UniversalInteger(numberOfShifts).vhdlAbs() );
  }
  
  RValue *retval = clone();
  if ((numberOfShifts == 0) || (lhsSize == 0)) {
    return retval;
  }
  
  EnumerationLiteral first_value = (EnumerationLiteral&) getField(0);
  
  if (numberOfShifts > lhsSize) {
    return retval;
  }
  
  for(i = lhsSize; i >= numberOfShifts; i--) {
    EnumerationLiteral& enumerationPtr = (EnumerationLiteral &) retval->getField(i);
    EnumerationLiteral& enumerationLhs = (EnumerationLiteral &) getField(i - numberOfShifts);
    enumerationPtr = enumerationLhs;
  }

  for(i = 0; i < lhsSize; i++) {
    const_cast<RValue &>(retval->getField(i)) = first_value;
  }
  
  return retval;
}

Value
CompositeLiteral::vhdlRor( const RValue &rhs ) const {
  int lhsSize = length().getIntValue();
  int numberOfShifts = rhs.getIntValue();
  int i = 0, src_pos = 0;
  
  if (numberOfShifts < 0) {
    return vhdlSra( UniversalInteger(numberOfShifts).vhdlAbs() );
  }

  RValue *retval = clone();
  numberOfShifts = numberOfShifts % lhsSize;
  
  if ((numberOfShifts == 0) || (lhsSize == 0)) {
    return retval;
  }

  // Copy tail of lhs to head of return value
  for(i = 0, src_pos = lhsSize - numberOfShifts; (src_pos < lhsSize); src_pos++, src_pos++) {
    EnumerationLiteral &enumerationPtr = (EnumerationLiteral &) retval->getField(i);
    EnumerationLiteral &enumerationLhs = (EnumerationLiteral &) getField(src_pos);
    enumerationPtr = enumerationLhs;
  }

  // Copy head of lhs to tail of return value
  for(src_pos = 0, i = numberOfShifts; i < lhsSize; src_pos++, i++)  {
    EnumerationLiteral& enumerationPtr = (EnumerationLiteral &) retval->getField(i);
    EnumerationLiteral& enumerationLhs = (EnumerationLiteral &) getField(src_pos);
    enumerationPtr = enumerationLhs;
  }
  
  return retval;
}

Value
CompositeLiteral::vhdlRol( const RValue &rhs ) const {
  int lhsSize = length().getIntValue();
  int numberOfShifts = rhs.getIntValue();
  int i = 0, src_pos = 0;
  
  if (numberOfShifts < 0) {
    return vhdlSla( UniversalInteger(numberOfShifts).vhdlAbs() );
  }
  
  RValue *retval = clone();
  numberOfShifts   = numberOfShifts % lhsSize;
  
  if ((numberOfShifts == 0) || (lhsSize == 0)) {
    return retval;
  }
  
  // Copy head of lhs to tail of return value
  for(src_pos = 0, i = lhsSize - numberOfShifts; (src_pos < numberOfShifts); i++, src_pos++) {
    EnumerationLiteral& enumerationPtr = (EnumerationLiteral &) retval->getField(i);
    EnumerationLiteral& enumerationLhs = (EnumerationLiteral &) getField(src_pos);
    enumerationPtr = enumerationLhs;
  }
  
  // Copy tail of lhs to head of return value
  for(i = 0, src_pos = lhsSize - numberOfShifts; src_pos < lhsSize; src_pos++, i++)  {
    EnumerationLiteral& enumerationPtr = (EnumerationLiteral &) retval->getField(i);
    EnumerationLiteral& enumerationLhs = (EnumerationLiteral &) getField(src_pos);
    enumerationPtr = enumerationLhs;
  }
  
  return retval;
}

Value
CompositeLiteral::vhdlConcatenate( const RValue &rhs ) const {
  RValue *retval = clone();
  for( int i = 0; i < rhs.length(UniversalInteger(1)).getIntValue(); i++ ){
    ASSERT(dynamic_cast<CompositeLiteral *>(retval) != 0 );
    dynamic_cast<CompositeLiteral *>(retval)->append( rhs.getField(i) );
  }

  return retval;
}

Value
CompositeLiteral::bitOperation( const RValue &rhsAggregate,
				Value(EnumerationLiteral::*operation)(const RValue&)const )const {
  int lhs_size = length().getIntValue();
  int rhs_size = rhsAggregate.length().getIntValue();
  
  if(lhs_size != rhs_size) {
    cerr << "Aggregates of different sizes are ANDed. Bailing out" << endl;
    abort();
  }

  RValue *retval = clone();
  for( int i = 0; i < lhs_size; i++ ){
    const EnumerationLiteral &lhs = dynamic_cast<const EnumerationLiteral &>(getField(i));
    const EnumerationLiteral &rhs = dynamic_cast<const EnumerationLiteral &>(rhsAggregate.getField(i));
    const_cast<RValue &>(retval->getField(i)) = (&(lhs)->*operation)(rhs);
  }
  
  return retval;
}

int
CompositeLiteral::storageIndex( int elementIndex ) const {
  int retval = elementIndex;
  if( myTypeInfo != 0 ){
    retval = myTypeInfo->storageIndex( elementIndex );
  }
  else{
    const ScalarTypeInfo &tempInfo = divineIndexTypeInfo();
    // Assume an index of 1 to <>...
    retval = tempInfo.storageIndex( elementIndex );
  }
  ASSERT( retval >= 0 );
  return retval;
}

void
CompositeLiteral::buildElements( const CompositeTypeInfo &typeInfo,
				 vector<RValue *> &elements,
				 const SubElementFactory *subElementFactory,
				 int numElements ){
  typeInfo.constructFields( elements, subElementFactory, numElements );
}

void
CompositeLiteral::cloneElements( const vector<RValue *> &thatElements ){
  for( vector<RValue *>::const_iterator i = thatElements.begin();
       i < thatElements.end();
       i++ ){
    myElements.push_back( (*i)->clone() );
  }
}

const RValue &
CompositeLiteral::readVal() const {
  return *this;
}

RValue &
CompositeLiteral::readVal(){
  return *const_cast<CompositeLiteral *>(this);
}

const string &
CompositeLiteral::getUnit() const {
  abort();
}

LONG
CompositeLiteral::getValue() const {
  abort();
}

int
CompositeLiteral::getUnits() const {
  abort();
}

const EnumerationLiteral &
CompositeLiteral::vhdlEqual( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this == rhs );
}

const EnumerationLiteral &
CompositeLiteral::vhdlNotEqual( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this != rhs );
}

const EnumerationLiteral &
CompositeLiteral::vhdlGreaterThan( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this > rhs );
}

const EnumerationLiteral &
CompositeLiteral::vhdlGreaterThanEqual( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this >= rhs );
}

const EnumerationLiteral &
CompositeLiteral::vhdlLessThan( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this < rhs );
}

const EnumerationLiteral &
CompositeLiteral::vhdlLessThanEqual( const RValue &rhs ) const {
  return EnumerationLiteral::toBoolean( *this <= rhs );
}

RValue &
CompositeLiteral::all(){
  abort();
}

const RValue &
CompositeLiteral::all() const {
  abort();
}

void
CompositeLiteral::updateVal( const RValue &that ){
  *this = that;
}

void
CompositeLiteral::insertElements( const string &initValue ){
  ASSERT( myTypeInfo != 0 );
  if( dynamic_cast<const ArrayTypeInfo *>(myTypeInfo) != 0 ){
    const ArrayTypeInfo &typeInfo = *dynamic_cast<const ArrayTypeInfo *>(myTypeInfo);
    ASSERT( typeInfo.getElementTypeInfo().isCharacterType() );
    const EnumerationTypeInfo &characterTypeInfo =
      dynamic_cast<const EnumerationTypeInfo &>(typeInfo.getElementTypeInfo());
    RangeIterator i = typeInfo.range(defaultIndex()).iterator();
    bool finished = initValue.length() == 0 || !i.hasNext();
    unsigned int count = 0;
    while( !finished ){
      int index = typeInfo.storageIndex(i.next());
      int pos = characterTypeInfo.getIndex( string("'") + initValue[index] + "'" );
      ASSERT( pos >= 0 );
      myElements.push_back( new EnumerationLiteral( characterTypeInfo, pos ) );
      count++;
      finished = !i.hasNext() || count == initValue.length();
    }
  }
  else{
    cerr << "Internal error in CompositeLiteral::insertElements, don't know how to "
	 << "handle a " << getTypeInfo().getKindText() << endl;
    abort();
  }
}

void
CompositeLiteral::insertElements( const RValue &initValue ){
  ASSERT( myTypeInfo != 0 );
  RangeIterator i = initValue.range(defaultIndex()).iterator();
  while( i.hasNext() ){
    myElements.push_back( initValue[i.next()].clone() );
  }
}

void
CompositeLiteral::sliceElements( CompositeLiteral &that ){
  ASSERT( myTypeInfo != 0 );
  RangeIterator i = dynamic_cast<const ArrayTypeInfo *>(myTypeInfo)->getIndexTypeInfo().iterator();
  myElements.resize( i.length() );
  while( i.hasNext() ){
    int current = i.next();
    myElements[storageIndex(current)] = &that[ current ];
  }
}

CompositeTypeInfo *
CompositeLiteral::buildSliceTypeInfo( const CompositeLiteral &that,
				      const ScalarTypeInfo &sliceSpecifier ){
  const ArrayTypeInfo &thatsTypeInfo =
    dynamic_cast<const ArrayTypeInfo &>(that.getTypeInfo());

  return new ArrayTypeInfo( thatsTypeInfo.getElementTypeInfo(),
			    sliceSpecifier );
}

const UniversalInteger &
CompositeLiteral::defaultIndex(){
  static UniversalInteger i(1);
  return i;
}

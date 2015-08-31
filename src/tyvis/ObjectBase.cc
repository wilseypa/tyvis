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

#include "ObjectBase.hh"
#include "TypeInfo.hh"
#include "EnumerationLiteral.hh"
#include "std_standardPkg.hh"
#include "SignalNetinfo.hh"
#include "SubElementFactory.hh"
#include "Element.hh"
#include "ElementInfo.hh"

ObjectBase::ObjectBase() :
  myTypeInfo(0),
  myData(0){}

ObjectBase::ObjectBase( const TypeInfo &initTypeInfo,
			const string &initName ) :
  myTypeInfo( initTypeInfo.clone() ),
  myName( initName ),
  myData( myTypeInfo->create() ){}

ObjectBase::ObjectBase( SignalNetinfo &parent,
                        const IntegerTypeInfo &initTypeInfo ) :
  myTypeInfo( new ArrayTypeInfo(dynamic_cast<const ArrayTypeInfo &>
                (parent.getTypeInfo()).getElementTypeInfo(), initTypeInfo) ),
  myName (string("") ),
  myData( myTypeInfo->create( ) ){
  dynamic_cast<CompositeLiteral *>(myData)->clearElements();
  int start = initTypeInfo.left().getIntValue();
  int end = initTypeInfo.right().getIntValue();
  int i, step;
  if (start > end)
    step = -1;
  else
    step = 1;
  for (i=start ; i != end+step ; i += step){
    dynamic_cast<CompositeLiteral *>(myData)->append(&parent[i]);
  }
}

ObjectBase::ObjectBase( const ArrayTypeInfo &initTypeInfo,
                        TyvisVarArgs &siIn ) :
  myTypeInfo( initTypeInfo.clone() ),
  myName (string("") ),
  myData( myTypeInfo->create( ) ){
  dynamic_cast<CompositeLiteral *>(myData)->clearElements();
  int left = myTypeInfo->left().getIntValue();
  int right = myTypeInfo->right().getIntValue();
  int size = right-left;
  int i, a, b;
  a = 1;
  if (size < 0) {
    a = -1;
    size = -size;
  }
  b = -a*left;
  ++size;
  vector<RValue *> vals(size, static_cast<RValue *>(0));
  int inc, cleft, cright;
  while (siIn.hasNext() ){
    const ArrayInfo & c = siIn.choice();
    RValue *currVal = siIn.next();
    cleft = c.left();
    cright = c.right();
    if (cleft == cright) {
      ASSERT(vals[cleft*a+b] == 0);
      vals[cleft*a+b] = currVal;
    }
    else {
      if (cleft > cright)
        inc = -1;
      else
        inc = 1;
      for ( i = cleft ; i != cright+inc ; i += inc ) {
        ASSERT(vals[i*a+b] == 0);
        vals[i*a+b] = &currVal->getField(inc*(i-cleft));
      }
    }
  }
  for (i= 0; i<size ; ++i) {
    ASSERT(vals[i] != 0);
    dynamic_cast<CompositeLiteral *>(myData)->append(vals[i]);
  }
}

ObjectBase::ObjectBase( const TypeInfo &initTypeInfo,
			const string &initName,
			const RValue &initValue ) :
  myTypeInfo( initTypeInfo.clone() ),
  myName( initName ),
  myData( initValue.clone() ){}

ObjectBase::ObjectBase( const SubElementFactory &subElementFactory ) :
  myTypeInfo( subElementFactory.getParentTypeInfo().clone() ),
  myName( subElementFactory.getParentName() ),
  myData( myTypeInfo->create( &subElementFactory ) ){}

ObjectBase::ObjectBase( const ObjectBase &that ) :
  LValue( that ),
  myTypeInfo( that.myTypeInfo->clone() ),
  myName( that.myName ),
  myData( that.myData->clone() ){}

ObjectBase::~ObjectBase(){
  delete myData;
  myData = 0;
  delete myTypeInfo;
  myTypeInfo = 0;
}

ObjectBase &
ObjectBase::operator=( const ObjectBase &that ) {
  if( myTypeInfo == 0 ){
    myTypeInfo = that.myTypeInfo ? that.getTypeInfo().clone() : 0;
  }
  updateVal( that.readVal() );
  
  return *this;
}

RValue &
ObjectBase::operator=( const RValue &that ){
  updateVal( that );
  return *this;
}

RValue *
ObjectBase::clone() const {
	  return new ObjectBase( *this );
}

const RValue &
ObjectBase::operator[]( int index ) const {
  ASSERT( myData != 0 ); 
  return const_cast<ObjectBase *>(this)->operator[](index);
}

RValue &
ObjectBase::operator[]( int index ){
  ASSERT( myData != 0 );
  return (*myData)[index];
}

const RValue &
ObjectBase::operator[]( const RValue &index ) const {
  ASSERT( myData != 0 );
  return operator[]( index.getIntValue() );
}

RValue &
ObjectBase::operator[]( const RValue &index ){
  ASSERT( myData != 0 );
  return operator[]( index.getIntValue() );
}

bool
ObjectBase::isCompositeType() const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->isCompositeType();
}

bool
ObjectBase::isScalarType() const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->isScalarType();
}

bool
ObjectBase::isPhysicalType() const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->isPhysicalType();
}

Value
ObjectBase::length( const RValue &index ) const {
  ASSERT( myData != 0 );
  return myData->length( index );
}

LONG
ObjectBase::getValue() const {
  ASSERT( myData != 0 );
  return myData->getValue();
}

int
ObjectBase::getUnits() const {
  ASSERT( myData != 0 );
  return myData->getUnits();
}

void
ObjectBase::print( ostream &os ) const {
  ASSERT( myData != 0 );
  myData->print(os);
}

void
ObjectBase::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "id = \"" << myName << "\" ";
  os << "TypeInfo *myTypeInfo: ";
  if (myTypeInfo)
    myTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";

  os << NL(indent) << "RValue *myData: ";
  if (myData)
    myData->debugPrint(os, indent+5);
  else
    os << " is null";

  return;
}

string
ObjectBase::toString() const {
  ASSERT( myData != 0 );
  return myData->toString();
}

void
ObjectBase::vcdPrint( ostream &os ) const {
  ASSERT( myData != 0 );
  myData->vcdPrint(os);
}

const RValue &
ObjectBase::readVal() const {
  ASSERT( myData != 0 );
  return *myData;
}

RValue &
ObjectBase::readVal(){
  ASSERT( myData != 0 );
  return *const_cast<RValue *>(myData);
}

void
ObjectBase::updateVal( const RValue &newValue ){
  if (myData) {
    *myData = newValue;
  } else {
    // Don't have a myData object to call operator= on. This probably shouldn't happen
    cerr << "ObjectBase::updateVal called while myData is null. This could indicate a bug in tyvis." << endl;
    myData = newValue.clone();
  }
/* DNS: this is where sginals are actually set:
  cout << "Updating " << getName() << " to ";
  newValue.print(cout);
  cout << endl;
*/
}

bool
ObjectBase::operator==( const RValue &compareTo ) const {
  return readVal().operator==( compareTo );
}

bool
ObjectBase::operator!=( const RValue &compareTo ) const {
  return readVal().operator!=( compareTo );
}

bool
ObjectBase::operator>( const RValue &compareTo ) const {
  return readVal().operator>( compareTo );
}

bool
ObjectBase::operator>=( const RValue &compareTo ) const {
  return readVal().operator>=( compareTo );
}
bool
ObjectBase::operator<( const RValue &compareTo ) const {
  return readVal().operator<( compareTo );
}

bool
ObjectBase::operator<=( const RValue &compareTo ) const {
  return readVal().operator<=( compareTo );
}

const EnumerationLiteral &
ObjectBase::vhdlEqual( const RValue &that ) const {
  return EnumerationLiteral::toBoolean( readVal() == that );
}

const EnumerationLiteral &
ObjectBase::vhdlNotEqual( const RValue &that ) const {
  return EnumerationLiteral::toBoolean( readVal() != that );
}

const EnumerationLiteral &
ObjectBase::vhdlGreaterThan( const RValue &that ) const {
  return EnumerationLiteral::toBoolean( readVal() > that );
}

const EnumerationLiteral &
ObjectBase::vhdlGreaterThanEqual( const RValue &that ) const {
  return EnumerationLiteral::toBoolean( readVal() >= that );
}

const EnumerationLiteral &
ObjectBase::vhdlLessThan( const RValue &that ) const {
  return EnumerationLiteral::toBoolean( readVal() < that );
}

const EnumerationLiteral &
ObjectBase::vhdlLessThanEqual( const RValue &that ) const {
  return EnumerationLiteral::toBoolean( readVal() <= that );
}

Value
ObjectBase::vhdlConcatenate( const RValue &that ) const {
  return readVal().vhdlConcatenate( that );
}

const RValue &
ObjectBase::getField( int index ) const {
  return myData->getField( index );
}

RValue &
ObjectBase::getField( int index ){
  return myData->getField( index );
}

const RValue &
ObjectBase::getField( const RValue &index ) const {
  return myData->getField( index );
}

RValue &
ObjectBase::getField( const RValue &index ){
  return myData->getField( index );
}

const string &
ObjectBase::getUnit() const {
  return myData->getUnit();
}


const TypeInfo &
ObjectBase::getTypeInfo() const {
  ASSERT( myTypeInfo != 0 );
  return *myTypeInfo;
}

const string &
ObjectBase::getName() const{
  return myName;
}

Value
ObjectBase::left( const RValue &param ) const {
  ASSERT( myData != 0 );
  ASSERT( myTypeInfo != 0 );
  if( myTypeInfo->isCompositeType() ){
    return myData->left( param );
  }
  else{
    return myTypeInfo->left( param );
  }
}

Value
ObjectBase::right( const RValue &param ) const {
  ASSERT( myData != 0 );
  ASSERT( myTypeInfo != 0 );
  if( myTypeInfo->isCompositeType() ){
    return myData->right( param );
  }
  else{
    return myTypeInfo->right( param );
  }
}

Value
ObjectBase::high( const RValue &param ) const {
  ASSERT( myData != 0 );
  ASSERT( myTypeInfo != 0 );
  if( myTypeInfo->isCompositeType() ){
    return myData->high( param );
  }
  else{
    return myTypeInfo->high( param );
  }
}

Value
ObjectBase::low( const RValue &param ) const {
  ASSERT( myData != 0 );
  ASSERT( myTypeInfo != 0 );
  if( myTypeInfo->isCompositeType() ){
    return myData->low( param );
  }
  else{
    return myTypeInfo->low( param );
  }
}

const ScalarTypeInfo &
ObjectBase::range( const RValue &index ) const {
  if( getTypeInfo().isCompositeType() ){
    return dynamic_cast<const CompositeTypeInfo &>(getTypeInfo()).getIndexTypeInfo(index);
  }
  else{
    return getTypeInfo().range();
  }
}

const ScalarTypeInfo &
ObjectBase::reverse_range( const RValue &index ) const {
  cerr << "Error - ObjectBase::reverse_range called!" << endl;
  abort();
  return dynamic_cast<const ScalarTypeInfo &>(getTypeInfo());  
}

Value
ObjectBase::ascending( const RValue &index ) const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->ascending( index );
}

Value
ObjectBase::vhdlUnaryPlus( ) const {
  abort();
}

Value
ObjectBase::vhdlUnaryMinus( ) const {
  abort();
}

Value
ObjectBase::vhdlPlus( const RValue &that ) const {
  return getTypeInfo().vhdlPlus( readVal(), that );
}

Value
ObjectBase::vhdlMinus( const RValue &that ) const {
  return getTypeInfo().vhdlMinus( readVal(), that );
}

Value
ObjectBase::vhdlMultiply( const RValue &that ) const {
  return getTypeInfo().vhdlMultiply( readVal(), that );
}

Value
ObjectBase::vhdlDivide( const RValue &that ) const {
  return getTypeInfo().vhdlDivide( readVal(), that );
}

Value
ObjectBase::vhdlMod( const RValue &that ) const {
  return getTypeInfo().vhdlMod( readVal(), that );
}

Value
ObjectBase::vhdlRem( const RValue &that ) const {
  return getTypeInfo().vhdlRem( readVal(), that );
}

Value
ObjectBase::vhdlPow( const RValue &that ) const {
  return getTypeInfo().vhdlPow( readVal(), that );
}

Value
ObjectBase::vhdlNot() const {
  return getTypeInfo().vhdlNot( readVal() );
}

Value
ObjectBase::vhdlXor( const RValue &rhs ) const {
  return getTypeInfo().vhdlXor( readVal(), rhs );  
}

Value
ObjectBase::vhdlXnor( const RValue &rhs ) const {
  return getTypeInfo().vhdlXnor( readVal(), rhs );  
}

RValue &
ObjectBase::all(){
  ASSERT( myData != 0 );
  return myData->all();
}

const RValue &
ObjectBase::all() const {
  return const_cast<ObjectBase *>(this)->all();
}

SignalNetinfo &
ObjectBase::addToFanOut( VHDLKernel * ){
  cerr << "Error - ObjectBase::addToFanOut called - aborting!" << endl;
  abort();
  return *new SignalNetinfo;
}

SignalNetinfo &
ObjectBase::addChild( SignalNetinfo &, VHDLKernel * ){
  cerr << "Error - ObjectBase::addChild( SignalNetinfo &, VHDLKernel * ) called - aborting!" << endl;
  abort();
  return *new SignalNetinfo;
}

void
ObjectBase::voidElements(){
  dynamic_cast<CompositeLiteral *>(myData)->voidElements();
}

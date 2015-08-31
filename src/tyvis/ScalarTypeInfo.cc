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
#include "IntegerLiteral.hh"
#include "EnumerationLiteral.hh"
#include "std_standardPkg.hh"

bool
ScalarTypeInfo::operator==( const TypeInfo &that ) const {
  bool retval = false;
  if( that.isScalarType() ){
    retval = (left() == that.left());
    retval = retval && (getDirection() == that.getDirection());
    retval = retval && (right() == that.right());
  }
  return retval;  
}

ScalarTypeInfo::ScalarTypeInfo()
  : myLeft( INTEGER_MIN.clone() ),
    myDirn( ArrayInfo::to ),
    myRight( INTEGER_MAX.clone() ){}

ScalarTypeInfo::ScalarTypeInfo( const RValue *initLeft,
				ArrayInfo::ArrayDirn_t initDirn,
				const RValue *initRight ) :
  myLeft( initLeft ),
  myDirn( initDirn ),
  myRight( initRight ){}

ScalarTypeInfo::ScalarTypeInfo( const RValue &initLeft,
				ArrayInfo::ArrayDirn_t initDirn,
				const RValue &initRight ) :
  myLeft( initLeft.clone() ),
  myDirn( initDirn ),
  myRight( initRight.clone() ){}

ScalarTypeInfo::ScalarTypeInfo( const ScalarTypeInfo &that ) :
  TypeInfo(that),
  myLeft( that.left().clone() ),
  myDirn( that.getDirection() ),
  myRight( that.right().clone() ){}

ScalarTypeInfo &
ScalarTypeInfo::operator=( const ScalarTypeInfo &that ){
  if( &that != this ){
    delete myLeft;
    delete myRight;
    
    myLeft = that.left().clone();
    myDirn = that.myDirn;
    myRight = that.right().clone();
  }

  return *this;
}

void
ScalarTypeInfo::setLeft( const RValue &newLeft ){
  delete myLeft;
  myLeft = newLeft.clone();
}

void
ScalarTypeInfo::setDirection( ArrayInfo::ArrayDirn_t newDirection ){
  myDirn = newDirection;
}

void
ScalarTypeInfo::setRight( const RValue &newRight ){
  delete myRight;
  myRight = newRight.clone();
}

ScalarTypeInfo::~ScalarTypeInfo(){
  delete myLeft;
  myLeft = 0;
  delete myRight;
  myRight = 0;  
}

StringLiteral
ScalarTypeInfo::image( const RValue &imageOf ) const {
  return VHDL_STRING( imageOf.toString() );
}

const RValue &
ScalarTypeInfo::high( const RValue &index ) const {
  ASSERT( index.getIntValue() == 1 );
  if( ascending().getIntValue() ){
    return right();
  }
  else{
    return left();
  }
}

const RValue &
ScalarTypeInfo::low( const RValue &index ) const {
  ASSERT( index.getIntValue() == 1 );

  if( ascending().getIntValue() ){
    return left();
  }
  else{
    return right();
  }
}

const EnumerationLiteral &
ScalarTypeInfo::ascending( const RValue &index ) const {
  ASSERT( index.getIntValue() == 1 );
  if( getDirection() == ArrayInfo::to ){
    return SAVANT_BOOLEAN_TRUE;
  }
  else{
    return SAVANT_BOOLEAN_FALSE;
  }
}

RangeIterator
ScalarTypeInfo::iterator( const RValue &index ) const {
  ASSERT( index.getIntValue() == 1 );
  return RangeIterator( this );
}

UniversalInteger
ScalarTypeInfo::length( const RValue &index ) const {
  ASSERT( index.getIntValue() == 1 );
  int retval = high().getIntValue() - low().getIntValue() + 1;
  if( retval <  0 ) retval = 0;
  return UniversalInteger(retval);
}

const ScalarTypeInfo &
ScalarTypeInfo::range( const RValue &index) const {
  return *this;
}

const ScalarTypeInfo &
ScalarTypeInfo::reverse_range( const RValue &index) const {
  cerr << "ScalarTypeInfo::reverse_range not implemented" << endl;
  abort();
}

int
ScalarTypeInfo::storageIndex( int elementIndex ) const {
  int retval = -1;
  if( getDirection() == ArrayInfo::to ){
    retval = ( elementIndex - left().getIntValue() );
  }
  else{
    retval = ( left().getIntValue() - elementIndex );
  }
  return retval;
}

void
ScalarTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "Range: " << left().getIntValue();
  if (getDirection() == ArrayInfo::to)
    os << " to ";
  else if (getDirection() == ArrayInfo::downto)
    os << " downto ";
  else
    os << " :: ";
  os << right().getIntValue();
  return;
}

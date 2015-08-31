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

#include "RangeIterator.hh"
#include "IntegerTypeInfo.hh"
#include "ScalarTypeInfo.hh"
#include "ArrayInfo.hh"

RangeIterator::RangeIterator( const RangeIterator &that ) :
  myRange( that.myRange ),
  pos( that.pos ),
  _hasNext( that._hasNext ){}

RangeIterator::~RangeIterator(){
  if (myRange)
    delete myRange;
}

RangeIterator::RangeIterator( const ScalarTypeInfo *initRange ) :
  myRange( dynamic_cast<ScalarTypeInfo *>(initRange->clone()) ){
  reset();
}

bool
RangeIterator::hasNext() const {
  return _hasNext;
}

void
RangeIterator::setHasNext(){
  if( myRange->getDirection() == ArrayInfo::to ){
    _hasNext = ( pos <= myRange->right().getIntValue() );
  }
  else{
    _hasNext = ( pos >= myRange->right().getIntValue() );
  }
}

int
RangeIterator::next(){
  // The ordering of these operations is tricky.  Be leary of messing with
  // it :-)
  int retval = pos;
  if (pos == myRange->right().getIntValue())
    _hasNext = false;
  if( myRange->getDirection() == ArrayInfo::to ){
    pos++;
  }
  else{
    pos--;
  }
  return retval;
}

int
RangeIterator::peek(){
  return pos;
}

void
RangeIterator::set(int pos_in){
  if ( myRange->getDirection() == ArrayInfo::to ){
    if ( pos >= myRange->left().getIntValue() && pos <= myRange->right().getIntValue() )
      pos = pos_in;
  }
  else{
    if ( pos <= myRange->left().getIntValue() && pos >= myRange->right().getIntValue() )
      pos = pos_in;
  }
  ASSERT( pos == pos_in );
  setHasNext();
}

int 
RangeIterator::length() const {
  int retval = -1;
  if( myRange->getDirection() == ArrayInfo::to ){
    retval = myRange->right().getIntValue() - myRange->left().getIntValue() + 1;
  }
  else{
    retval = myRange->left().getIntValue() - myRange->right().getIntValue() + 1;
  }
  return retval;
}

void
RangeIterator::reset(){
  pos = myRange->left().getIntValue();
  setHasNext();
}

void
RangeIterator::reset( const ScalarTypeInfo & newRange ){
  *myRange = newRange;
  reset();
}

void
RangeIterator::reset( const ArrayInfo & newRange ){
  myRange->setLeft(UniversalInteger(newRange.left()));
  myRange->setRight(UniversalInteger(newRange.right()));
  reset();
}

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

#include "Aggregate.hh"
#include "ArrayTypeInfo.hh"
#include "EnumerationTypeInfo.hh"
#include "IntegerTypeInfo.hh"
#include "ObjectBase.hh"
#include "StringLiteral.hh"
#include "std_standardPkg.hh"
#include "SubElementFactory.hh"
#include <clutils/StringUtilities.h>

//ArrayTypeInfo constructors
ArrayTypeInfo::ArrayTypeInfo() :
  myIndexTypeInfo(0),
  myElementTypeInfo(0){}

ArrayTypeInfo::ArrayTypeInfo( const TypeInfo &initElementTypeInfo,
			      const ScalarTypeInfo &initRange ) :
  myIndexTypeInfo( dynamic_cast<ScalarTypeInfo *>(initRange.clone()) ),
  myElementTypeInfo( initElementTypeInfo.clone() ){}

ArrayTypeInfo::ArrayTypeInfo( const ArrayTypeInfo &that ) :
  CompositeTypeInfo( that ),
  myIndexTypeInfo( that.myIndexTypeInfo?dynamic_cast<ScalarTypeInfo *>(that.myIndexTypeInfo->clone()):0 ),
  myElementTypeInfo( that.myElementTypeInfo?that.myElementTypeInfo->clone():0 ){}

ArrayTypeInfo::~ArrayTypeInfo(){
  delete myIndexTypeInfo;
  delete myElementTypeInfo;
  myIndexTypeInfo = 0;
  myElementTypeInfo = 0;
}

bool
ArrayTypeInfo::operator==( const TypeInfo &that ) const {
  bool retval = false;
  if( dynamic_cast<const ArrayTypeInfo *>(&that) ){
    const ArrayTypeInfo *thatArrayType = dynamic_cast<const ArrayTypeInfo *>(&that);
    retval = (*myIndexTypeInfo == *thatArrayType->myIndexTypeInfo);
    retval = retval && (*myElementTypeInfo == *thatArrayType->myElementTypeInfo);
  }
  return retval;
}

TypeInfo *
ArrayTypeInfo::clone() const {
  return new ArrayTypeInfo( *this );
}

bool
ArrayTypeInfo::isUnconstrained() const {
  return !isConstrained();
}

bool
ArrayTypeInfo::isCharacterType() const {
  return getElementTypeInfo().isCharacterType();
}

int
ArrayTypeInfo::numDimensions() const {
  cerr << "ArrayTypeInfo::numDimensions() called!" << endl;
  abort();
}

bool
ArrayTypeInfo::isConstrained() const {
  bool retval = true;
  if( myIndexTypeInfo && myIndexTypeInfo->left() == std_standard_integerTypeInfo().left() ){
    retval = false;
  }
  else if ( myIndexTypeInfo && myIndexTypeInfo->left() == std_standard_integerTypeInfo().right() ){
    retval = false;
  }
  else if ( myIndexTypeInfo && myIndexTypeInfo->right() == std_standard_integerTypeInfo().right() ){
    retval = false;
  }
  else if ( myIndexTypeInfo && myIndexTypeInfo->right() == std_standard_integerTypeInfo().left() ){
    retval = false;
  }
  else {
    if( dynamic_cast<const ArrayTypeInfo *>(myElementTypeInfo) != 0 ){
      retval = dynamic_cast<const ArrayTypeInfo *>(myElementTypeInfo)->isConstrained();
    }
  }

  return retval;
}

const TypeInfo &
ArrayTypeInfo::getElementTypeInfo( const RValue &index ) const {
  const TypeInfo *retval = 0;
  ASSERT( myElementTypeInfo != 0 );
  if( index.getIntValue() > 1 ){
    retval = &dynamic_cast<const ArrayTypeInfo *>
      (myElementTypeInfo)->getElementTypeInfo( UniversalInteger( index.getIntValue() - 1 ) );
  }
  else{
    retval = myElementTypeInfo;
  }
  return *retval;
}

const ScalarTypeInfo &
ArrayTypeInfo::getIndexTypeInfo( const RValue &index ) const {
  const ScalarTypeInfo *retval = 0;
  ASSERT( myIndexTypeInfo != 0 );
  if( index.getIntValue() > 1 ){
    retval = &dynamic_cast<const ArrayTypeInfo *>(myElementTypeInfo)->getIndexTypeInfo( UniversalInteger( index.getIntValue() - 1 ) );
  }
  else{
    retval = myIndexTypeInfo;
  }
  return *retval;
}

RValue *
ArrayTypeInfo::create( const SubElementFactory *subElementFactory ) const {
  RValue *retval = 0;
  if( subElementFactory != 0 ){
    if( isCharacterType() ){
      retval = new StringLiteral( *subElementFactory );
    }
    else {
      retval = new Aggregate( *subElementFactory );
    }
  }
  else{
    if( isCharacterType() ){
      retval = new StringLiteral( *this );
    }
    else {
      retval = new Aggregate( *this );
    }
  }
  return retval;
}

string
ArrayTypeInfo::getSubFieldName( const string &prefix, int d ) const {
  return prefix + "(" + intToString(d) + ")";
}

const TypeInfo &
ArrayTypeInfo::getFieldInfo( int ) const {
  return getElementTypeInfo();
}

void
ArrayTypeInfo::constructFields( vector<RValue *> &elements, 
				const SubElementFactory *subElementFactory,
				int numElements ) const {
  if( numElements == -1 ){
    RangeIterator i = range(defaultIndex()).iterator();
    while( i.hasNext() ){
      int fieldIndex = i.next();
      RValue *element = 0;
      if( subElementFactory ){
	element = (*subElementFactory)(fieldIndex);
      }
      else{
	element = getElementTypeInfo().create();
      }
      elements.push_back(element);
    }
  }
  else{
    ASSERT( numElements > 0 );
    for( int i = 0; i < numElements; i++ ){
      RValue *element = 0;
      if( subElementFactory ){
	element = (*subElementFactory)(i);
      }
      else{
	element = getElementTypeInfo().create();
      }
      elements.push_back(element);
    }
  }
}

int
ArrayTypeInfo::storageIndex( int elementIndex ) const {
  int retval = -1;

  ASSERT( myIndexTypeInfo != 0 );
  retval = myIndexTypeInfo->storageIndex( elementIndex );
  ASSERT( retval >= 0 );

  return retval;
}

const RValue &
ArrayTypeInfo::left( const RValue &dimension ) const {
  if ( dimension.getIntValue() == 1 ){
    return myIndexTypeInfo->left();
  }
  else{
    return myElementTypeInfo->left( UniversalInteger( dimension.getIntValue() - 1 ));
  }
}

const RValue &
ArrayTypeInfo::right( const RValue &dimension ) const {
  if ( dimension.getIntValue() == 1 ){
    return myIndexTypeInfo->right();
  }
  else{
    return myElementTypeInfo->right( UniversalInteger( dimension.getIntValue() - 1 ));
  }
}

const RValue &
ArrayTypeInfo::low( const RValue &dimension ) const {
  if ( dimension.getIntValue() == 1 ){
    return myIndexTypeInfo->low();
  }
  else{
    return myElementTypeInfo->low( UniversalInteger( dimension.getIntValue() - 1 ));
  }
}

const RValue &
ArrayTypeInfo::high( const RValue &dimension ) const {
  if ( dimension.getIntValue() == 1 ){
    return myIndexTypeInfo->high();
  }
  else{
    return myElementTypeInfo->high( UniversalInteger( dimension.getIntValue() - 1 ));
  }
}

const ScalarTypeInfo &
ArrayTypeInfo::range( const RValue &index ) const {
  if( index.getIntValue() == 1 ){
    ASSERT( myIndexTypeInfo != 0 );
    return *myIndexTypeInfo;
  }
  else{
    return myElementTypeInfo->range( UniversalInteger( index.getIntValue() - 1 ));
  }
}

const ScalarTypeInfo &
ArrayTypeInfo::reverse_range( const RValue & ) const {
  abort();
}

UniversalInteger
ArrayTypeInfo::length( const RValue &index ) const {
  return range( index ).length();
}


Value
ArrayTypeInfo::valueFromAggregate( const string &initValue ) const {
  return Value(Aggregate( *this, initValue ));
}

Value
ArrayTypeInfo::valueFromString( const string &initValue ) const {
  return Value( StringLiteral( *this, initValue ));
}

Value
ArrayTypeInfo::value( const string &initValue ) const {
  Value retval;

  if( initValue[0] == '(' ){
    retval = valueFromAggregate( initValue );
  }
  else {
    retval = valueFromString( initValue );
  }
  return retval;
}

void
ArrayTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "TypeInfo *myElementTypeInfo:";
  if (myElementTypeInfo)
    myElementTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";
  os << NL(indent) << "ScalarTypeInfo *myIndexTypeInfo:";
  if (myIndexTypeInfo)
    myIndexTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";

  return;
}

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

#include "Aggregate.hh"
#include "ArrayTypeInfo.hh"
#include "EnumerationLiteral.hh"
#include "UniversalInteger.hh"
#include "TyvisVarArgs.hh"
#include <clutils/Iterator.h>

using std::ends;

Aggregate::Aggregate() : CompositeLiteral(){}

Aggregate::Aggregate( const ArrayTypeInfo &typeInfo )
  : CompositeLiteral( typeInfo ){}

Aggregate::Aggregate( const ArrayTypeInfo &typeInfo,
		      TyvisVarArgs &initValue )
  : CompositeLiteral( typeInfo, initValue ){}

Aggregate::Aggregate( TyvisVarArgs &initValue ) :
  CompositeLiteral( initValue ){}

Aggregate::Aggregate( const ArrayTypeInfo &typeInfo,
		      const string &initValue ) :
  CompositeLiteral( typeInfo, initValue ){}


Aggregate::Aggregate( const SubElementFactory &subElementFactory ) :
  CompositeLiteral( subElementFactory ){}

Aggregate::Aggregate( const Aggregate &that ) :
  CompositeLiteral( that ){}

Aggregate::Aggregate( Aggregate &that,
		      const ScalarTypeInfo &sliceSpecifier ) :
  CompositeLiteral( that, sliceSpecifier ){}

Aggregate::Aggregate( const ArrayTypeInfo &typeInfo,
		      const RValue &initValue ) :
  CompositeLiteral( typeInfo, initValue ){}

Aggregate::~Aggregate(){}

void 
Aggregate::printDimension( ostream &os, int dimension ) const {
  abort();
}

void 
Aggregate::print( ostream &os ) const {
  os << toString();
}

string
Aggregate::toString() const { 
  string retval;
  retval += '(';
  for( int i = 0; i < length().getIntValue(); i++ ){
    retval += getField(i).toString();
    if( i + 1 < length().getIntValue() ){
      retval += ", ";
    }
  }
  retval += ')';

  return retval;
}
		       
RValue *
Aggregate::clone() const {
  return new Aggregate(*this);
}

const string &
Aggregate::getDataType() const {
  return getAggregateDataType();
}

const string &
Aggregate::getAggregateDataType(){
  static const string arrayDataType( "Aggregate" );
  return arrayDataType;
}

void 
Aggregate::serialize( SerializedInstance * ) const {
  cerr << "Aggregate::serialize not yet implemented!" << endl;
  abort();
}

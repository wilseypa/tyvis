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

#include "StringLiteral.hh"
#include "EnumerationLiteral.hh"
#include "UniversalInteger.hh"
#include "std_standardPkg.hh"

using std::ends;

StringLiteral::StringLiteral() : Slice(){}

StringLiteral::StringLiteral( const ArrayTypeInfo &typeInfo )
  : Slice( typeInfo ){}

StringLiteral::StringLiteral( const ArrayTypeInfo &typeInfo,
			      const string &initValue ) :
  Slice( typeInfo, initValue ){}

StringLiteral::StringLiteral( const SubElementFactory &subElementFactory ) :
  Slice( subElementFactory ){}

StringLiteral::StringLiteral( const StringLiteral &that ) :
  Slice( that ){}

StringLiteral::StringLiteral( StringLiteral &that,
			      const ScalarTypeInfo &sliceSpecifier ) :
  Slice( that, sliceSpecifier ){}

StringLiteral::~StringLiteral(){}

void 
StringLiteral::printDimension( ostream &os, int dimension ) const {
  abort();
}

void 
StringLiteral::print( ostream &os ) const {
  os << toString();
}

string
StringLiteral::toString() const { 
  string retval;
  int len = length().getIntValue();
  for( int i = 0; i < len; i++ ){
    string elementString = getField(i).toString();
    if (elementString[0] == '\'' && elementString[2] == '\'') {
      retval += elementString[1];
    }
    else {
      retval += elementString;
    }
  }
  return retval;
}

RValue *
StringLiteral::clone() const {
  return new StringLiteral(*this);
}

const string &
StringLiteral::getDataType() const {
  return getStringLiteralDataType();
}

const string &
StringLiteral::getStringLiteralDataType(){
  static const string arrayDataType( "StringLiteral" );
  return arrayDataType;
}

void 
StringLiteral::serialize( SerializedInstance * ) const {
  cerr << "StringLiteral::serialize not yet implemented!" << endl;
  abort();
}

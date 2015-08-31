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

#include "Types.hh"
#include "Record.hh"
#include "UniversalInteger.hh"
#include "EnumerationLiteral.hh"
#include "VHDLKernel.hh"
#include "Element.hh"

Record::~Record(){}

Record::Record( const RecordTypeInfo &typeInfo,
		TyvisVarArgs &initValue ) :
  CompositeLiteral( typeInfo, initValue ){}

/** 
    Constructor
*/
Record::Record( const RecordTypeInfo &typeInfo ) :
  CompositeLiteral( typeInfo ){}

  
Record::Record( const TypeInfo &typeInfo,
	const RValue &initValue ) :
  CompositeLiteral( typeInfo, initValue ){}

Record::Record( const SubElementFactory &factory ) :
  CompositeLiteral( factory ){}
  
/** 
    Copy Constructor
*/
Record::Record( const Record &that ) :
  CompositeLiteral( that ){}


RValue *
Record::clone() const {
  return new Record(*this);
}


RValue &
Record::operator=( const RValue &source ){
  return operator=(static_cast<const Record &>(source));
}

Record &
Record::operator=(const Record &that ){
  CompositeLiteral::operator=( that );
  return *this;
}

const string &
Record::getDataType() const {
  return getRecordDataType();
}

const string &
Record::getRecordDataType(){
  static const string retval("Record");
  return retval;
}

void
Record::serialize( SerializedInstance * ) const {
  abort();
}

string
Record::toString() const {
  bool first = true;
  string retval = "(";
  for( int i = 0;
       i < static_cast<const RecordTypeInfo &>(getTypeInfo()).length();
       i++, first = false ){
    if( !first ){
      retval += ", ";
    }
    retval += static_cast<const RecordTypeInfo &>(getTypeInfo()).getFieldInfo(i).getIdentifier();
    retval += "=>";
    retval += getField(i).toString();
  }
  retval += ")";

  return retval;
}

void
Record::print( ostream &os ) const {
  os << toString();
}


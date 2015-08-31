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

#include "Element.hh"
#include "IntegerTypeInfo.hh"
#include "RecordTypeInfo.hh"
#include "Record.hh"
#include "SubElementFactory.hh"
#include <cstdarg>

RecordTypeInfo::RecordTypeInfo() :
  indexTypeInfo(0){}

RecordTypeInfo::~RecordTypeInfo(){
  for( vector<ElementInfo *>::const_iterator i = myElementInfos.begin();
       i < myElementInfos.end();
       i++ ){
    delete *i;
  }
  myElementInfos.clear();
  delete indexTypeInfo;
  indexTypeInfo = 0;
}


RecordTypeInfo::RecordTypeInfo( const vector<ElementInfo *> &initElements ) :
  CompositeTypeInfo(),
  indexTypeInfo(0){
  cloneElementInfos( initElements );
}

RecordTypeInfo::RecordTypeInfo(const RecordTypeInfo &that) :
  CompositeTypeInfo( that ),
  indexTypeInfo( that.indexTypeInfo ? dynamic_cast<ScalarTypeInfo *>(that.indexTypeInfo->clone())
		 : 0 ){
  cloneElementInfos( that.myElementInfos );
}

TypeInfo *
RecordTypeInfo::clone() const {
  return new RecordTypeInfo( *this );
}

bool
RecordTypeInfo::operator==( const TypeInfo &that ) const {
  bool retval = false;
  if( dynamic_cast<const RecordTypeInfo *>( &that ) ){
    const RecordTypeInfo *thatRecordType = dynamic_cast<const RecordTypeInfo *>( &that );
    if( myElementInfos.size() && thatRecordType->myElementInfos.size() ){
      bool retval = true;
      for( unsigned int i = 0;
	   i < myElementInfos.size();
	   i++ ){
	// I know this is ugly...
	if( myElementInfos[i] == thatRecordType->myElementInfos[i] ){}
	else{
	  retval = false;
	  break;
	}
      }
    }
  }
  return retval;
}

string
RecordTypeInfo::getSubFieldName( const string &prefix, int d ) const {
  return prefix + "." + getFieldInfo(d).getIdentifier();
}


const ElementInfo &
RecordTypeInfo::getFieldInfo(const int index) const {
  ElementInfo *retval = 0;
  assert( index >= 0 );
  assert( index < getFieldCount() );
  retval = myElementInfos[index];
  assert( retval != 0 );
  return *retval;
}

int
RecordTypeInfo::getFieldCount() const {
  return myElementInfos.size();
}

int
RecordTypeInfo::length() const {
  return getFieldCount();
}

void
RecordTypeInfo::addField( const ElementInfo &toAdd ){
  ElementInfo *clone = dynamic_cast<ElementInfo *>(toAdd.clone());
  assert( clone != 0 );
  myElementInfos.push_back( clone );
}

RecordTypeInfo&
RecordTypeInfo::operator=(const RecordTypeInfo &that) {
  indexTypeInfo =
    that.indexTypeInfo ? dynamic_cast<const ScalarTypeInfo *>(that.indexTypeInfo->clone()) : 0;
  cloneElementInfos( that.myElementInfos );
  return *this;
}

void
RecordTypeInfo::cloneElementInfos( const vector<ElementInfo *> &those ){
  for( vector<ElementInfo *>::const_iterator i = those.begin();
       i < those.end();
       i++ ){
    addField( *(*i) );
  }
}

RValue *
RecordTypeInfo::create( const SubElementFactory *subElementFactory ) const {
  RValue *retval = 0;
  if( subElementFactory != 0 ){
    retval = new Record( *subElementFactory );
  }
  else{
    retval = new Record( *this );
  }
  return retval;
}

void
RecordTypeInfo::constructFields( vector<RValue *> &fieldVector,
				 const SubElementFactory *subElementFactory,
				 int numElements ) const {
  ASSERT( numElements == -1 ); // >0 is for unconstrained vectorso
  int counter = 0;
  for( vector<ElementInfo *>::const_iterator i = myElementInfos.begin();
       i < myElementInfos.end();
       i++ ){
    if( subElementFactory != 0 ){
      fieldVector.push_back( subElementFactory->operator()(counter) );
      counter++;
    }
    else{
      fieldVector.push_back( (*i)->create(0) );
    }
  }
}

const TypeInfo &
RecordTypeInfo::getElementTypeInfo( const RValue &index ) const {
  return getFieldInfo( index.getIntValue() ).getTypeInfo();
}

const ScalarTypeInfo &
RecordTypeInfo::getIndexTypeInfo( const RValue &index ) const {
  ASSERT( index.getIntValue() == 1 );
  if( !indexTypeInfo ){
    indexTypeInfo = new IntegerTypeInfo( 0, ArrayInfo::to, myElementInfos.size() - 1 );
  }
  return *indexTypeInfo;
}

void
RecordTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "ScalarTypeInfo *indexTypeInfo:";
  if (indexTypeInfo)
    indexTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";
  os << NL(indent) << "vector<ElementInfo *> myElementInfos";
  if (myElementInfos.size() > 0) {
    for (int i=0 ; i<myElementInfos.size() ; ++i) {
      os << NL(indent) << "(" << i << ") ElementInfo*: ";
      if (myElementInfos[i]) {
        os << "\"" << myElementInfos[i]->getIdentifier() << "\"";
        myElementInfos[i]->getTypeInfo().debugPrint(os, indent+2);
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

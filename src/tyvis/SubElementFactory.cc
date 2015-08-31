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

#include "SubElementFactory.hh"
#include "Variable.hh"
#include "Signal.hh"
#include "SignalNetinfo.hh"
#include <iostream>
using std::cerr;
using std::endl;

SubElementFactory::SubElementFactory( const TypeInfo &initParentTypeInfo,
				      ObjectBase *initParent,
				      const string &initParentName,
				      ObjectType initObjectType ) :
  myParentTypeInfo( initParentTypeInfo.readVal() ),
  myParent( initParent ),
  myParentName( initParentName ),
  myObjectType( initObjectType ){}

SubElementFactory::SubElementFactory( const SubElementFactory &that ) :
  myParentTypeInfo( that.myParentTypeInfo ),
  myParent( that.myParent ),
  myParentName( that.myParentName ),
  myObjectType( that.myObjectType ){}

SubElementFactory::~SubElementFactory(){
  // Our parent certainly is not our memory to delete.  Of course, we can
  // only hope that it stays valid through our own lifetime.
}

RValue *
SubElementFactory::operator()( const int subField ) const {
  RValue *retval = 0;
  if( getParentTypeInfo().isCompositeType() ){
    switch( myObjectType ){
    case VARIABLE:
      retval = createVariable( subField );
      break;
    case SIGNAL:
      retval = createSignal( subField );
      break;
    case NETINFO:
      retval = createNetInfo( subField );
      break;
    default:
      cerr << "Unknown ObjectType " << myObjectType << " in SubElementFactory()" << endl;
      abort();
    }
  }
  else{
    // This is not a composite type... Simply return a value.
    retval = getParentTypeInfo().create();
  }
  return retval;
}

const TypeInfo &
SubElementFactory::getParentTypeInfo() const {
  return myParentTypeInfo;
}

const string &
SubElementFactory::getParentName() const {
  return myParentName;
}

SubElementFactory *
SubElementFactory::clone() const {
  return new SubElementFactory( *this );
}

Variable *
SubElementFactory::createVariable( const int subField ) const {
  return new Variable( myParentTypeInfo.getFieldInfo(subField),
		       myParentTypeInfo.getSubFieldName( myParentName, subField ) );
}

Signal *
SubElementFactory::createSignal( const int subField ) const {
  Signal *retval = new Signal( myParentTypeInfo.getFieldInfo(subField),
			       myParentTypeInfo.getSubFieldName( myParentName, subField ) );
  // The parent is not fully constructed yet so a dynamic_cast will not
  // work here.
  retval->setParentSignal( reinterpret_cast<SignalBase *>(myParent) );
  return retval;
}

SignalNetinfo *
SubElementFactory::createNetInfo( const int subField ) const {
  SignalNetinfo *retval = new SignalNetinfo( myParentTypeInfo.getFieldInfo(subField),
					     myParentTypeInfo.getSubFieldName( myParentName,
									       subField ) );
  return retval;
}

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

#include "ElementInfo.hh"
#include "PhysicalLiteral.hh"
#include "Element.hh"

ElementInfo::ElementInfo( const TypeInfo &initTypeInfo, 
			  const string &initIdentifier ) :
  myTypeInfo( initTypeInfo.clone() ),
  myIdentifier( initIdentifier ){}

ElementInfo::ElementInfo( const ElementInfo &that ) :
  TypeInfo( that ),
  myTypeInfo( that.myTypeInfo ? that.myTypeInfo->clone() : 0 ),
  myIdentifier( that.getIdentifier() ){}

TypeInfo *
ElementInfo::clone() const {
  return new ElementInfo( *this );
}

ElementInfo &
ElementInfo::operator=( const ElementInfo &that ){
  TypeInfo::operator=( that );

  myTypeInfo = that.myTypeInfo ? that.myTypeInfo->clone() : 0;
  myIdentifier = that.getIdentifier();

  return *this;
}

bool
ElementInfo::operator==( const TypeInfo &that ) const {
  bool retval = false;
  if( dynamic_cast<const ElementInfo *>( &that ) ){
    const ElementInfo *thatElementInfo = dynamic_cast<const ElementInfo *>( &that );
    retval = (*myTypeInfo == *thatElementInfo->myTypeInfo);
    retval = retval && ( myIdentifier == thatElementInfo->getIdentifier() );
  }
  return retval;
}

ElementInfo::~ElementInfo(){
  delete myTypeInfo;
}

const string &
ElementInfo::getIdentifier() const {
  return myIdentifier;
}

const TypeInfo &
ElementInfo::getTypeInfo() const {
  ASSERT( myTypeInfo != 0 );
  return *myTypeInfo;
}

const ScalarTypeInfo &
ElementInfo::range(const RValue &index) const {
  return myTypeInfo->range(index);
}

const ScalarTypeInfo &
ElementInfo::reverse_range(const RValue &index) const {
  return myTypeInfo->reverse_range(index);
}

RValue *
ElementInfo::create( const SubElementFactory *factory ) const{
  Element *retval = 0;
//   ASSERT( myTypeInfo != 0 );
//   if( retval != 0 ){
//     cerr << "Doh!";
//     abort();
//   }
//   else{
    retval = new Element( *myTypeInfo, myIdentifier );
//   }
  return retval;
}

bool
ElementInfo::isCompositeType() const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->isCompositeType();
}
 
bool
ElementInfo::isScalarType() const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->isScalarType();
}
 
bool
ElementInfo::isPhysicalType() const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->isPhysicalType();
}
 
bool
ElementInfo::isCharacterType() const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->isCharacterType();
}
 
void
ElementInfo::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent);
  os << "id = \"" << myIdentifier << "\" ";
  os << "TypeInfo *myTypeInfo: ";
  if (myTypeInfo)
    myTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";
  return;
}


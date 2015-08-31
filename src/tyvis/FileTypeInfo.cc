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

#include "FileTypeInfo.hh"
#include "UniversalInteger.hh"

FileTypeInfo::FileTypeInfo( const TypeInfo &elementTypeInfo ) :
  myElementTypeInfo( elementTypeInfo.clone() ){}

FileTypeInfo::FileTypeInfo( const FileTypeInfo &that ) :
  TypeInfo( that ),
  myElementTypeInfo( that.getElementTypeInfo().clone() ){}

FileTypeInfo::~FileTypeInfo(){
  delete myElementTypeInfo;
  myElementTypeInfo = 0;
}

TypeInfo *
FileTypeInfo::clone() const {
  return new FileTypeInfo( *this );
}

bool
FileTypeInfo::operator==( const TypeInfo &that ) const {
  bool retval = false;
  if( dynamic_cast<const FileTypeInfo *>( &that ) ){
    const FileTypeInfo *thatFileType = dynamic_cast<const FileTypeInfo *>( &that );
    retval = (*myElementTypeInfo == *thatFileType->myElementTypeInfo);
  }
  return retval;
}

RValue *
FileTypeInfo::create( const SubElementFactory * ) const {
  // Just so we return something, should not be used anywhere...
  return new UniversalInteger(0);
}

string
FileTypeInfo::writeToString( const RValue &value ) const {
  // We should actually ask the type to do this.  But right now this will
  // work.
  return value.toString();
}

const TypeInfo &
FileTypeInfo::getElementTypeInfo() const {
  ASSERT( myElementTypeInfo != 0 );
  return *myElementTypeInfo;
}

void
FileTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent);  
  os << "TypeInfo *myElementTypeInfo: ";
  if (myElementTypeInfo)
    myElementTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";
  return;
}

#include "ElementDeclaration.hh"
#include "PhysicalLiteral.hh"

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

ElementDeclaration::ElementDeclaration( const TypeInfo &initTypeInfo, 
					const string &initIdentifier ) :
  ObjectBase( initTypeInfo ),
  myIdentifier( initIdentifier ){}

ElementDeclaration::ElementDeclaration( const ElementDeclaration &that ) :
  ObjectBase( that ),
  myIdentifier( that.getIdentifier() ){}

ElementDeclaration &
ElementDeclaration::operator=( const ElementDeclaration &that ){
  ObjectBase::operator=(that);
  myIdentifier = that.getIdentifier();

  return *this;
}

bool
ElementDeclaration::operator==( const ElementDeclaration &that ) const {
  bool retval = ObjectBase::operator==( that );
  if( retval &&
      (myIdentifier == that.getIdentifier()) ){
    retval = true;
  }
  return retval;
}

ElementDeclaration::~ElementDeclaration(){}

const string &
ElementDeclaration::getIdentifier() const {
  return myIdentifier;
}

void
ElementDeclaration::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "id = " << myIdentifier;
  ObjectBase::debugPrintDetails( os, indent);

  return;
}

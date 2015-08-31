#include "UnitDeclaration.hh"
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

UnitDeclaration::UnitDeclaration( const string &initIdentifier,
				  const RValue &initMultiplier ) :
  myIdentifier( initIdentifier ),
  myMultiplier( dynamic_cast<PhysicalLiteral *>
		(PhysicalLiteral( initMultiplier.getInt64Value(), initIdentifier ).clone()) ){}

UnitDeclaration::UnitDeclaration( const string &initIdentifier,
				  const PhysicalLiteral &initMultiplier ) :
  myIdentifier( initIdentifier ),
  myMultiplier( dynamic_cast<PhysicalLiteral *>(initMultiplier.clone()) ){}

UnitDeclaration::UnitDeclaration( const UnitDeclaration &that ) :
  myIdentifier( that.getIdentifier() ),
  myMultiplier( dynamic_cast<PhysicalLiteral *>(that.getMultiplier().clone()) ){}

UnitDeclaration::UnitDeclaration():
  myIdentifier(""),
  myMultiplier(0){}

UnitDeclaration &
UnitDeclaration::operator=( const UnitDeclaration &that ){
  myIdentifier = that.getIdentifier();
  myMultiplier = dynamic_cast<PhysicalLiteral *>(that.getMultiplier().clone());
  return *this;
}

bool
UnitDeclaration::operator==( const UnitDeclaration &that ) const {
  bool retval = false;
  if( myIdentifier == that.getIdentifier() ){
    if( (myMultiplier == that.myMultiplier) ||
	(myMultiplier && (*myMultiplier == that.getMultiplier() )) ){
      retval = true;
    }
  }
  return retval;
}


UnitDeclaration::~UnitDeclaration(){
  delete myMultiplier;
  myMultiplier = 0;
}

const string &
UnitDeclaration::getIdentifier() const {
  return myIdentifier;
}

const PhysicalLiteral &
UnitDeclaration::getMultiplier() const {
  return *myMultiplier;
}

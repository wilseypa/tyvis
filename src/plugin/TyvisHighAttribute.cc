
// Copyright (c) 1996-2003 The University of Cincinnati.
// All rights reserved.

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

// Authors: Philip A. Wilsey    philip.wilsey@ieee.org
//          Dale E. Martin      dmartin@cliftonlabs.com

//---------------------------------------------------------------------------

#include "TyvisConstantDeclaration.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisHighAttribute.hh"
#include "TyvisScalarTypeDefinition.hh"

const string &
TyvisHighAttribute::_get_cc_attribute_name() const {
  static const string retval("high");
  return retval;
}

TyvisDeclaration *
TyvisHighAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
						 TyvisTypeDefinition *decl_type ){
  TyvisDeclaration *retval;

  if( _get_suffix() == NULL ){
    TyvisConstantDeclaration *const_decl = _build_constant_declaration( decl_name, decl_type );

    ASSERT( decl_type->is_scalar_type() == TRUE );
    TyvisScalarTypeDefinition *as_scalar = dynamic_cast<TyvisScalarTypeDefinition *>(decl_type);

    // If the direction is NULL, then this must be deteremined at runtime.  See
    // billowitch tc404.vhdl
    if( as_scalar->get_direction() != NULL ){
      if( as_scalar->is_ascending_range() == TRUE ){
	const_decl->IIRBase_ConstantDeclaration::set_value( as_scalar->get_right() );
      }
      else{
	const_decl->IIRBase_ConstantDeclaration::set_value( as_scalar->get_left() );
      }
    }

    retval = const_decl;
  }
  else{
    retval = _build_function_declaration( decl_name, decl_type );
  }

  return retval;
}

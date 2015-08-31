
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

#include "TyvisAssociationList.hh"
#include "TyvisAggregate.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisConstantDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisSliceName.hh"
#include "TyvisQualifiedExpression.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisUserAttribute.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>

void 
TyvisUserAttribute::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisUserAttribute::_publish_cc_rvalue" );	
  ASSERT(_get_prefix() != NULL);
  ASSERT(_get_prefix()->is_resolved() == TRUE);
  ASSERT(_get_suffix() != NULL);
  ASSERT(_get_suffix()->is_resolved() == TRUE);

  _get_prefix()->_add_decl_into_declaration_collection(declarations);
  _get_suffix()->_add_decl_into_declaration_collection(declarations);

  TyvisAttributeSpecification* attr_spec = _get_prefix()->_get_attribute_specification(_get_suffix());

  if (_get_suffix()->get_kind() == IIR_SLICE_NAME)
    _cc_out << "Value(";
  _publish_cc_prefix_string( _cc_out );
  attr_spec->_publish_cc_rvalue( _cc_out , declarations );

  if( dynamic_cast<TyvisDeclaration *>(_get_suffix()) == NULL ) {
    if ((_get_suffix()->get_kind() != IIR_INDEXED_NAME) &&
	(_get_suffix()->get_kind() != IIR_SLICE_NAME)) {
      _cc_out << ".";
    }
    
    if (_get_suffix()->get_kind() == IIR_SLICE_NAME)
      dynamic_cast<TyvisSliceName *>(_get_suffix())->_publish_cc_rvalue_noprefix( _cc_out , declarations );
    else
      _get_suffix()->_publish_cc_rvalue( _cc_out , declarations );
  }
}

void 
TyvisUserAttribute::_publish_cc_bounds( published_file &_cc_out, PublishData * ) {
  ASSERT(_get_prefix() != NULL);
  ASSERT(_get_prefix()->is_resolved() == TRUE);
  ASSERT(_get_suffix() != NULL);
  ASSERT(_get_suffix()->is_resolved() == TRUE);
  _cc_out << "nullInfo()";
}

const string &
TyvisUserAttribute::_get_cc_attribute_name() const {
  if( attributeString == "" ){
    // We shouldn't have to do this but since most of the API is not
    // const-safe we have to for now.
    TyvisUserAttribute *mutableThis = const_cast<TyvisUserAttribute *>(this);
    
    if( mutableThis->_get_suffix() ){
      if( mutableThis->_get_suffix()->is_name() ){
	mutableThis->attributeString =
	  dynamic_cast<TyvisTextLiteral *>(dynamic_cast<TyvisName *>(mutableThis->_get_suffix())->get_prefix_string())->convert_to_string();
      }
      else if ( dynamic_cast<TyvisDeclaration *>(mutableThis->_get_suffix()) ){
	mutableThis->attributeString = mutableThis->_get_suffix()->_get_mangled_declarator()->convert_to_string();
      }
    }
  }
  return attributeString;
}

IIR_Boolean 
TyvisUserAttribute::_is_globally_static_primary(){
  TyvisDeclaration *implicit_decl = _get_implicit_declaration();

  ASSERT( implicit_decl != NULL );
  ASSERT( implicit_decl->get_kind() == IIR_CONSTANT_DECLARATION );
  TyvisConstantDeclaration *as_constant = dynamic_cast<TyvisConstantDeclaration *>(implicit_decl);
  ASSERT( as_constant->_get_value() != NULL );

  return as_constant->_get_value()->_is_globally_static();
}

TyvisDeclaration *
TyvisUserAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name, 
						 TyvisTypeDefinition *decl_type){
  return _build_constant_declaration( decl_name, decl_type );
}

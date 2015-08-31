
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

#include "Tyvis.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisQualifiedExpression.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void
TyvisQualifiedExpression::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisQualifiedExpression::_publish_cc_headers" );
  _get_type_mark()->_publish_cc_include( _cc_out );
}

void
TyvisQualifiedExpression::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisQualifiedExpression::_publish_cc" );
  _get_expression()->_publish_cc_rvalue( _cc_out, declarations );
}

void
TyvisQualifiedExpression::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisQualifiedExpression::_publish_cc" );
  _get_expression()->_publish_cc_lvalue( _cc_out , declarations );
}

void
TyvisQualifiedExpression::_add_decl_into_declaration_collection(PublishData *declarations){
  _get_expression()->_add_decl_into_declaration_collection(declarations);
}

Tyvis*
TyvisQualifiedExpression::_get_expression() {
  return dynamic_cast<Tyvis *>(get_expression());
}

TyvisTypeDefinition *
TyvisQualifiedExpression::_get_type_mark() {
  return dynamic_cast<TyvisTypeDefinition *>(get_type_mark());
}

void
TyvisQualifiedExpression::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_expression()->_build_sensitivity_list(sensitivity_list);
}

IIR_Boolean 
TyvisQualifiedExpression::_is_globally_static_primary(){
  ASSERT( _get_expression() != NULL );
  ASSERT( _get_expression()->is_resolved() == TRUE );
  return _get_expression()->_is_globally_static();
}

void 
TyvisQualifiedExpression::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_expression()->_get_list_of_input_signals(list);
}

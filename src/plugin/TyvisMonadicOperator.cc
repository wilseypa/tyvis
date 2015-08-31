
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

#include "TyvisAbsoluteOperator.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIdentityOperator.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisMonadicOperator.hh"
#include "TyvisNegationOperator.hh"
#include "TyvisNotOperator.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>

void 
TyvisMonadicOperator::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisMonadicOperator::_publish_cc_rvalue" );	
  ASSERT(_get_subtype() != NULL);
  _get_operand()->_publish_cc_rvalue( _cc_out, declarations );  
  _cc_out << "." << _get_cc_operator_name() << "()";
}

void 
TyvisMonadicOperator::_publish_cc_lvalue( published_file &(_cc_out), PublishData *declarations ) {
  ASSERT(get_subtype() != NULL);
  _get_operand()->_publish_cc_lvalue( _cc_out , declarations );  
  _cc_out << "." << _get_cc_operator_name() << "()";
}

void 
TyvisMonadicOperator::_publish_cc_headers( published_file &_cc_out ) {
  ASSERT(_get_operand() != NULL);
  _get_operand()->_publish_cc_headers( _cc_out );
}

void 
TyvisMonadicOperator::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(_get_subtype() != NULL);
  _get_operand()->_publish_cc_wait_data( _cc_out, declarations );  
  _cc_out << "." << _get_cc_operator_name() << "()";
}

void
TyvisMonadicOperator::_publish_cc_ams_function(published_file &_cc_out, PublishData *declarations) {
  _publish_cc_ams_operator_name(_cc_out);
  _cc_out << "(";
  _get_operand()->_publish_cc_ams_function(_cc_out, declarations);
  _cc_out << ")";
}

void
TyvisMonadicOperator::_publish_cc_ams_operator_name( published_file & ) {
  _report_undefined_fn("_publish_cc_ams_operator_name()");
}

void
TyvisMonadicOperator::_publish_cc_ams_function_call_in_simult_stmt( published_file &_cc_out ) {
  _get_operand()->_publish_cc_ams_function_call_in_simult_stmt(_cc_out);
}

Tyvis * 
TyvisMonadicOperator::_transmute() {
  TyvisAssociationElementByExpression* associationelement;
  TyvisFunctionCall* functioncall = new TyvisFunctionCall;
  copy_location( this, functioncall );

  functioncall->set_implementation(_get_implementation());

  associationelement = new TyvisAssociationElementByExpression;
  copy_location( this, associationelement );
  associationelement->set_actual(get_operand());
  functioncall->get_parameter_association_list()->append(associationelement);

  return functioncall;
}

void 
TyvisMonadicOperator::_add_decl_into_declaration_collection( PublishData *declarations ) {
  _get_operand()->_add_decl_into_declaration_collection(declarations);
}

TyvisSubprogramDeclaration *
TyvisMonadicOperator::_get_implementation() {
  return dynamic_cast<TyvisSubprogramDeclaration *>(IIRBase_MonadicOperator::get_implementation());
}

Tyvis *
TyvisMonadicOperator::_get_operand() {
  return dynamic_cast<Tyvis *>(get_operand());
}

IIR_Boolean
TyvisMonadicOperator::_is_static_expression() {
  return _get_operand()->_is_static_expression();
}

void
TyvisMonadicOperator::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_operand()->_build_sensitivity_list(sensitivity_list);
}

void
TyvisMonadicOperator::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_operand()->_get_list_of_input_signals(list); 
}

void
TyvisMonadicOperator::_set_stmt_qty_index( IIR_Int32 *index,
                                                  savant::set<TyvisDeclaration> *quantity_set,
                                                  PublishData *declarations,
                                                  PublishData *arch_declarations) {
  _get_operand()->_set_stmt_qty_index(index, quantity_set, declarations, arch_declarations);
}

void
TyvisMonadicOperator::_flush_stmt_index() {   
  _get_operand()->_flush_stmt_index();
}

void
TyvisMonadicOperator::_set_stmt_signal_index(IIR_Int32 *index,
                                                     savant::set<TyvisDeclaration> *signal_set) {
  _get_operand()->_set_stmt_signal_index(index, signal_set);
}

void
TyvisMonadicOperator::_build_generic_parameter_set(savant::set<TyvisDeclaration> *to_build) {
  _get_operand()->_build_generic_parameter_set(to_build);
}

void
TyvisMonadicOperator::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_operand()->_build_above_attribute_set(to_build);
}

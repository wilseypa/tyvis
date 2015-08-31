
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

#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisContributionAttribute.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDyadicOperator.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisReferenceAttribute.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisTerminalDeclaration.hh"

#include "published_file.hh"
#include <iostream>

using std::cerr;

void
TyvisDyadicOperator::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisDyadicOperator::_publish_cc_headers" );

  ASSERT(get_left_operand() != NULL);
  ASSERT(get_right_operand() != NULL);
  _get_left_operand()->_publish_cc_headers( _cc_out );
  _get_right_operand()->_publish_cc_headers( _cc_out );
}

void
TyvisDyadicOperator::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDyadicOperator::_publish_cc_lvalue" );
  _publish_cc_xvalue( _cc_out, declarations, &Tyvis::_publish_cc_lvalue );
}

void
TyvisDyadicOperator::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDyadicOperator::_publish_cc_rvalue" );
  _publish_cc_xvalue( _cc_out, declarations, &Tyvis::_publish_cc_rvalue );
}

void
TyvisDyadicOperator::_publish_cc_xvalue( published_file &_cc_out,
                                                PublishData *declarations,
                                                void (Tyvis::*_publish_function)( published_file &, PublishData * )) {
  CC_REF( _cc_out, "TyvisDyadicOperator::_publish_cc_xvalue" );

  (_get_left_operand()->*_publish_function)( _cc_out, declarations );
  _cc_out << "." << _get_cc_operator_name() << OS("(");
  (_get_right_operand()->*_publish_function)( _cc_out, declarations );
  _cc_out << CS( ")" );
}

void
TyvisDyadicOperator::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDyadicOperator::_publish_cc_wait_data" );
  _publish_cc_xvalue( _cc_out, declarations, &Tyvis::_publish_cc_wait_data );
}

void
TyvisDyadicOperator::_publish_cc_ams_function(published_file &_cc_out, PublishData *declarations) {
  _cc_out << "  equationNode *node" <<_stmt_node_index;
  _cc_out << " = new equationNode('O',";
  switch(get_kind()) {
  case IIR_ADDITION_OPERATOR:{
    _cc_out<<1;
    break;
  }
  case IIR_SUBTRACTION_OPERATOR:{
    _cc_out<<2;
    break;
  }
  case IIR_MULTIPLICATION_OPERATOR:{
    _cc_out<<3;
    break;
  }
  case IIR_DIVISION_OPERATOR:{
    _cc_out<<4;
    break;
  }
  default:{
    cerr<<"Unknown operator type in TyvisDyadicOperator::_get_function_name()";
    abort();
  }
  }
  _cc_out << ",";
  _cc_out << "node" <<(int)(_stmt_node_index/2);
  _cc_out << ");" << NL();
  _get_left_operand()->_publish_cc_ams_function(_cc_out, declarations);
  _get_right_operand()->_publish_cc_ams_function(_cc_out, declarations);
}

void
TyvisDyadicOperator::_publish_cc_ams_operator_name( published_file & ) {
  _report_undefined_fn("_publish_cc_ams_operator_name()");
}

void
TyvisDyadicOperator::_publish_cc_ams_function_call_in_simult_stmt(published_file &_cc_out) {
  _get_left_operand()->_publish_cc_ams_function_call_in_simult_stmt(_cc_out);
  _get_right_operand()->_publish_cc_ams_function_call_in_simult_stmt(_cc_out);
}

Tyvis * 
TyvisDyadicOperator::_transmute() {
  TyvisFunctionCall* functioncall = new TyvisFunctionCall;
  TyvisAssociationElementByExpression* associationelement;

  copy_location( this, functioncall );
  functioncall->set_implementation(_get_implementation());

  associationelement = new TyvisAssociationElementByExpression;
  copy_location( this, associationelement );
  associationelement->set_actual(get_left_operand());
  functioncall->get_parameter_association_list()->append(associationelement);

  associationelement = new TyvisAssociationElementByExpression;
  copy_location( this, associationelement );
  associationelement->set_actual(get_right_operand());
  functioncall->get_parameter_association_list()->append(associationelement);

  return functioncall;
}

void 
TyvisDyadicOperator::_add_decl_into_declaration_collection(PublishData *declarations) {
  _get_left_operand()->_add_decl_into_declaration_collection(declarations);
  _get_right_operand()->_add_decl_into_declaration_collection(declarations);
}

Tyvis *
TyvisDyadicOperator::_get_left_operand() {
  return dynamic_cast<Tyvis *>(get_left_operand());
}

Tyvis *
TyvisDyadicOperator::_get_right_operand() {
  return dynamic_cast<Tyvis *>(get_right_operand());
}

TyvisSubprogramDeclaration *
TyvisDyadicOperator::_get_implementation() {
  return dynamic_cast<TyvisSubprogramDeclaration *>(IIRBase_DyadicOperator::get_implementation());
}

IIR_Boolean
TyvisDyadicOperator::_is_static_expression() {
  if ((_get_left_operand()->_is_static_expression() == TRUE) &&
      (_get_right_operand()->_is_static_expression() == TRUE)) {
    return TRUE;
  }
  return FALSE;
}

void
TyvisDyadicOperator::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_left_operand()->_build_sensitivity_list(sensitivity_list);
  _get_right_operand()->_build_sensitivity_list(sensitivity_list);
}

void
TyvisDyadicOperator::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_left_operand()->_get_list_of_input_signals(list);
  _get_right_operand()->_get_list_of_input_signals(list);
}

void
TyvisDyadicOperator::_set_stmt_node_index(IIR_Int32 *index, bool _is_right_child, bool &reducibleFlag) {
  ASSERT(index!=NULL);
  if (_is_right_child == true) {
   _stmt_node_index = 2 * (*index)+ 1;
  }
  else {
   _stmt_node_index = 2 * (*index);
  }
  _get_left_operand()->_set_stmt_node_index(&_stmt_node_index, false, reducibleFlag);
  _get_right_operand()->_set_stmt_node_index(&_stmt_node_index, true, reducibleFlag);
}

void
TyvisDyadicOperator::_set_stmt_qty_index( IIR_Int32 *index,
                                                 savant::set<TyvisDeclaration> *quantity_set,
                                                 PublishData *declarations,
                                                 PublishData *arch_declarations) {
  _get_left_operand()->_set_stmt_qty_index(index, quantity_set, declarations, arch_declarations);
  _get_right_operand()->_set_stmt_qty_index(index, quantity_set, declarations, arch_declarations);
}

void
TyvisDyadicOperator::_flush_stmt_index() {
  _get_left_operand()->_flush_stmt_index();
  _get_right_operand()->_flush_stmt_index();
}

IIR_Boolean   
TyvisDyadicOperator::_contribution_quantity_found() {
  IIR_Boolean retval = FALSE;
  retval = _get_left_operand()->_contribution_quantity_found() ||
    _get_right_operand()->_contribution_quantity_found();
  return retval;
}

IIR_Boolean
TyvisDyadicOperator::_reference_quantity_found() {
  IIR_Boolean retval = FALSE;
  retval = _get_left_operand()->_reference_quantity_found() ||
    _get_right_operand()->_reference_quantity_found();
  return retval;
}

IIR_Boolean
TyvisDyadicOperator::_differential_quantity_found() {
  IIR_Boolean retval = FALSE;
  retval = _get_left_operand()->_differential_quantity_found() ||
    _get_right_operand()->_differential_quantity_found();
  return retval;
}

void
TyvisDyadicOperator::_build_contribution_quantity_list(dl_list<TyvisContributionAttribute>
                                                           *contribution_quantity_list) {
  _get_left_operand()->_build_contribution_quantity_list(contribution_quantity_list);
  _get_right_operand()->_build_contribution_quantity_list(contribution_quantity_list);
}

void
TyvisDyadicOperator::_build_differential_quantity_list(dl_list<TyvisDotAttribute>
                                                           *differential_quantity_list) {
  _get_left_operand()->_build_differential_quantity_list(differential_quantity_list);
  _get_right_operand()->_build_differential_quantity_list(differential_quantity_list);
}

void
TyvisDyadicOperator::_build_generic_parameter_set( savant::set<TyvisDeclaration> *to_build) {
  _get_left_operand()->_build_generic_parameter_set(to_build);
  _get_right_operand()->_build_generic_parameter_set(to_build);
}

void
TyvisDyadicOperator::_set_stmt_signal_index( IIR_Int32 *index,
                                                    savant::set<TyvisDeclaration> *signal_set) {
  _get_left_operand()->_set_stmt_signal_index(index, signal_set);
  _get_right_operand()->_set_stmt_signal_index(index, signal_set);
}

void
TyvisDyadicOperator::_build_reference_quantity_list(dl_list<TyvisReferenceAttribute> *reference_quantity_list) {
  _get_left_operand()->_build_reference_quantity_list(reference_quantity_list);
  _get_right_operand()->_build_reference_quantity_list(reference_quantity_list);
}

void
TyvisDyadicOperator::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_left_operand()->_build_above_attribute_set(to_build);
  _get_right_operand()->_build_above_attribute_set(to_build);
}


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

#include "TyvisDesignFile.hh"
#include "TyvisElsif.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisIfStatement.hh"
#include "TyvisSequentialStatement.hh"
#include "TyvisSequentialStatementList.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/StandardPackage.hh"
#include "published_file.hh"

TyvisElsif::TyvisElsif() {
  set_then_sequence_of_statements(new TyvisSequentialStatementList());
}

TyvisElsif::~TyvisElsif() {
  //Release the list memory
  delete get_then_sequence_of_statements();
}

void 
TyvisElsif::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  ASSERT(_get_condition()->is_resolved() == TRUE);

  _cc_out << OS("else if (SAVANT_BOOLEAN_TRUE ==");
  _get_condition()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << CS(")") << OS("{");
  if (_get_then_sequence_of_statements()->size() != 0) {
    _get_then_sequence_of_statements()->_publish_cc( _cc_out, declarations );
  }
  _cc_out << CS("}");
    
  if (_get_else_clause() != NULL) {
    _get_else_clause()->_publish_cc( _cc_out, declarations );
  }
}

void
TyvisElsif::_publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations ) {
  _get_then_sequence_of_statements()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
  if (_get_else_clause() != NULL) {
    _get_else_clause()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
  }
}

TyvisSequentialStatementList *
TyvisElsif::_get_then_sequence_of_statements() {
  return dynamic_cast<TyvisSequentialStatementList *>(get_then_sequence_of_statements());
}

Tyvis *
TyvisElsif::_get_condition() {
  return dynamic_cast<Tyvis *>(get_condition());
}

TyvisElsif*
TyvisElsif::_get_else_clause() {
  return dynamic_cast<TyvisElsif *>(get_else_clause());
}

void
TyvisElsif::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  if(_get_condition() != NULL) {
    _get_condition()->_get_list_of_input_signals(list);
  }
  if(_get_else_clause() != NULL) {
    _get_else_clause()->_get_list_of_input_signals(list);
  }
  _get_then_sequence_of_statements()->_get_list_of_input_signals(list);
}

void
TyvisElsif::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  if( _get_else_clause() != NULL ){
    _get_else_clause()->_get_signal_source_info(siginfo);
  }
  _get_then_sequence_of_statements()->_get_signal_source_info(siginfo);
}

void
TyvisElsif::_build_wait_list(dl_list<TyvisWaitStatement>* list) {
  _get_then_sequence_of_statements()->_build_wait_list(list);
  if (_get_else_clause() != NULL) {
    _get_else_clause()->_build_wait_list(list);
  }  
}

void 
TyvisElsif::_build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *list) {
  _get_then_sequence_of_statements()->_build_procedure_call_stmt_list(list);
  if (_get_else_clause() != NULL) {
    _get_else_clause()->_build_procedure_call_stmt_list(list);
  }
}

void
TyvisElsif::_publish_cc_static_type_info_fn( published_file &_cc_out,
                                             PublishData *declarations ){
  if (_get_then_sequence_of_statements()->size() != 0) {
    _get_then_sequence_of_statements()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
    
  if (_get_else_clause() != NULL) {
    _get_else_clause()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
}

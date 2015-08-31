
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
#include "TyvisLabel.hh"
#include "TyvisSequentialStatementList.hh"

#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>
 
using std::cerr;
using std::endl;

TyvisIfStatement::TyvisIfStatement() {
  set_then_sequence(new TyvisSequentialStatementList());
  set_else_sequence(new TyvisSequentialStatementList());
}

TyvisIfStatement::~TyvisIfStatement() {
  //Release the list memory
  delete get_then_sequence();
  delete get_else_sequence();
}

void 
TyvisIfStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  CC_REF( _cc_out, "TyvisIfStatement::_publish_cc" );
  if (get_label() != NULL) {
    _get_label()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << ":" << NL();
  }
    
  _cc_out << OS("if( SAVANT_BOOLEAN_TRUE  == ");
  _get_condition()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << CS(")") << OS("{");
    
  if (_get_then_sequence()->size() != 0) {
    _get_then_sequence()->_publish_cc( _cc_out, declarations );
  }
  _cc_out << CS("}");

  if (_get_elsif() != NULL) {
    _get_elsif()->_publish_cc( _cc_out, declarations );
  }

  if( _get_else_sequence()->size() > 0){
    _cc_out << OS("else {");
    _get_else_sequence()->_publish_cc( _cc_out, declarations );
    _cc_out << CS("}");
  }
}

void
TyvisIfStatement::_publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations ) {
  _get_then_sequence()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
  if (get_elsif() != NULL) {
    _get_elsif()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
  }
  _get_else_sequence()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
}

Tyvis *
TyvisIfStatement::_get_condition() {
  return dynamic_cast<Tyvis *>(get_condition());
}

TyvisElsif *
TyvisIfStatement::_get_elsif() {
  return dynamic_cast<TyvisElsif *>(get_elsif());
}

TyvisSequentialStatementList *
TyvisIfStatement::_get_else_sequence() {
  return dynamic_cast<TyvisSequentialStatementList *>(get_else_sequence());
}

void
TyvisIfStatement::_get_list_of_input_signals(  savant::set<Tyvis>* list ){
  _get_condition()->_get_list_of_input_signals(list);
  if (_get_elsif() != NULL) {
    _get_elsif()->_get_list_of_input_signals(list);
  }
  _get_then_sequence()->_get_list_of_input_signals(list);
  _get_else_sequence()->_get_list_of_input_signals(list);
}

void
TyvisIfStatement::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  if (_get_elsif() != NULL) {
    _get_elsif()->_get_signal_source_info(siginfo);
  }
  _get_then_sequence()->_get_signal_source_info(siginfo);
  _get_else_sequence()->_get_signal_source_info(siginfo);
}

void
TyvisIfStatement::_build_wait_list(dl_list<TyvisWaitStatement>* list) {
  _get_then_sequence()->_build_wait_list(list);
  if (get_elsif() != NULL) {
    _get_elsif()->_build_wait_list(list);
  }
  _get_else_sequence()->_build_wait_list(list);
}

void 
TyvisIfStatement::_build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *list) {
  _get_then_sequence()->_build_procedure_call_stmt_list(list);
  if (_get_elsif() != NULL) {
    _get_elsif()->_build_procedure_call_stmt_list(list);
  }
  _get_else_sequence()->_build_procedure_call_stmt_list(list);
}

TyvisSequentialStatementList *
TyvisIfStatement::_get_then_sequence() {
  return dynamic_cast<TyvisSequentialStatementList *>(get_then_sequence());
}

void
TyvisIfStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  ASSERT(_get_condition() != NULL);
  _get_condition()->_build_above_attribute_set(to_build);
  if (_get_then_sequence()->size() != 0) {
    _get_then_sequence()->_build_above_attribute_set(to_build);
  }
  if (_get_elsif() != NULL) {
    _get_elsif()->_build_above_attribute_set(to_build);
  }
  if (_get_else_sequence()->size() != 0) {
    _get_else_sequence()->_build_above_attribute_set(to_build);
  }
}

void
TyvisIfStatement::_publish_cc_static_type_info_fn( published_file &_cc_out,
					 PublishData *declarations ){
  if( _get_then_sequence() && _get_then_sequence()->size() > 0 ){
    CC_REF( _cc_out, "TyvisIfStatement::_publish_cc_type_info" );
    _get_then_sequence()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
  if( _get_elsif() ){
    CC_REF( _cc_out, "TyvisIfStatement::_publish_cc_type_info" );
    _get_elsif()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
  if( _get_else_sequence() && _get_else_sequence()->size() > 0 ){
    CC_REF( _cc_out, "TyvisIfStatement::_publish_cc_type_info" );
    _get_else_sequence()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
}


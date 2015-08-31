
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
#include "TyvisLabel.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisWhileLoopStatement.hh"
#include "savant/savant.hh"
#include "published_file.hh"

TyvisWhileLoopStatement::TyvisWhileLoopStatement() {
  set_sequence_of_statements(new TyvisSequentialStatementList());
}

TyvisWhileLoopStatement::~TyvisWhileLoopStatement() {
  //Release the list memory
  delete get_sequence_of_statements();
} 

void
TyvisWhileLoopStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  _cc_out << *(_get_mangled_label()) << ":;" << NL();

  _publish_cc_next_label( _cc_out );
  // Semicolon necessary because empty VHDL while loop is possible but a
  // label MUST be followed by a statement in C++.
  _cc_out << ":;" << NL();

  _cc_out << "while" << OS("(");
  if(_get_while_condition() == NULL) { // it is a vhdl loop statement
    _cc_out << "true";
  } else {			// it is a vhdl while loop statement
    ASSERT(_get_while_condition()->is_resolved() == TRUE);
    _cc_out << "SAVANT_BOOLEAN_TRUE  == ";
    _get_while_condition()->_publish_cc_rvalue( _cc_out , declarations );
  }
  _cc_out << ")" << OS("{");
  if (_get_sequence_of_statements()->size() != 0) {
    _get_sequence_of_statements()->_publish_cc( _cc_out, declarations );
  } else {
    _cc_out << ";" << NL();
  }
  
  _cc_out << CS("}");
  _publish_cc_exit_label( _cc_out );
  _cc_out << ":;" << NL();
}

void
TyvisWhileLoopStatement::_publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations ) {
  _get_sequence_of_statements()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
}

TyvisSequentialStatementList *
TyvisWhileLoopStatement::_get_sequence_of_statements() {
  return dynamic_cast<TyvisSequentialStatementList *>(get_sequence_of_statements());
}

Tyvis *
TyvisWhileLoopStatement::_get_while_condition() {
  return dynamic_cast<Tyvis *>(get_while_condition());
}

void
TyvisWhileLoopStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  if( _get_while_condition() != NULL ){
    _get_while_condition()->_get_list_of_input_signals(list);
  }
  _get_sequence_of_statements()->_get_list_of_input_signals(list);
}

void
TyvisWhileLoopStatement::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  _get_sequence_of_statements()->_get_signal_source_info(siginfo);
}

void
TyvisWhileLoopStatement::_build_wait_list(dl_list<TyvisWaitStatement>* list) {
  _get_sequence_of_statements()->_build_wait_list(list);
}

void 
TyvisWhileLoopStatement::_build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *list) {
  _get_sequence_of_statements()->_build_procedure_call_stmt_list(list);
}

void
TyvisWhileLoopStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  if(_get_while_condition() != NULL) {
    _get_while_condition()->_build_above_attribute_set(to_build);
  }
  _get_sequence_of_statements()->_build_above_attribute_set(to_build);
}

void
TyvisWhileLoopStatement::_publish_cc_static_type_info_fn( published_file &_cc_out,
						PublishData *declarations ){
  _get_sequence_of_statements()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}


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

#include "TyvisCaseStatement.hh"
#include "TyvisCaseStatementAlternative.hh"
#include "TyvisCaseStatementAlternativeList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisSequentialStatementList.hh"
#include "savant/error_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>

TyvisCaseStatement::TyvisCaseStatement(){
  set_case_statement_alternatives(new TyvisCaseStatementAlternativeList());
}

TyvisCaseStatement::~TyvisCaseStatement(){
  //Release the list memory
  delete get_case_statement_alternatives();
}

void
TyvisCaseStatement::_publish_cc( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisCaseStatement::_publish_cc" );
  if (_get_label() != NULL) {
    _get_label()->_publish_cc_lvalue( _cc_out, declarations );
    _cc_out << ":" << NL();
  }

  Tyvis* save_node = _get_current_publish_node();
  _set_current_publish_node( _get_expression() );
  _cc_out << "if( false ){}" << NL();
  _get_case_statement_alternatives()->_publish_cc( _cc_out, declarations );
  _set_current_publish_node( save_node );
  _cc_out << NL();
}

void
TyvisCaseStatement::_publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations ) {

  CC_REF(_cc_out, "TyvisCaseStatement::_publish_cc_decl_for_loop_iterator");

  _get_case_statement_alternatives()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
}

Tyvis *
TyvisCaseStatement::_get_case_statement_expression(){
  return dynamic_cast<Tyvis *>(get_expression());
}

Tyvis *
TyvisCaseStatement::_get_expression() {
  return dynamic_cast<Tyvis *>(get_expression());
}

TyvisCaseStatementAlternativeList *
TyvisCaseStatement::_get_case_statement_alternatives() {
  return dynamic_cast<TyvisCaseStatementAlternativeList *>(get_case_statement_alternatives());
}

void
TyvisCaseStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_expression()->_get_list_of_input_signals(list);
  _get_case_statement_alternatives()->_get_list_of_input_signals(list);
}

void
TyvisCaseStatement::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  _get_case_statement_alternatives()->_get_signal_source_info(siginfo);
}

void
TyvisCaseStatement::_build_wait_list(dl_list<TyvisWaitStatement>* list) {
  _get_case_statement_alternatives()->_build_wait_list(list);
}

void 
TyvisCaseStatement::_build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *list){
  TyvisCaseStatementAlternative *alt = NULL;
  for (alt = dynamic_cast<TyvisCaseStatementAlternative *>(get_case_statement_alternatives()->first());
       alt != NULL;
       alt = dynamic_cast<TyvisCaseStatementAlternative *>(get_case_statement_alternatives()->successor(alt))) {
    alt->_get_sequence_of_statements()->_build_procedure_call_stmt_list(list);
  }
}

void
TyvisCaseStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_expression()->_build_above_attribute_set(to_build);
  _get_case_statement_alternatives()->_build_above_attribute_set(to_build);
}


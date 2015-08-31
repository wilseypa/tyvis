
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
#include "TyvisCaseStatementAlternative.hh"
#include "TyvisSequentialStatement.hh"
#include "TyvisSequentialStatementList.hh"

TyvisCaseStatementAlternative::TyvisCaseStatementAlternative() {
  set_sequence_of_statements(new TyvisSequentialStatementList());
}

TyvisCaseStatementAlternative::~TyvisCaseStatementAlternative() {
  //Release the list memory
  delete get_sequence_of_statements();
}

void
TyvisCaseStatementAlternative::_publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisCaseStatementAlternative::_publish_cc_decl_for_loop_iterator" );

  _get_sequence_of_statements()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
}

TyvisSequentialStatementList *
TyvisCaseStatementAlternative::_get_sequence_of_statements() {
  return dynamic_cast<TyvisSequentialStatementList *>(get_sequence_of_statements());
}

void 
TyvisCaseStatementAlternative::_get_list_of_input_signals(  savant::set<Tyvis>* list ){
  _get_sequence_of_statements()->_get_list_of_input_signals(list);
}

void 
TyvisCaseStatementAlternative::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  _get_sequence_of_statements()->_get_signal_source_info(siginfo);
}

void 
TyvisCaseStatementAlternative::_build_wait_list(dl_list<TyvisWaitStatement>* list) {
  _get_sequence_of_statements()->_build_wait_list(list);
}

void
TyvisCaseStatementAlternative::_publish_cc_static_type_info_fn( published_file &_cc_out,
						      PublishData *declarations ){
  _get_sequence_of_statements()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

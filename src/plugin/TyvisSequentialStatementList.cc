
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

#include "TyvisProcedureCallStatement.hh"
#include "TyvisIdentifier.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisSequentialStatement.hh"
#include "TyvisSignalAssignmentStatement.hh"

#include "savant/savant.hh"
#include "published_file.hh"
#include <StringUtilities.h>

void 
TyvisSequentialStatementList::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  CC_REF( _cc_out, "TyvisSequentialStatementList::_publish_cc" );
  TyvisSequentialStatement *stmt = NULL;
  for (stmt = dynamic_cast<TyvisSequentialStatement *>(first());
       stmt != NULL; stmt = dynamic_cast<TyvisSequentialStatement *>(successor(stmt))) {
    if(stmt->get_line_number() != -1 &&
       stmt->get_file_name() != NULL) {
      _cc_out.insert_comment( string( "line " ) + intToString( stmt->get_line_number() ) +
			      string( " of file " ) + stmt->_get_file_name()->_to_string()
			      + "(" + stmt->get_kind_text() + ")" );
    }
    stmt->_publish_cc( _cc_out, declarations );
  }
}

void
TyvisSequentialStatementList::_publish_cc_decl_for_loop_iterator( published_file &_cc_out,
								  PublishData *declarations ) {
  TyvisSequentialStatement* stmt = dynamic_cast<TyvisSequentialStatement *>(first());
  while(stmt != NULL) {
    stmt->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
    stmt = dynamic_cast<TyvisSequentialStatement *>(successor(stmt));
  }
}  

void
TyvisSequentialStatementList::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  TyvisSequentialStatement* stmt = dynamic_cast<TyvisSequentialStatement *>(first());
  while( stmt != NULL ) {
    stmt->_get_list_of_input_signals(list);
    stmt = dynamic_cast<TyvisSequentialStatement *>(successor(stmt));
  }
}

void
TyvisSequentialStatementList::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  TyvisSequentialStatement* stmt = dynamic_cast<TyvisSequentialStatement *>(first());
  while( stmt != NULL ){
    // XXX SK This has to be handled for all statements.
    if(stmt->has_statement_list() || 
       dynamic_cast<TyvisSignalAssignmentStatement *>(stmt) != NULL ||
       stmt->get_kind() == IIR_PROCEDURE_CALL_STATEMENT) {
      stmt->_get_signal_source_info(siginfo);
    }
    stmt = dynamic_cast<TyvisSequentialStatement *>(successor(stmt));
  }
}

void 
TyvisSequentialStatementList::_build_wait_list(dl_list<TyvisWaitStatement> *list) {
  TyvisSequentialStatement *stmt = dynamic_cast<TyvisSequentialStatement *>(first());
  while (stmt != NULL) {
    stmt->_build_wait_list(list);
    stmt = dynamic_cast<TyvisSequentialStatement *>(successor(stmt));
  }
}

void 
TyvisSequentialStatementList::_build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *list) {
  TyvisSequentialStatement *stmt = dynamic_cast<TyvisSequentialStatement *>(first());
  while (stmt != NULL) {
    stmt->_build_procedure_call_stmt_list(list);
    stmt = dynamic_cast<TyvisSequentialStatement *>(successor(stmt));
  }
}

void
TyvisSequentialStatementList::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  TyvisSequentialStatement *stmt = dynamic_cast<TyvisSequentialStatement *>(first());
  while (stmt != NULL) {
    stmt->_build_above_attribute_set(to_build);
    stmt = dynamic_cast<TyvisSequentialStatement *>(successor(stmt));
  }
}


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
#include "TyvisCaseStatementAlternativeList.hh"
#include "TyvisCaseStatementAlternative.hh"
#include "savant/savant.hh"

void 
TyvisCaseStatementAlternativeList::_publish_cc( published_file &_cc_out,
						PublishData *declarations ){
  
  TyvisCaseStatementAlternative *stmt = dynamic_cast<TyvisCaseStatementAlternative *>(first());
  CC_REF( _cc_out, "TyvisCaseStatementAlternativeList::_publish_cc" );
  while( stmt != 0 ){
    stmt->_publish_cc( _cc_out, declarations);
    stmt = dynamic_cast<TyvisCaseStatementAlternative *>(successor(stmt));
  }
}

void
TyvisCaseStatementAlternativeList::_publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations ) {
  TyvisCaseStatementAlternative* stmt = dynamic_cast<TyvisCaseStatementAlternative *>(first());
  while( stmt != NULL ){
    stmt->_publish_cc_decl_for_loop_iterator( _cc_out, declarations);
    stmt = dynamic_cast<TyvisCaseStatementAlternative *>(successor(stmt));
  }
}

void
TyvisCaseStatementAlternativeList::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  TyvisCaseStatementAlternative *node = dynamic_cast<TyvisCaseStatementAlternative *>(first());
  while( node != NULL ) {
    node->_get_list_of_input_signals(list);
    node = dynamic_cast<TyvisCaseStatementAlternative *>(successor(node));
  }
}

void
TyvisCaseStatementAlternativeList::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  TyvisCaseStatementAlternative* stmt = dynamic_cast<TyvisCaseStatementAlternative *>(first());
  while( stmt != NULL ){
    stmt->_get_signal_source_info(siginfo);
    stmt = dynamic_cast<TyvisCaseStatementAlternative *>(successor(stmt));
  }
}

void
TyvisCaseStatementAlternativeList::_build_wait_list(dl_list<TyvisWaitStatement>* list) {
  TyvisCaseStatementAlternative *stmt = dynamic_cast<TyvisCaseStatementAlternative *>(first());

  while (stmt != NULL) {
    stmt->_build_wait_list(list);
    stmt = dynamic_cast<TyvisCaseStatementAlternative *>(successor(stmt));
  }
}

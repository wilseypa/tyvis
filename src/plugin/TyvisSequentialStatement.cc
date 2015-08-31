
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

#include "TyvisLabel.hh"
#include "TyvisProcedureCallStatement.hh"
#include "TyvisReturnStatement.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void
TyvisSequentialStatement::_publish_cc_exit_label( published_file &_cc_out ) {
  // Hard return to put the label on the left edge
  _cc_out << "\n" << *_get_mangled_label() << "_exit_";
}

void
TyvisSequentialStatement::_publish_cc_next_label( published_file &_cc_out ) {
  // Hard return to put the label on the left edge
  _cc_out << "\n" << *_get_mangled_label() << "_next_";
}

void
TyvisSequentialStatement::_publish_cc_decl_for_loop_iterator( published_file &, PublishData * ) {}

TyvisLabel *
TyvisSequentialStatement::_get_process_stmt_label(){
  return NULL;
}

void
TyvisSequentialStatement::_set_process_stmt_label(TyvisLabel *) {
}

void 
TyvisSequentialStatement::_build_wait_list(dl_list<TyvisWaitStatement> *) {}

void
TyvisSequentialStatement::_build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *) {
  // This is the default method.  This will be overridden in the required
  // classes
}

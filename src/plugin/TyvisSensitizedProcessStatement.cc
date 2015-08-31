
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

#include "TyvisAttribute.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignFile.hh"
#include "TyvisElementDeclarationList.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisName.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSensitizedProcessStatement.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisWaitStatement.hh"

#include "savant/error_func.hh"
#include "savant/set.hh"
#include <sstream>


TyvisSensitizedProcessStatement::TyvisSensitizedProcessStatement() {
  set_sensitivity_list(new TyvisDesignatorList());
}

TyvisSensitizedProcessStatement::~TyvisSensitizedProcessStatement() {
  //Release the list memory
  delete get_sensitivity_list();
}

void
TyvisSensitizedProcessStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  Tyvis* process;

  process = _transmute();
  process->_publish_cc_lvalue( _cc_out , declarations );
}

Tyvis * 
TyvisSensitizedProcessStatement::_transmute() {
  TyvisProcessStatement* pstmt = new TyvisProcessStatement;
  TyvisWaitStatement* wstmt = new TyvisWaitStatement;

  copy_location( this, wstmt );
  wstmt->set_sensitivity_list(get_sensitivity_list());

  copy_location( this, pstmt );
  pstmt->set_label(get_label());
  pstmt->set_postponed(get_postponed());
  pstmt->set_process_declarative_part(get_process_declarative_part());
  pstmt->get_process_declarative_part()->set_declarative_region( pstmt );

  pstmt->set_process_statement_part(get_process_statement_part());
  pstmt->get_process_statement_part()->append(wstmt);

  return pstmt;  
}

TyvisDesignatorList *
TyvisSensitizedProcessStatement::_get_sensitivity_list() {
  return dynamic_cast<TyvisDesignatorList *>(get_sensitivity_list());
}

void
TyvisSensitizedProcessStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_sensitivity_list()->_get_list_of_input_signals(list);
  get_process_statement_part()->_get_list_of_input_signals(list);
}

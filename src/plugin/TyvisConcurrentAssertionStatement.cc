
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

#include "TyvisConcurrentAssertionStatement.hh"
#include "Tyvis.hh"
#include "TyvisAssertionStatement.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisLabel.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisWaitStatement.hh"
#include <sstream>
#include "TyvisIdentifier.hh"


void
TyvisConcurrentAssertionStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ){
  _publish_cc_assertion_or_report_statement( _cc_out, declarations );
}

Tyvis * 
TyvisConcurrentAssertionStatement::_transmute() {
  TyvisProcessStatement* pstmt = new TyvisProcessStatement;
  TyvisWaitStatement* wstmt = new TyvisWaitStatement;
  TyvisAssertionStatement* astmt = new TyvisAssertionStatement;

  copy_location( this, pstmt );
  copy_location( this, wstmt );
  copy_location( this, astmt );

  ASSERT(get_file_name() != NULL);

  pstmt->set_design_file( get_design_file( ) );
  pstmt->set_line_number(get_line_number());

  pstmt->set_postponed(get_postponed());
  pstmt->set_label(get_label());

  astmt->set_design_file( get_design_file() );
  astmt->set_line_number(get_line_number());

  astmt->set_assertion_condition(get_assertion_condition());
  astmt->set_report_expression(get_report_expression());
  astmt->set_severity_expression(get_severity_expression());

  pstmt->get_process_statement_part()->append(astmt);
  TyvisDesignatorList sensitivity_list;
  _build_sensitivity_list(&sensitivity_list);
  wstmt->_get_sensitivity_list()->_add_signals_to_sensitivity_list(&sensitivity_list);
  pstmt->get_process_statement_part()->append(wstmt);
  return pstmt;
}

void
TyvisConcurrentAssertionStatement::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_assertion_condition()->_build_sensitivity_list(sensitivity_list);
}

Tyvis *
TyvisConcurrentAssertionStatement::_get_assertion_condition(){ 
  return dynamic_cast<Tyvis *>(get_assertion_condition()); 
}

Tyvis *
TyvisConcurrentAssertionStatement::_get_report_expression(){ 
  return dynamic_cast<Tyvis *>(get_report_expression());
}

Tyvis *
TyvisConcurrentAssertionStatement::_get_severity_expression() { 
  return dynamic_cast<Tyvis *>(get_severity_expression()); 
}

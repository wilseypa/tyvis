
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

#include "TyvisArchitectureStatementList.hh"
#include "TyvisAssociationElement.hh"
#include "TyvisAssociationList.hh"
#include "TyvisConcurrentProcedureCallStatement.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisIndexedName.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisLabel.hh"
#include "TyvisProcedureCallStatement.hh"
#include "TyvisProcedureDeclaration.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisWaitStatement.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include <sstream>

TyvisConcurrentProcedureCallStatement::TyvisConcurrentProcedureCallStatement() {
  set_actual_parameter_part(new TyvisAssociationList());
  set_process_statement_part(new TyvisArchitectureStatementList());
}

TyvisConcurrentProcedureCallStatement::~TyvisConcurrentProcedureCallStatement() {
  //Release the list memory
  delete get_actual_parameter_part();
  delete get_process_statement_part();
}

Tyvis * 
TyvisConcurrentProcedureCallStatement::_transmute() {
  TyvisProcessStatement* pstmt = new TyvisProcessStatement();
  TyvisWaitStatement* wstmt = new TyvisWaitStatement();
  TyvisProcedureCallStatement* pcstmt = new TyvisProcedureCallStatement();

  copy_location( this, pstmt );
  copy_location( this, wstmt );
  copy_location( this, pcstmt );

  pstmt->set_label(_get_label());
  pstmt->set_declarative_region(get_declarative_region());
  pstmt->set_postponed(get_postponed());
  pcstmt->set_procedure_name(_get_procedure_name());
  // Copy the actual parameters.
  TyvisAssociationElement *elem = dynamic_cast<TyvisAssociationElement *>(get_actual_parameter_part()->first());
  while(elem != NULL) {
    pcstmt->get_actual_parameter_part()->append(elem);
    elem = dynamic_cast<TyvisAssociationElement *>(get_actual_parameter_part()->successor(elem));
  }

  pstmt->get_process_statement_part()->append(pcstmt);

  TyvisDesignatorList sensitivity_list;
  _build_sensitivity_list(&sensitivity_list);
  wstmt->_get_sensitivity_list()->_add_signals_to_sensitivity_list(&sensitivity_list);

  pstmt->get_process_statement_part()->append(wstmt);

  return pstmt;
}

Tyvis *
TyvisConcurrentProcedureCallStatement::_get_procedure_name() {
  return dynamic_cast<Tyvis *>(get_procedure_name());
}

TyvisAssociationList *
TyvisConcurrentProcedureCallStatement::_get_actual_parameter_part() {
  return dynamic_cast<TyvisAssociationList *>(get_actual_parameter_part());
}

void
TyvisConcurrentProcedureCallStatement::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  TyvisAssociationElement *elem = dynamic_cast<TyvisAssociationElement *>(get_actual_parameter_part()->first());

  while(elem != NULL) {
    ASSERT(elem->is_resolved());
    if(elem->_get_actual()->is_signal() == TRUE ||
       elem->_get_actual()->get_kind() == IIR_FUNCTION_CALL) {
      ASSERT(elem->_get_formal()->is_interface() == TRUE);
      TyvisInterfaceDeclaration *decl = dynamic_cast<TyvisInterfaceDeclaration *>(elem->get_formal());
      if(decl->get_mode() == IIR_IN_MODE ||
	 decl->get_mode() == IIR_INOUT_MODE) {
	elem->_get_actual()->_build_sensitivity_list(sensitivity_list);
      }
    }
    elem = dynamic_cast<TyvisAssociationElement *>(get_actual_parameter_part()->successor(elem));
  } // while
}

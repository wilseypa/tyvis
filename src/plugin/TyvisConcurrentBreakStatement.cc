
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

#include "TyvisAttributeDeclaration.hh"
#include "TyvisBreakElement.hh"
#include "TyvisBreakList.hh"
#include "TyvisBreakStatement.hh"
#include "TyvisConcurrentBreakStatement.hh"
#include "TyvisDeclaration.hh"
#include "TyvisDesignator.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisEventAttribute.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisWaitStatement.hh"

#include "savant/set.hh"
#include "published_file.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include <sstream>
#include "savant/break_handler.hh"

TyvisConcurrentBreakStatement::TyvisConcurrentBreakStatement() {
  set_concurrent_break_list(new TyvisBreakList());
  set_sensitivity_list(new TyvisDesignatorList());
}

TyvisConcurrentBreakStatement::~TyvisConcurrentBreakStatement() {
  delete get_concurrent_break_list();
  delete get_sensitivity_list();
}

void
TyvisConcurrentBreakStatement::_publish_cc_ams() {}

void
TyvisConcurrentBreakStatement::_publish_cc() {}

void
TyvisConcurrentBreakStatement::_publish_createNetInfo(char *, 
                                                      published_file &_cc_out, 
                                                      PublishData *declarations) {
  string objectname;
  objectname = ", " + _get_current_elab_name() +"_elab_obj->id);\n" ;
  Tyvis* signals = dynamic_cast<TyvisDesignatorExplicit *>(get_sensitivity_list()->first());
  int numelements = get_sensitivity_list()->size();
  for(int i=0; i < numelements; i++) { 
    _cc_out << "Add(";
    switch(signals->get_kind()) {
    case IIR_SIGNAL_INTERFACE_DECLARATION:
      dynamic_cast<TyvisDeclaration*>(signals)->_publish_cc_elaborate( _cc_out, declarations );
      break;
    case IIR_DESIGNATOR_EXPLICIT:
      dynamic_cast<TyvisDeclaration*>(dynamic_cast<TyvisDesignatorExplicit*>(signals)->_get_name())->_publish_cc_elaborate(_cc_out, declarations);
      break;
    default:
      signals->_publish_cc_elaborate(_cc_out, declarations);
      break;
    }
    _cc_out << objectname;
    signals = dynamic_cast<Tyvis *>(get_sensitivity_list()->successor(dynamic_cast<TyvisDesignator*>(signals)));
  }
}

Tyvis *
TyvisConcurrentBreakStatement::_transmute() {
  
  TyvisProcessStatement* pstmt = new TyvisProcessStatement;
  TyvisWaitStatement* wstmt = new TyvisWaitStatement;
  
  copy_location( this, wstmt ); 
  copy_location( this, pstmt );
  
  TyvisBreakElement *break_element = NULL;
  TyvisBreakStatement *break_stmt = new TyvisBreakStatement();
  
  TyvisDesignator *designator = NULL;
   savant::set<Tyvis> signallist;
  TyvisDeclaration *signals = NULL;
  
  pstmt->set_design_file( get_design_file( ) );
  pstmt->set_line_number(get_line_number());
  
  pstmt->set_label(_get_label());
  pstmt->set_postponed(FALSE);
  
  break_stmt->set_condition(_get_condition());
  for ( break_element = dynamic_cast<TyvisBreakElement *>(get_concurrent_break_list()->first());
	break_element != NULL;
	break_element = dynamic_cast<TyvisBreakElement *>(get_concurrent_break_list()->successor(break_element)) ) {
    break_stmt->get_break_list()->append(break_element);
  }
  
  for ( designator = dynamic_cast<TyvisDesignator *>(get_sensitivity_list()->first());
	designator != NULL;
	designator = dynamic_cast<TyvisDesignator *>(get_sensitivity_list()->successor(designator)) ) {
    wstmt->get_sensitivity_list()->append(designator);
  }
  if(_get_condition() != NULL) {
    _get_condition()->_get_list_of_input_signals(&signallist);
    for(signals = dynamic_cast<TyvisDeclaration *>(signallist.getElement());
	signals != NULL;
	signals = dynamic_cast<TyvisDeclaration *>(signallist.getNextElement())) {
      designator = new TyvisDesignatorExplicit;
      designator = new TyvisDesignatorExplicit;
      dynamic_cast<TyvisDesignatorExplicit*>(designator)->set_name(signals);
      wstmt->get_sensitivity_list()->append(designator);
    }
  }
  wstmt->set_condition_clause(NULL);
  wstmt->set_timeout_clause(NULL);
  pstmt->get_process_statement_part()->append(break_stmt);
  pstmt->get_process_statement_part()->append(wstmt);
  return pstmt;
}

TyvisDesignatorList *
TyvisConcurrentBreakStatement::_get_sensitivity_list() {
  return dynamic_cast<TyvisDesignatorList *>(get_sensitivity_list());
}

Tyvis *
TyvisConcurrentBreakStatement::_get_condition() {
  return dynamic_cast<Tyvis *>(get_condition());
}

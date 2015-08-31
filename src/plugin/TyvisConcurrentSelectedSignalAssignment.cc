
// Copyright (c) The University of Cincinnati.  All rights reserved.

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

#include "TyvisConcurrentSelectedSignalAssignment.hh"
#include "TyvisCaseStatement.hh"
#include "TyvisCaseStatementAlternativeByChoices.hh"
#include "TyvisCaseStatementAlternativeByExpression.hh"
#include "TyvisCaseStatementAlternativeByOthers.hh"
#include "TyvisCaseStatementAlternativeList.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIfStatement.hh"
#include "TyvisLabel.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSelectedWaveform.hh"
#include "TyvisSelectedWaveformList.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisSignalAssignmentStatement.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisWaitStatement.hh"
#include "TyvisWaveformElement.hh"
#include "TyvisWaveformList.hh"

#include "savant/StandardPackage.hh"
#include "savant/error_func.hh"

TyvisConcurrentSelectedSignalAssignment::TyvisConcurrentSelectedSignalAssignment()
{
  set_selected_waveforms(new TyvisSelectedWaveformList());
}

TyvisConcurrentSelectedSignalAssignment::~TyvisConcurrentSelectedSignalAssignment() {
  //Release the list memory
  delete get_selected_waveforms();
}

Tyvis * 
TyvisConcurrentSelectedSignalAssignment::_transmute() {

  TyvisProcessStatement* pstmt = new TyvisProcessStatement;
  TyvisWaitStatement* wstmt = new TyvisWaitStatement;
  TyvisIfStatement* guardIfStmt = NULL;

  copy_location( this, pstmt );
  copy_location( this, wstmt );

  TyvisSignalAssignmentStatement* sastmt                  = NULL;
  TyvisCaseStatement* casestmt                            = NULL;
  Tyvis* cndtion;

  pstmt->set_label(get_label());
  pstmt->set_postponed(get_postponed());

  if(get_selected_waveforms()->size() == 0) {
    cerr << "TyvisConcurrentSelectedSignalAssignment: Encountered Conditional Signal Assignment Stmt with an empty waveform list.\n";
    abort();
    return NULL;
  }

  // If it is a guarded sign assignemnt then the transform is done as per
  // LRM chapter 9.5  
  if (get_guarded() == TRUE) {
    ASSERT(_get_guard_signal() != NULL);
    cndtion = _get_guard_signal();
    guardIfStmt = new TyvisIfStatement();
    copy_location( this, guardIfStmt );
    guardIfStmt->set_condition(cndtion);
      
    // else clause 
    // The disconnection specification applies only if the target is a
    // guarded target - section 9.5 of LRM
    if (_get_target()->get_signal_kind() == IIR_BUS_KIND || _get_target()->get_signal_kind() == IIR_REGISTER_KIND) {
      sastmt = new TyvisSignalAssignmentStatement;
      copy_location( this, sastmt );
      sastmt->set_target(get_target());
      sastmt->set_delay_mechanism(get_delay_mechanism());
      sastmt->set_reject_time_expression(get_reject_time_expression());
      TyvisWaveformElement *wave = new TyvisWaveformElement();
      copy_location( this, wave );
      // Post a null transaction 
      TyvisStringLiteral *nullLit = dynamic_cast<TyvisStringLiteral *>(TyvisStringLiteral::get("null", 4, get_design_file()->get_class_factory()));
      nullLit->set_subtype(get_target()->get_subtype());
      wave->set_value(nullLit);
      // Set the disconnection specification
      wave->set_time(NULL);
      sastmt->get_waveform()->append(wave);
      guardIfStmt->get_else_sequence()->append(sastmt);
    }

    ASSERT (guardIfStmt != NULL);
    pstmt->get_process_statement_part()->append(guardIfStmt);
  }
  
  casestmt = new TyvisCaseStatement();
  copy_location( this, casestmt );
  casestmt->set_expression(get_expression());
  TyvisCaseStatementAlternativeList *new_list = 
    dynamic_cast<TyvisCaseStatementAlternativeList *>(build_alternative_list(true));
  casestmt->set_case_statement_alternatives(new_list);

  if (guardIfStmt != NULL) {
    guardIfStmt->get_then_sequence()->append(casestmt);
  } else {
    pstmt->get_process_statement_part()->append(casestmt);
  }

  TyvisDesignatorList sensitivity_list;
  this->_build_sensitivity_list(&sensitivity_list);
  // Adding the guard signal to the sensitivity list for GSA's
  if (get_guarded() == TRUE) {
    _get_guard_signal()->_build_sensitivity_list(&sensitivity_list);
  }
  wstmt->_get_sensitivity_list()->_add_signals_to_sensitivity_list(&sensitivity_list);
  pstmt->get_process_statement_part()->append(wstmt);  
  return pstmt;  
}

Tyvis *
TyvisConcurrentSelectedSignalAssignment::_get_case_statement_expression(){
  return dynamic_cast<Tyvis *>(get_expression());
}

Tyvis *
TyvisConcurrentSelectedSignalAssignment::_get_expression() {
  return dynamic_cast<Tyvis *>(get_expression());
}

void
TyvisConcurrentSelectedSignalAssignment::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_expression()->_build_sensitivity_list(sensitivity_list);
  _get_selected_waveforms()->_build_sensitivity_list(sensitivity_list);
}

Tyvis *
TyvisConcurrentSelectedSignalAssignment::_get_target(){
  return dynamic_cast<Tyvis *>(get_target());
}

TyvisSelectedWaveformList *
TyvisConcurrentSelectedSignalAssignment::_get_selected_waveforms() {
  return dynamic_cast<TyvisSelectedWaveformList *>(get_selected_waveforms());
}

Tyvis *
TyvisConcurrentSelectedSignalAssignment::_get_reject_time_expression(){
  return dynamic_cast<Tyvis *>(get_reject_time_expression());
}

TyvisSignalDeclaration *
TyvisConcurrentSelectedSignalAssignment::_get_guard_signal() {
  return dynamic_cast<TyvisSignalDeclaration *>(get_guard_signal());
}

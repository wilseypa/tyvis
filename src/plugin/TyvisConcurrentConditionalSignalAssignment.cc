
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

#include "TyvisConcurrentConditionalSignalAssignment.hh"
#include "TyvisConditionalWaveform.hh"
#include "TyvisConditionalWaveformList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisElsif.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIfStatement.hh"
#include "TyvisLabel.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisSignalAssignmentStatement.hh"
#include "TyvisWaitStatement.hh"
#include "TyvisWaveformElement.hh"
#include "TyvisWaveformList.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisDyadicOperator.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisLibraryDeclaration.hh"

#include "published_header_file.hh"
#include "published_cc_file.hh"
#include "savant/IRKind.hh"
#include "savant/StandardPackage.hh"
#include "savant/error_func.hh"

TyvisConcurrentConditionalSignalAssignment::TyvisConcurrentConditionalSignalAssignment() 
{ 
  set_conditional_waveforms(new TyvisConditionalWaveformList());
}

IIR_Boolean
TyvisConcurrentConditionalSignalAssignment::_is_simultaneous_statement() {
  return TRUE;
}

TyvisConcurrentConditionalSignalAssignment::~TyvisConcurrentConditionalSignalAssignment() {
  //Release the list memory
  delete get_conditional_waveforms();
}

void
TyvisConcurrentConditionalSignalAssignment::_publish_cc_main(published_file& main_printer) {
   static int num = 0;
   std::string *current_name = _get_full_current_publish_name();
   std::string myname = *current_name + "logicGate" + std::to_string(num++);
   _add_current_publish_name( myname );
   // FIXME: Only simple assignments are covered for the time being.
   assert(_get_conditional_waveforms()->num_elements() == 1);
   assert(_get_conditional_waveforms()->first()->get_waveform()->num_elements() == 1);
  TyvisDyadicOperator* dyaop = dynamic_cast<TyvisDyadicOperator*>(_get_conditional_waveforms()->first()->get_waveform()->first()->get_value());
  assert(dyaop);
  switch(dyaop->get_kind()) {
     case IIR_XNOR_OPERATOR:
        dyaop->_publish_cc_main( main_printer );
        break;
     default:
        cerr << "ERROR! Dyadic operation " << dyaop->get_kind_text()
             << " is not supported for the moment\n";
        abort();
        break;
  }
   const std::string left_operand = dynamic_cast<TyvisSignalInterfaceDeclaration*>(dynamic_cast<TyvisDyadicOperator*>(_get_conditional_waveforms()->first()->get_waveform()->first()->get_value())->get_left_operand())->get_declarator()->get_text();
   const std::string right_operand = dynamic_cast<TyvisSignalInterfaceDeclaration*>(dynamic_cast<TyvisDyadicOperator*>(_get_conditional_waveforms()->first()->get_waveform()->first()->get_value())->get_right_operand())->get_declarator()->get_text();
   const std::string target = _get_target()->get_declarator()->get_text();
   main_printer << myname << "->addSignal( \"" << target << "\");" << NL();
   main_printer << myname << "->addOutput( \"" << target << "\"," << NL()
                << "\"" << *current_name << "\"," << NL()
                << " \"" << target << "\" );" << NL();
   main_printer << *current_name << "->addOutput( \"" << right_operand << "\"," << NL()
                << "\"" << myname << "\"," << NL()
                << " \"" << right_operand << "\");" << NL();
   main_printer << *current_name << "->addOutput( \"" << left_operand << "\"," << NL()
                << "\"" << myname << "\"," << NL()
                << " \"" << left_operand << "\");" << NL();
  _remove_current_publish_name( );
   delete current_name;
}

void 
TyvisConcurrentConditionalSignalAssignment::_publish_cc( published_file &, PublishData * ) {
  // Include the declarative region to which this process belongs
  // FIXME: Only simple assignments are covered for the time being.
  assert(_get_conditional_waveforms()->num_elements() == 1);
  assert(_get_conditional_waveforms()->first()->get_waveform()->num_elements() == 1);
  cerr << "Publishing concurrent assignment.\n";
  
  TyvisDyadicOperator* dyaop = dynamic_cast<TyvisDyadicOperator*>(_get_conditional_waveforms()->first()->get_waveform()->first()->get_value());
  assert(dyaop);
  switch(dyaop->get_kind()) {
     case IIR_XNOR_OPERATOR:
        dyaop->_publish_cc();
        break;
     default:
        cerr << "ERROR! Dyadic operation " << dyaop->get_kind_text()
             << " is not supported for the moment\n";
        abort();
        break;
  }

  // Include all the stuff in the declarative part of process in this file
  //get_process_declarative_part()->_publish_cc( process_decls_header_file, declarations );
}

Tyvis * 
TyvisConcurrentConditionalSignalAssignment::_transmute() {
  return this;
  /*
  //### yet to take care of guarded signal assignment
  TyvisProcessStatement* pstmt = new TyvisProcessStatement;
  TyvisWaitStatement* wstmt = new TyvisWaitStatement;

  copy_location( this, pstmt );
  copy_location( this, wstmt );

  TyvisSignalAssignmentStatement* sastmt = NULL;
  TyvisIfStatement* guardIfStmt = NULL;
  TyvisIfStatement* ifstmt = NULL;
  TyvisElsif* elsestmt = NULL;
  int ifflag = 0;
  Tyvis* prevstmt = NULL;
  Tyvis* ifelsifstmt = NULL;
  Tyvis* cndtion = NULL;
  pstmt->set_label(get_label());
  pstmt->set_postponed(get_postponed());

  if(get_conditional_waveforms()->size() == 0) {
    cerr << "TyvisConcurrentConditionalSignalAssignmentStatement: Encountered Conditional Signal Assignment Stmt with an empty waveform list.\n";
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
      TyvisStringLiteral *nullLit = dynamic_cast<TyvisStringLiteral *>(IIRBase_StringLiteral::get("null", 4, get_design_file()->get_class_factory() ));
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

  int numelements = get_conditional_waveforms()->size();
  TyvisConditionalWaveform* waveform = dynamic_cast<TyvisConditionalWaveform *>(get_conditional_waveforms()->first());

  if(numelements == 1) {

    sastmt = new TyvisSignalAssignmentStatement;
    copy_location( this, sastmt );
    sastmt->set_target(get_target());
    sastmt->set_delay_mechanism(get_delay_mechanism());
    sastmt->set_reject_time_expression(get_reject_time_expression());
    sastmt->set_waveform(waveform->get_waveform());

    if (waveform->get_condition() != NULL) {
      ifstmt = new TyvisIfStatement();
      copy_location( this, ifstmt );
      ifstmt->get_then_sequence()->append(sastmt);
      ifstmt->set_condition(waveform->get_condition());
      
      if (guardIfStmt != NULL) {
	guardIfStmt->get_then_sequence()->append(ifstmt);
      } else {
	pstmt->get_process_statement_part()->append(ifstmt);
      }
    } else {
      if (guardIfStmt != NULL) {
	guardIfStmt->get_then_sequence()->append(sastmt);
      } else {
	pstmt->get_process_statement_part()->append(sastmt);
      }
    }
  }
  else {
    for(; waveform!= NULL; ) {
      sastmt =  new TyvisSignalAssignmentStatement;
      copy_location( this, sastmt );
      sastmt->set_target(get_target());
      sastmt->set_delay_mechanism(get_delay_mechanism());
      sastmt->set_reject_time_expression(get_reject_time_expression());
      sastmt->set_waveform(waveform->get_waveform());
      cndtion = waveform->_get_condition();
      
      if(!ifflag) {
	ifstmt = new TyvisIfStatement();
	copy_location( this, ifstmt );
	prevstmt = ifstmt;
	ifelsifstmt = ifstmt;
	ifstmt->get_then_sequence()->append(sastmt);
	ifstmt->set_condition(cndtion);
	ifflag =1;
      }
      else {
	if(cndtion != NULL) {
	  elsestmt= new TyvisElsif();
	  copy_location( this, elsestmt );
	  ifelsifstmt = elsestmt;
	  elsestmt->set_condition(cndtion);
	  elsestmt->get_then_sequence_of_statements()->append(sastmt);
	  if(ifflag == 1) {
	    (dynamic_cast<TyvisIfStatement*>(prevstmt))->set_elsif(dynamic_cast<TyvisElsif*>(ifelsifstmt));
	    ifflag = 2;
	  }
	  else {
	    (dynamic_cast<TyvisElsif*>(prevstmt))->set_else_clause(dynamic_cast<TyvisElsif*>(ifelsifstmt));
	    ifflag = 2;
	  }
	}
	else {
	  ifstmt->get_else_sequence()->append(sastmt);
	}
      }
      prevstmt = ifelsifstmt;
      waveform = dynamic_cast<TyvisConditionalWaveform *>(get_conditional_waveforms()->successor(waveform));
    }
    if (guardIfStmt != NULL) {
      guardIfStmt->get_then_sequence()->append(ifstmt);
    } else {
      pstmt->get_process_statement_part()->append(ifstmt);
    }
  }
  TyvisDesignatorList sensitivity_list;
  _build_sensitivity_list(&sensitivity_list);
  // Adding the guard signal to the sensitivity list for GSA's
  if (get_guarded() == TRUE) {
    _get_guard_signal()->_build_sensitivity_list(&sensitivity_list);
  }
  wstmt->_get_sensitivity_list()->_add_signals_to_sensitivity_list(&sensitivity_list);
  pstmt->get_process_statement_part()->append(wstmt);  
  return pstmt;*/
}

void 
TyvisConcurrentConditionalSignalAssignment::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_conditional_waveforms()->_build_sensitivity_list(sensitivity_list);
}

Tyvis *
TyvisConcurrentConditionalSignalAssignment::_get_target(){
  return dynamic_cast<Tyvis *>(get_target());
}

TyvisConditionalWaveformList *
TyvisConcurrentConditionalSignalAssignment::_get_conditional_waveforms() {
  return dynamic_cast<TyvisConditionalWaveformList *>(get_conditional_waveforms());
}

Tyvis *
TyvisConcurrentConditionalSignalAssignment::_get_reject_time_expression(){
  return dynamic_cast<Tyvis *>(get_reject_time_expression());
}

TyvisSignalDeclaration *
TyvisConcurrentConditionalSignalAssignment::_get_guard_signal() {
  return dynamic_cast<TyvisSignalDeclaration *>(get_guard_signal());
}

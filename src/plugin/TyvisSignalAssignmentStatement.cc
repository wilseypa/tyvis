
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

#include "TyvisAliasDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisLabel.hh"
#include "TyvisSignalAssignmentStatement.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisWaveformElement.hh"
#include "TyvisWaveformList.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include "PublishData.hh"

TyvisSignalAssignmentStatement::TyvisSignalAssignmentStatement() {
  set_waveform(new TyvisWaveformList());
}

TyvisSignalAssignmentStatement::~TyvisSignalAssignmentStatement() {
  //Release the list memory
  delete get_waveform();
}

void
TyvisSignalAssignmentStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  CC_REF( _cc_out, "TyvisSignalAssignmentStatement::_publish_cc" );

  ASSERT(_get_target()->is_resolved());
  TyvisWaveformElement* waveform_element = dynamic_cast<TyvisWaveformElement *>(get_waveform()->first());
  // Indicates the first itereation.  We need this to publish the
  // waveforms other than the first with transport delay mechanism.
  IIR_Boolean first = TRUE;

  while(waveform_element != NULL) {
    if (get_target()->get_kind() == IIR_AGGREGATE) {
      // Create a temporary aggregate and use that in the signal assignment
      _cc_out.open_scope( "{" );
      _cc_out << "Aggregate tempAggregate = ";
      _get_target()->_publish_cc_rvalue( _cc_out, declarations );
      _cc_out << ";";
    }
    
    if(_is_currently_publishing_subprogram() == TRUE) {
      _cc_out << "processPtr->";
    }

    if(waveform_element->_is_null_transaction() == TRUE) {
      _cc_out << OS("assignNullTransaction(");
    }
    else {
      _cc_out << OS( "assignSignal( " );
    }

    _publish_cc_target( _cc_out, declarations );

    _cc_out << "," << NL();

    if(_is_currently_publishing_subprogram() == TRUE) {
      _cc_out << "processPtr,";
    }
    else {
      _cc_out << "this,";
    }      
    _cc_out << NL();

    waveform_element->_publish_cc_rvalue( _cc_out , declarations ); // data source and delay value

    // Now the reject time.
    _publish_cc_time_expression( _cc_out, first, declarations );
     _cc_out << "," << NL();
     _get_target()->_publish_cc_bounds( _cc_out, declarations ); 
     _cc_out << "," << NL(); 
     waveform_element->_publish_cc_bounds( _cc_out, declarations ); 
    _cc_out << CS( ");" );
    waveform_element = dynamic_cast<TyvisWaveformElement *>(get_waveform()->successor(waveform_element));
    first = FALSE;

    if (get_target()->get_kind() == IIR_AGGREGATE) {
      _cc_out.close_scope("}");
    }
  }
}

void
TyvisSignalAssignmentStatement::_publish_cc_target( published_file &_cc_out, PublishData *declarations ){
  _publish_cc_include( _cc_out, "tyvis/SignalBase.hh" );
  TyvisDeclaration *destsig  = _get_target()->_get_prefix_declaration();
  if( destsig != NULL && declarations->in_collection(destsig) == FALSE ){
    declarations->add_declaration(destsig);
  }
  _cc_out << "dynamic_cast<SignalBase &>(";
  switch(get_target()->get_kind()) {
  case IIR_SLICE_NAME:
    destsig->_publish_cc_signal_lvalue( _cc_out , declarations );
    break;
    
  case IIR_AGGREGATE:
    _cc_out << "tempAggregate";
    break;
    
  default:
    _get_target()->_publish_cc_signal_lvalue( _cc_out , declarations );
  }
  _cc_out << ")";
}

void
TyvisSignalAssignmentStatement::_publish_cc_time_expression( published_file &_cc_out, 
                                                                    IIR_Boolean first,
                                                                    PublishData *declarations){
  // REJECT TIME
  if( first == FALSE || get_delay_mechanism() == IIR_TRANSPORT_DELAY ){
    _cc_out << "SAVANT_ZERO_TIME";
  } 
  else {
    if( _get_reject_time_expression() != NULL ){
      _get_reject_time_expression()->_publish_cc_rvalue( _cc_out , declarations );
    }
    else{
      if( get_waveform()->first()->get_time() != NULL ){
	dynamic_cast<Tyvis *>(get_waveform()->first()->get_time())->_publish_cc_rvalue( _cc_out , declarations );
      } else {
	_cc_out << "SAVANT_ZERO_TIME";
      }
    } 
  }
}

TyvisWaveformList *
TyvisSignalAssignmentStatement::_get_waveform() {
  return dynamic_cast<TyvisWaveformList *>(get_waveform());
}

void
TyvisSignalAssignmentStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_waveform()->_get_list_of_input_signals(list);
}

void
TyvisSignalAssignmentStatement::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  ASSERT(_get_target()->is_signal() == TRUE);
  Tyvis* my_target = _get_target();
  ASSERT(my_target != NULL);
  if(my_target->get_kind() == IIR_ALIAS_DECLARATION) {
    siginfo->add( (dynamic_cast<TyvisAliasDeclaration*>(my_target))->_get_name() );
  } 
  else if( my_target->is_name() == TRUE  || 
	   my_target->is_aggregate_expression() == TRUE ){
    my_target->_get_signal_source_info(siginfo);
  }
  else {
    ASSERT( dynamic_cast<TyvisDeclaration *>(my_target) != NULL );
    siginfo->add( my_target );
  }
}

Tyvis *
TyvisSignalAssignmentStatement::_get_target(){
  return dynamic_cast<Tyvis *>(get_target());
}

Tyvis *
TyvisSignalAssignmentStatement::_get_reject_time_expression(){
  return dynamic_cast<Tyvis *>(get_reject_time_expression());
}

void
TyvisSignalAssignmentStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_waveform()->_build_above_attribute_set(to_build);
}

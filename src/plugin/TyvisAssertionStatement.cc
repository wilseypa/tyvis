
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
#include "TyvisAssertionStatement.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"


void 
TyvisAssertionStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  _publish_cc_assertion_or_report_statement( _cc_out, declarations );
}

void
TyvisAssertionStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_assertion_condition()->_get_list_of_input_signals(list);
  if( _get_report_expression() != NULL ){
    _get_report_expression()->_get_list_of_input_signals(list);
  }
  if( _get_severity_expression() != NULL ){
    _get_severity_expression()->_get_list_of_input_signals(list);
  }
}

void
TyvisAssertionStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_assertion_condition()->_build_above_attribute_set(to_build);
  if(_get_report_expression() != NULL) {
    _get_report_expression()->_build_above_attribute_set(to_build);
  }
  if(_get_severity_expression() != NULL) {
    _get_severity_expression()->_build_above_attribute_set(to_build);
  }
}

Tyvis *
TyvisAssertionStatement::_get_assertion_condition(){ 
  return dynamic_cast<Tyvis *>(get_assertion_condition()); 
}

Tyvis *
TyvisAssertionStatement::_get_report_expression(){ 
  return dynamic_cast<Tyvis *>(get_report_expression()); 
}

Tyvis *
TyvisAssertionStatement::_get_severity_expression(){ 
  return dynamic_cast<Tyvis *>(get_severity_expression()); 
}


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

#include "TyvisDesignFile.hh"
#include "TyvisExitStatement.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisList.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"


void 
TyvisExitStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {

  if (_get_label() != NULL) {
    _get_label()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << ": ";
  }

  if (get_condition() != NULL) {
    ASSERT(_get_condition()->is_resolved() == TRUE);
    _cc_out << "if( SAVANT_BOOLEAN_TRUE == ";
    _get_condition()->_publish_cc_rvalue( _cc_out , declarations );
    _cc_out << ")" << OS("{");
  }
  if (_get_enclosing_loop() != NULL) {
    //ASSERT(get_enclosing_loop()->is_resolved() == TRUE);
    _cc_out << "goto ";
    _get_enclosing_loop()->_publish_cc_exit_label( _cc_out );
    _cc_out << ";" << NL();
  }
  else {
    _cc_out << "break;" << NL();
  }
  if (_get_condition() != NULL) {
    _cc_out << CS("}");
  }
}

Tyvis *
TyvisExitStatement::_get_condition() {
  return dynamic_cast<Tyvis *>(get_condition());
}

TyvisSequentialStatement *
TyvisExitStatement::_get_enclosing_loop() {
  return dynamic_cast<TyvisSequentialStatement *>(get_enclosing_loop());
}

void
TyvisExitStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  if(_get_condition() != NULL) {
    _get_condition()->_get_list_of_input_signals(list);
  }
}

void
TyvisExitStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  if(_get_condition() != NULL) {
    _get_condition()->_build_above_attribute_set(to_build);
  }
}

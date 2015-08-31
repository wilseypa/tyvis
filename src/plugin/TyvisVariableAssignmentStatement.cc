
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
#include "TyvisLabel.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisVariableAssignmentStatement.hh"

#include "savant/error_func.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>


void 
TyvisVariableAssignmentStatement::_publish_cc( published_file &_cc_out,
					       PublishData *declarations ){
  CC_REF( _cc_out, "Tyvis::VariableAssignmentStatement_publish_cc" );
  if (_get_label() != NULL) {
    // Leaving this hard return here so that the label sits on the left
    // edge of the source.
    _cc_out << "\n";
    _get_label()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << ":" << NL();
  }
  _publish_cc_include( _cc_out, "tyvis/Variable.hh" );
  _cc_out << OS("Variable::assign(");
  _get_target()->_publish_cc_variable_lvalue( _cc_out, declarations );
  _cc_out << "," << NL();
  _get_expression()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << CS(");");
}

Tyvis *
TyvisVariableAssignmentStatement::_get_expression() {
  return dynamic_cast<Tyvis *>(get_expression());
}

void
TyvisVariableAssignmentStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_expression()->_get_list_of_input_signals(list);
}

Tyvis *
TyvisVariableAssignmentStatement::_get_target() {
  return dynamic_cast<Tyvis *>(get_target());
}

void
TyvisVariableAssignmentStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  ASSERT(_get_expression() != NULL);
  _get_expression()->_build_above_attribute_set(to_build);
}

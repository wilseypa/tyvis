
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

#include "TyvisArchitectureStatement.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisSimultaneousIfStatement.hh"
#include "TyvisSimultaneousElsif.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/resolution_func.hh"
#include "published_file.hh"
#include "savant/StandardPackage.hh"
#include "savant/set.hh"

TyvisSimultaneousElsif::TyvisSimultaneousElsif() {
  set_then_sequence_of_statements(new TyvisArchitectureStatementList());
}

TyvisSimultaneousElsif::~TyvisSimultaneousElsif() {
  delete get_then_sequence_of_statements();
}

void
TyvisSimultaneousElsif::_publish_cc_ams_function_prototype(published_file &_cc_out) {
  TyvisArchitectureStatement *current_stmt = NULL;
  
  _cc_out << NL() << "int" << NL();
  _cc_out << "ams_condition_";
  _cc_out << this;
  _cc_out << "( component *, double * );" << NL();
  
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_then_sequence_of_statements()->first());
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_then_sequence_of_statements()->successor(current_stmt)) ) {
    (dynamic_cast <TyvisSimultaneousStatement*>(current_stmt))->_publish_cc_ams_function_prototype(_cc_out);
  }
  if (_get_else_clause() != NULL) {
    _get_else_clause()->_publish_cc_ams_function_prototype(_cc_out);
  }
}

void
TyvisSimultaneousElsif::_publish_cc_ams_function_body(published_file &_cc_out, PublishData *declarations, PublishData *arch_declarations) {
  TyvisArchitectureStatement *current_stmt = NULL;
  Tyvis *condition = _get_condition();
  PublishedUnit _saved_publishing_unit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(SIMULTANEOUS_IF);
  _stmt_qty_index = 0;
  _stmt_signal_index = 0;
  condition->_set_stmt_qty_index(&_stmt_qty_index, &_unique_qtys, declarations, arch_declarations);
  condition->_set_stmt_signal_index(&_stmt_signal_index, &_unique_signals);
  condition->_build_generic_parameter_set(&_unique_generic_constants);

  _cc_out << NL() << "int" << NL();
  _cc_out << "ams_condition_";
  _cc_out << this;
  _cc_out << "( component *currentCondition, double *qty ) {" << NL();
  _cc_out << "  currentCondition = currentCondition;\n"
          << "  qty = qty;\n";
  _cc_out << "  return ( ( SAVANT_BOOLEAN_TRUE == ";
  condition->_publish_cc_ams_function(_cc_out, declarations);
  _cc_out << " ) ? 1 : 0 );" << NL();
  _cc_out << "}" << NL() << NL();
  condition->_flush_stmt_index();
  
  _set_currently_publishing_unit(_saved_publishing_unit);
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_then_sequence_of_statements()->first());
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_then_sequence_of_statements()->successor(current_stmt)) ) {
    (dynamic_cast <TyvisSimultaneousStatement *> (current_stmt))->_publish_cc_ams_function_body(_cc_out);
  }
  if (_get_else_clause() != NULL) {
    _get_else_clause()->_publish_cc_ams_function_body(_cc_out, declarations, arch_declarations);
  }
}

void
TyvisSimultaneousElsif::_publish_cc_characteristic_expressions(TyvisArchitectureStatement::SimultaneousIfPublishingPart current_part, published_file &_cc_out, PublishData *declarations) {
  
  TyvisArchitectureStatement *current_stmt = NULL;
  
  IIR_Int32 temp = _unique_qtys.size();
  TyvisDeclaration *current_set_qty = NULL;
  
  _cc_out << "  equation = new condition(\"ams_condition_";
  _cc_out << this;
  _cc_out << "\", ams_condition_";
  _cc_out << this;
  _cc_out << ", ";
  _cc_out << _unique_qtys.size();
  if (_unique_qtys.size() > 0) {
    _cc_out << ", ";
  }
  for( current_set_qty = _unique_qtys.getElement();
       current_set_qty != NULL;
       current_set_qty = _unique_qtys.getNextElement() ) {
    if (temp > 0) {
      _cc_out << "&(";
      current_set_qty->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ") ";
      if (temp > 1) {
        _cc_out << ", ";
      }
      temp--;
    }
  }
  
  _cc_out << " );" << NL();
  _cc_out << "  equation->setEnclosingBlock(this);" << NL();
  
  switch(current_part) {
  case TyvisArchitectureStatement::None: {
    _cc_out << "  add(equation);" << NL() << NL() << "  ";
  }
    break;
  case TyvisArchitectureStatement::IF_PART: {
    _cc_out << " add(equation, YES);" << NL() << NL() << "  ";
  }
  case TyvisArchitectureStatement::ELSE_PART: {
    _cc_out << " add(equation, NO);" << NL() << NL() << "  ";
  }
    break;
  default: {
    cerr << "Wrong enumeration for publishing simultaneous "
         << "if/elsif statement!!" << endl;
    cerr << "Aborting VHDL-AMS to C++ code generation ..." << endl;
    abort();
  }
    break;
  }
  _cc_out << "nextCondition(equation);" << NL() << NL() << "  ";
  
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_then_sequence_of_statements()->first());
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_then_sequence_of_statements()->successor(current_stmt)) ) {
    ( dynamic_cast <TyvisSimultaneousStatement *>(current_stmt) )->
      _publish_cc_characteristic_expressions(TyvisArchitectureStatement:: IF_PART,_cc_out);
  }
  if (_get_else_clause() != NULL) {
    _get_else_clause()->_publish_cc_characteristic_expressions(TyvisArchitectureStatement::ELSE_PART,_cc_out, declarations);
  }
}

void
TyvisSimultaneousElsif::_publish_cc( published_file &, PublishData * ){
  // Do nothing
}

Tyvis *
TyvisSimultaneousElsif::_get_condition() {
  return dynamic_cast<Tyvis *>(get_condition());
}

TyvisArchitectureStatementList *
TyvisSimultaneousElsif::_get_then_sequence_of_statements() {
  return dynamic_cast<TyvisArchitectureStatementList *>(get_then_sequence_of_statements());
}

TyvisSimultaneousElsif *
TyvisSimultaneousElsif::_get_else_clause() {
  return dynamic_cast<TyvisSimultaneousElsif *>(get_else_clause());
}

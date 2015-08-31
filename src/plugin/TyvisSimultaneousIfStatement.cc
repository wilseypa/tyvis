
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
#include "TyvisDesignFile.hh"
#include "TyvisElsif.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisSimultaneousElsif.hh"
#include "TyvisSimultaneousIfStatement.hh"
#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"

TyvisSimultaneousIfStatement::TyvisSimultaneousIfStatement() {
  set_then_statement_list(new TyvisArchitectureStatementList());
  set_else_statement_list(new TyvisArchitectureStatementList());
}

TyvisSimultaneousIfStatement::~TyvisSimultaneousIfStatement() {
  delete get_then_statement_list();
  delete get_else_statement_list();
}

void
TyvisSimultaneousIfStatement::_publish_cc( published_file &, PublishData * ) {}

void
TyvisSimultaneousIfStatement::_publish_cc_ams_function_prototype(published_file &_cc_out,
								 PublishData *declarations) {

  TyvisArchitectureStatement *current_stmt = NULL;
  
  _cc_out << NL() << "int" << NL();
  _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );
  _cc_out << "( component *, double * );" << NL();
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_then_statement_list()->first() );
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_then_statement_list()->successor(current_stmt) ) ) {
    (dynamic_cast <TyvisSimultaneousStatement *>(current_stmt))->_publish_cc_ams_function_prototype(_cc_out);
  }
  if(_get_elsif() != NULL) {
    _get_elsif()->_publish_cc_ams_function_prototype(_cc_out);
  }
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_else_statement_list()->first() );
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_else_statement_list()->successor(current_stmt) ) ) {
    (dynamic_cast <TyvisSimultaneousStatement *>(current_stmt))->_publish_cc_ams_function_prototype(_cc_out);
  }
}

void
TyvisSimultaneousIfStatement::_publish_cc_ams_function_body(published_file &_cc_out,
							    PublishData *declarations,
							    PublishData *arch_declarations) {
  TyvisArchitectureStatement *current_stmt = NULL;
  Tyvis *condition = _get_condition();
  TyvisDeclaration *current_signal = NULL;
  PublishedUnit _saved_publishing_unit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(SIMULTANEOUS_IF);
  _stmt_qty_index = 0;
  _stmt_signal_index = 0;

  condition->_set_stmt_qty_index(&_stmt_qty_index, &_unique_qtys, declarations, arch_declarations);
  condition->_set_stmt_signal_index(&_stmt_signal_index, &_unique_signals);
  condition->_build_generic_parameter_set(&_unique_generic_constants);
  
  _cc_out << NL() << "int" << NL();
  _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );
  _cc_out << "(component *currentCondition, double *qty ) {" << NL();
  _cc_out << "  currentCondition = currentCondition;\n"
          << "  qty = qty;\n";
  
  for(current_signal = _unique_signals.getElement();
      current_signal != NULL;
      current_signal = _unique_signals.getNextElement()) {
    _cc_out << "  ";
    current_signal->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "  ";
    current_signal->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "(ObjectBase::VARIABLE, currentCondition->locateSignal(";
    _cc_out << current_signal->_get_stmt_signal_index() << "));" << NL();
  }
  _cc_out << "  return ( ( SAVANT_BOOLEAN_TRUE == ";
  condition->_publish_cc_ams_function(_cc_out, declarations);
  _cc_out << " ) ? 1 : 0 );" << NL();
  _cc_out << "}" << NL() ;
  
  condition->_flush_stmt_index();
    
  _set_currently_publishing_unit(_saved_publishing_unit);
  
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_then_statement_list()->first() );
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_then_statement_list()->successor(current_stmt) ) ) {
    (dynamic_cast <TyvisSimultaneousStatement *>(current_stmt))->_publish_cc_ams_function_body(_cc_out);
  }
  if(_get_elsif() != NULL){
    _get_elsif()->_publish_cc_ams_function_body(_cc_out, declarations, arch_declarations);
  }
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_else_statement_list()->first() );
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_else_statement_list()->successor(current_stmt) ) ) {
    (dynamic_cast <TyvisSimultaneousStatement *>(current_stmt))->_publish_cc_ams_function_body(_cc_out);
  }
}

void
TyvisSimultaneousIfStatement::_publish_cc_static_type_info_fn( published_file &_cc_out,
						     PublishData *declarations ){
  if( _get_then_statement_list() && _get_then_statement_list()->size() > 0 ){
    CC_REF( _cc_out, "TyvisSimultaneousIfStatement::_publish_cc_type_info" );
    _get_then_statement_list()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
  if( _get_elsif() ){
    CC_REF( _cc_out, "TyvisSimultaneousIfStatement::_publish_cc_type_info" );
    _get_elsif()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
  if( _get_else_statement_list() && _get_else_statement_list()->size() > 0 ){
    CC_REF( _cc_out, "TyvisSimultaneousIfStatement::_publish_cc_type_info" );
    _get_else_statement_list()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
}


void
TyvisSimultaneousIfStatement::_publish_cc_characteristic_expressions(TyvisArchitectureStatement::SimultaneousIfPublishingPart current_part,
                                                                            published_file &_cc_out,
                                                                            PublishData *declarations) {
  
  TyvisArchitectureStatement *current_stmt = NULL ;
  IIR_Int32 temp = _unique_qtys.size();
  TyvisDeclaration *current_set_qty = NULL;
  TyvisDeclaration *current_set_signal = NULL;
  
  _cc_out << "  equation = new condition(\"";
  _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );
  _cc_out << "\", ";
  _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );
  _cc_out << ", ";
  _cc_out << _unique_qtys.size();
  if(_unique_qtys.size() > 0 ) {
    _cc_out << ", ";
    for( current_set_qty = _unique_qtys.getElement();
	 current_set_qty != NULL;
	 current_set_qty = _unique_qtys.getNextElement() ) {
      if( temp > 0 ) {
	_cc_out << "&(";
	current_set_qty->_publish_cc_lvalue( _cc_out , declarations );
	_cc_out << ") ";
	if( temp > 1 ) {
	  _cc_out << ", ";
	}
	temp--;
      }
    }
  }
  _cc_out << " );" << NL();
  _cc_out << "  equation->setEnclosingBlock(this);" << NL();
  for(current_set_signal = _unique_signals.getElement();
      current_set_signal != NULL;
      current_set_signal = _unique_signals.getNextElement() ) {
    _cc_out << "  equationSignal = new signalDS(&(";
    current_set_signal->_publish_cc_elaborate(_cc_out, declarations);
    _cc_out << "), (";
    current_set_signal->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << ") );\n";
    _cc_out << "  equation->addSignal(equationSignal);\n";
  }
  switch(current_part) {
  case TyvisArchitectureStatement::None: {
    _cc_out << "add(equation);" << NL() << NL() << "  ";
  }
    break;
  case TyvisArchitectureStatement::IF_PART: {
    _cc_out << "add(equation, YES);" << NL() << NL() << "  ";
  }
  case TyvisArchitectureStatement::ELSE_PART: {
    _cc_out << "add(equation, NO);" << NL() << NL() << "  ";
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
  _cc_out << "nextCondition(equation);" << NL() << "  ";
  SimultaneousIfPublishingPart stmt_publishing_part =
    TyvisArchitectureStatement::None;
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_then_statement_list()->first() );
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_then_statement_list()->successor(current_stmt) ) ) {
    stmt_publishing_part = TyvisArchitectureStatement::IF_PART;
    current_stmt->_publish_cc_characteristic_expressions(stmt_publishing_part,_cc_out);
  }
  stmt_publishing_part = TyvisArchitectureStatement::ELSE_PART;
  if(_get_elsif() != NULL) {
    _get_elsif()->_publish_cc_characteristic_expressions(stmt_publishing_part,_cc_out, declarations);
  }
  for( current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_else_statement_list()->first() );
       current_stmt != NULL;
       current_stmt = dynamic_cast<TyvisArchitectureStatement *>( _get_else_statement_list()->successor(current_stmt) ) ) {
    current_stmt->_publish_cc_characteristic_expressions(stmt_publishing_part,_cc_out);
  }
  TyvisSimultaneousElsif *temp1 = _get_elsif();
  TyvisSimultaneousElsif *temp2 = NULL;
  if( temp1 != NULL) {
    _cc_out << "  removeCondition();" << NL();
    temp2 = _get_elsif()->_get_else_clause();
    while(temp2 != NULL) {
      _cc_out << "  removeCondition();" << NL();
      temp2 = temp2->_get_else_clause();
    }
  }
  _cc_out << "  removeCondition();" << NL() ;
}


IIR_Boolean
TyvisSimultaneousIfStatement::_is_simultaneous_statement() {
  return TRUE;
}

Tyvis *
TyvisSimultaneousIfStatement::_get_condition() {
  return dynamic_cast<Tyvis *>(get_condition());
}

TyvisArchitectureStatementList *
TyvisSimultaneousIfStatement::_get_then_statement_list() {
  return dynamic_cast<TyvisArchitectureStatementList *>(get_then_statement_list());
}

TyvisArchitectureStatementList *
TyvisSimultaneousIfStatement::_get_else_statement_list() {
  return dynamic_cast<TyvisArchitectureStatementList *>(get_else_statement_list());
}

TyvisSimultaneousElsif *
TyvisSimultaneousIfStatement::_get_elsif() {
  return dynamic_cast<TyvisSimultaneousElsif *>(get_elsif());
}



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

#include "TyvisForLoopStatement.hh"
#include "TyvisIdentifier.hh"
#include "TyvisConstantDeclaration.hh"
#include "TyvisRangeTypeDefinition.hh"
#include "TyvisLabel.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisConstantDeclaration.hh"
#include "TyvisAttribute.hh"
#include "TyvisAccessTypeDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "PublishData.hh"

#include <sstream>
#include "published_file.hh"

TyvisForLoopStatement::TyvisForLoopStatement() {
  set_sequence_of_statements(new TyvisSequentialStatementList());
}

TyvisForLoopStatement::~TyvisForLoopStatement() {
  //Release the list memory
  delete get_sequence_of_statements();
}

void 
TyvisForLoopStatement::_publish_cc( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(_get_iteration_scheme() != NULL);
  ASSERT(_get_iteration_scheme()->is_resolved() == TRUE);

  CC_REF( _cc_out, "TyvisForLoopStatement::_publish_cc" );
  _cc_out << "\n" << *_get_mangled_label() << ":" << NL();
  _publish_cc_while_loop_begin( _cc_out, declarations );
  _cc_out << OS("{");
  _get_sequence_of_statements()->_publish_cc( _cc_out, declarations );
  _publish_cc_next_label( _cc_out );
  _cc_out << ":;" << NL();
  _cc_out << "static_cast<void>(" << _get_cc_iterator_name() << ".next());" << NL();
  _cc_out << OS("Variable::assign(");
  _get_iteration_scheme()->_publish_cc_lvalue( _cc_out, declarations );
  _cc_out << "," << NL()
	  << "UniversalInteger(" << _get_cc_iterator_name() << ".peek())"
	  << CS(");") << NL();
  _cc_out << CS("}");  
  _publish_cc_exit_label( _cc_out );
  _cc_out << ":;" << NL();
}

void
TyvisForLoopStatement::_publish_cc_static_type_info_fn( published_file &_cc_out,
					      PublishData *declarations ){
  if( _get_iteration_scheme()->get_subtype()->is_locally_static() ){
    CC_REF( _cc_out, "TyvisForLoopStatement::_publish_cc_type_info" );
    _get_iteration_scheme()->_get_subtype()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
  if( _get_sequence_of_statements() && _get_sequence_of_statements()->size() > 0 ){
    CC_REF( _cc_out, "TyvisForLoopStatement::_publish_cc_type_info" );
    _get_sequence_of_statements()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
}

string
TyvisForLoopStatement::_get_cc_iterator_name(){
  return string("_iter_" + _get_iteration_scheme()->_get_mangled_declarator()->convert_to_string() );
}

void
TyvisForLoopStatement::_publish_cc_decl_for_loop_iterator( published_file &_cc_out,
							   PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisForLoopStatement::_publish_cc_decl_for_loop_iterator" );
  if( _is_currently_publishing_subprogram() ){
    _get_iteration_scheme()->_publish_cc_decl_with_constructor_args( _cc_out, declarations );
  }
  _cc_out << "RangeIterator " << _get_cc_iterator_name() << " = ";
  _get_iteration_scheme()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << ".range(UniversalInteger(1)).iterator();" << NL();
  _cc_out << _get_cc_iterator_name() << ".set(";
  _get_iteration_scheme()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << ".getIntValue());" << NL();
  _get_sequence_of_statements()->_publish_cc_decl_for_loop_iterator( _cc_out,
								     declarations );
}

void
TyvisForLoopStatement::_publish_cc_while_loop_begin( published_file &_cc_out,
						     PublishData * declarations ){
//DNS: I do not understand why this is needed!  I think because non-declaration code is being
// written by declaration methods.
  PublishData dummy;
  CC_REF( _cc_out, "TyvisForLoopStatement::_publish_cc_while_loop_begin" );
  _cc_out << _get_cc_iterator_name() << OS(".reset(");
  if (dynamic_cast<TyvisIntegerSubtypeDefinition *>(get_iteration_scheme()->get_subtype())) {
    dynamic_cast<TyvisIntegerSubtypeDefinition *>
      (get_iteration_scheme()->get_subtype())->_publish_cc_array_info( _cc_out, &dummy );
  }
  _cc_out << CS(");") << NL();

  _cc_out << OS("Variable::assign(");
  _get_iteration_scheme()->_publish_cc_lvalue( _cc_out, declarations );
  _cc_out << "," << NL()
	  << "UniversalInteger(" << _get_cc_iterator_name() << ".peek())"
	  << CS(");") << NL();
  _cc_out << "while( " << _get_cc_iterator_name() << ".hasNext() )";
}

TyvisSequentialStatementList *
TyvisForLoopStatement::_get_sequence_of_statements() {
  return dynamic_cast<TyvisSequentialStatementList *>(get_sequence_of_statements());
}

TyvisConstantDeclaration *
TyvisForLoopStatement::_get_iteration_scheme() {
  return dynamic_cast<TyvisConstantDeclaration *>(get_iteration_scheme());
}

void
TyvisForLoopStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_sequence_of_statements()->_get_list_of_input_signals(list) ;
}

void
TyvisForLoopStatement::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  _get_sequence_of_statements()->_get_signal_source_info(siginfo);
}

void
TyvisForLoopStatement::_build_wait_list(dl_list<TyvisWaitStatement>* list) {
  _get_sequence_of_statements()->_build_wait_list(list);
}

void 
TyvisForLoopStatement::_build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *list) {
  _get_sequence_of_statements()->_build_procedure_call_stmt_list(list);
}

void
TyvisForLoopStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_sequence_of_statements()->_build_above_attribute_set(to_build);
}

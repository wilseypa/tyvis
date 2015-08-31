
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

#include "TyvisArraySubtypeDefinition.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisCaseStatementAlternative.hh"
#include "TyvisCaseStatementAlternativeList.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryUnit.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "TyvisSimpleName.hh"
#include "TyvisStatement.hh"
#include "TyvisWaveformElement.hh"
#include "TyvisWaveformList.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>
#include "savant/StandardPackage.hh"

TyvisStatement::TyvisStatement() { _mangled_label = NULL; }
TyvisStatement::~TyvisStatement() { delete _mangled_label; }

void
TyvisStatement::_publish_cc_assertion_condition( published_file &_cc_out,
						 PublishData *declarations ){
  CC_REF( _cc_out, "TyvisStatement::_publish_cc_assertion_condition( published_file &_cc_out )" );
  _get_assertion_condition()->_publish_cc_rvalue( _cc_out, declarations );
}

void 
TyvisStatement::_publish_cc_assertion_or_report_statement( published_file &_cc_out,
							   PublishData *declarations ){
  CC_REF( _cc_out, "TyvisStatement::_publish_cc_assertion_or_report_statement( published_file &_cc_out )" );

  _cc_out<< OS( "processPtr->vhdlAssertOrReport(" );
  // If this is actually an assertion statement, this will publish a c++
  // boolean expression.  Else it will publish "true".
  if( _is_assert_statement() == true ){
    _cc_out << "true";
  }
  else{
    _cc_out << "false";
  }
  _cc_out << "," << NL();
  _publish_cc_assertion_condition( _cc_out, declarations );
  _cc_out << "," << NL();
  _cc_out << "savantnow," << NL();

  if (_get_report_expression() != NULL) {
    _get_report_expression()->_publish_cc_rvalue( _cc_out, declarations );
  }
  else {
    // A empty string...
    _cc_out << "VHDL_STRING(\"\")";
  }
  _cc_out << "," << NL();
  if(_get_severity_expression() != NULL) {
    _get_severity_expression()->_publish_cc_rvalue( _cc_out, declarations );
  } 
  else {
    // This corresponds to the NOTE level.
    dynamic_cast<TyvisEnumerationSubtypeDefinition *>(get_design_file()->get_standard_package()->get_severity_level_type())->_get_left()->_publish_cc_rvalue( _cc_out , declarations );
  }
  _cc_out << "," << NL()
  // More information on the location of the assertion statement.
	  << "\" at " << *_get_file_name() << ":" << get_line_number() << "\"" << CS(");");
}

void
TyvisStatement::_mangle_label() const {
  TyvisIdentifier *new_identifier = NULL;
  ostringstream ident_stream;
  string ident_str;
  
  if (_get_label() != NULL) {
    if (_get_label()->_get_declarative_region() != NULL) {
      ident_stream << *(_get_label()->_get_declarative_region()->_get_mangled_declarator())
		   << "_";
    }
    ident_stream << *(_get_label());
  }
  else {
    //    ident_stream << "ALBL_" << this;
    if( _get_current_architecture_name() != "" ) {
      ident_stream << _get_current_architecture_name() << "_";
    }
    ident_stream << "l" << get_line_number();
  }
    
  ident_str       = ident_stream.str();
  new_identifier = dynamic_cast<TyvisIdentifier *>(TyvisIdentifier::get( ident_str, get_design_file()->get_class_factory() ));

  const_cast<TyvisStatement *>(this)->_set_label(new TyvisLabel());
  const_cast<TyvisStatement *>(this)->copy_location( _get_mangled_label() );
  _get_mangled_label()->set_declarator(new_identifier);
}

TyvisLabel*
TyvisStatement::_get_mangled_label() const {
  if (_mangled_label == NULL) {
    _mangle_label();
  }

  return _mangled_label;
}

// This and associated functions (_get_label(), _mangle_label(), _set_label())
// were duplicated in process, concurrent generate for & if, block, for loop
// statements earlier. Hence removed the redundant code from other places and
// moved it down here.

TyvisTextLiteral*
TyvisStatement::_get_declarator(){
  ASSERT (_get_label() != NULL);
  return _get_label()->_get_declarator();
}

TyvisTextLiteral*
TyvisStatement::_get_mangled_declarator(){
  ASSERT (_get_mangled_label() != NULL);
  return _get_mangled_label()->_get_declarator();
}

TyvisLibraryUnit *
TyvisStatement::_get_design_unit(){
  TyvisLibraryUnit *retval = 0;

  if( dynamic_cast<TyvisLibraryUnit *>(_get_declarative_region()) != 0 ){
    retval = dynamic_cast<TyvisLibraryUnit *>(_get_declarative_region());
  }
  else{
    ASSERT( dynamic_cast<TyvisStatement *>(_get_declarative_region()) != 0 );
    retval = dynamic_cast<TyvisStatement *>(_get_declarative_region())->_get_design_unit();
  }

  return retval;
}

const string
TyvisStatement::_get_design_unit_name(){
  ASSERT( _get_design_unit() != 0 );
  return _get_design_unit()->_get_cc_elaboration_class_name();
}
    
void
TyvisStatement::_set_label(TyvisLabel* new_label){
  _mangled_label = new_label;
}

Tyvis *
TyvisStatement::_get_case_statement_expression(){
  _report_undefined_fn("_get_case_statement_expression()");
  return NULL;
}

TyvisLabel*
TyvisStatement::_get_label() const {
  return dynamic_cast<TyvisLabel *>(get_label());
}

Tyvis *
TyvisStatement::_get_target(){
  _report_undefined_fn("_get_target()");
  return NULL;
}

Tyvis *
TyvisStatement::_get_reject_time_expression(){
  _report_undefined_fn("_get_reject_time_expression");
  return NULL;
}

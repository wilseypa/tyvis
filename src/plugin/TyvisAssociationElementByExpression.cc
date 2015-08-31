
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
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisCaseStatementAlternativeByChoices.hh"
#include "TyvisChoice.hh"
#include "TyvisChoiceList.hh"
#include "TyvisConcurrentStatement.hh"
#include "TyvisDesignFile.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisLabel.hh"
#include "TyvisRangeAttribute.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisTextLiteral.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "published_file.hh"
#include <sstream>
#include "savant/set.hh"

void 
TyvisAssociationElementByExpression::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(is_resolved() == TRUE);

  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_rvalue");
  _get_actual()->_publish_cc_rvalue( _cc_out, declarations );
}

void
TyvisAssociationElementByExpression::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(is_resolved() == TRUE);
  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_lvalue");
  _get_actual()->_publish_cc_lvalue( _cc_out, declarations );
}

const string
TyvisAssociationElementByExpression::_publish_cc_magic_prefix( published_file &_cc_out ){
  ostringstream prefix;

  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_magic_prefix");

  ASSERT( _get_current_publish_node() != NULL );
  ASSERT( dynamic_cast<TyvisConcurrentStatement*>(_get_current_publish_node()) != NULL ||
	  _get_current_publish_node()->get_kind() == IIR_LABEL);

  prefix << "enclosingArch->";
  if( dynamic_cast<TyvisArchitectureStatement*>(_get_current_publish_node()) != NULL ){
    prefix << *(dynamic_cast<TyvisArchitectureStatement*>(_get_current_publish_node())->_get_mangled_label()->_get_mangled_declarator());
  }
  else {
    ASSERT( _get_current_publish_node()->get_kind() == IIR_LABEL );
    TyvisLabel *label = dynamic_cast<TyvisLabel*>(_get_current_publish_node());
    if( label->get_statement() == NULL ){
      //The label that we have is already a mangled one
      prefix << *( label->_get_mangled_declarator() );
    }
    else {
      //get the mangled label from unmangled one
      prefix << *(label->_get_statement()->_get_mangled_label()->_get_mangled_declarator());
    }
  }
  prefix << "_elab_obj->";
  return prefix.str();
}

void 
TyvisAssociationElementByExpression::_publish_cc_generic_map_aspect( published_file &_cc_out,
								     PublishData *declarations ) {
  ASSERT(is_resolved() == TRUE);
  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_generic_map_aspect");
  const string prefix_string = _publish_cc_magic_prefix( _cc_out );
  _set_publish_prefix_string( prefix_string );
//  _publish_cc_prefix_string( _cc_out ); //DRH
  _get_actual()->_publish_cc_rvalue( _cc_out, declarations );
  _set_publish_prefix_string( "" );
}

void
TyvisAssociationElementByExpression::_publish_cc_generic_map_aspect_for_conc_stmts( published_file &_cc_out,
										    PublishData *declarations ){
  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_generic_map_aspect_for_conc_stmts");
  ASSERT(is_resolved() == TRUE);
  _get_actual()->_publish_cc_rvalue( _cc_out, declarations );
}

void 
TyvisAssociationElementByExpression::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_elaborate");

  _get_actual()->_publish_cc_elaborate( _cc_out, declarations );
}

void
TyvisAssociationElementByExpression::_publish_cc_aggregate_iterator( published_file &_cc_out,
								     PublishData *declarations ){

  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_aggregate_iterator");
  _cc_out << "( ";
  if (_get_aggregate_iterator_subtype() != NULL) {
    ASSERT(_get_aggregate_iterator_subtype()->_get_base_type_left() != NULL);
    _get_aggregate_iterator_subtype()->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    if( _get_aggregate_iterator_subtype()->_get_base_type_direction()->is_ascending_range()){
      _cc_out << " + ";
    }
    else {
      _cc_out << " - ";
    }
  }
  else {
    _cc_out << "1 + ";
  }
  
  _cc_out << _get_aggregate_iterator_counter() << " )";
}

void 
TyvisAssociationElementByExpression::_publish_cc_non_aggregate_object_init( published_file &_cc_out,
                                     PublishData *declarations, bool left ) {
  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_non_aggregate_object_init");
  if (left)
    _cc_out << "&";
  _get_actual()->_publish_cc_rvalue( _cc_out, declarations );
}

void 
TyvisAssociationElementByExpression::_publish_cc_state_object_init( published_file &_cc_out,
								    PublishData *declarations ) {
  _publish_cc_state_object_init_work(_cc_out, declarations, true);
}


void 
TyvisAssociationElementByExpression::_publish_cc_state_l_object_init( published_file &_cc_out,
								    PublishData *declarations ) {
  _publish_cc_state_object_init_work(_cc_out, declarations, false);
}

void 
TyvisAssociationElementByExpression::_publish_cc_state_object_init_work( published_file &_cc_out,
								    PublishData *declarations,
                                                                    bool left ) {
  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_state_object_init");

  _publish_cc_include( _cc_out, "tyvis/ElementAssociation.hh", true );
  _publish_cc_include( _cc_out, "tyvis/ArrayInfo.hh", true );

  if(_get_actual()->get_kind() == IIR_OTHERS_INITIALIZATION) {
    _cc_out << OS("ElementAssociation(others(),");
    _publish_cc_non_aggregate_object_init( _cc_out, declarations, left );
    _cc_out << CS(")");
  }
  else{
    if (_get_formal() == NULL) {
      _cc_out << OS("ElementAssociation(");
      _cc_out << "nullInfo()";
      _cc_out << "," << NL();
      _publish_cc_non_aggregate_object_init( _cc_out, declarations, left );
      _cc_out << CS(")");
    }
    else {
      // Formal is not null.
      if( _get_formal()->get_kind() == IIR_CASE_STATEMENT_ALTERNATIVE_BY_CHOICES ) {
	TyvisCaseStatementAlternativeByChoices *case_choices = 
	  dynamic_cast<TyvisCaseStatementAlternativeByChoices*>(_get_formal());
	TyvisChoice* current_choice = dynamic_cast<TyvisChoice *>(case_choices->get_choices()->first());
	while(current_choice != NULL) {
          _cc_out << OS("ElementAssociation(");
	  current_choice->_get_value()->_publish_cc_universal_value( _cc_out, declarations );
	  _cc_out << "," << NL();
	  _publish_cc_non_aggregate_object_init( _cc_out, declarations, left );
	  _cc_out << CS(")");
	  current_choice = dynamic_cast<TyvisChoice *>(case_choices->get_choices()->successor(current_choice));
	  if(current_choice != NULL) {
	    _cc_out << "," << NL();
	  }
	}
      }
      else if( _get_formal()->get_kind() == IIR_RANGE_ATTRIBUTE ){
        _cc_out << OS("ElementAssociation(");

	if (_get_formal()->_get_subtype()->is_anonymous() == FALSE) {
	  _get_formal()->_get_subtype()->_publish_cc_array_info( _cc_out );
	}
	else {
	  // This is a nonymous range..have to pick up the range from the formal
	  // variable associated with the range attribute...
	  TyvisRangeAttribute *rangeAttribute = dynamic_cast<TyvisRangeAttribute *>(_get_formal());
	  ASSERT ( rangeAttribute->get_kind() == IIR_RANGE_ATTRIBUTE );
	  _cc_out << OS("ArrayInfo(");
	  rangeAttribute->_publish_cc_range( _cc_out, declarations );
	  _cc_out << CS(")");
	}
	
	_cc_out << "," << NL();
	_publish_cc_non_aggregate_object_init( _cc_out, declarations, left );
	_cc_out << CS(")");
      }
      else {
	// We've got a formal and it's not "by choices".
        _cc_out << OS("ElementAssociation(");
	_get_formal()->_publish_cc_universal_value( _cc_out, declarations );
	_cc_out << "," << NL();
	_publish_cc_non_aggregate_object_init( _cc_out, declarations, left );
	_cc_out << CS(")");
      }
    }
  }
}


// This method publishes the element as an argument to a subprogram
// (function/procudure).  This is different from _publish_cc because this
// publishes just the declarators.
// For example, 
//   "var" will be published by _publish_cc_rvalue( _cc_out ) as 
//     "state.current->var" and as
//     "var" by _publish_cc_subprogram_arguments( _cc_out ).
// Assumption: get_actual() returns one of TyvisDeclaration (any), TyvisLiteral
// or TyvisIndexedName 
void
TyvisAssociationElementByExpression::_publish_cc_subprogram_arguments( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(_get_actual()->is_resolved());

  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_subprogram_arguments");

  if(dynamic_cast<TyvisDeclaration *>(_get_actual()) != NULL) {
    _cc_out << *dynamic_cast<TyvisDeclaration *>(_get_actual())->_get_mangled_declarator();
  }
  else if(_get_actual()->get_kind() == IIR_INDEXED_NAME) {
    _get_actual()->_publish_cc_subprogram_arg( _cc_out );
  }
  else {    
    _get_actual()->_publish_cc_rvalue( _cc_out, declarations );
  }
}

void
TyvisAssociationElementByExpression::_publish_cc_first_objectParameter( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisAssociationElementByExpression::_publish_cc_first_objectParameter");

  _get_actual()->_publish_cc_first_objectParameter( _cc_out, declarations );
}

void 
TyvisAssociationElementByExpression::_add_decl_into_declaration_collection( PublishData *declarations ) {
  if(_get_actual() != NULL) {
    _get_actual()->_add_decl_into_declaration_collection(declarations);
  }
}

IIR_Int32
TyvisAssociationElementByExpression::_get_number_of_element_associations() {

  if(_get_actual()->get_kind() == IIR_OTHERS_INITIALIZATION) {
    return 1;
  }

  if (_get_formal() == NULL) {
    return 1;
  }
  
  if(_get_formal()->get_kind() == IIR_CASE_STATEMENT_ALTERNATIVE_BY_CHOICES) {
    TyvisCaseStatementAlternativeByChoices* case_choices = dynamic_cast<TyvisCaseStatementAlternativeByChoices*>(_get_formal());
    return case_choices->get_choices()->size();
  }
  else {
    return 1;
  }
  return 1;
}

Tyvis *
TyvisAssociationElementByExpression::_get_actual() {
  return dynamic_cast<Tyvis *>(get_actual());
}

void
TyvisAssociationElementByExpression::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list){
  _get_actual()->_build_sensitivity_list(sensitivity_list);
}

IIR_Boolean 
TyvisAssociationElementByExpression::_is_globally_static_for_array(){
  IIR_Boolean retval = TRUE;

  if( _get_formal() != NULL &&
      dynamic_cast<TyvisScalarTypeDefinition *>(_get_formal()) &&
      _get_formal()->_is_globally_static() == FALSE ){
    retval = FALSE;
  }    

  // Can't have an OPEN array element association.
  ASSERT( _get_actual() != NULL );
  
  if( _get_actual()->_is_globally_static() == FALSE ){
    retval = FALSE;
  }
  
  return retval;
}

IIR_Boolean 
TyvisAssociationElementByExpression::_is_globally_static_for_record(){
  // Can't have an OPEN record element association.
  ASSERT( _get_actual() != NULL );

  return _get_actual()->_is_globally_static();
}

IIR_Boolean 
TyvisAssociationElementByExpression::_is_globally_static_for_function(){
  if( _get_actual() == NULL || _get_actual()->_is_globally_static() == TRUE ){
    return TRUE;
  }
  else{
    return FALSE;
  }
}

void
TyvisAssociationElementByExpression::_get_signal_source_info(  savant::set<Tyvis> *siginfo) {
  if(_get_formal() != NULL) {
    ASSERT(is_resolved() == TRUE);
    ASSERT(_get_formal() != NULL);
    ASSERT(_get_formal()->is_resolved() == TRUE);
    if( _get_formal()->is_signal() ){
      ASSERT(_get_formal()->is_interface() == TRUE);
      IIR_Mode signal_mode = (dynamic_cast<TyvisInterfaceDeclaration *>(_get_formal()))->get_mode();
      if(signal_mode == IIR_OUT_MODE || signal_mode == IIR_INOUT_MODE) {
	siginfo->add( _get_actual() );
      }
    }
  }
  else {
    ASSERT(_get_actual()->is_resolved() == TRUE);
    _get_actual()->_get_signal_source_info(siginfo);
  }
}

void
TyvisAssociationElementByExpression::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  if(_get_actual() != NULL) {
   _get_actual()->_build_above_attribute_set(to_build);
  }
}

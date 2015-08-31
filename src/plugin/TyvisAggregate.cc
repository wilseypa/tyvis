
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

#include "TyvisAggregate.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisElementDeclaration.hh"
#include "TyvisEnumerationTypeDefinition.hh"
#include "TyvisIntegerLiteral32.hh"
#include "TyvisRecordTypeDefinition.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisTextLiteral.hh"

#include "savant/error_func.hh"
#include "published_file.hh"
#include <sstream>
#include <iostream>

TyvisAggregate::TyvisAggregate()
{
  set_element_association_list(new TyvisAssociationList());
}

TyvisAggregate::~TyvisAggregate()
{
  delete get_element_association_list();
}

void
TyvisAggregate::_publish_cc_xvalue_begin( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisAggregate::_publish_cc_xvalue_begin" );

  if( _get_subtype() && _get_subtype()->is_record_type() ){
    _publish_cc_include( _cc_out, "tyvis/Record.hh" );
    _cc_out << "Record";
  }
  else{
    _publish_cc_include( _cc_out, "tyvis/Aggregate.hh" );
    _cc_out << "Aggregate";
  }
}

void
TyvisAggregate::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAggregate::_publish_cc_rvalue" );
  _publish_cc_xvalue_begin( _cc_out );
  _cc_out << OS("(");
  _publish_cc_constructor_args( _cc_out, declarations, "" );
  _cc_out << CS(")");
}

void
TyvisAggregate::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAggregate::_publish_cc_lvalue" );
  _publish_cc_xvalue_begin( _cc_out );
  _cc_out << OS("(");
  _publish_cc_constructor_args( _cc_out, declarations, "LVALUE" );
  _cc_out << CS(")");
}

void
TyvisAggregate::_publish_cc_constructor_args( published_file &_cc_out,
					      PublishData *declarations,
					      const string &varArgsArg ) {
  CC_REF( _cc_out, "TyvisAggregate::_publish_cc_constructors_args" );
  _publish_cc_include( _cc_out, "tyvis/TyvisVarArgs.hh" );
  ASSERT(_get_subtype() != NULL);
  if( !_get_subtype()->_is_anonymous() ){
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << ", " << NL();
  }
  if( get_element_association_list()->size() > 0 ){
    _cc_out << "TyvisVarArgs(" << varArgsArg << ") << " << NL();
    if (varArgsArg == "LVALUE")
      _get_element_association_list()->_publish_cc_associations( _cc_out, declarations, " <<" );
    else
      _get_element_association_list()->_publish_cc_l_associations( _cc_out, declarations, " <<" );
  }
}

void
TyvisAggregate::_publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(TyvisExpression::_get_subtype() != NULL);
  TyvisTypeDefinition *old_aggreate_type = _get_aggregate_iterator_subtype();
  int  old_aggreate_count = _get_aggregate_iterator_counter();

  CC_REF( _cc_out, "TyvisAggregate::_publish_cc_initialization_value" );

  _cc_out << "TyvisVarArgs() <<" << NL();
  
  if (TyvisExpression::_get_subtype()->is_unconstrained_array_type() == FALSE) {
    _set_aggregate_iterator_subtype(TyvisExpression::_get_subtype()->_get_resolved_index_subtype());
    _set_aggregate_iterator_counter(0);
  }
  else {
    ASSERT(TyvisExpression::_get_subtype()->_get_resolved_index_subtype()->_get_resolved_base_type()->is_scalar_type() == TRUE);
    ASSERT(dynamic_cast<TyvisScalarTypeDefinition *>(TyvisExpression::_get_subtype()->_get_resolved_index_subtype()->_get_resolved_base_type())->get_left() != NULL );
    _set_aggregate_iterator_subtype(TyvisExpression::_get_subtype()->_get_resolved_index_subtype()->_get_resolved_base_type());
  }
  
  dynamic_cast<TyvisAssociationList *>(get_element_association_list())->_publish_cc_state_object_init( _cc_out, declarations );

  _set_aggregate_iterator_counter(old_aggreate_count);
  _set_aggregate_iterator_subtype(old_aggreate_type);
}

void
TyvisAggregate::_publish_cc_state_object_init( published_file &, PublishData * ){}

void
TyvisAggregate::_publish_cc_state_l_object_init( published_file &, PublishData * ){}

void
TyvisAggregate::_publish_cc_element_list( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisAggregate::_publish_cc_element_list" );
  _get_element_association_list()->_publish_cc_lvalue( _cc_out, declarations );
}

void
TyvisAggregate::_publish_cc_multi_dimensional_range( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisAggregate::_publish_cc_multi_dimensional_range" );
  
  this->_publish_cc_range( _cc_out, declarations );

  TyvisTypeDefinition* element_subtype = TyvisExpression::_get_subtype()->_get_element_subtype();

  if (element_subtype->is_element() == TRUE) {
    // Do not publish ranges for the multi-dimensional array case.
    return;
  }
  
  Tyvis* aggregate = dynamic_cast<Tyvis *>(get_element_association_list()->first());
  while(aggregate->is_aggregate_expression() == TRUE) {
    _cc_out << "," << NL();
    aggregate->_publish_cc_range( _cc_out, declarations );
    ASSERT(aggregate->get_kind() == IIR_AGGREGATE);
    aggregate = dynamic_cast<Tyvis *>(((dynamic_cast<TyvisAggregate *>(aggregate))->get_element_association_list())->first());
  }
  if((aggregate->is_array_type() == TRUE) ||
     (aggregate->is_record_type() == TRUE)) {
    _cc_out << "," << NL();
    aggregate->_publish_cc_range( _cc_out, declarations );
  }
}

void
TyvisAggregate::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {
  TyvisScalarTypeDefinition *index_subtype = TyvisExpression::_get_subtype()->_get_resolved_index_subtype();
  TyvisScalarTypeDefinition *baseType = NULL;

  CC_REF( _cc_out, "TyvisAggregate::_publish_cc_range" );
  
  ASSERT(index_subtype->is_scalar_type() == TRUE);

  if (index_subtype->get_left() == NULL) {
    baseType = dynamic_cast<TyvisScalarTypeDefinition *>(index_subtype->_get_resolved_base_type());
    if (baseType->get_left() == NULL) {
      baseType = dynamic_cast<TyvisScalarTypeDefinition *>(index_subtype->_get_bottom_base_type());
    }
    
    ASSERT(baseType->get_left() != NULL);
    
    if (baseType->is_ascending_range() == TRUE) {
      baseType->_publish_cc_left( _cc_out, declarations );
      
      _cc_out << "," << NL()
	      << "ArrayInfo::to," << NL();
      
      baseType->_publish_cc_left( _cc_out, declarations );
      _cc_out << " + " << (_get_number_of_element_associations() - 1);
    }
    else {
      baseType->_publish_cc_left( _cc_out, declarations );
      _cc_out << " + " << (_get_number_of_element_associations() - 1);
      
      _cc_out << "," << NL()
	      << "ArrayInfo::downto," << NL();
      
      baseType->_publish_cc_left( _cc_out, declarations );
    }
  }
  else {
    if (index_subtype->is_ascending_range() == TRUE) {
      index_subtype->_publish_cc_left( _cc_out, declarations );
      
      _cc_out << ","
	      << "ArrayInfo::to," << NL();
      
      index_subtype->_publish_cc_right( _cc_out, declarations );
    }
    else {
      index_subtype->_publish_cc_left( _cc_out, declarations );
      
      _cc_out << "," << NL()
	      << "ArrayInfo::downto,";
      
      index_subtype->_publish_cc_right( _cc_out, declarations );
    }
  }
}

TyvisAssociationList *
TyvisAggregate::_get_element_association_list() {
  return dynamic_cast<TyvisAssociationList *>(get_element_association_list());
}


void
TyvisAggregate::_add_decl_into_declaration_collection(PublishData *declarations) {
  _get_element_association_list()->_add_decl_into_declaration_collection(declarations);
}

IIR_Int32
TyvisAggregate::_get_number_of_element_associations() {
  return _get_element_association_list()->_get_number_of_element_associations();
}

TyvisDeclaration*
TyvisAggregate::_get_prefix_declaration(){
  return NULL;
}

void
TyvisAggregate::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_element_association_list()->_build_sensitivity_list(sensitivity_list);
}

IIR_Boolean 
TyvisAggregate::_is_globally_static_primary(){
  IIR_Boolean retval = TRUE;
  ASSERT( is_resolved() == TRUE );

  if( TyvisExpression::_get_subtype()->is_array_type() == TRUE ){
    retval = _get_element_association_list()->_is_globally_static_for_array();
  }
  else{
    ASSERT( TyvisExpression::_get_subtype()->is_record_type() == TRUE );
    retval = _get_element_association_list()->_is_globally_static_for_record();
  }

  return retval;
}

void
TyvisAggregate::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  dynamic_cast<TyvisAssociationList *>(get_element_association_list())->_get_signal_source_info(siginfo);
}

void
TyvisAggregate::_get_list_of_input_signals(  savant::set<Tyvis> *new_list ){
  _get_element_association_list()->_get_list_of_input_signals(new_list);
}

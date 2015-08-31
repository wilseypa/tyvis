
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

#include "TyvisAccessTypeDefinition.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisAttributeDeclaration.hh"
#include "TyvisConcurrentStatement.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisIndexedName.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSliceName.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisTypeConversion.hh"
#include "TyvisTypeDeclaration.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>

using std::cerr;

void 
TyvisIndexedName::_publish_cc_object_name( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_lvalue( _cc_out , declarations );
}

void 
TyvisIndexedName::_publish_cc_name_elaborate( published_file &_cc_out, PublishData *declarations ) {
  if ((_get_suffix()->_is_globally_static() == TRUE) || (_get_suffix()->is_constant() == TRUE))  {
    _publish_cc_elaborate( _cc_out, declarations );
  }
  else  { 
    _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  }
}

void 
TyvisIndexedName::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisIndexedName::_publish_cc_lvalue" );
  _cc_out << "dynamic_cast<LValue &>(";
  _publish_cc_rvalue(_cc_out, declarations);
  _cc_out << ")";
}

void 
TyvisIndexedName::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisIndexedName::_publish_cc_rvalue" );	
  ASSERT ( _get_subtype() != NULL );
  _get_prefix()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << "[";
  if(_get_suffix()->get_kind() == IIR_ASSOCIATION_LIST) {
    _get_suffix()->_publish_cc_for_index( _cc_out, declarations );
  } 
  else {
    _get_suffix()->_publish_cc_rvalue( _cc_out, declarations );
  }      
  _cc_out << "]";
}

void
TyvisIndexedName::_publish_cc_index_name_as_for_loop( published_file &_cc_out, PublishData *declarations ){
  _cc_out << "for(int generateConstant = ";
  _get_suffix()->_publish_cc_value( _cc_out, declarations );
  _cc_out << ";" << NL()
          << "generateConstant == ";
  _get_suffix()->_publish_cc_value( _cc_out, declarations );
  _cc_out << ";" << NL()
          << "generateConstant++)" << NL();
}

Tyvis *
TyvisIndexedName::_get_cc_instantiated_unit(){
  ASSERT( _get_suffix()->get_kind() != 0 );
  ASSERT( dynamic_cast<TyvisArchitectureDeclaration *>(_get_suffix()) != NULL );
  return _get_suffix();
}

void 
TyvisIndexedName::_publish_cc_data( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_data( _cc_out, declarations );
  _cc_out << "[ ";
  _get_suffix()->_publish_cc_data( _cc_out, declarations );
  _cc_out << "]";
}

void
TyvisIndexedName::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {
  _get_suffix()->_publish_cc_data( _cc_out, declarations );
}

void 
TyvisIndexedName::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {
  _get_suffix()->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisIndexedName::_publish_cc_range( published_file &_cc_out, PublishData *declarations ){
  _get_suffix()->_publish_cc_range( _cc_out, declarations );
}

void 
TyvisIndexedName::_publish_cc_declarator( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(_get_prefix() != NULL);
  ASSERT(_get_prefix()->is_resolved());

  _get_prefix()->_publish_cc_declarator( _cc_out, declarations );
  if(_get_suffix()->_is_static_expression() == TRUE) {
    _cc_out << "[";
    _get_suffix()->_publish_cc_rvalue( _cc_out , declarations );
    _cc_out << "]";
  }
}

void
TyvisIndexedName::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  Tyvis* name_prefix;
  name_prefix = _get_prefix();
  switch(name_prefix->get_kind()) {
  case IIR_ENTITY_DECLARATION:
    name_prefix->_publish_cc_declarator( _cc_out, declarations );
    _cc_out << "_";
    _get_suffix()->_publish_cc_declarator( _cc_out, declarations );
    break;
  case IIR_SIGNAL_DECLARATION:
    if( _get_current_publish_node() != NULL &&
	dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node()) != NULL ) {
      TyvisConcurrentStatement *tmpStmt = dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node());
      TyvisSignalDeclaration *tmpSignal = dynamic_cast<TyvisSignalDeclaration *>(name_prefix);
      ASSERT ( tmpSignal->_get_declarative_region() != NULL );
      tmpStmt->_publish_cc_scoping_prefix( _cc_out.get_stream(),
					   tmpSignal->_get_declarative_region(),
					   this);
    }
    
    _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
    if ((_get_suffix()->_is_static_expression() == TRUE) ||
	(_get_suffix()->is_constant() == TRUE)) {
      _cc_out << "[";
      _get_suffix()->_publish_cc_array_index( _cc_out, declarations );
      _cc_out << "]";
    }
    break;
  case IIR_SIGNAL_INTERFACE_DECLARATION:
    _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
    if ((_get_suffix()->_is_static_expression() == TRUE) ||
       (_get_suffix()->is_constant() == TRUE)) {
      _cc_out << "[";
      if(_get_suffix()->is_operator() == TRUE) {
	_get_suffix()->_publish_cc_rvalue( _cc_out , declarations );
      }
      else {
	if (_get_suffix()->get_kind() != IIR_ASSOCIATION_LIST) {
	  _get_suffix()->_publish_cc_elaborate( _cc_out, declarations );
	}
	else {
	  _get_suffix()->_publish_cc_for_index( _cc_out, declarations );
	}
      }
      _cc_out << "]";
    }
    break;
  default:
    _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "(";
    _get_suffix()->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << ")";
    break;
  }
}

void
TyvisIndexedName::_publish_cc_subprogram_arg( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(is_resolved());

  _publish_cc_declarator( _cc_out, declarations );
}

void
TyvisIndexedName::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_wait_data( _cc_out, declarations );
  _cc_out << "[";
  _get_suffix()->_publish_cc_wait_data( _cc_out, declarations );
  _cc_out << "]";
}

const string
TyvisIndexedName::_get_cc_tyvis_type(){
  return _get_subtype()->_get_cc_tyvis_type();
}

void
TyvisIndexedName::_add_decl_into_declaration_collection(PublishData *declarations) {
  _get_prefix()->_add_decl_into_declaration_collection(declarations);
  _get_suffix()->_add_decl_into_declaration_collection(declarations);
}  
  
IIR_Mode
TyvisIndexedName::_get_mode() {
  return _get_prefix()->_get_mode();
}

TyvisGenericList *
TyvisIndexedName::_get_generic_list(){
  // This assumes that we have something like entity(arch)
  ASSERT( _get_prefix() != NULL );
  return _get_prefix()->_get_generic_list();
}

IIR_Boolean
TyvisIndexedName::_is_published_attribute_in_state(SignalAttribute attrib) {
  return _get_prefix_declaration()->_is_published_attribute_in_state(attrib);
}

IIR_Boolean
TyvisIndexedName::_is_published_attribute_in_constructor(SignalAttribute attrib) {
  return _get_prefix_declaration()->_is_published_attribute_in_constructor(attrib);
}

IIR_Boolean
TyvisIndexedName::_is_published_attribute_in_initstate(SignalAttribute attrib) {
  return _get_prefix_declaration()->_is_published_attribute_in_initstate(attrib);
}

void
TyvisIndexedName::_add_published_attribute_in_state(SignalAttribute attrib) {
  _get_prefix_declaration()->_add_published_attribute_in_state(attrib);
}

void
TyvisIndexedName::_add_published_attribute_in_constructor(SignalAttribute attrib) {
  _get_prefix_declaration()->_add_published_attribute_in_constructor(attrib);
}
  
void
TyvisIndexedName::_add_published_attribute_in_initstate(SignalAttribute attrib) {
  _get_prefix_declaration()->_add_published_attribute_in_initstate(attrib);
}

TyvisDeclaration*
TyvisIndexedName::_get_prefix_declaration() {
  return _get_prefix()->_get_prefix_declaration();
}

Tyvis *
TyvisIndexedName::_get_suffix(){
  ASSERT( get_suffix() != NULL );

  return dynamic_cast<Tyvis *>(get_suffix());
}

Tyvis *
TyvisIndexedName::_get_prefix(){
  ASSERT( get_prefix() != NULL );

  return dynamic_cast<Tyvis *>(get_prefix());
}

IIR_Boolean 
TyvisIndexedName::_is_globally_static_primary(){
  ASSERT( _get_prefix() != NULL );
  ASSERT( _get_suffix() != NULL );
  ASSERT( is_resolved() == TRUE );

  if( _get_prefix()->_is_globally_static() == TRUE && _get_suffix()->_is_globally_static() ){
    return TRUE;
  }
  else{
    return FALSE;
  }
}

void
TyvisIndexedName::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  if( _get_prefix()->is_signal() == TRUE ){
    if( _get_suffix()->get_kind() == IIR_CONSTANT_DECLARATION &&
	(dynamic_cast<TyvisDeclaration *>(_get_suffix()))->is_implicit_declaration() == TRUE){
      _get_prefix()->_get_list_of_input_signals(list);
    } 
    else {
      TyvisName::_get_list_of_input_signals(list);
    }
  }
}

void
TyvisIndexedName::_set_passed_through_out_port(IIR_Boolean val) {
  ASSERT ( _get_prefix() != NULL );
  _get_prefix()->_set_passed_through_out_port(val);
}

TyvisPortList *
TyvisIndexedName::_get_port_list(){
  // This assumes that we have something like entity(arch)
  ASSERT( _get_prefix() != NULL );
  return _get_prefix()->_get_port_list();
}

IIR_Boolean 
TyvisIndexedName::_is_resolved_signal(){
  ASSERT( is_resolved() == TRUE );
  return ((_get_prefix()->is_signal() == TRUE &&
	   _get_subtype()->_get_resolution_function() != NULL) == TRUE);
}

TyvisDeclaration*
TyvisIndexedName::_get_package_declaration() {
  return _get_prefix()->_get_package_declaration();
}

Tyvis *
TyvisIndexedName::_get_declarative_region(){
  ASSERT( _get_prefix_declaration() != 0 );
  return _get_prefix_declaration()->_get_declarative_region();
}

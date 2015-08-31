
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
#include "TyvisAliasDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIdentifier.hh"
#include "TyvisObjectDeclaration.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

void 
TyvisAliasDeclaration::_publish_cc_elaborate_alias_init( published_file &_cc_out,
							 PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_elaborate_alias_init" );
  if(_get_name()->is_object() && _get_name()->is_signal()) {
    _publish_cc_declarator_with_constructor_args( _cc_out, declarations );
  }
}

void 
TyvisAliasDeclaration::_publish_cc_elaborate_alias_definition( published_file &_cc_out,
							       PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_elaborate_alias_definition" );
  if(_get_name()->is_object() && _get_name()->is_signal()) {
    _publish_cc_declarator_with_constructor_args( _cc_out, declarations );
  }
}

void 
TyvisAliasDeclaration::_publish_cc_constructor_args( published_file &_cc_out,
						     PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_constructor_args" );

  if(_get_name()->is_object() == TRUE) {
    _get_name()->_publish_cc_constructor_args( _cc_out, declarations );
  }
}

void
TyvisAliasDeclaration::_publish_cc_universal_type( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_universal_type" );
  ASSERT ( _get_name() != NULL );
  ASSERT ( _get_name()->_get_subtype() != NULL );
  _get_name()->_get_subtype()->_publish_cc_universal_type( _cc_out );
}

void 
TyvisAliasDeclaration::_publish_cc_elaborate( published_file &_cc_out,
					      PublishData *declarations ){
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_elaborate" );
  if( _get_name()->is_object() ){
    _get_mangled_declarator()->_publish_cc_elaborate( _cc_out, declarations );
  }
}

void
TyvisAliasDeclaration::_publish_cc_type_info_xxx( published_file &_cc_out,
						  PublishData *declarations ){
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_type_info_xxx" );
  if(_get_subtype() != NULL) {
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  }
  else{
    ASSERT( _get_name() != 0 );
    _get_name()->_publish_cc_type_info_xxx( _cc_out, declarations );
  }
}

void 
TyvisAliasDeclaration::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_bounds" );
  
  if(_get_name()->is_object()) {
    if(_get_subtype() != NULL) {
      _get_subtype()->_publish_cc_bounds( _cc_out, declarations );
    }
    else {
      _get_name()->_get_subtype()->_publish_cc_bounds( _cc_out, declarations );
    }  
  }
}

void 
TyvisAliasDeclaration::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_lvalue" );
  ASSERT( get_declarator()->get_kind() == IIR_IDENTIFIER );
  _add_necessary_decls_into_declaration_collection( declarations );

  TyvisDeclaration::_set_scoping_prefix();
  Tyvis::_publish_cc_prefix_string( _cc_out );

  _cc_out << *_get_mangled_declarator();
}

void
TyvisAliasDeclaration::_publish_cc_signal_net_info( published_file &_cc_out,
						    cc_publish_mode mode,
						    PublishData *declarations ){
  ASSERT( _get_name() != 0 );
  ASSERT( dynamic_cast<TyvisDeclaration *>( _get_name() ) != 0 );
  dynamic_cast<TyvisDeclaration *>(_get_name())->_publish_cc_signal_net_info( _cc_out,
									      mode,
									      declarations );
}

void 
TyvisAliasDeclaration::_publish_cc_decl_original_object( published_file &_cc_out,
							 PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_decl_original_object" );
  
  // Check for original object in Scope.  If the original object is not in
  // scope, bring it in scope and publish its declaration.
  if(dynamic_cast<TyvisDeclaration *>(_get_name()) != NULL) {
    if (!declarations->in_collection(dynamic_cast<TyvisDeclaration*>(_get_name()))) {
      declarations->add_declaration(dynamic_cast<TyvisDeclaration*>(_get_name()));
      _get_name()->_publish_cc_decl( _cc_out, declarations );
    }
  }
  else {
    if(_get_name()->is_name()) {
      TyvisDeclaration* declname = _get_name()->_get_prefix_declaration();
      ASSERT(declname != NULL);
      if (!declarations->in_collection(dynamic_cast<TyvisDeclaration*>(declname))) {
	declarations->add_declaration(dynamic_cast<TyvisDeclaration*>(declname));
	declname->_publish_cc_decl( _cc_out , declarations );
      }
    }
  }
}

void 
TyvisAliasDeclaration::_publish_cc_package_decl( published_file &_cc_out,
						 PublishData *declarations ){
  _publish_cc_decl( _cc_out, declarations );
}

void 
TyvisAliasDeclaration::_publish_cc_decl( published_file &_cc_out,
					 PublishData * ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_decl" );
  _publish_cc_include( _cc_out, "tyvis/Alias.hh" );
  _cc_out << "Alias " << *_get_mangled_declarator() << ";" << NL();
}

void 
TyvisAliasDeclaration::_publish_cc_alias_object_init( published_file &_cc_out,
						      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_alias_object_init" );
  if( _get_name()->is_object() ){
    _publish_cc_declarator_with_constructor_args( _cc_out, declarations );
  }
}

void 
TyvisAliasDeclaration::_publish_cc_state_object_init( published_file &_cc_out,
						      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_state_object_init" );
  if(_get_name()->is_object()) {
    _cc_out << *_get_mangled_declarator() << OS("(");
    _publish_cc_bounds(_cc_out, declarations );
    _cc_out << "," << NL();
    _get_name()->_publish_cc_rvalue( _cc_out, declarations );
    _cc_out << CS("),");
  }
}

void
TyvisAliasDeclaration::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisAliasDeclaration::_add_decl_into_declaration_collection( PublishData *declarations ) {
  if(_get_name()->is_object()) {
    _get_name()->_add_decl_into_declaration_collection(declarations);
  }
  if(_get_subtype() != NULL) {
    _get_subtype()->_add_decl_into_declaration_collection(declarations);
  }
}

Tyvis *
TyvisAliasDeclaration::_get_name() {
  return dynamic_cast<Tyvis *>(get_name());
}

void
TyvisAliasDeclaration::_add_declarations_in_initializations( PublishData *declarations ) {
  _add_decl_into_declaration_collection(declarations);
}

void
TyvisAliasDeclaration::_add_necessary_decls_into_declaration_collection( PublishData *declarations ) {
  TyvisAliasDeclaration     *tempAliasDecl  = NULL;
  TyvisDeclaration          *tempDecl       = NULL;

  ASSERT ( declarations != NULL );

  if (_get_name()->get_kind() == IIR_ALIAS_DECLARATION) {
    tempAliasDecl = dynamic_cast<TyvisAliasDeclaration *> (_get_name());
    ASSERT ( tempAliasDecl->get_kind() == IIR_ALIAS_DECLARATION );
    tempAliasDecl->_add_necessary_decls_into_declaration_collection(declarations);
  }
  else {
    tempDecl = dynamic_cast<TyvisDeclaration *> (_get_name());
    if (tempDecl != NULL) {
      if (!declarations->in_collection(tempDecl)) {
	declarations->add_declaration(tempDecl);
      }
    }
    
    if (_get_name()->is_name() == TRUE) {
      tempDecl = dynamic_cast<TyvisDeclaration *> (_get_name()->_get_prefix_declaration());
      ASSERT ( tempDecl != NULL );
      ASSERT ( dynamic_cast<TyvisDeclaration *>(tempDecl) != NULL );
      if (tempDecl->get_kind() == IIR_ALIAS_DECLARATION) {
	tempAliasDecl = dynamic_cast<TyvisAliasDeclaration *>(tempDecl);
	tempAliasDecl->_add_necessary_decls_into_declaration_collection(declarations);
      }

      if (!declarations->in_collection(tempDecl)) {
	declarations->add_declaration(tempDecl);
      }
    }
  }
  
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
}

IIR_Int32
TyvisAliasDeclaration::get_num_indexes( ){
  ASSERT( _get_name()->is_resolved() == TRUE );
  return _get_name()->_get_subtype()->get_num_indexes( );
}

TyvisDeclaration*
TyvisAliasDeclaration::_get_prefix_declaration() {
  return this;
}

void 
TyvisAliasDeclaration::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_name()->_build_sensitivity_list(sensitivity_list);
}

IIR_Boolean 
TyvisAliasDeclaration::_is_globally_static_primary(){
  ASSERT ( _get_name()->is_resolved() == TRUE );
  return _get_name()->_is_globally_static_primary();
}

void 
TyvisAliasDeclaration::_set_driver_addition_flag(IIR_Boolean flag) {
  if(_get_name()->is_signal() == TRUE) {
    TyvisDeclaration* signal_decl = dynamic_cast<TyvisDeclaration*>(_get_name());
    signal_decl->_set_driver_addition_flag(flag);
  }
}

TyvisAttributeSpecificationList* 
TyvisAliasDeclaration::_get_attribute_specification_list() {
  ASSERT(_get_name() != NULL);
  return (dynamic_cast<TyvisDeclaration*>(_get_name())->_get_attribute_specification_list());
}

void
TyvisAliasDeclaration::_publish_cc_decl_with_constructor_args( published_file &_cc_out, 
							       PublishData *declarations ){
  if( _get_name()->is_object() ){
//OLD:    _cc_out << _get_name()->_get_cc_object_type() << " ";
    _publish_cc_include( _cc_out, "tyvis/Alias.hh" );
    _cc_out << "Alias ";
    _cc_out << *_get_mangled_declarator();

    _publish_cc_declarator_with_constructor_args( _cc_out, declarations );
  }
}

void
TyvisAliasDeclaration::_publish_cc_declarator_with_constructor_args( published_file &_cc_out, 
								     PublishData *declarations ){
  CC_REF( _cc_out, "TyvisAliasDeclaration::_publish_cc_declarator_with_constructor_args" );
  _cc_out << OS("(");
  _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  _cc_out << "," << NL();
  _get_name()->_publish_cc_rvalue( _cc_out, declarations );
  if( _get_value() != 0 ){
    _cc_out << "," << NL();
    _get_value()->_publish_cc_rvalue( _cc_out, declarations );
  }
  _cc_out << CS(")");
}

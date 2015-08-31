
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
#include "TyvisAttributeDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFileDeclaration.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisLibraryUnitList.hh"
#include "TyvisLibraryUnit.hh"
#include "TyvisRecordTypeDefinition.hh"
#include "TyvisSelectedName.hh"
#include "TyvisSimpleName.hh"
#include "TyvisTextLiteral.hh"

#include "savant/error_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>
#include "savant/library_manager.hh"
#include "PublishData.hh"

extern bool debug_symbol_table;

void
TyvisSelectedName::_publish_cc_object_name( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_lvalue( _cc_out , declarations );
}

void 
TyvisSelectedName::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSelectedName::_publish_cc_lvalue" );	
  _cc_out << "dynamic_cast<LValue &>(";
  _publish_cc_rvalue( _cc_out, declarations );
  _cc_out << ")";
}

void 
TyvisSelectedName::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSelectedName::_publish_cc_rvalue" );	
  if ((_get_currently_publishing_unit() == PROCESS) ||
      (_get_currently_publishing_unit() == PROCESS_STATE)) {
    if(_get_package_declaration() != NULL) {
      if(!declarations->in_collection(_get_package_declaration())) {
	declarations->add_declaration(_get_package_declaration());
      }
    }
  }

  ASSERT( is_resolved() == TRUE );
  if( _get_prefix_declaration() != NULL && 
      _get_prefix_declaration()->is_object() == TRUE &&
      dynamic_cast<TyvisLabel *>(_get_prefix()) == NULL ){
    _get_prefix()->_publish_cc_lvalue( _cc_out , declarations );    
    if ( dynamic_cast<TyvisAccessTypeDefinition *>(_get_prefix()->_get_subtype()) != NULL )  {
      if (_get_suffix()->get_kind() != IIR_DESIGNATOR_BY_ALL) {
	//DRH extra ) in output _cc_out << ".all())";
	_cc_out << ".all()";
      }
    }    
    _cc_out << ".";
  }

  if ((_get_prefix()->_get_subtype() != NULL) &&
      (_get_prefix()->_get_subtype()->is_record_type() == TRUE)) {
    // Okay... for record types we don't publish the field name, but rather access
    // the field using the field number...
    
    TyvisRecordTypeDefinition *recType = NULL;

    if ( dynamic_cast<TyvisAccessTypeDefinition *>(_get_prefix()->_get_subtype()) == NULL )  {
      recType = dynamic_cast<TyvisRecordTypeDefinition *>( _get_prefix()->_get_subtype() );
    }
    else {
      recType = dynamic_cast<TyvisRecordTypeDefinition *>( ((dynamic_cast<TyvisAccessTypeDefinition *>( _get_prefix()->_get_subtype()))->_get_designated_type()) );
    }

    ASSERT ( recType != NULL );
    ASSERT ( dynamic_cast<TyvisRecordTypeDefinition *>(recType) != NULL );
    _cc_out << "getField(" << recType->_get_field_number(_get_suffix()) << ")";
  }
  else {
    _get_suffix()->_publish_cc_lvalue( _cc_out , declarations );
  }
}

void 
TyvisSelectedName::_publish_cc_declarator( published_file &_cc_out, PublishData *declarations ) {
  if(_get_prefix()->is_signal() == TRUE) {
    _get_prefix()->_publish_cc_declarator( _cc_out, declarations );
  }
  else {
    _get_suffix()->_publish_cc_declarator( _cc_out, declarations );
  }
}

void
TyvisSelectedName::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << ".";
  _get_suffix()->_publish_cc_elaborate( _cc_out, declarations );
}

void
TyvisSelectedName::_publish_cc_state_object_init( published_file &_cc_out,
						  PublishData *declarations ){
  _publish_cc_lvalue( _cc_out , declarations );
  _cc_out << ",";
}

void
TyvisSelectedName::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisSelectedName::_publish_cc_bounds( published_file &_cc_out, PublishData * ) {
  _cc_out << "defaultInfo()";
}

TyvisArchitectureDeclaration *
TyvisSelectedName::_get_cc_instantiated_architecture(){
  return _get_suffix()->_get_cc_instantiated_architecture();
}

TyvisEntityDeclaration *
TyvisSelectedName::_get_cc_instantiated_entity(){
  return _get_suffix()->_get_cc_instantiated_entity();
}

void 
TyvisSelectedName::_add_decl_into_declaration_collection(PublishData *declarations) {
  _get_prefix()->_add_decl_into_declaration_collection(declarations);
  _get_suffix()->_add_decl_into_declaration_collection(declarations);
}

TyvisGenericList *
TyvisSelectedName::_get_generic_list(){
  ASSERT( is_resolved() == TRUE );
  ASSERT( _get_suffix() != NULL );
  return _get_suffix()->_get_generic_list();
}

TyvisDeclaration*
TyvisSelectedName::_get_prefix_declaration() {
  // Since we mangle names, we need not have a selected name.  We just
  // need the object declaration, so return the suffix.
  if( dynamic_cast<TyvisLabel *>(_get_prefix()) != NULL ) {
    ASSERT( dynamic_cast<TyvisDeclaration *>(_get_prefix()) != NULL );
    return dynamic_cast<TyvisDeclaration *>( get_suffix() );
  } 
  else {
    return _get_prefix()->_get_prefix_declaration();
  }
}

Tyvis *
TyvisSelectedName::_get_suffix(){
  ASSERT( get_suffix() != NULL );
  return dynamic_cast<Tyvis *>(get_suffix());
}

IIR_Boolean 
TyvisSelectedName::_is_globally_static(){
  IIR_Boolean retval = TRUE;
  
  if( _get_prefix()->_is_globally_static() == FALSE || 
      _get_suffix()->_is_globally_static() == FALSE ){
    retval = FALSE;
  }

  return retval;
}

void
TyvisSelectedName::_set_passed_through_out_port(IIR_Boolean val) {
  ASSERT ( _get_prefix() != NULL );
  _get_prefix()->_set_passed_through_out_port(val);
}

TyvisPortList *
TyvisSelectedName::_get_port_list(){
  ASSERT( is_resolved() == TRUE );
  ASSERT( _get_suffix() != NULL );
  return _get_suffix()->_get_port_list();
}

TyvisDeclaration*
TyvisSelectedName::_get_package_declaration() {
  if(_get_suffix()->get_kind() == IIR_PACKAGE_DECLARATION) {
    return dynamic_cast<TyvisDeclaration*>( _get_suffix() );
  }
  else {
    return _get_prefix()->_get_package_declaration();
  }
}

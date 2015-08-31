
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

#include "TyvisAttributeDeclaration.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIdentifier.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/error_func.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

TyvisAttributeSpecification::TyvisAttributeSpecification()
{
  set_entity_name_list(new TyvisDesignatorList());
}

TyvisAttributeSpecification::~TyvisAttributeSpecification() {
  //Release the list memory
  delete get_entity_name_list();
}

void 
TyvisAttributeSpecification::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisAttributeSpecification::_publish_cc" );

  //the declarations can be NULL if we are not publishing a process
  //or procedure. So I add it the necessary declarations only when they 
  //appear in a process or procedure
  if(declarations != NULL) {
    if( !declarations->in_collection(this) ){
      declarations->add_declaration(this);
    }
    _get_value()->_add_decl_into_declaration_collection(declarations);
  }
  _get_entity_name_list()->_publish_cc_elements( _cc_out, declarations );
  dynamic_cast<TyvisDesignator *>(get_entity_name_list()->first())->_publish_cc_entity_class( _cc_out );
  _cc_out << "_" << *_get_mangled_declarator() << "Attribute";
}

void
TyvisAttributeSpecification::_publish_cc_package_decl( published_file &_cc_out,
						       PublishData *declarations ){
  _cc_out << "extern ";
  _publish_cc_decl( _cc_out, declarations );
}

void
TyvisAttributeSpecification::_publish_cc_package_definition( published_file &_cc_out,
							     PublishData *declarations ){
  _publish_cc_decl_with_constructor( _cc_out, declarations );
}

void 
TyvisAttributeSpecification::_publish_cc_decl( published_file &_cc_out,
					       PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAttributeSpecification::_publish_cc_decl" );

  ASSERT(_get_declaration() != NULL);
  ASSERT(_get_declaration()->is_resolved() == TRUE);
  //DRH - ensure that Variable is defined when referenced (fix for tc115.vhd failure)
  _get_subtype()->_publish_cc_include( _cc_out , "tyvis/Variable.hh" );
  _cc_out << "Variable ";
  _publish_cc_lvalue( _cc_out, declarations );
  _cc_out << ";" << NL();
}

void 
TyvisAttributeSpecification::_publish_cc_decl_constructor_args( published_file &_cc_out,
								PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisAttributeSpecification::_publish_cc_decl_constructor_args" );

  _cc_out << OS("(");
  _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  _cc_out << "," << NL() 
	  << "\"" << *_get_mangled_declarator() << "\"," << NL();
  _get_value()->_publish_cc_initialization_value( _cc_out, declarations );
  _cc_out << CS(")");
}

void 
TyvisAttributeSpecification::_publish_cc_decl_with_constructor( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisAttributeSpecification::_publish_cc_decl_with_constructor" );

  ASSERT(_get_declaration() != NULL);
  ASSERT(_get_declaration()->is_resolved() == TRUE);
  _cc_out << "Variable ";
  _publish_cc_lvalue( _cc_out, declarations );
  _publish_cc_decl_constructor_args( _cc_out, declarations );
  _cc_out << ";" << NL();
}

void 
TyvisAttributeSpecification::_publish_cc_state_object_init( published_file &_cc_out,
							    PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAttributeSpecification::_publish_cc_state_object_init" );

  _publish_cc_lvalue( _cc_out, declarations );
  _publish_cc_decl_constructor_args( _cc_out, declarations );
  _cc_out << ", ";
}

void
TyvisAttributeSpecification::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisAttributeSpecification::_publish_cc_headers( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisAttributeSpecification::_publish_cc_headers" );

  if( _get_subtype() != NULL ){
    _get_subtype()->_publish_cc_headers( _cc_out );
    _get_subtype()->_publish_cc_include( _cc_out );
  }

  _get_value()->_publish_cc_headers( _cc_out );
}

void 
TyvisAttributeSpecification::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisAttributeSpecification::_publish_cc_bounds" );

  _get_value()->_publish_cc_bounds( _cc_out, declarations );
}

const string 
TyvisAttributeSpecification::_get_cc_value_string(){
  string retval;
  if( _get_value() != 0 ){
    retval = _get_value()->_to_string();
  }
  return retval;
}

Tyvis *
TyvisAttributeSpecification::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_AttributeSpecification::get_value());
}

TyvisDeclaration *
TyvisAttributeSpecification::_get_declaration(){
  return dynamic_cast<TyvisDeclaration *>(get_declaration());
}

IIR_Boolean 
TyvisAttributeSpecification::_is_foreign_attribute_specification( ){
  IIR_Boolean retval = false;
  if( TyvisTextLiteral::cmp( _get_declarator(), "foreign" ) == 0 ){
    retval = true;
  }
  return retval;
}

TyvisDesignatorList *
TyvisAttributeSpecification::_get_entity_name_list() {
  return dynamic_cast<TyvisDesignatorList *>(get_entity_name_list());
}

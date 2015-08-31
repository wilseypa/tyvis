
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

#include "TyvisFunctionDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisAttribute.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisSignalDeclaration.hh"

#include "savant/error_func.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

void
TyvisFunctionDeclaration::_publish_cc_include( published_file &_cc_out ){
  if( _get_declarative_region() != 0 ){
    _get_declarative_region()->_publish_cc_include( _cc_out );
  }
}

void 
TyvisFunctionDeclaration::_publish_cc_bounds( published_file &_cc_out,
					      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_bounds" );
  _get_return_type()->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisFunctionDeclaration::_publish_cc_necessary_copying( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_necessary_copying" );;
  if (is_implicit_declaration() == TRUE) {
    ASSERT ( _get_attribute_name() != NULL );
    _get_attribute_name()->_publish_cc_necessary_copying( _cc_out );
  }
}

void
TyvisFunctionDeclaration::_publish_cc_read_or_write( published_file &_cc_out,
						     const string &functionName, 
						     const string &streamName ) {

  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_read_or_write" );

  if (is_implicit_declaration() == TRUE) {
    ASSERT ( _get_attribute_name()  != NULL );
    _get_attribute_name()->_publish_cc_read_or_write( _cc_out, functionName, streamName );
  }
}

void
TyvisFunctionDeclaration::_publish_cc_decl( published_file &(_cc_out),
					    PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_decl" );
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  if( is_implicit_declaration() ){
    _get_attribute_name()->_publish_cc_necessary_decl_in_state( _cc_out );
    return;
  }
  _publish_cc_subprogram_body( _cc_out, declarations );
}

void
TyvisFunctionDeclaration::_publish_cc_type_conversion_function_name( published_file &_cc_out,
								     PublishData * ){
  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_type_conversion_function_name" );

  _cc_out << *_get_mangled_declarator();
/*DNS: this does not match name of generated function!
  _cc_out << "Type";
  _cc_out << dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->first())->_get_subtype()->_get_cc_type_name();
  _cc_out << "Ret";
  _cc_out << _get_return_type()->_get_cc_type_name();
  _cc_out << "_TCF";
*/
}

void
TyvisFunctionDeclaration::_publish_cc_binding_name(ostream &os) {
  os << *_get_mangled_declarator();
}

void
TyvisFunctionDeclaration::_publish_cc_type_conversion_function( published_file &_cc_out,
								PublishData *declarations ){
  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_type_conversion_function" );

  _cc_out.insert_comment( "This is the type conversion function definition definition" );
  _cc_out << "Value " << NL();
  _publish_cc_type_conversion_function_name( _cc_out, declarations );
  _cc_out << OS("(") << "VHDLProcess *processPtr," << NL()
	  << "RValue *savantvalue" << CS(")") << OS("{");
  dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->first())->_publish_cc_decl_with_constructor_args( _cc_out, declarations );
  _cc_out << ";";
  _cc_out << *dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->first())->_get_mangled_declarator()
	  << ".updateVal( savantvalue->readVal() );" << NL();

  // Publish any declarations in this function 
  _publish_cc_declarations( _cc_out, declarations );
  // Publish the body of the function.  
  _get_subprogram_body()->_publish_cc( _cc_out, declarations );
  _cc_out << CS("}");
}

void
TyvisFunctionDeclaration::_publish_cc_init_function( published_file &_cc_out,
						     PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_init_function" );

  ASSERT(is_implicit_declaration() == TRUE);
  if(is_implicit_declaration() == TRUE) {
    _get_attribute_name()->_publish_cc_init( _cc_out, declarations );
  }

}

void
TyvisFunctionDeclaration::_publish_cc_state_object_init( published_file &_cc_out,
							 PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_state_object_init" );

  if( is_implicit_declaration() == TRUE) {
    _get_attribute_name()->_publish_cc_state_object_init( _cc_out, declarations );
  }  

}  

void
TyvisFunctionDeclaration::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisFunctionDeclaration::_publish_cc_wait_data( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_wait_data" );
  _cc_out << *_get_mangled_declarator();
}

// Mangling of functions and procedures is done here as a constnat string
// "savant" is added as a suffix to the function name.
//
// This cannot be done at the subprogram level as for functions, the
// return type is added a prefix while for procedures it is not.

string
TyvisFunctionDeclaration::_mangle_declarator() {
  ostringstream newMangledDeclarator;
  TyvisInterfaceDeclaration *interface_element;

  IIR_Boolean _is_operator = is_operator();

  if( _is_operator == TRUE && is_implicit_declaration() == FALSE ){
    _is_operator = FALSE;
  }
  
  if( _get_declarative_region() != NULL && 
      get_kind() != IIR_LIBRARY_DECLARATION && 
      _is_operator == FALSE ){
    newMangledDeclarator << *(_get_declarative_region()->_get_mangled_declarator());
    newMangledDeclarator << "_";
  }

  if( dynamic_cast<TyvisStringLiteral *>(get_declarator()) ){
    IIR_Boolean _is_unary_operator = FALSE;
    if( get_interface_declarations()->size() == 1 ){
      _is_unary_operator = TRUE;
    }
    newMangledDeclarator << dynamic_cast<TyvisStringLiteral *>(_get_declarator())->_convert_function_name(_is_unary_operator);
  } 
  else {
    newMangledDeclarator << *_get_declarator();
  }
  
  //Mangling with the argument types
  if( _is_operator == FALSE ){
    if(get_interface_declarations()->size() >= 1) {
      interface_element = dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->first());
      while( interface_element != NULL ){
	ASSERT( interface_element->_get_subtype() != NULL );
	if( interface_element->_get_subtype()->_get_declaration() == NULL ){
	  if( interface_element->_get_subtype()->_get_bottom_base_type() != NULL &&
	      interface_element->_get_subtype()->_get_bottom_base_type()->_get_declaration() != NULL){
	    newMangledDeclarator << 
	      *(interface_element->_get_subtype()->_get_bottom_base_type()->_get_declaration()->_get_declarator());
	  }
	}
	else {
	  newMangledDeclarator <<*(interface_element->_get_subtype()->_get_declaration()->_get_declarator());
	}
	interface_element = dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->successor(interface_element));
      }
    }
  }
  
  if( !_is_operator ){
    newMangledDeclarator << "_" << *_get_return_type()->_get_mangled_declarator();
  }

  return newMangledDeclarator.str();
}

TyvisDeclarationList *
TyvisFunctionDeclaration::_get_subprogram_declarations() {
  return dynamic_cast<TyvisDeclarationList *>(get_subprogram_declarations());
}

IIR_Int32
TyvisFunctionDeclaration::get_num_indexes(){
  return _get_return_type()->get_num_indexes();
}

IIR_Boolean
TyvisFunctionDeclaration::_is_possible_resolution_function() {
  TyvisInterfaceDeclaration *defn = dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->first());
  //Conditions 1 & 2 checked in first if
  if(_num_subprogram_args() == 1 && defn->is_constant() && defn->_get_subtype()->is_unconstrained_array_type()  ) {
    //condition 3.
    TyvisDeclaration* return_decl = _get_return_type()->_get_declaration();
    TyvisDeclaration* element_decl = (dynamic_cast<TyvisArrayTypeDefinition *>(defn->_get_subtype()))->_get_element_subtype()->_get_declaration();
    if(return_decl == element_decl) {
      return TRUE;
    }
  }
  return FALSE;
}

IIR_Boolean
TyvisFunctionDeclaration::_is_possible_type_conversion_function() {
  if(_num_subprogram_args() == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

TyvisTypeDefinition* 
TyvisFunctionDeclaration::_get_return_type() {
  return dynamic_cast<TyvisTypeDefinition *>(get_return_type());
}

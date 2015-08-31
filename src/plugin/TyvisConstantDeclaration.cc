
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

#include "TyvisConstantDeclaration.hh"
#include "TyvisAttribute.hh"
#include "TyvisConcurrentGenerateForStatement.hh"
#include "TyvisDesignFile.hh"
#include "TyvisLabel.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisTextLiteral.hh"

#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"
#include <clutils/StringUtilities.h>

string
TyvisConstantDeclaration::_mangle_declarator(){
  // Here, we mangle the constant's declarator with it's line number to
  // make it unique.  This is not required in all the cases, however, we
  // have to mangle the iterator in a for loop statement since we could
  // have two for loops with the same declarator.
  string mangledDeclarator = TyvisDeclaration::_mangle_declarator();
  if( is_implicit_declaration() ) {
    mangledDeclarator +=( "_L_" + intToString( get_line_number() ) );
  }
  return mangledDeclarator;
}

void 
TyvisConstantDeclaration::_publish_cc_elaborate( published_file &_cc_out,
						 PublishData *declarations ){
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_elaborate" );
  if ( IIRBase_ConstantDeclaration::is_implicit_declaration() == TRUE ) {
    _cc_out << *_get_mangled_declarator();
  }
  else {
    // This ASSERT is not entirely correct but is in place to catch
    // probable errors.
    ASSERT(_get_value() != NULL);
    _get_value()->_publish_cc_rvalue( _cc_out, declarations );
  }
}

void
TyvisConstantDeclaration::_publish_cc_lvalue( published_file &_cc_out,
					      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_lvalue" );

  _set_scoping_prefix();

  if ((_get_declarative_region()->get_kind() != IIR_ENTITY_DECLARATION) &&
      (_get_declarative_region()->get_kind() != IIR_PACKAGE_DECLARATION) &&
      (_get_declarative_region()->get_kind() != IIR_ARCHITECTURE_DECLARATION)){
    Tyvis::_publish_cc_prefix_string( _cc_out );
  }
  
  TyvisObjectDeclaration::_publish_cc_lvalue( _cc_out, declarations );

  if (get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION
      || get_declarative_region()->get_kind() == IIR_ARCHITECTURE_DECLARATION) {
    _cc_out << "Func()";
  }

  // The constant declaration has to be added to the cgen symbol table
  // after the constants in it's value part have been added.  This makes
  // sure that during C++ publishing, the values used to initialize this
  // constant are added before this.
  // WARNING: Temporary fix: Should not reach here if implicit declaration.


  // cgen_sum_tab_ptr may be NULL during elaboration, such as when the
  // index constraint contains a const. decl.
  if( declarations != NULL ){
    if( _get_value() != NULL) {
      _get_value()->_add_decl_into_declaration_collection(declarations);
    }
    if( !declarations->in_collection(this) ){
      declarations->add_declaration(this);
    }
  }
  TyvisDeclaration::_reset_scoping_prefix();
}

void 
TyvisConstantDeclaration::_publish_cc_init( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_init" );

  if( IIRBase_ConstantDeclaration::is_implicit_declaration() ){
    if( _get_declarative_region()  && 
	_get_declarative_region()->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT ) {
      (dynamic_cast<TyvisConcurrentGenerateForStatement *>(_get_declarative_region()))->_publish_cc_init( _cc_out, _get_current_publish_node(), declarations);
    }
    return;
  }
}

void
TyvisConstantDeclaration::_publish_cc_constant_object_init( published_file &_cc_out,
							    PublishData *declarations ){

  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_constant_object_init" );

  if(_get_subtype()->get_kind() == IIR_ACCESS_TYPE_DEFINITION ||
     _get_subtype()->get_kind() == IIR_ACCESS_SUBTYPE_DEFINITION) {
    return;
  }
  _publish_cc_lvalue( _cc_out, declarations );
  _publish_cc_constructor_args( _cc_out, declarations );
}

void
TyvisConstantDeclaration::_publish_cc_locally_static_value( published_file &_cc_out,
							    PublishData *declarations ){
  if( _get_value() != 0 ){
    CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_locally_static_value" );
    _get_value()->_publish_cc_rvalue( _cc_out, declarations );
  }
  else{
    CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_locally_static_value" );
    ASSERT( _get_subtype() != 0 );
    _get_subtype()->_publish_cc_init_val( _cc_out, declarations );
  }
}

void
TyvisConstantDeclaration::_publish_cc_initialization_value( published_file &_cc_out,
							    PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_initialization_value" );

  _publish_cc_lvalue( _cc_out, declarations );
}

void
TyvisConstantDeclaration::_publish_cc_data( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_data" );

  _publish_cc_prefix_string( _cc_out );
  _publish_cc_lvalue( _cc_out, declarations );
}

void
TyvisConstantDeclaration::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_wait_data" );

  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  _cc_out << "s->";
  _publish_cc_lvalue( _cc_out, declarations );
}

void
TyvisConstantDeclaration::_publish_cc_universal_value( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_universal_value" );
  _publish_cc_lvalue( _cc_out, declarations );
}

void
TyvisConstantDeclaration::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_value" );

  _publish_cc_universal_value( _cc_out, declarations );
}

const string
TyvisConstantDeclaration::_get_cc_object_type() {
  return "Variable";
}

void
TyvisConstantDeclaration::_publish_cc_ams_function( published_file &_cc_out,
						    PublishData *declarations) {
  _get_value()->_publish_cc_ams_function(_cc_out, declarations);  
}

void
TyvisConstantDeclaration::_publish_cc_package_decl( published_file &_cc_out,
						    PublishData *declarations ){
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_package_decl" );
  if( get_subtype()->is_locally_static() ){
     // Need to publish forward reference for some TypeInfos used in declarations
     _publish_cc_static_type_info_fn( _cc_out, declarations );
  }
  // Need to define deferred constant in PACKAGE BODY, declared extern in PACKAGE
  if(_get_declarative_region()->get_kind() == IIR_PACKAGE_BODY_DECLARATION ||
		  get_value() != NULL ){
  _cc_out << "const ";
  _publish_cc_decl_with_constructor_args( _cc_out, declarations );
  } 
  else{ 
    _cc_out << "extern const ";
    _cc_out << _get_cc_object_type() <<  NL();
    _cc_out << *_get_mangled_declarator() << ";" << NL();
  }  
}

void
TyvisConstantDeclaration::_publish_cc_package_definition( published_file &_cc_out,
							  PublishData *declarations ){
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_package_definition" );

  ASSERT(_get_declarative_region()->get_kind() == IIR_PACKAGE_DECLARATION ||
	 _get_declarative_region()->get_kind() == IIR_PACKAGE_BODY_DECLARATION);
    _publish_cc_decl_with_constructor_args( _cc_out, declarations );
}


void
TyvisConstantDeclaration::_publish_cc_decl_with_constructor_args( published_file &_cc_out,
								  PublishData *declarations ){
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_decl_with_constructor_args" );

  if (get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION
      || get_declarative_region()->get_kind() == IIR_ARCHITECTURE_DECLARATION) {
    // Constant declarations published as globals need to be wrapped in functions that
    // perform initialization. This avoids the "static initialization fiasco" wherein dependent
    // global variables are not initialized in the correct order.
    // Note that this does not solve the problem of destructor interdependence in global variables.
    _cc_out << _get_cc_object_type() << "& " << *_get_mangled_declarator() << "Func() {" << NL();
    _cc_out << "static ";
  }
  TyvisObjectDeclaration::_publish_cc_decl_with_constructor_args( _cc_out, declarations );
  _cc_out << ";" << NL();
  if (get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION
      || get_declarative_region()->get_kind() == IIR_ARCHITECTURE_DECLARATION) {
    _cc_out << "return " << *_get_mangled_declarator() << ";" << NL();
    _cc_out << "}" << NL();
  }
}

void
TyvisConstantDeclaration::_publish_cc_declarator_with_constructor_args( published_file &_cc_out,
								      PublishData *declarations ){
  CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_with_constructor_args" );
  if (get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION
      || get_declarative_region()->get_kind() == IIR_ARCHITECTURE_DECLARATION) {
    _cc_out << *_get_mangled_declarator();
  } else {
    _publish_cc_lvalue( _cc_out , declarations );
  }
  _publish_cc_constructor_args( _cc_out, declarations );
}

void
TyvisConstantDeclaration::_publish_cc_state_object_init( published_file &_cc_out,
							 PublishData *declarations ){
  if ( _get_declarative_region()->get_kind() != IIR_PROCESS_STATEMENT &&
       _get_declarative_region()->get_kind() != IIR_ARCHITECTURE_DECLARATION &&
       _get_declarative_region()->get_kind() != IIR_PACKAGE_DECLARATION &&
       _get_declarative_region()->get_kind() != IIR_PACKAGE_BODY_DECLARATION ){
    CC_REF( _cc_out, "TyvisConstantDeclaration::_publish_cc_state_object_init" );
    TyvisObjectDeclaration::_publish_cc_state_object_init( _cc_out, declarations );
  }
}

void
TyvisConstantDeclaration::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisConstantDeclaration::_set_scoping_prefix() {
  ostringstream prefix;
  TyvisProcessStatement *process_ptr = _get_current_process_statement();
  
  if (process_ptr == NULL) {
    return;
  }

  if((_get_currently_publishing_unit() != Tyvis::PROCEDURE) &&
     (_get_currently_publishing_unit() != Tyvis::FUNCTION) &&
     (_get_currently_publishing_unit() != PROCESS)) {
    _set_publish_prefix_string( "" );
    return;
  }
  
  if ( _get_currently_publishing_unit() == PROCESS &&
       _get_declarative_region() == _get_current_process_statement() ){
    _set_publish_prefix_string( "" );
    return;
  }
  
  switch (_get_declarative_region()->get_kind()) {
  case IIR_BLOCK_STATEMENT:
  case IIR_ENTITY_DECLARATION:
  case IIR_ARCHITECTURE_DECLARATION:
    if (_get_currently_publishing_unit() == PROCESS) {
      prefix << "(("
	     << process_ptr->_get_declarative_region()->_get_cc_elaboration_class_name()
	     << " *) getProc())->";
    }
    else {
      prefix << "((" << *(process_ptr->_get_mangled_label()) << " *) processPtr)->";
    }
    
    process_ptr->_publish_cc_scoping_prefix( prefix, 
					     _get_declarative_region(), 
					     process_ptr );
    break;

  case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
  case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    if (_get_currently_publishing_unit() == PROCESS) {
      prefix << "currentState->";
    }
    else {
      prefix << "((" << *(process_ptr->_get_mangled_label()) << "_state *) processPtr->getState())->";
    }
    break;
    
    
  case IIR_PROCESS_STATEMENT:
    prefix << "((" << *(process_ptr->_get_mangled_label()) << " *) processPtr)->";
    break;

  case IIR_FOR_LOOP_STATEMENT:
    if (_is_currently_publishing_subprogram() == TRUE) {
      break;
    }
    if (_get_currently_publishing_unit() == PROCESS) {
      prefix << "currentState->";
      break;
    }
    
  case IIR_PACKAGE_DECLARATION:
    // Available globally.
    break;
    
  default:
    cerr << "Error - TyvisConstantDeclaration::_set_scoping_prefix() :"
	 << " unhandled case."  << endl;
    abort();
  }
  _set_publish_prefix_string( prefix.str() );
}

Tyvis*
TyvisConstantDeclaration::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_ConstantDeclaration::get_value());
}

IIR_Boolean
TyvisConstantDeclaration::_is_static_expression() {
  if (_get_declarative_region()->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT) {
    return TRUE;
  }
  
  return (!IIRBase_ConstantDeclaration::is_implicit_declaration());
}

IIR_Boolean 
TyvisConstantDeclaration::_is_globally_static_primary(){
  // Inferred from rules given in c), d), and e), pg 109 of the LRM.
  return TRUE;
}

void
TyvisConstantDeclaration::_set_stmt_node_index(IIR_Int32 *index, bool _is_right_child, bool &reducibleFlag) {
   _get_value()->_set_stmt_node_index(index, _is_right_child, reducibleFlag);
}

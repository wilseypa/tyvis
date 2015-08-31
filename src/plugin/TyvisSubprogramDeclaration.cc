
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

#include "TyvisAliasDeclaration.hh"
#include "TyvisAttribute.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisLabel.hh"
#include "TyvisProcedureDeclaration.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisReturnStatement.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisSubtypeDeclaration.hh"
#include "savant/resolution_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include "PublishData.hh"

TyvisSubprogramDeclaration::TyvisSubprogramDeclaration(){
  set_interface_declarations(new TyvisInterfaceList());
  set_subprogram_declarations(new TyvisDeclarationList());
  set_subprogram_body(new TyvisSequentialStatementList());
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisSubprogramDeclaration::~TyvisSubprogramDeclaration() {
  //Release the list memory
  delete get_interface_declarations();
  delete get_subprogram_declarations();
  delete get_subprogram_body();
  delete get_attributes();
}

void
TyvisSubprogramDeclaration::_publish_cc_rvalue( published_file &_cc_out,
						PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisFunctionDeclaration::_publish_cc_rvalue" );
  
  ASSERT( _get_declarator()->get_kind() == IIR_IDENTIFIER ||
	  _get_declarator()->get_kind() == IIR_STRING_LITERAL );
  
  if( declarations != NULL && !declarations->in_collection(this) ){
      declarations->add_declaration(this);
  }
  if( is_implicit_declaration() && _get_attribute_name() != NULL ){
    _get_attribute_name()->_publish_cc_lvalue( _cc_out , declarations );
  }
  else{
    _publish_cc_subprogram_call_name( _cc_out, declarations );
  }
}

void 
TyvisSubprogramDeclaration::_publish_cc_include( published_file &_cc_out ){
  // First the default includes.
  if( contains_body() ){
    // The types used in the interface declarations.
    TyvisInterfaceDeclaration *current_decl =
      dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->first());
    while (current_decl != NULL) {
      switch(current_decl->get_kind()) {
      case IIR_SIGNAL_INTERFACE_DECLARATION:
      case IIR_VARIABLE_INTERFACE_DECLARATION:
      case IIR_CONSTANT_INTERFACE_DECLARATION:
      case IIR_TYPE_DECLARATION:
      case IIR_SUBTYPE_DECLARATION:
	current_decl->_publish_cc_include( _cc_out );
	break;
      case IIR_FILE_INTERFACE_DECLARATION:
	// Do Nothing!!
	break;
      default:
	cerr << "TyvisSubprogramDeclaration.cc_publish_cc_headers( published_file &_cc_out ):"
	     << " Unknown declaration type: " 
	     << current_decl->get_kind_text() << endl;
	break;
      }
      current_decl =
	dynamic_cast<TyvisInterfaceDeclaration *>
	(get_interface_declarations()->successor(current_decl));
    }
  }
}

void 
TyvisSubprogramDeclaration::_publish_cc_declarations( published_file &_cc_out,
						      PublishData *declarations ) {
  TyvisDeclaration *decl =
    dynamic_cast<TyvisDeclaration *>(get_subprogram_declarations()->first());
  CC_REF( _cc_out, "TyvisSubprogramDeclaration::_publish_cc_declarations" );

  while (decl != NULL) {
    // Some publishing changes the output file, so to return to this
    // file, we have to save it and restore it here.
    CC_REF( _cc_out, "TyvisSubprogramDeclaration::_publish_cc_declarations" );
    switch(decl->get_kind()) {
    case IIR_TYPE_DECLARATION:
    case IIR_SUBTYPE_DECLARATION:
      // These have been published earlier.  Nothing to be done here.
      break;
    case IIR_SIGNAL_DECLARATION:
    case IIR_VARIABLE_DECLARATION:
    case IIR_CONSTANT_DECLARATION:
    case IIR_ALIAS_DECLARATION:
    case IIR_FILE_DECLARATION:
      decl->_publish_cc_decl_with_constructor_args( _cc_out , declarations );
      _cc_out << ";" << NL();
      break;
    case IIR_ATTRIBUTE_SPECIFICATION:
      (dynamic_cast<TyvisAttributeSpecification *>(decl))->_publish_cc_decl_with_constructor( _cc_out, declarations );
      break;
    case IIR_ATTRIBUTE_DECLARATION:
    case IIR_FILE_INTERFACE_DECLARATION:
    case IIR_FUNCTION_DECLARATION: 
    case IIR_PROCEDURE_DECLARATION:
      break;
    default:
      cerr << "TyvisSubprogramDeclaration.cc_publish_cc_declarations( published_file &_cc_out, PublishData *declarations ):"
	   << " Unknown declaration type: " << decl->get_kind_text() << endl;
      break;
    }
    decl = dynamic_cast<TyvisDeclaration *>(get_subprogram_declarations()->successor(decl));
  }

  // After the declarations are over, we publish the identifiers necessary
  // for th for loop statements in the body.
  _get_subprogram_body()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
}

void
TyvisSubprogramDeclaration::_publish_cc_package_decl( published_file &_cc_out,
						      PublishData *declarations ){
  _publish_cc_prototype( _cc_out, declarations );
}

void
TyvisSubprogramDeclaration::_publish_cc_package_definition( published_file &_cc_out,
							    PublishData *declarations ){
  _publish_cc_decl( _cc_out, declarations );
}

void 
TyvisSubprogramDeclaration::_publish_cc_function_prototype( published_file &_cc_out,
							    PublishData *declarations,
							    bool publish_semicolon ){
  _publish_cc_include( _cc_out, "tyvis/Value.hh" );
  if( _get_return_type() ){
    _cc_out << "Value" << NL();
  }
  else {
    _cc_out << "int" << NL();
  }
  _cc_out << *_get_mangled_declarator() << OS("(VHDLProcess *processPtr");
  if( get_interface_declarations()->size() != 0 ){
    _cc_out << "," << NL();
    _get_interface_declarations()->_publish_cc_decl_subprogram_args( _cc_out, declarations );
  }
  string close(")");
  if( publish_semicolon ){
    close += ";";
  }
  _cc_out << CS(close);
}

void 
TyvisSubprogramDeclaration::_publish_cc_prototype( published_file &_cc_out,
						   PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSubprogramDeclaration::_publish_cc_prototype" );

  _publish_cc_savantnow_process_ptr( _cc_out );
  _publish_cc_function_prototype( _cc_out, declarations, true );
  if( _is_possible_type_conversion_function() ){
    // Publish the prototype of the function, as a type conversion
    // function.
    _cc_out << "Value " << NL();
    _publish_cc_type_conversion_function_name( _cc_out, declarations );
    _cc_out << "( VHDLProcess *, RValue * );" << NL();
  }
    
  _publish_cc_savantnow_no_process_ptr( _cc_out );
  _get_subprogram_declarations()->_publish_cc_prototype( _cc_out, declarations );
}

void 
TyvisSubprogramDeclaration::_publish_cc_subprogram_body( published_file &_cc_out,
							 PublishData *declarations ){
  if( contains_body() ){
    CC_REF( _cc_out, "TyvisSubprogramDeclaration::_publish_cc_decl" );
    ASSERT(declarations != NULL);
    _get_subprogram_declarations()->_publish_cc_decl_subprograms_and_types( _cc_out,
									    declarations );
    PublishedUnit _saved_publishing_unit = _get_currently_publishing_unit();
    if( is_procedure_declaration() ){
      _set_currently_publishing_unit(Tyvis::PROCEDURE);
    }
    else{
      _set_currently_publishing_unit(Tyvis::FUNCTION);
    }

    // Redefine savantnow for a subprogram.
    _publish_cc_savantnow_process_ptr( _cc_out );
    // Publish headers required by subprogram. TypeInfos should be defined w/in headers
    _get_subprogram_declarations()->_publish_cc_include( _cc_out );
    _publish_cc_function_prototype( _cc_out, declarations, false );
    _cc_out << OS("{");
    _get_interface_declarations()->_publish_cc_decl_subprogram_arg_vars( _cc_out, declarations );
    _publish_cc_define_current_state( _cc_out );
    // Publish any declarations in this subprogram 
    _publish_cc_declarations( _cc_out, declarations );
    // Publish initialization for wait statement (in procedure) implementation
    if( is_procedure_declaration() ){
      (dynamic_cast<TyvisProcedureDeclaration *>(this))->_publish_cc_procedure_stuff( _cc_out, declarations );
    }
    // Publish the body of the subprogram.  
    _set_publish_prefix_string( "" );
    _get_subprogram_body()->_publish_cc( _cc_out, declarations );
    if( is_procedure_declaration() ){
      _cc_out << "return NORMAL_RETURN;" << NL();
    }
    _cc_out << CS("}");
    
    // Restore the definition of savantnow
    _publish_cc_savantnow_no_process_ptr( _cc_out );
    _set_currently_publishing_unit(_saved_publishing_unit);
  }
}

void
TyvisSubprogramDeclaration::_publish_cc_type_conversion_function_name( published_file &,
								       PublishData * ){
  _report_undefined_fn("_publish_cc_type_conversion_function_name( _cc_out, declarations )");
}

void
TyvisSubprogramDeclaration::_publish_cc_implicit_file_close( published_file &_cc_out,
							     PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSubprogramDeclaration::_publish_cc_implicit_file_close" );
  TyvisDeclaration *decl = 
    dynamic_cast<TyvisDeclaration *>(_get_subprogram_declarations()->first());
  while (decl != NULL) {
    if( decl->is_file() ){
      CC_REF( _cc_out, "TyvisSubprogramDeclaration::_publish_cc_implicit_file_close" );
      _cc_out << "savantfile_close(processPtr, ";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ");" << NL();
    }
    decl = 
      dynamic_cast<TyvisDeclaration *>(_get_subprogram_declarations()->successor(decl));
  }
}

void
TyvisSubprogramDeclaration::_publish_cc_define_current_state(published_file &_cc_out) {
  // We will redefine state only if this subprogram has been
  // declared within the declarative region of a process statement..

  ASSERT ( _get_declarative_region() != NULL );
  
  if (_get_declarative_region()->get_kind() == IIR_PROCESS_STATEMENT) {
    TyvisProcessStatement *ps = dynamic_cast<TyvisProcessStatement *>( _get_declarative_region() );
    _cc_out << "  " << *(ps->_get_mangled_label()) << "_state *currentState = static_cast<"
	    << *(ps->_get_mangled_label()) << "_state *>(processPtr->getState())";
    _cc_out.end_statement();
  }
}

IIR_Boolean
TyvisSubprogramDeclaration::_is_implicit_file_operation(){
  return( is_implicit_declaration() &&
	  ((IIRBase_TextLiteral::cmp( get_declarator(),
				      "file_open" ) == 0 ) ||
	   (IIRBase_TextLiteral::cmp( get_declarator(),
				      "file_close" ) == 0 ) ||
	   (IIRBase_TextLiteral::cmp( get_declarator(),
				      "read" ) == 0 ) ||
	   (IIRBase_TextLiteral::cmp( get_declarator(),
				      "write" ) == 0 ) ||
	   (IIRBase_TextLiteral::cmp( get_declarator(),
				      "endfile" ) == 0 )) );
}

IIR_Boolean
TyvisSubprogramDeclaration::_is_implicit_operation(){
  return( is_implicit_declaration() &&
	  ((IIRBase_TextLiteral::cmp( get_declarator(),
				      "deallocate" ) == 0 )) );
}

void
TyvisSubprogramDeclaration::_publish_cc_subprogram_call_name( published_file &_cc_out,
							      PublishData * ){
  if( _is_implicit_file_operation() ){
    _publish_cc_implicit_file_operation_call_name( _cc_out );
  }else if( _is_implicit_operation() ){
    _publish_cc_implicit_operation_call_name( _cc_out );
  }else{
    _cc_out << *_get_mangled_declarator();
  }
}

void
TyvisSubprogramDeclaration::_publish_cc_implicit_file_operation_call_name( published_file &_cc_out ){
  _cc_out << "savant" << *get_declarator();
}

void 
TyvisSubprogramDeclaration::_publish_cc_implicit_operation_call_name( published_file &_cc_out ){
  if((IIRBase_TextLiteral::cmp( get_declarator(), "deallocate" ) == 0 )){	
    _cc_out << "savant" << *get_declarator();
  }
}

TyvisInterfaceList *
TyvisSubprogramDeclaration::_get_interface_declarations() {
  return dynamic_cast<TyvisInterfaceList *>(get_interface_declarations());
}

TyvisSequentialStatementList *
TyvisSubprogramDeclaration::_get_subprogram_body() {
  return dynamic_cast<TyvisSequentialStatementList *>(get_subprogram_body());
}

TyvisDeclarationList *
TyvisSubprogramDeclaration::_get_subprogram_declarations() {
  return dynamic_cast<TyvisDeclarationList *>(get_subprogram_declarations());
}

TyvisTypeDefinition* 
TyvisSubprogramDeclaration::_get_return_type() {
  return NULL;
}

IIR_Int32
TyvisSubprogramDeclaration::_num_subprogram_args(){
  return get_interface_declarations()->size();
}

TyvisAttributeSpecificationList* 
TyvisSubprogramDeclaration::_get_attribute_specification_list() {
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

void 
TyvisSubprogramDeclaration::_publish_cc_static_type_info_fn( published_file &_cc_out,
						  PublishData *declarations ){
  CC_REF( _cc_out, "TyvisSubprogramDeclaration::_publish_cc_type_info" );
  _get_interface_declarations()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_subprogram_declarations()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_subprogram_body()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}


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

#include "TyvisAttributeSpecificationList.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisAccessTypeDefinition.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

string
TyvisInterfaceDeclaration::_get_cc_argument_name(){
  string declarator = _get_mangled_declarator()->convert_to_string();
  return "_" + declarator;
}


void 
TyvisInterfaceDeclaration::_publish_cc_decl_subprogram_arg( published_file &_cc_out, 
                                                            PublishData *declarations){

  CC_REF( _cc_out, "TyvisInterfaceDeclaration::_publish_cc_decl_subprogram_arg" );

  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }

  if( get_mode() == IIR_IN_MODE && !is_signal() ){
    _cc_out << "const RValue &" << _get_cc_argument_name();
  }
  else{
    _cc_out << "LValue &" << *_get_mangled_declarator();
  }
}

void 
TyvisInterfaceDeclaration::_publish_cc_subprogram_arg_var( published_file &_cc_out, 
							   PublishData *declarations ){

  if( get_mode() == IIR_IN_MODE && !is_signal() ){  
    CC_REF( _cc_out, "TyvisInterfaceDeclaration::_publish_cc_subprogram_arg_var" );
    _get_subtype()->_publish_cc_universal_type( _cc_out );
    _cc_out << " " << *_get_mangled_declarator() <<  OS("(");
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "," << NL()
	    << _get_cc_argument_name()
	    << CS(")");
  }
}

void
TyvisInterfaceDeclaration::_publish_cc_bounds( published_file &_cc_out,
					       PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisInterfaceDeclaration::_publish_cc_bounds" );
  ASSERT(_get_subtype() != NULL);
  _get_subtype()->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisInterfaceDeclaration::_publish_cc_universal_type( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisInterfaceDeclaration::_publish_cc_universal_type" );
  ASSERT(_get_subtype() != NULL);
  _get_subtype()->_publish_cc_universal_type( _cc_out );
}

void
TyvisInterfaceDeclaration::_publish_cc_value( published_file &_cc_out,
					      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisInterfaceDeclaration::_publish_cc_value" );
  _publish_cc_rvalue( _cc_out, declarations );
  // In case this is a constant Interface then we have a pointer in case
  // its a generate for statement
  if ( get_kind() == IIR_CONSTANT_INTERFACE_DECLARATION && 
       _get_currently_publishing_unit() == Tyvis::GENERATE_FOR) {
    _cc_out << "->";
  } else {
    _cc_out << ".";
  }
  _cc_out << "getIntValue()";
}

void 
TyvisInterfaceDeclaration::_add_decl_into_cgen_symbol_table(PublishData *declarations) {
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  if(_get_value() != NULL) {
    _get_value()->_add_decl_into_declaration_collection(declarations);
  }
}

Tyvis *
TyvisInterfaceDeclaration::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_InterfaceDeclaration::get_value());
}

void
TyvisInterfaceDeclaration::_add_declarations_in_initializations(PublishData *declarations) {
  if(_get_value() != NULL) {
    _get_value()->_add_decl_into_declaration_collection(declarations);
  }
  if(_get_subtype() != NULL) {
    _get_subtype()->_add_decl_into_declaration_collection(declarations);
  }
}

IIR_Int32 
TyvisInterfaceDeclaration::get_num_indexes(){
  return _get_subtype()->get_num_indexes();
}

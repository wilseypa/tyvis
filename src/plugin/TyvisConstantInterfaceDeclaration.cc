
// Copyright (c) The University of Cincinnati.
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

#include "TyvisAttributeSpecification.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisConstantInterfaceDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisTypeDefinition.hh"
#include "savant/error_func.hh"
#include "published_file.hh"
#include "PublishData.hh"  //DRH
#include <sstream>
#include "savant/set.hh"


void
TyvisConstantInterfaceDeclaration::_publish_cc_rvalue( published_file &_cc_out,
                                                       PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisConstantInterfaceDeclaration::_publish_cc" );

  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  Tyvis::_publish_cc_prefix_string( _cc_out );
  _publish_cc_lvalue( _cc_out , declarations );

//   _cc_out << "/* kind=" << get_declarative_region()->get_kind_text() << " */";
  if (get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION) {
    _cc_out << "Func()";
  }
  CC_REF( _cc_out, get_declarative_region()->get_kind_text() );

}

void
TyvisConstantInterfaceDeclaration::_publish_cc_elaborate( published_file &_cc_out,
							  PublishData *declarations ){
  CC_REF( _cc_out, "TyvisConstantInterfaceDeclaration::_publish_cc_elaborate" );
  _publish_cc_decl( _cc_out, declarations );
}

void
TyvisConstantInterfaceDeclaration::_publish_cc_elaborate_as_pointers( published_file &_cc_out,
                                                                      PublishData *declarations,
                                                                      IIR_Boolean publishDefinition){

  CC_REF( _cc_out, "TyvisConstantInterfaceDeclaration::_publish_cc_elaborate_as_pointers" );
  if( publishDefinition ){
    // Constant interface declarations published as globals need to be wrapped in functions that
    // perform initialization. This avoids the "static initialization fiasco" wherein dependent
    // global variables are not initialized in the correct order.
    // Note that this does not solve the problem of destructor interdependence in global variables.
    if (get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION) {
      _cc_out << _get_cc_object_type() << "& " << *_get_mangled_declarator() << "Func() {" << NL();
      _cc_out << "static ";
    }
    _publish_cc_decl_with_constructor_args( _cc_out, declarations );
    _cc_out << ";" << NL();
    if (get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION) {
      _cc_out << "return " << *_get_mangled_declarator() << ";" << NL();
      _cc_out << "}" << NL();
    }
  }
  else{
    if (get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION) {
      // Initialization function declaration
      _publish_cc_include( _cc_out );
      _cc_out << _get_cc_object_type() << "& " << *_get_mangled_declarator() << "Func();" << NL();
    } else {
      _cc_out << "extern ";
      _publish_cc_decl( _cc_out, declarations );
    }
  }
}



void 
TyvisConstantInterfaceDeclaration::_publish_cc_universal_value( published_file &_cc_out,
								PublishData *declarations ){
  CC_REF( _cc_out, "TyvisConstantInterfaceDeclaration::_publish_cc_universal_value" );
  _publish_cc_rvalue( _cc_out, declarations );
}

void
TyvisConstantInterfaceDeclaration::_publish_cc_headers( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisConstantInterfaceDeclaration::_publish_cc_headers" );

  TyvisObjectDeclaration::_publish_cc_headers( _cc_out );
  if (_get_currently_publishing_unit() == Tyvis::TYPE) {
    if (_get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION) {
      _get_declarative_region()->_publish_cc_include_decls( _cc_out );
    }
  }
}

const string
TyvisConstantInterfaceDeclaration::_get_cc_object_type() {
  return "Variable";
}

const string
TyvisConstantInterfaceDeclaration::_get_cc_tyvis_type(){
  //  CC_REF( _cc_out, "TyvisConstantInterfaceDeclaration::_publish_cc_tyvis_type" );
  ASSERT ( _get_subtype() != NULL );
  return _get_subtype()->_get_cc_tyvis_type();
}

void
TyvisConstantInterfaceDeclaration::_publish_cc_ams_function( published_file &_cc_out,
							     PublishData *declarations) {
  if (_get_currently_publishing_unit() == IIR::SIMULTANEOUS_IF) {
    _cc_out << *_get_mangled_declarator();
  }
  else {
    if (_get_currently_publishing_unit() == IIR::ARCHITECTURE_DECL || 
        _get_currently_publishing_unit() == IIR::SIMULTANEOUS_STATEMENT) {
      _cc_out << "new equationNode('N', ((";
      _get_subtype()->_publish_cc_universal_type(_cc_out);
      _cc_out << "*)&("
              << *_get_mangled_declarator()
              << ".getVHDLData()))->val, node" 
              << (int)(_stmt_node_index/2) << ");" << NL();
    }
    else {
      _get_value()->_publish_cc_value( _cc_out, declarations);
    }
  }
}

Tyvis *
TyvisConstantInterfaceDeclaration::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_InterfaceDeclaration::get_value());
}

void
TyvisConstantInterfaceDeclaration::_set_stmt_node_index(IIR_Int32 *index, 
                                                        bool _is_right_child, 
                                                        bool &) {
  if ( _is_right_child ) {
   _stmt_node_index = 2 * (*index)+ 1;
  }
  else {
   _stmt_node_index = 2 * (*index);
  }
}

void
TyvisConstantInterfaceDeclaration::_build_generic_parameter_set(savant::set<TyvisDeclaration> *to_build) {
  to_build->add(this);
}


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
#include "TyvisTypeDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisLibraryUnit.hh"

#include "savant/savant.hh"
#include <sstream>
#include "published_file.hh"

TyvisTypeDeclaration::TyvisTypeDeclaration() {
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisTypeDeclaration::~TyvisTypeDeclaration() {
  //Release the list memory
  delete get_attributes();
}

void
TyvisTypeDeclaration::_publish_cc_package_decl( published_file &_cc_out,
						PublishData *declarations ){
  CC_REF( _cc_out, "TyvisTypeDeclaration::_publish_cc_package_decl" );
  _publish_cc_static_type_info_fn( _cc_out, declarations );
}


void 
TyvisTypeDeclaration::_publish_cc_lvalue( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisTypeDeclaration::_publish_cc_lvalue" );
  _publish_cc_include( _cc_out );
  _cc_out << *_get_mangled_declarator();
}

void 
TyvisTypeDeclaration::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisTypeDeclaration::_publish_cc_headers" );
  _get_type_definition()->_publish_cc_include( _cc_out );
  _get_type_definition()->_publish_cc_headers( _cc_out );
}

void
TyvisTypeDeclaration::_publish_cc_type_info_xxx( published_file &_cc_out,
						 PublishData *declarations ){
  CC_REF( _cc_out, "TyvisTypeDeclaration::_publish_cc_type_info_xxx" );
  _get_type_definition()->_publish_cc_type_info_xxx( _cc_out, declarations );
}

void
TyvisTypeDeclaration::_publish_cc_universal_type( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisTypeDeclaration::_publish_cc_universal_type" );
  _get_type_definition()->_publish_cc_universal_type( _cc_out );
}

void 
TyvisTypeDeclaration::_publish_cc_static_type_info_fn( published_file &_cc_out,
						       PublishData *declarations ){
  CC_REF( _cc_out, "TyvisTypeDeclaration::_publish_cc_type_info" );
  _get_type_definition()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void 
TyvisTypeDeclaration::_publish_cc_range( published_file &, PublishData * ){
  cerr << "TyvisTypeDeclaration::_publish_cc_range( published_file &, PublishData * ) called!! ";
  abort();
}

void
TyvisTypeDeclaration::_publish_cc_class_name( ostream &os ){
  os << *_get_mangled_declarator();
}

void
TyvisTypeDeclaration::_publish_cc_include( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisTypeDeclaration::_publish_cc_include" );
  if( _get_declarative_region() &&
      dynamic_cast<TyvisLibraryUnit *>(_get_declarative_region()) ){
    _get_declarative_region()->_publish_cc_include( _cc_out );
  }
}

void 
TyvisTypeDeclaration::_add_decl_into_declaration_collection(PublishData *declarations) {
  _get_type_definition()->_add_decl_into_declaration_collection(declarations);
}

TyvisTypeDefinition *
TyvisTypeDeclaration::_get_type_definition() {
  return dynamic_cast<TyvisTypeDefinition *>(get_type());
}

IIR_Boolean
TyvisTypeDeclaration::_is_kernel_type() {
  return _get_type_definition()->_is_kernel_type();
}

IIR_Int32 
TyvisTypeDeclaration::get_num_indexes(){
  if( _get_type_definition() != NULL ){
    return _get_type_definition()->get_num_indexes();
  }
  else{
    return -1;
  }
}

TyvisAttributeSpecificationList* 
TyvisTypeDeclaration::_get_attribute_specification_list() { 
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}


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
#include "TyvisSubtypeDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisDesignFile.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisFunctionDeclaration.hh"
#include "savant/set.hh"
#include "published_cc_file.hh"
#include "published_header_file.hh"
#include <sstream>

TyvisSubtypeDeclaration::TyvisSubtypeDeclaration() {
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisSubtypeDeclaration::~TyvisSubtypeDeclaration() {
  delete get_attributes();
}

void 
TyvisSubtypeDeclaration::_publish_cc_lvalue( published_file &(_cc_out), PublishData * ) {
  //CC_REF( _cc_out, "TyvisSubtypeDeclaration::_publish_cc_lvalue" );
  _publish_cc_include( _cc_out );
  _cc_out << *_get_mangled_declarator();
}

void
TyvisSubtypeDeclaration::_publish_cc_universal_type( published_file &_cc_out ){

  CC_REF( _cc_out, "TyvisSubtypeDeclaration::_publish_cc_universal_type" );

  _get_subtype()->_publish_cc_universal_type( _cc_out );
}

void 
TyvisSubtypeDeclaration::_publish_cc_headers( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisSubtypeDeclaration::_publish_cc_headers" );

  _publish_cc_include( _cc_out );
  _get_subtype()->_publish_cc_headers( _cc_out );
}

void 
TyvisSubtypeDeclaration::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSubtypeDeclaration::_publish_cc_range" );

  // cerr << "TyvisSubtypeDeclaration::_publish_cc_range( published_file &_cc_out ) called!! " 
  // << "Trying to work around\n";
  _get_subtype()->_publish_cc_range( _cc_out, declarations );
}

void
TyvisSubtypeDeclaration::_publish_cc_package_decl( published_file &_cc_out,
						PublishData *declarations ){
  _publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisSubtypeDeclaration::_publish_cc_static_type_info_fn( published_file &_cc_out,
						PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSubtypeDeclaration::_publish_cc_type_info" );
  _get_subtype()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _cc_out << ";" << NL();
}

void
TyvisSubtypeDeclaration::_publish_cc_include( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisSubtypeDeclaration::_publish_cc_include" );
  _get_subtype()->_publish_cc_include( _cc_out );
}

TyvisAttributeSpecificationList *
TyvisSubtypeDeclaration::_get_attributes() {
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

IIR_Boolean
TyvisSubtypeDeclaration::_is_kernel_type() {
  return _get_subtype()->_is_kernel_type();
}

IIR_Boolean
TyvisSubtypeDeclaration::_is_resolved_type() {
  return _get_subtype()->_is_resolved_type();
}

IIR_Int32 
TyvisSubtypeDeclaration::get_num_indexes(){
  ASSERT( _get_subtype() != NULL );
  return _get_subtype()->get_num_indexes();
}

TyvisAttributeSpecificationList*
TyvisSubtypeDeclaration::_get_attribute_specification_list(){
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

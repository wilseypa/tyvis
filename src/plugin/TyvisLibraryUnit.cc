
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

#include "TyvisAttributeSpecification.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisLibraryUnit.hh"
#include "TyvisTextLiteral.hh"

#include "PublishData.hh"
#include "published_file.hh"
#include "published_makefile.hh"

#include <sstream>

TyvisLibraryUnit::TyvisLibraryUnit() :
  _declarations( new PublishData() ){}

TyvisLibraryUnit::~TyvisLibraryUnit(){}

void 
TyvisLibraryUnit::_publish_cc_include( published_file &_cc_out,
				       const char *const to_insert ) {
  CC_REF( _cc_out, "TyvisLibraryUnit::_publish_cc_include" );
  ostringstream file_name_stream;
  _publish_cc_binding_name( file_name_stream );
  string binding_name = file_name_stream.str();
  if( binding_name != "" ){
    if( binding_name == "std_textioPkg" ){
      // Needed due to how stuff lays out after "make install"
      binding_name = "tyvis/std_textioPkg";
    }
    string suffix;
    if( to_insert != 0 ){
      suffix = to_insert;
    }
    suffix += ".hh";
    Tyvis::_publish_cc_include( _cc_out, binding_name + suffix );
  }
}

void 
TyvisLibraryUnit::_publish_cc_include( published_file &_cc_out ){
  _publish_cc_include( _cc_out, 0 );
  _publish_cc_include_decls( _cc_out );
}

void
TyvisLibraryUnit::_publish_cc_makefile_stub(){
  string unit_name = _get_mangled_declarator()->convert_to_string();
  published_makefile makefile( _get_work_library()->get_path_to_directory(),
			       "Makefile." + unit_name,
			       this );

  makefile.add_library( _get_library() );
  makefile.add_libraries( _get_context_items() );
}

void 
TyvisLibraryUnit::_publish_cc_include_decls( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisLibraryUnit::_publish_cc_include_decls" );
  ostringstream filename;
  _publish_cc_include_decls_prefix( filename );
  filename << ".hpp";
  Tyvis::_publish_cc_include( _cc_out, filename.str() );
}

void 
TyvisLibraryUnit::_publish_cc_include_elab( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisLibraryUnit::_publish_cc_include_elab" );
  const string filename = _get_cc_elaboration_class_name() + ".hpp";
  Tyvis::_publish_cc_include( _cc_out, filename );
}

void 
TyvisLibraryUnit::_publish_cc_elaborate(){
  _report_undefined_fn("_publish_cc_elaborate()");
}

TyvisEntityDeclaration* 
TyvisLibraryUnit::_get_entity() {
  _report_undefined_fn("_get_entity()");
  return NULL;
}

TyvisDeclarationList *
TyvisLibraryUnit::_get_context_items() {
  return dynamic_cast<TyvisDeclarationList *>(get_context_items());
}

TyvisLibraryDeclaration *
TyvisLibraryUnit::_get_library(){
  ASSERT( _get_declarative_region() != NULL );
  ASSERT( _get_declarative_region()->get_kind() == IIR_LIBRARY_DECLARATION );
  return dynamic_cast<TyvisLibraryDeclaration *>(_get_declarative_region());
}

const string
TyvisLibraryUnit::_get_library_directory(){
  ASSERT( _get_library() != NULL );
  return _get_library()->get_path_to_directory();
}


TyvisAttributeSpecificationList* 
TyvisLibraryUnit::_get_attribute_specification_list() { 
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes()); 
}

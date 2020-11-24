
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
#include "TyvisDeclarationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFileDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisPackageBodyDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisTypeDeclaration.hh"
#include "savant/error_func.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include <sstream>
#include "PublishData.hh"

TyvisPackageBodyDeclaration::TyvisPackageBodyDeclaration()
{
  set_package_declarative_part(new TyvisDeclarationList());
}

TyvisPackageBodyDeclaration::~TyvisPackageBodyDeclaration() {
  //Release the list memory
  delete get_package_declarative_part();
}

void 
TyvisPackageBodyDeclaration::_publish_cc(){
  _set_currently_publishing_unit(Tyvis::PACKAGE_BODY);

  _publish_cc_hh_file(_declarations);
  _publish_cc_cc_file(_declarations);

  _set_currently_publishing_unit(NONE);
}

void 
TyvisPackageBodyDeclaration::_publish_cc_package_name( ostream &os ){
  os << *(_get_mangled_declarator()) << "Pkg";
}

void
TyvisPackageBodyDeclaration::_publish_cc_binding_name( ostream &os ){
  os << _get_cc_package_name();
}

void
TyvisPackageBodyDeclaration::_publish_cc_hh_file(PublishData *declarations){
  published_header_file header_file( _get_library_directory(),
				     _get_cc_package_body_name(),
				     this );

  CC_REF( header_file, "TyvisPackageBodyDeclaration::_publish_cc_hh_file" );

  _get_context_items()->_publish_cc( header_file, declarations );
  _publish_cc_headers( header_file );
 
  _get_package_declarative_part()->_publish_cc_package_declarations( header_file, declarations );

  CC_REF( header_file, "TyvisPackageBodyDeclaration::_publish_cc_hh_file" );
}

void
TyvisPackageBodyDeclaration::_publish_cc_cc_file(PublishData *declarations){
  published_cc_file cc_file( _get_library_directory(), 
			     _get_cc_package_body_name(),
			     this);
  CC_REF( cc_file, "TyvisPackageBodyDeclaration::_publish_cc_cc_file" );
  Tyvis::_publish_cc_include( cc_file, _get_cc_package_body_name() + ".hh" );
  _get_package_declarative_part()->_publish_cc_static_type_info_fn( cc_file, declarations );
  
  //initialize the decl
  TyvisDeclaration *decl =
    dynamic_cast<TyvisDeclaration *>(get_package_declarative_part()->first());
  while( decl != NULL ){
    switch(decl->get_kind()) {
      // These have been dealt with elsewhere.
    case IIR_CONSTANT_DECLARATION:
    case IIR_TYPE_DECLARATION:
    case IIR_SUBTYPE_DECLARATION:
      break;
    case IIR_PROCEDURE_DECLARATION:
    case IIR_FUNCTION_DECLARATION:
      decl->_publish_cc_decl( cc_file, declarations );
      break;
    case IIR_COMPONENT_DECLARATION:
    case IIR_USE_CLAUSE:
      break;
    case IIR_FILE_DECLARATION:  // This is a guess - DEM
      (dynamic_cast<TyvisFileDeclaration *>( decl ))->_publish_cc_shared_file_decl( cc_file, declarations );
      break;
    default:{
      ostringstream err;
      err << "Unimplemented declaration type: " << decl->get_kind_text()
	  << " in TyvisPackageBodyDeclaration::_publish_cc_cc_file( published_file &cc_file ).";
      report_error( decl, err.str() );
      break;
    }
    }
    decl = dynamic_cast<TyvisDeclaration *>(get_package_declarative_part()->successor(decl));
  }
}

void 
TyvisPackageBodyDeclaration::_publish_cc_headers( published_file &_cc_out ) {
  Tyvis *old_node = _get_current_publish_node();
  _set_current_publish_node( this );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/VHDLProcess.hpp" );
  Tyvis::_publish_cc_include( _cc_out, _get_cc_package_name() + ".hh" );
  TyvisDeclaration *decl = 
    dynamic_cast<TyvisDeclaration *>(get_package_declarative_part()->first());
  while (decl != NULL) {
    decl->_publish_cc_include( _cc_out );
    decl = 
      dynamic_cast<TyvisDeclaration *>(get_package_declarative_part()->successor(decl));
  }
  _set_current_publish_node( old_node );
}  

void
TyvisPackageBodyDeclaration::_publish_cc_include_decls_prefix( ostream &os ){
  _publish_cc_package_name( os );
}

TyvisDeclarationList *
TyvisPackageBodyDeclaration::_get_package_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>(get_package_declarative_part());
}

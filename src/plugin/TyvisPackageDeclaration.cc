//
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
#include "TyvisConstantDeclaration.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFileDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisNatureDeclaration.hh"
#include "TyvisObjectDeclaration.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisTerminalDeclaration.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisScalarNatureDefinition.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisSubtypeDeclaration.hh"

#include "published_header_file.hh"
#include "published_cc_file.hh"
#include "savant/resolution_func.hh"
#include <sstream>
#include "savant/language_processing_control.hh"
#include "PublishData.hh"

extern language_processing_control *lang_proc;

TyvisPackageDeclaration::TyvisPackageDeclaration()
{ 
  set_package_declarative_part(new TyvisDeclarationList());
}

TyvisPackageDeclaration::~TyvisPackageDeclaration() {
  //Release the list memory
  delete get_package_declarative_part();
}

void 
TyvisPackageDeclaration::_publish_cc(){
  const string _prev_publish_name = _get_current_publish_name(); // Save previous value.
  Tyvis *_prev_publish_node = _get_current_publish_node();
  _set_current_publish_node( this );
  _set_current_publish_name( _get_cc_package_name() );

  _set_currently_publishing_unit(Tyvis::PACKAGE_PUB);
  _publish_cc_makefile_stub();
  _publish_cc_header(_declarations);
  _publish_cc_cc_file(_declarations);
  _set_currently_publishing_unit(NONE);

  _set_current_publish_name( _prev_publish_name );
  _set_current_publish_node( _prev_publish_node );
}

void
TyvisPackageDeclaration::_publish_cc_binding_name(ostream &os) {
  os << _get_cc_package_name();
}

void 
TyvisPackageDeclaration::_publish_cc_header(PublishData *declarations){
  published_header_file header_file( _get_library_directory(),
				     _get_cc_package_name(),
				     this );
  CC_REF( header_file, "TyvisPackageDeclaration::_publish_cc_header" );
  Tyvis::_publish_cc_include( header_file, "tyvis/STDTypes.hh" );
  if(lang_proc->processing_vhdl_ams()) {
    Tyvis::_publish_cc_include( header_file, "tyvis/AMSType.hh" );
  }
  _get_context_items()->_publish_cc( header_file, declarations );
  _publish_cc_headers( header_file );
  _get_package_declarative_part()->_publish_cc_package_declarations( header_file, declarations );
}

void 
TyvisPackageDeclaration::_publish_cc_headers( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisPackageDeclaration::_publish_cc_headers" );

  Tyvis::_publish_cc_include( _cc_out, "tyvis/VHDLProcess.hh" );

  TyvisDeclaration *decl = 
    dynamic_cast<TyvisDeclaration *>(get_package_declarative_part()->first());
  while (decl != NULL) {
    decl->_publish_cc_include( _cc_out );
    decl = 
      dynamic_cast<TyvisDeclaration *>(get_package_declarative_part()->successor(decl));
  }
}  

void
TyvisPackageDeclaration::_publish_cc_cc_file( PublishData * ){
  published_cc_file cc_file( _get_library_directory(),
			     _get_current_publish_name(),
			     this );
  CC_REF( cc_file, "TyvisPackageDeclaration::_publish_cc_cc_file" );
  _publish_cc_include( cc_file );
  //  _get_package_declarative_part()->_publish_cc_package_definitions( cc_file, declarations );
}

void
TyvisPackageDeclaration::_publish_cc_include( published_file &_cc_out ){
  if( _get_declarative_region() != 0) {
    if ( dynamic_cast<TyvisDeclaration *>(_get_declarative_region()) &&
      (IIRBase_TextLiteral::cmp(dynamic_cast<IIR_Declaration *>(_get_declarative_region())->get_declarator(), "std")) == 0){
      //_publish_cc_include( _cc_out, string("tyvis/") + _get_cc_package_name() + ".hh" );
    } 
    else{
      //_publish_cc_include( _cc_out, _get_cc_package_name() + ".hh" );
    }
  }
}

void
TyvisPackageDeclaration::_publish_cc_include_decls_prefix( ostream &os ) {
  os << _get_cc_package_name();
}

TyvisDeclarationList *
TyvisPackageDeclaration::_get_package_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>(get_package_declarative_part());
}

TyvisDeclaration*
TyvisPackageDeclaration::_get_prefix_declaration() {
  return this;
}

TyvisDeclaration*
TyvisPackageDeclaration::_get_package_declaration() {
  return this;
}


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

#include "TyvisDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisSignature.hh"

#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "published_file.hh"

using std::cerr;

void 
TyvisDesignatorExplicit::_publish_cc_entity_class( published_file &_cc_out ){
  _publish_entity_class( _cc_out.get_stream() );
}

void
TyvisDesignatorExplicit::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  _get_name()->_publish_cc_elaborate( _cc_out, declarations );
}

void
TyvisDesignatorExplicit::_publish_cc_lvalue( published_file &(_cc_out), PublishData *declarations ) {
  _get_name()->_publish_cc_lvalue( _cc_out , declarations );
}

void
TyvisDesignatorExplicit::_publish_cc_designator( published_file &_cc_out, PublishData *declarations ) {
  _get_name()->_publish_cc_declarator( _cc_out, declarations );
}

Tyvis *
TyvisDesignatorExplicit::_get_name() {
  return dynamic_cast<Tyvis *>(get_name());
}

void 
TyvisDesignatorExplicit::_add_decl_into_declaration_collection(PublishData *declarations) {
  ASSERT(_get_name() != NULL);
  ASSERT(_get_name()->is_resolved() == TRUE);
  _get_name()->_add_decl_into_declaration_collection(declarations);
}

void
TyvisDesignatorExplicit::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  if( _get_name() && _get_name()->is_signal() == TRUE ){
    _get_name()->_get_list_of_input_signals( list );
  }
}

IIR_Boolean 
TyvisDesignatorExplicit::_is_globally_static(){
  ASSERT( _get_name() != NULL );
  return _get_name()->is_locally_static();
}

void 
TyvisDesignatorExplicit::_publish_entity_class( ostream &os ) {
  TyvisDeclaration *decl = NULL;
  ASSERT( _get_name()->is_resolved() == TRUE);
  ASSERT( dynamic_cast<TyvisDeclaration *>(_get_name()) != NULL );

  decl = dynamic_cast<TyvisDeclaration*>(get_name());
  switch (decl->get_declaration_type()) {
  case IIR_Declaration::VARIABLE:
  case IIR_Declaration::SHARED_VARIABLE:
  case IIR_Declaration::INTERFACE_VARIABLE:
    os << "variable";
    break;
  case IIR_Declaration::TYPE:
    os << "type";
    break;
  case IIR_Declaration::SUBTYPE:
    os << "subtype";
    break;
  case IIR_Declaration::SIGNAL:
  case IIR_Declaration::INTERFACE_SIGNAL:
    os << "signal";
    break;
  case IIR_Declaration::PROCEDURE:
    os << "procedure";
    break;
  case IIR_Declaration::FUNCTION:
    os << "function";
    break;
  case IIR_Declaration::S_FILE:
  case IIR_Declaration::INTERFACE_FILE:
    os << "file";
    break;
  case IIR_Declaration::ENTITY:
    os << "entity";
    break;
  case IIR_Declaration::CONSTANT:
  case IIR_Declaration::INTERFACE_CONSTANT:
    os << "constant";
    break;
  case IIR_Declaration::CONFIGURATION:
    os << "configuration";
    break;
  case IIR_Declaration::COMPONENT:
    os << "component";
    break;
  case IIR_Declaration::ARCHITECTURE:
    os << "architecture";
    break;
  case IIR_Declaration::PACKAGE_DT:
    os << "package";
    break;
  case IIR_Declaration::LABEL:
    os << "label";
    break;
  case IIR_Declaration::LITERAL:
    os << "literal";
    break;
  case IIR_Declaration::UNITS:
    os << "units";
    break;
  case IIR_Declaration::GROUP:
    os << "group";
    break;
  default:
    cerr << "Oops!  Unsopported entity class in "
	 << "TyvisDesignatorExplicit::_publish_entity_class\n";
    abort();
  }
}

void
TyvisDesignatorExplicit::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_name()->_build_above_attribute_set(to_build);
}

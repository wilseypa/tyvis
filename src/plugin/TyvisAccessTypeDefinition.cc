
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

#include "TyvisAccessSubtypeDefinition.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisProcedureDeclaration.hh"
#include "TyvisConstantInterfaceDeclaration.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisScalarTypeDefinition.hh"

#include "savant/error_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>

void 
TyvisAccessTypeDefinition::_add_decl_into_declaration_collection(PublishData *declarations){
  ASSERT(_get_designated_type() != NULL);
  if (_get_designated_type() != this) {
    _get_designated_type()->_add_decl_into_declaration_collection(declarations);
  }
}

const string
TyvisAccessTypeDefinition::_get_cc_tyvis_type(){
  return "AccessTypeInfo";
}

void
TyvisAccessTypeDefinition::_publish_cc_universal_type( published_file &_cc_out ){
  _cc_out << "AccessObject";
}

void 
TyvisAccessTypeDefinition::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAccessTypeDefinition::_publish_cc_headers" );
  _get_designated_type()->_publish_cc_include( _cc_out );
}

void 
TyvisAccessTypeDefinition::_publish_cc_lvalue( published_file &_cc_out,
					       PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAccessTypeDefinition::_publish_cc" );
  _get_declaration()->_publish_cc_type_info_xxx( _cc_out, declarations );
}

void
TyvisAccessTypeDefinition::_publish_cc_type_info_args( published_file &_cc_out,
						       PublishData *declarations,
						       const string & ){
  ASSERT( _get_designated_type() != 0 );
  CC_REF( _cc_out, "TyvisAccessTypeDefinition::_publish_cc_type_info_args" );
  _get_designated_type()->_publish_cc_type_info_xxx( _cc_out, declarations );
}

IIR_Int32
TyvisAccessTypeDefinition::get_num_indexes(){
  TyvisTypeDefinition *type = _get_designated_type();

  ASSERT(type != NULL);
  return type->get_num_indexes();
}

TyvisTypeDefinition *
TyvisAccessTypeDefinition::_get_designated_type() {
  return dynamic_cast<TyvisTypeDefinition *>(get_designated_type());
}

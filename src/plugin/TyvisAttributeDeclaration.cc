
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
#include "TyvisAttributeDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisAttribute.hh"
#include "TyvisIdentifier.hh"
#include "savant/set.hh"


void 
TyvisAttributeDeclaration::_add_decl_into_declaration_collection(PublishData *) {
  // Dummy function
}

void 
TyvisAttributeDeclaration::_publish_cc_lvalue( published_file &, PublishData * ){
  // Dummy method.
}

void 
TyvisAttributeDeclaration::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisAttributeDeclaration::_publish_cc_elaborate" );

  if(is_implicit_declaration()) {
    _get_attribute()->_publish_cc_elaborate( _cc_out, declarations );
  }
  else {
    if(_get_attribute() != NULL) {
      _get_attribute()->_publish_cc_elaborate( _cc_out, declarations );     
    }
    else {
      // _cc_out << get_declarator();
      _get_mangled_declarator()->_publish_cc_elaborate( _cc_out, declarations );
    }
  }
}

void 
TyvisAttributeDeclaration::_publish_cc_headers( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisAttributeDeclaration::_publish_cc_headers" );

  _get_subtype()->_publish_cc_headers( _cc_out );
}

TyvisAttribute *
TyvisAttributeDeclaration::_get_attribute() {
  return dynamic_cast<TyvisAttribute *>(get_attribute());
}

TyvisDeclaration*
TyvisAttributeDeclaration::_get_prefix_declaration(){
  return this;
}


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

#include "savant/savant.hh"
#include "TyvisDesignFile.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisVariableDeclaration.hh"

#include "published_file.hh"
#include "PublishData.hh"

#include <sstream>
using std::ostringstream;

void 
TyvisVariableDeclaration::_publish_cc_lvalue( published_file &_cc_out,
					      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisVariableDeclaration::_publish_cc" );
  
  if (!declarations->in_collection(this)) {
    if (_get_value() != NULL) {
      _get_value()->_add_decl_into_declaration_collection(declarations);
    }
    declarations->add_declaration(this);
  }

  TyvisDeclaration::_set_scoping_prefix();
  Tyvis::_publish_cc_prefix_string( _cc_out );
  TyvisObjectDeclaration::_publish_cc_lvalue( _cc_out , declarations );
  TyvisDeclaration::_reset_scoping_prefix();
}

void
TyvisVariableDeclaration::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisVariableDeclaration::_publish_cc_value" );
  
  if (_get_value() != NULL) {
    _get_value()->_publish_cc_value( _cc_out, declarations );
  } 
  else {
    if( dynamic_cast<TyvisScalarTypeDefinition *>(_get_subtype()) ){ 
      dynamic_cast<TyvisScalarTypeDefinition *>(_get_subtype())->_get_left()->_publish_cc_value( _cc_out, declarations );
    }
  }
}

void 
TyvisVariableDeclaration::_publish_cc_universal_value( published_file &_cc_out,
						       PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisVariableDeclaration::_publish_cc_universal_value" );
  
  if (_get_value() != NULL) {
    _get_value()->_publish_cc_universal_value( _cc_out, declarations );
  }
  else {
    TyvisScalarTypeDefinition *typeDef = 
      dynamic_cast<TyvisScalarTypeDefinition *>(_get_subtype());
    if ( typeDef ){
      typeDef->_get_base_type_left()->_publish_cc_universal_value( _cc_out, declarations );
    }
  }
}

void 
TyvisVariableDeclaration::_publish_cc_init( published_file &_cc_out,
					    PublishData * ) {
  CC_REF( _cc_out, "TyvisVariableDeclaration::_publish_cc_init" );  
}

void 
TyvisVariableDeclaration::_publish_cc_data( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisVariableDeclaration::_publish_cc_data" );
  
  _cc_out << "getState()->"
          << *_get_mangled_declarator();
}

void 
TyvisVariableDeclaration::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisVariableDeclaration::_publish_cc_wait_data" );
  
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  _cc_out << "s->"
          << *_get_mangled_declarator();
}

const string
TyvisVariableDeclaration::_get_cc_object_type() {
  return "Variable";
}

const string
TyvisVariableDeclaration::_get_cc_tyvis_type(){
  return _get_subtype()->_get_cc_tyvis_type();
}

Tyvis *
TyvisVariableDeclaration::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_VariableDeclaration::get_value());
}

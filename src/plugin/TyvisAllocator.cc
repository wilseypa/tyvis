
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
#include "TyvisAllocator.hh"
#include "TyvisAccessTypeDefinition.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/set.hh"
#include "published_file.hh"

void
TyvisAllocator::_publish_cc_lvalue( published_file &_cc_out, 
				    PublishData *declarations ) {
  _publish_cc_include( _cc_out, "tyvis/AccessObject.hh" );
  CC_REF( _cc_out, "TyvisAllocator::_publish_cc" );
  _cc_out << OS("AccessObject(");
  _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  if(_get_value() != NULL){
    _cc_out << ", " << NL();
    _get_value()->_publish_cc_initialization_value( _cc_out, declarations );
  }
  _cc_out << CS(")");
}

void
TyvisAllocator::_publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisAllocator::_publish_cc_initialization_value" );
  
  if(_get_value() != NULL){
    //_cc_out << ", ";
    if ((_get_value()->_get_subtype()->is_scalar_type() == TRUE) ||
	(dynamic_cast<TyvisTypeDefinition *>(this->IIRBase_Expression::get_subtype())->is_access_type() == TRUE) && 
	(dynamic_cast<TyvisStringLiteral *>(_get_value()) == NULL)) {
      _cc_out << "*(";
      _get_value()->_publish_cc_state_object_init( _cc_out, declarations );
      _cc_out << ")";
    }
    else {
      _get_value()->_publish_cc_state_object_init( _cc_out, declarations );
    }
  }
  else {
    _cc_out << " *( new ";
    _get_type_mark()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "(ObjectBase::VARIABLE";
    if(_get_type_mark()->is_array_type() == TRUE) {
      _get_type_mark()->_publish_cc_constructor_args( _cc_out, declarations );
    }
    _cc_out << "))";
  }
}

void 
TyvisAllocator::_add_decl_into_declaration_collection(PublishData *declarations){
  if(_get_value() != NULL){
    _get_value()->_add_decl_into_declaration_collection(declarations);
  }
}

Tyvis *
TyvisAllocator::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_Allocator::get_value());
}

TyvisTypeDefinition *
TyvisAllocator::_get_type_mark() {
  return dynamic_cast<TyvisTypeDefinition *>(get_type_mark());
}

IIR_Boolean 
TyvisAllocator::_is_globally_static_primary(){
  ASSERT( dynamic_cast<TyvisTypeDefinition *>(get_type_mark()) != NULL );
  return dynamic_cast<TyvisTypeDefinition *>(get_type_mark())->_is_globally_static();
}

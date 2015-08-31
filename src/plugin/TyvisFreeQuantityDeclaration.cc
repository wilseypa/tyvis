
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

#include "TyvisAttribute.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFloatingPointLiteral.hh"
#include "TyvisFreeQuantityDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisStringLiteral.hh"

#include "published_file.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <string.h>
#include "PublishData.hh"

TyvisFreeQuantityDeclaration::TyvisFreeQuantityDeclaration() {
  ATOL = 1e-3;
  RTOL = 1e-12;
  _stmt_qty_index = -1;
}

void
TyvisFreeQuantityDeclaration::_publish_cc_lvalue(published_file &_cc_out, PublishData *declarations) {
  CC_REF( _cc_out, "TyvisFreeQuantityDeclaration::_publish_cc_lvalue" );	
  if(is_implicit_declaration() == FALSE ) {
    if (declarations != NULL &&
        !declarations->in_collection(this)) {
      declarations->add_declaration(this);
    }
  }
  _cc_out << *_get_mangled_declarator();	
}

void
TyvisFreeQuantityDeclaration::_publish_cc_ams_function( published_file &_cc_out,
							PublishData *declarations ){
  if( get_is_currently_publishing_simultaneous_if_stmt() == TRUE ) {
    ASSERT(get_subtype() != NULL);
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "(ObjectBase::VARIABLE,qty["
            << _get_stmt_qty_index() << "])";
  }
  else {
    _cc_out << "  new equationNode('Q'," << _get_stmt_qty_index() << ",node"
            << (int)(_stmt_node_index[_qty_occurence_index++]/2) <<");" <<NL();
  }
}

const string
TyvisFreeQuantityDeclaration::_get_cc_object_type(){
  if(is_implicit_declaration() == TRUE) {
    return "ImplicitQuantity";
  } 
  else {
    return "Quantity";
  }
}

void
TyvisFreeQuantityDeclaration::_publish_cc_init(published_file &_cc_out, PublishData *declarations) {
  ASSERT(is_implicit_declaration() == FALSE);
  _publish_cc_init_quantity(_cc_out, declarations);
}

void
TyvisFreeQuantityDeclaration::_publish_cc_init_quantity(published_file &_cc_out, PublishData *declarations) {
  if(is_implicit_declaration() == TRUE) {
    _get_attribute_name()->_publish_cc_necessary_decl_init(_cc_out, declarations);
  }
}

void
TyvisFreeQuantityDeclaration::_publish_cc_tolerances(published_file &_cc_out) {
  _cc_out << RTOL;
  _cc_out << ", " ;
  _cc_out << ATOL;
  _cc_out << ", ";
}

void TyvisFreeQuantityDeclaration::_publish_cc_decl( published_file &_cc_out, PublishData * ) {
  CC_REF(_cc_out,"TyvisFreeQuantityDeclaration::_publish_cc_decl");
  _cc_out << "AMSType ";
  _cc_out << *_get_mangled_declarator();
  _cc_out << ";" << NL();
}

Tyvis *
TyvisFreeQuantityDeclaration::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_FreeQuantityDeclaration::get_value());
}

void
TyvisFreeQuantityDeclaration::_set_stmt_qty_index(IIR_Int32 *index,
                                                  savant::set<IIR_Declaration> *quantity_set,
                                                  PublishData *,
                                                  PublishData *arch_declarations) {
  if(!arch_declarations->in_collection(this)) {
    arch_declarations->add_declaration(this);
  }
  if( _get_stmt_qty_index() == -1 && index != NULL ){
    _stmt_qty_index = *index;
    (*index)++;
  }
  if( quantity_set != NULL)
    quantity_set->add(this);
}

IIR_Int32
TyvisFreeQuantityDeclaration::_get_stmt_qty_index() {
  return _stmt_qty_index;
}

void
TyvisFreeQuantityDeclaration::_flush_stmt_index() {
 _stmt_qty_index = -1;
}

void
TyvisFreeQuantityDeclaration::_set_stmt_node_index(IIR_Int32 *index, 
                                                   bool _is_right_child, 
                                                   bool &) {
  if ( _is_right_child ) {
   _stmt_node_index.push_back(2 * (*index)+ 1);
  }
  else {
   _stmt_node_index.push_back(2 * (*index));
  }
}

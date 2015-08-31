
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
#include "TyvisBranchQuantityDeclaration.hh"
#include "TyvisDeclaration.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIndexedName.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisTerminalDeclaration.hh"
#include "TyvisTypeDefinition.hh"

#include <string.h>
#include "savant/set.hh"
#include "published_file.hh"
#include "PublishData.hh"

TyvisBranchQuantityDeclaration::TyvisBranchQuantityDeclaration() {
  set_across_aspect_identifier_list(new TyvisDesignatorList());
  set_through_aspect_designator_list(new TyvisDesignatorList());

  RTOL = 1e-3;
  ATOL = 1e-12;

  _stmt_qty_index = -1;
}

TyvisBranchQuantityDeclaration::~TyvisBranchQuantityDeclaration() {
  delete get_across_aspect_identifier_list();
  delete get_through_aspect_designator_list();
}

void
TyvisBranchQuantityDeclaration::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisBranchQuantityDeclaration::_publish_cc_lvalue" );	
  ASSERT( get_declarator()->get_kind() == IIR_IDENTIFIER );
  if ( declarations != NULL &&
      !declarations->in_collection(this) ){
    declarations->add_declaration(this);
  }
  
  Tyvis::_publish_cc_prefix_string((published_file&)_cc_out);
  _cc_out << *_get_mangled_declarator();
}

void
TyvisBranchQuantityDeclaration::_publish_cc_ams_function(published_file &, PublishData *) {
  /*
  if (_currently_publishing_simultaneous_if_stmt == TRUE) {
    ASSERT(get_subtype() != NULL);
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "(ObjectBase::VARIABLE, qty["<<_get_stmt_qty_index() <<"])";
  }
  else {
    _cc_out << "qty[" << _get_stmt_qty_index() << "]";
  }
  */
}

void
TyvisBranchQuantityDeclaration::_publish_cc_decl( published_file &_cc_out, PublishData * ) {
  _cc_out << "AMSType " << *_get_mangled_declarator() << ";" << NL();
}

void
TyvisBranchQuantityDeclaration::_publish_cc_tolerances( published_file &_cc_out ){
  _cc_out << RTOL;
  _cc_out << ", ";
  _cc_out << ATOL;
  _cc_out << ", ";
}

TyvisName*
TyvisBranchQuantityDeclaration::_get_plus_terminal_name() {
  return dynamic_cast<TyvisName *>(get_plus_terminal_name());
}

TyvisName*
TyvisBranchQuantityDeclaration::_get_minus_terminal_name() {
  return dynamic_cast<TyvisName *>(get_minus_terminal_name());
}

Tyvis *
TyvisBranchQuantityDeclaration::_get_across_aspect_expression() {
  return dynamic_cast<Tyvis *>(get_across_aspect_expression());
}

Tyvis *
TyvisBranchQuantityDeclaration::_get_through_aspect_expression() {
  return dynamic_cast<Tyvis *>(get_through_aspect_expression());
}

void
TyvisBranchQuantityDeclaration::_set_stmt_qty_index( IIR_Int32 *index,
                                                     savant::set<TyvisDeclaration> *quantity_set,
                                                     PublishData *declarations,
                                                     PublishData *arch_declarations ) {
  if (!arch_declarations->in_collection(this)) {
    arch_declarations->add_declaration(this);
  }

  if (declarations != NULL) {
    if (!declarations->in_collection(this)) {
      declarations->add_declaration(this);
    }
  }
    
  if ( _get_stmt_qty_index() == -1 && index != NULL) {
    _stmt_qty_index = *index;
    (*index)++;  
  }
  if (quantity_set != NULL)
    quantity_set->add(this);
}     

IIR_Int32
TyvisBranchQuantityDeclaration::_get_stmt_qty_index() {
  return _stmt_qty_index;
}

void
TyvisBranchQuantityDeclaration::_flush_stmt_index() {
  _stmt_qty_index = -1;
}     

void
TyvisBranchQuantityDeclaration::_set_stmt_node_index(IIR_Int32 *index, 
                                                     bool _is_right_child, 
                                                     bool &) {
  if ( _is_right_child ) {
   _stmt_node_index.push_back(2 * (*index) + 1);
  }
  else {
   _stmt_node_index.push_back(2 * (*index));
  }
}

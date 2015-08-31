
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

#include "TyvisAboveAttribute.hh"
#include "TyvisAttribute.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIdentifier.hh"
#include "TyvisQuantityDeclaration.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisSignalDeclaration.hh"

#include "PublishData.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/error_func.hh"
#include "savant/StandardPackage.hh"
#include "published_file.hh"

const string &
TyvisAboveAttribute::_get_cc_attribute_name() const {
  static const string retval("above");
  return retval;
}

TyvisDeclaration*
TyvisAboveAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
						  TyvisTypeDefinition *decl_type) {
  return dynamic_cast<TyvisDeclaration *>(_build_signal_declaration( decl_name, decl_type ));
}

void
TyvisAboveAttribute::_set_stmt_signal_index( IIR_Int32 *index,
					     savant::set<TyvisDeclaration> *signal_set,
					     PublishData *declarations,
					     PublishData *arch_declarations) {
  TyvisDeclaration *implicit_decl = TyvisAttribute::_get_implicit_declaration();
  
  if (declarations != NULL) {
    if (!declarations->in_collection(implicit_decl))
      declarations->add_declaration(implicit_decl);
  }
  
  ASSERT(arch_declarations != NULL);
  if (!arch_declarations->in_collection(implicit_decl))
    arch_declarations->add_declaration(implicit_decl);
  TyvisAttribute::_get_implicit_declaration()->_set_stmt_signal_index(index, signal_set);
}

IIR_Int32
TyvisAboveAttribute::_get_stmt_signal_index() {
  return TyvisAttribute::_get_implicit_declaration()->_get_stmt_signal_index();
}

void
TyvisAboveAttribute::flush_stmt_index() {
  dynamic_cast<TyvisAttribute*>(this)->_get_implicit_declaration()->_flush_stmt_index();
}

void
TyvisAboveAttribute::_publish_cc_rvalue(published_file &_cc_out, PublishData *declarations) {
  CC_REF( _cc_out, "TyvisAboveAttribute::publish_cc_rvalue" );
  TyvisDeclaration *implicit_decl = TyvisAttribute::_get_implicit_declaration();
  ASSERT( get_prefix() != NULL );
  ASSERT( get_prefix()->is_resolved() == TRUE);
  ASSERT( dynamic_cast<TyvisQuantityDeclaration *>(get_prefix()) != NULL );
  ASSERT(implicit_decl != NULL);
  ASSERT(implicit_decl->is_resolved() == TRUE);
  
  if(declarations != NULL) {
    if (!declarations->in_collection(implicit_decl)) {
      declarations->add_declaration(implicit_decl);
    }
  }
  _cc_out << "currentState->";
  _cc_out << *implicit_decl->_get_mangled_declarator();
}

void
TyvisAboveAttribute::_publish_cc_ams_function(published_file & _cc_out, PublishData *declarations) {
  if (_get_currently_publishing_unit() == SIMULTANEOUS_IF) {
    TyvisAttribute::_get_implicit_declaration()->_publish_cc_ams_function(_cc_out, declarations);
  }
}

void
TyvisAboveAttribute::_publish_cc_necessary_decl_in_state(published_file& _cc_out, PublishData *declarations ) {
  ASSERT(get_prefix() != NULL);
  ASSERT(get_prefix()->is_resolved());
  _publish_cc_implicit_signal_declaration(_cc_out, declarations);
}

void
TyvisAboveAttribute::_publish_cc_init_val(ostream& _cc_out) {
  _cc_out << "true";
};

void
TyvisAboveAttribute::_publish_cc_name_in_caps(ostream& _cc_out) {
  _cc_out << "ABOVE";
}

void
TyvisAboveAttribute::_publish_cc_attrib_type(ostream& _cc_out) {
  _cc_out << "signal";
}

void
TyvisAboveAttribute::_publish_cc_sigtype(ostream& _cc_out) {
  _cc_out << " EXPLICIT;\n";
}

void
TyvisAboveAttribute::_publish_cc_necessary_decl_init(published_file &, PublishData *) {
  ASSERT(get_prefix() != NULL);
  ASSERT(get_prefix()->is_resolved() == TRUE);
  ASSERT((get_prefix()->is_signal() == TRUE) ||
         (dynamic_cast<TyvisAttribute *>(get_prefix()) != NULL) ||
         (dynamic_cast<TyvisQuantityDeclaration *>(get_prefix()) != NULL));
}

void
TyvisAboveAttribute::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  to_build->add(this);
}

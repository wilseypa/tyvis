
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

#include "TyvisBranchQuantityDeclaration.hh"
#include "TyvisDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFreeQuantityDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisZTFAttribute.hh"

#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "published_file.hh"
#include "savant/set.hh"
#include "PublishData.hh"

#include <sstream>
using std::ostringstream;

void
TyvisZTFAttribute::_publish_cc( published_file &, PublishData * ) {}

void
TyvisZTFAttribute::_publish_cc_decl(published_file &, PublishData *) {}

void
TyvisZTFAttribute::_publish_cc_ams_function(published_file &, PublishData *) {}

Tyvis *
TyvisZTFAttribute::_get_num() {
  return dynamic_cast<Tyvis *>(get_num());
}

Tyvis *
TyvisZTFAttribute::_get_den() {
  return dynamic_cast<Tyvis *>(get_den());
}

Tyvis *
TyvisZTFAttribute::_get_t() {
  return dynamic_cast<Tyvis *>(get_t());
}

Tyvis *
TyvisZTFAttribute::_get_initial_delay() {
  return dynamic_cast<Tyvis *>(get_initial_delay());
}

const string &
TyvisZTFAttribute::_get_cc_attribute_name() const {
  static const string retval("ztf");
  return retval;
}

void 
TyvisZTFAttribute::_set_stmt_qty_index(IIR_Int32 *index,  
                                              savant::set<TyvisDeclaration> *quantity_set,
                                              PublishData *declarations,
                                              PublishData *arch_declarations) {
  if(!declarations->in_collection(dynamic_cast<TyvisDeclaration*>(get_prefix())))
    declarations->add_declaration(dynamic_cast<TyvisDeclaration*>(get_prefix()));
  (dynamic_cast<TyvisAttribute*>(this))->_get_prefix()->_set_stmt_qty_index(index,quantity_set, declarations, arch_declarations);
}

TyvisDeclaration *
TyvisZTFAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
						TyvisTypeDefinition *decl_nature ) {
 return dynamic_cast<TyvisDeclaration*>(_build_free_quantity_declaration( decl_name, decl_nature ));
}

IIR_Int32
TyvisZTFAttribute::_get_stmt_qty_index() {
  return (dynamic_cast<TyvisAttribute*>(this))->//_get_implicit_declaration()->_get_stmt_qty_index();
  _get_prefix()->_get_stmt_qty_index();
}

void
TyvisZTFAttribute::_flush_stmt_index() {
  (dynamic_cast<TyvisAttribute*>(this))->_get_prefix()->_flush_stmt_index();
}

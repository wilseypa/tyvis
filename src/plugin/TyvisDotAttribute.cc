
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
#include "TyvisDesignFile.hh"
#include "TyvisDotAttribute.hh"
#include "TyvisFreeQuantityDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisTypeDeclaration.hh"
#include "TyvisTypeDefinition.hh"

#include "published_file.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/error_func.hh"
#include <sstream>
#include "savant/StandardPackage.hh"
#include "PublishData.hh"

void
TyvisDotAttribute::_publish_cc_ams_function(published_file &_cc_out, PublishData *declarations) {
  (dynamic_cast<TyvisAttribute*>(this))->_get_implicit_declaration()->_publish_cc_ams_function(_cc_out, declarations);
}

const string &
TyvisDotAttribute::_get_cc_attribute_name() const {
  static const string retval("dot");
  return retval;
}

void
TyvisDotAttribute::_set_stmt_qty_index(IIR_Int32 *index, savant::set<TyvisDeclaration> *quantity_set, PublishData *declarations, PublishData *arch_declarations) {
  if (arch_declarations !=NULL) {
    if (!arch_declarations->in_collection(dynamic_cast<TyvisDeclaration *>(get_prefix()))) {
      arch_declarations->add_declaration(dynamic_cast<TyvisDeclaration *>(get_prefix()));
    }
  }
  (dynamic_cast<TyvisAttribute*>(this))->_get_implicit_declaration()-> _set_stmt_qty_index(index, quantity_set, declarations, arch_declarations);
}

void
TyvisDotAttribute::_set_stmt_node_index(IIR_Int32 *index, bool _is_right_child, bool &reducibleFlag) {
  reducibleFlag = false;
  dynamic_cast<TyvisAttribute*>(this)->_get_implicit_declaration()->_set_stmt_node_index(index,_is_right_child,reducibleFlag);
}

TyvisDeclaration *
TyvisDotAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
						TyvisTypeDefinition *decl_nature ){
  if ( get_prefix()->get_kind() == IIR_FREE_QUANTITY_DECLARATION ) {
    return dynamic_cast<TyvisDeclaration *>(_build_free_quantity_declaration( decl_name, decl_nature ));
  }
  else if ( get_prefix()->get_kind() == IIR_BRANCH_QUANTITY_DECLARATION ) {
    return dynamic_cast<TyvisDeclaration *>(_build_branch_quantity_declaration( decl_name, decl_nature));
  } 
  else {
    ostringstream cerr;
    cerr << "|" << _get_prefix() 
	 << "| - Dot Attribute may only be applied to quantities." << endl;
    return NULL;
  }
}

IIR_Boolean
TyvisDotAttribute::_differential_quantity_found() {
  return TRUE;
}

void
TyvisDotAttribute::_build_differential_quantity_list(dl_list<TyvisDotAttribute> *
                                                         differential_quantity_list) {
  differential_quantity_list->append(this);
}

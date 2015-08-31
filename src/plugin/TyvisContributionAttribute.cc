
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
#include "TyvisContributionAttribute.hh"
#include "TyvisDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisFreeQuantityDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisNatureDeclaration.hh"
#include "TyvisNatureDefinition.hh"
#include "TyvisScalarNatureDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisTerminalDeclaration.hh"

#include "savant/set.hh"
#include "savant/error_func.hh"
#include "savant/StandardPackage.hh"
#include "published_file.hh"
#include "PublishData.hh"

void
TyvisContributionAttribute::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisContributionAttribute::_publish_cc_rvalue" );
  _publish_cc_ams_function(_cc_out, declarations);
}

void
TyvisContributionAttribute::_publish_cc_ams_function(published_file &_cc_out, PublishData *declarations) {
  dynamic_cast<TyvisAttribute*>(this)->_get_implicit_declaration()->_publish_cc_ams_function(_cc_out, declarations);
}

const string &
TyvisContributionAttribute::_get_cc_attribute_name() const {
  static const string retval("contribution");
  return retval;
}

void
TyvisContributionAttribute::_set_stmt_qty_index( IIR_Int32 *index,
                                                 savant::set<TyvisDeclaration> *quantity_set,
                                                 PublishData *declarations,
                                                 PublishData *arch_declarations) {

  if (declarations !=NULL) {
    if (!declarations->in_collection(dynamic_cast<TyvisDeclaration *>(get_prefix()))) {
      declarations->add_declaration(dynamic_cast<TyvisDeclaration *>(get_prefix()));
    }
  }
  if (arch_declarations !=NULL) {
    if (!arch_declarations->in_collection(dynamic_cast<TyvisDeclaration *>(get_prefix()))) {
      arch_declarations->add_declaration(dynamic_cast<TyvisDeclaration *>(get_prefix()));
    }
  }
  dynamic_cast<TyvisAttribute *>(this)->_get_implicit_declaration()->_set_stmt_qty_index(index,quantity_set, declarations, arch_declarations);
}

TyvisDeclaration *
TyvisContributionAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
							 TyvisTypeDefinition *decl_nature ){
  return dynamic_cast<TyvisDeclaration *>(_build_free_quantity_declaration( decl_name,
									    decl_nature ));
}

IIR_Boolean
TyvisContributionAttribute::_contribution_quantity_found() {
  return TRUE;
}

void
TyvisContributionAttribute::_build_contribution_quantity_list(dl_list<TyvisContributionAttribute> *contribution_quantity_list) {
  contribution_quantity_list->append(this);
}

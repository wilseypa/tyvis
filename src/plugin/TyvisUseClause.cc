
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

#include "TyvisDesignFile.hh"
#include "TyvisUseClause.hh"
#include "TyvisName.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisSelectedName.hh"
#include "TyvisSimpleName.hh"
#include "TyvisLibraryDeclaration.hh"

#include "savant/set.hh"
#include "savant/error_func.hh"
#include "published_file.hh"
#include <sstream>

void
TyvisUseClause::_publish_cc_headers( published_file &_cc_out ) {
  ASSERT(_get_package_declaration()->get_kind() == IIR_PACKAGE_DECLARATION);
  _get_package_declaration()->_publish_cc_include( _cc_out );
}

void
TyvisUseClause::_publish_cc_include( published_file &_cc_out ) {
  _publish_cc_headers(_cc_out);
}

void
TyvisUseClause::_publish_cc_package_name( published_file &_cc_out ){
  ASSERT(_get_selected_name() != NULL);
  ASSERT(_get_selected_name()->is_resolved());
  ASSERT(_get_selected_name()->get_kind() == IIR_SELECTED_NAME);

  TyvisDeclaration *pkg_name = _get_package_declaration();
  if(pkg_name != NULL) {
    ASSERT( pkg_name->get_kind() == IIR_PACKAGE_DECLARATION );
    _cc_out << dynamic_cast<TyvisPackageDeclaration *>(pkg_name)->_get_cc_package_name();
  }
}

TyvisName *
TyvisUseClause::_get_selected_name() {
  return dynamic_cast<TyvisName *>(get_selected_name());
}

TyvisDeclaration*
TyvisUseClause::_get_package_declaration() {
  TyvisDeclaration *retval = 0;
  if( dynamic_cast<TyvisSelectedName *>( get_selected_name() ) ){
    retval = dynamic_cast<TyvisSelectedName *>(get_selected_name())->_get_package_declaration();
  }
  return retval;
}

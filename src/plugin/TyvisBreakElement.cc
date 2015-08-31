
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

#include "TyvisBreakElement.hh"
#include "TyvisDeclaration.hh"
#include "TyvisName.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "published_file.hh"

void
TyvisBreakElement::_publish_cc_ams() {}

void
TyvisBreakElement::_publish_cc( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(_get_quantity_name() != NULL);
  ASSERT(_get_expression() != NULL);
  if ( _get_quantity_name()->_get_prefix() != NULL ) {
    _cc_out << "&";
    _get_quantity_name()->_get_prefix()->_publish_cc_lvalue(_cc_out, declarations);
    _cc_out << ", " ;
    _cc_out << "break_expr_func_" << this << "  ";
    if(_get_selector_quantity_name()!= NULL) {
      _cc_out << ", ";
      _get_selector_quantity_name()->_get_prefix()->_publish_cc_value( _cc_out, declarations );
    }
    _cc_out << " );\n";
  }
}


TyvisName *
TyvisBreakElement::_get_quantity_name() {
  return dynamic_cast<TyvisName *>(get_quantity_name());
}

Tyvis *
TyvisBreakElement::_get_expression() {
  return dynamic_cast<Tyvis *>(get_expression());
}

TyvisName *
TyvisBreakElement::_get_selector_quantity_name() {
  return dynamic_cast<TyvisName *>(get_selector_quantity_name());
}

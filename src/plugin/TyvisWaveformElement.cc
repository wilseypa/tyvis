
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
#include "TyvisWaveformElement.hh"
#include "TyvisWaveformElement.hh"
#include "TyvisList.hh"
#include "TyvisDeclaration.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "savant/StandardPackage.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void 
TyvisWaveformElement::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisWaveformElement::_publish_cc_rvalue" );	
  ASSERT( is_resolved() == TRUE );
  // Publish source
  if(_is_null_transaction() == FALSE) {
    if(_get_value()->get_kind() == IIR_SLICE_NAME) {
      _get_value()->_get_prefix_declaration()->_publish_cc_rvalue( _cc_out , declarations );
    } 
    else {
      _get_value()->_publish_cc_rvalue( _cc_out , declarations );
    }
    _cc_out << "," << NL();
  }
  // Publish delay time.
  if( _get_time() != NULL ){
    _get_time()->_publish_cc_rvalue( _cc_out , declarations );
    _cc_out << ",";
  }
  else {
    _cc_out << "SAVANT_ZERO_TIME, ";
  }
  _cc_out << NL();
}

void
TyvisWaveformElement::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {
  _get_value()->_publish_cc_bounds( _cc_out, declarations );
}

Tyvis *
TyvisWaveformElement::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_WaveformElement::get_value());
}

Tyvis *
TyvisWaveformElement::_get_time() {
  return dynamic_cast<Tyvis *>(get_time());
}
IIR_Boolean
TyvisWaveformElement::_is_null_transaction() {
  int retval = -1;
  if( dynamic_cast<TyvisStringLiteral *>( _get_value()) ) {
    ASSERT(_get_value()->get_kind() == IIR_STRING_LITERAL);
    TyvisStringLiteral* string_lit = dynamic_cast<TyvisStringLiteral*>(_get_value());
    retval = IIRBase_TextLiteral::cmp(string_lit, "null");
    if(retval == 0) {
      return TRUE;
    }
  }
  return FALSE;
}

void
TyvisWaveformElement::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_value()->_build_sensitivity_list(sensitivity_list);
}

void
TyvisWaveformElement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_value()->_get_list_of_input_signals(list);
}

void
TyvisWaveformElement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  ASSERT(_get_value() != NULL);
  _get_value()->_build_above_attribute_set(to_build);
}

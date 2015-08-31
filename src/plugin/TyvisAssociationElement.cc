
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
#include "TyvisAssociationElement.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisList.hh"

void
TyvisAssociationElement::_publish_cc_generic_map_aspect( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisAssociationElement::_publish_cc_generic_map_aspect" );

  _report_undefined_fn("_publish_cc_generic_map_aspect( _cc_out, declarations )");
}

void
TyvisAssociationElement::_publish_cc_generic_map_aspect_for_conc_stmts( published_file &_cc_out, PublishData * ){

  CC_REF( _cc_out, "TyvisAssociationElement::_publish_cc_generic_map_aspect_for_conc_stmts" );

  _report_undefined_fn("_publish_cc_generic_map_aspect_for_conc_stmts( _cc_out, declarations )");
}

void 
TyvisAssociationElement::_publish_cc_subprogram_arguments( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisAssociationElement::_publish_cc_subprogram_arguments" );

  _report_undefined_fn("_publish_cc_subprogram_arguments( _cc_out )");
}

IIR_Int32
TyvisAssociationElement::_get_number_of_element_associations() {
  _report_undefined_fn("_get_number_of_element_associations()");
  return 0;
}

Tyvis*
TyvisAssociationElement::_get_actual() {
  return 0;
}

Tyvis* 
TyvisAssociationElement::_get_formal() {
  return dynamic_cast<Tyvis *>(get_formal());
}

IIR_Boolean 
TyvisAssociationElement::_is_globally_static_for_array(){
  _report_undefined_fn("_is_globally_static_for_array()");
  abort();
  return FALSE;
}

IIR_Boolean 
TyvisAssociationElement::_is_globally_static_for_record(){
  _report_undefined_fn("_is_globally_static_for_record");
  abort();
  return FALSE;
}

IIR_Boolean 
TyvisAssociationElement::_is_globally_static_for_function(){
  _report_undefined_fn("_is_globally_static_for_function");
  abort();
  return FALSE;
}

void
TyvisAssociationElement::_get_list_of_input_signals(  savant::set<Tyvis>* list ){
  if( _get_actual() != NULL ){
    _get_actual()->_get_list_of_input_signals(list);
  }
}

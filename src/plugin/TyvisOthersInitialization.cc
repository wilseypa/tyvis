
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

#include "TyvisArrayTypeDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisElementDeclarationList.hh"
#include "TyvisElementDeclaration.hh"
#include "TyvisOthersInitialization.hh"
#include "TyvisTextLiteral.hh"
#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void 
TyvisOthersInitialization::_publish_cc_headers( published_file &_cc_out ) {
  ASSERT(_get_expression() != NULL);
  _get_expression()->_publish_cc_headers( _cc_out );
}

void 
TyvisOthersInitialization::_publish_cc_initialization_value( published_file &_cc_out,
							     PublishData *declarations ) {
  _cc_out << "1," << NL();
  _cc_out << OS("new ElementAssociation(Others,");
  if (_get_expression()->get_kind() == IIR_CONSTANT_DECLARATION) {
    _cc_out << "new ";
    _get_expression()->_publish_cc_rvalue( _cc_out, declarations );
  }
  else {
    _get_expression()->_publish_cc_state_object_init( _cc_out, declarations );
  }
  _cc_out << CS(")");
}

void 
TyvisOthersInitialization::_publish_cc_state_object_init( published_file &_cc_out,
							  PublishData *declarations ) {
  ASSERT(_get_subtype() != NULL);
  _cc_out << " new ";
  _get_expression()->_publish_cc_state_object_init( _cc_out, declarations );
  _cc_out << ",";
}

void
TyvisOthersInitialization::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisOthersInitialization::_publish_cc_rvalue( published_file &_cc_out,
					       PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisOthersInitialization::_publish_cc_rvalue" );	
  _get_expression()->_publish_cc_rvalue( _cc_out, declarations );
}

void 
TyvisOthersInitialization::_add_decl_into_declaration_collection(PublishData *declarations) {
  _get_expression()->_add_decl_into_declaration_collection(declarations);
}

Tyvis *
TyvisOthersInitialization::_get_expression() {
  return dynamic_cast<Tyvis *>(get_expression());
}

void 
TyvisOthersInitialization::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  ASSERT(_get_expression() != NULL);
  _get_expression()->_build_sensitivity_list(sensitivity_list);
}

void 
TyvisOthersInitialization::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  ASSERT(_get_expression() != NULL);
  _get_expression()->_get_list_of_input_signals(list);
}

IIR_Boolean 
TyvisOthersInitialization::_is_globally_static(){
  ASSERT( _get_expression() != NULL );

  return _get_expression()->_is_globally_static();  
}

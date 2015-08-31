
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

#include "TyvisTypeConversion.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisAssociationList.hh"
#include "TyvisPosAttribute.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisIdentifier.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void
TyvisTypeConversion::_publish_cc_headers( published_file &_cc_out ) {
  _get_type_mark()->_publish_cc_include( _cc_out );
}

void
TyvisTypeConversion::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisTypeConversion::_publish_cc" );
  _get_expression()->_publish_cc_rvalue( _cc_out , declarations );
}

void
TyvisTypeConversion::_publish_cc_ams_function(published_file &_cc_out, PublishData *) {
  TyvisPosAttribute *pos_attr = NULL;
  TyvisTextLiteral *func_declarator = NULL;
  if (get_expression()->get_kind() == IIR_POS_ATTRIBUTE) {
    pos_attr = dynamic_cast<TyvisPosAttribute*>(get_expression());
    if (pos_attr->get_suffix()->get_kind() == IIR_FUNCTION_CALL) {
      func_declarator =
        dynamic_cast<TyvisFunctionCall*>(pos_attr->get_suffix())->_get_implementation()->_get_declarator();
      if ((IIRBase_Identifier::cmp(func_declarator, "now") == 0)) {
        if(_get_currently_publishing_unit() == IIR::SIMULTANEOUS_STATEMENT) {
         _cc_out << "  new equationNode('T',0,node"
                 << (int)(_stmt_node_index/2) << ");" << NL();
        }
      }
    }
  }
}

TyvisTypeDefinition *
TyvisTypeConversion::_get_type_mark() {
  return dynamic_cast<TyvisTypeDefinition *>(get_type_mark());
}

Tyvis *
TyvisTypeConversion::_get_expression() {
  return dynamic_cast<Tyvis *>(get_expression());
}

void
TyvisTypeConversion::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_expression()->_build_sensitivity_list(sensitivity_list);
}

IIR_Boolean 
TyvisTypeConversion::_is_globally_static_primary(){
  ASSERT( _get_expression() != NULL );
  ASSERT( _get_expression()->is_resolved() == TRUE );

  return _get_expression()->_is_globally_static();
}

void
TyvisTypeConversion::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_expression()->_get_list_of_input_signals(list);
}
  
void
TyvisTypeConversion::_set_stmt_node_index(IIR_Int32 *index, bool _is_right_child, bool &) {
  if (_is_right_child == true) {
   _stmt_node_index = 2 * (*index)+ 1;
  }
  else {
   _stmt_node_index = 2 * (*index);
  }
}



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

#include "TyvisChoice.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisLiteral.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "published_file.hh"
#include <sstream>

void 
TyvisChoice::_publish_cc( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisChoice::_publish_cc" );

  //The publishing of choices will be organised once the way the
  // the different types of choices that will be returned by front-end
  // is known
  if( _get_value()->get_kind() == IIR_ENUMERATION_LITERAL || 
      dynamic_cast<TyvisLiteral *>(_get_value()) ) {
    _get_value()->_publish_cc_condition( _cc_out, declarations );
  }
  else if(_get_value()->is_subtype()) {
    //choice is a discrete range
    _publish_cc_discrete_range( _cc_out, declarations );
  }
  else {
    _get_value()->_publish_cc_rvalue( _cc_out, declarations );
  }
}

void 
TyvisChoice::_publish_cc_discrete_range( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisChoice::_publish_cc_discrete_range" );

  //The publishing of choices will be organised once the way the
  // the different types of choices that will be returned by front-end
  // is known
  TyvisScalarTypeDefinition *sub_type = dynamic_cast<TyvisScalarTypeDefinition*>(get_value());
  _cc_out << OS("(");
  _get_current_publish_node()->_publish_cc_rvalue( _cc_out, declarations );
  if( sub_type->is_ascending_range() ) {
    _cc_out << ".vhdlGreaterThanEqual";
  }
  else{
    _cc_out << ".vhdlLessThanEqual";
  }
 
  _cc_out << OS("(");
  sub_type->_get_left()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << CS(")");
  _cc_out << OS(".vhdlAnd(");
  _get_current_publish_node()->_publish_cc_rvalue( _cc_out, declarations );
  if( sub_type->is_ascending_range() ) {
    _cc_out << ".vhdlLessThanEqual";
  }
  else{
    _cc_out << ".vhdlGreaterThanEqual";
  }
  _cc_out << OS("(");
  sub_type->_get_right()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << CS(")");
  _cc_out << CS(")");
  _cc_out << CS(")");
}

Tyvis *
TyvisChoice::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_Choice::get_value());
}

void
TyvisChoice::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list){
  _get_value()->_build_sensitivity_list(sensitivity_list);
}

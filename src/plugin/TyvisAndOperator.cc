
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

#include "TyvisAndOperator.hh"
#include "TyvisTypeDefinition.hh"
#include "savant/savant.hh"
#include "published_file.hh"

const string &
TyvisAndOperator::_get_cc_operator_name() const {
  static const string operator_name = "vhdlAnd";
  return operator_name;
}

void
TyvisAndOperator::_publish_cc_xvalue( published_file &_cc_out,
                                      PublishData *declarations,
                                      void (Tyvis::*_publish_function)( published_file &, PublishData * )) {
  if( _get_subtype()->_is_bit_type() ||
      _get_subtype()->_is_boolean_type() ){
    _cc_out << "((";
    (_get_left_operand()->*_publish_function)( _cc_out, declarations );
    _cc_out << " == SAVANT_BOOLEAN_FALSE)"
	    << " ? SAVANT_BOOLEAN_FALSE : "
	    << "SAVANT_BOOLEAN_TRUE." 
	    << _get_cc_operator_name() << "(";
    (_get_right_operand()->*_publish_function)( _cc_out, declarations );
    _cc_out << "))";
  }
  else{
    TyvisDyadicOperator::_publish_cc_xvalue( _cc_out, declarations, _publish_function );
  }
}

void
TyvisAndOperator::_publish_cc_ams_operator_name(published_file &_cc_out) {
  _cc_out << " && ";
} 

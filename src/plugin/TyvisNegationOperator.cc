
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

#include "TyvisNegationOperator.hh"
#include "savant/savant.hh"
#include "published_file.hh"

const string &
TyvisNegationOperator::_get_cc_operator_name() const {
  static const string operator_name = "vhdlUnaryMinus";
  return operator_name;
}

void 
TyvisNegationOperator::_publish_cc_universal_value( published_file &_cc_out, PublishData *declarations ) {
  switch (get_operand()->get_kind()) {
  case  IIR_INTEGER_LITERAL:
  case  IIR_INTEGER_LITERAL32:
  case  IIR_INTEGER_LITERAL64:
    _cc_out << "-1 * (int) ";
    _get_operand()->_publish_cc_universal_value( _cc_out, declarations );
    break;
  case  IIR_FLOATING_POINT_LITERAL:
  case  IIR_FLOATING_POINT_LITERAL32:
  case  IIR_FLOATING_POINT_LITERAL64:
    _cc_out << "-1.0 * ";
    _get_operand()->_publish_cc_universal_value( _cc_out, declarations );
    break;
  default:
    _cc_out << "-";
    _get_operand()->_publish_cc_lvalue( _cc_out , declarations );
    break;
  }
}

void
TyvisNegationOperator::_publish_cc_ams_operator_name( published_file &_cc_out ) {
  _cc_out << "-";
}

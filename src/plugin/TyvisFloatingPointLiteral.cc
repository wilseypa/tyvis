
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
#include "TyvisFloatingPointLiteral.hh"
#include "TyvisFloatingSubtypeDefinition.hh"
#include "savant/StandardPackage.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>
#include <iomanip>

void 
TyvisFloatingPointLiteral::_publish_cc_data( published_file &_cc_out, PublishData * ) {
  _cc_out << print_value(10);
}

void 
TyvisFloatingPointLiteral::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisFloatingPointLiteral::_publish_cc_rvalue" );	
  _publish_cc_universal_value( _cc_out, declarations );
}

void 
TyvisFloatingPointLiteral::_publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations ) {
  _cc_out << "new ";
  _publish_cc_rvalue( _cc_out , declarations );
  _cc_out << ",";
}

void
TyvisFloatingPointLiteral::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisFloatingPointLiteral::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_data( _cc_out, declarations );
}

void 
TyvisFloatingPointLiteral::_publish_cc_universal_value( published_file &_cc_out, PublishData *declarations ) {
  _cc_out << "UniversalReal(";
  _publish_cc_data( _cc_out, declarations );
  _cc_out << ")";
}

void 
TyvisFloatingPointLiteral::_publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_universal_value( _cc_out, declarations );
}

void 
TyvisFloatingPointLiteral::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  _publish_cc_data( _cc_out, declarations );
}

void
TyvisFloatingPointLiteral::_publish_cc_bounds( published_file &_cc_out, PublishData * ) {
  _cc_out << "defaultInfo()";
}

void
TyvisFloatingPointLiteral::_publish_cc_headers( published_file &_cc_out ) {
  //_publish_cc_include( _cc_out, "tyvis/RealType.hh" );
}

void
TyvisFloatingPointLiteral::_publish_cc_ams_function( published_file &_cc_out, PublishData *declarations ) {

  if(_get_currently_publishing_unit() == SIMULTANEOUS_IF ) {
    _publish_cc_rvalue( _cc_out , declarations );
  }
  else {
    _cc_out << "  new equationNode('N',";
    _publish_cc_data(_cc_out, declarations);
    _cc_out << ",node" << (int)(_stmt_node_index/2) << ");" << NL();
  }
}

IIR_FP64
TyvisFloatingPointLiteral::_get_value() {
  register int i;
  IIR_FP64 value = 0.0;
  IIR_Int32 base = get_base();
  IIR_Char* man = get_mantissa();
  int sign = 1;

  ASSERT(get_mantissa_length() > 0);

  if (man[0] == '-') {
    sign = -1;
    value = 0.0;
  }
  else {
    value = _char_to_int(man[0]);
  }
  // calculate part left of the point
  for (i = 1; i < get_mantissa_length(); i++) {
    if (man[i] == '.') {
      i++;
      break;
    }
    value = value * base + sign * _char_to_int(man[i]);
  }
  // add on the part right of the point
  double divisor = base;
  for (/* continue with old value of i */; i < get_mantissa_length(); i++) {
    value = value + sign * _char_to_int(man[i])/divisor;
    divisor = divisor * base;
  }
  if (get_exponent_length() > 0) {
    IIR_Char* exp = get_exponent();
    IIR_Int32 exp_val;

    if (exp[0] == '+' || exp[0] == '-') {
      i = 2;
      exp_val = _char_to_int(exp[1]);
    }
    else {
      i = 1;
      exp_val = _char_to_int(exp[0]);
    }
    for (/* i already set */; i < get_exponent_length(); i++) {
      exp_val = exp_val * 10 + _char_to_int(exp[i]);
    }

    // exp_val is now the base-10 representation of the exponent.  The
    // value of this node is value * base^exp_val.  Care must be taken if
    // the exponent is < 0.
    if (exp[0] == '-') {
      for (i = 0; i < exp_val; i++) {
	value = value / base;
      }
    }
    else {
      for (i = 0; i < exp_val; i++) {
	value = value * base;
      }
    }
  }
  return value;
}

void
TyvisFloatingPointLiteral::_set_stmt_node_index(IIR_Int32 *index,
                                                bool _is_right_child,
                                                bool &) {
  if ( _is_right_child ) {
   _stmt_node_index = 2 * (*index)+ 1;
  }
  else {
   _stmt_node_index = 2 * (*index);
  }
}


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
#include "TyvisTypeDefinition.hh"
#include "TyvisConcatenationOperator.hh"
#include "TyvisArrayTypeDefinition.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"

void 
TyvisConcatenationOperator::_publish_cc_xvalue( published_file &_cc_out,
                                                PublishData *declarations,
                                                void (Tyvis::*_publish_function)( published_file &, PublishData *declarations )) {
  
  CC_REF( _cc_out, "TyvisConcatenationOperator::_publish_cc" );

  (_get_left_operand()->*_publish_function)( _cc_out, declarations );
  _cc_out << OS(".vhdlConcatenate(");
  (_get_right_operand()->*_publish_function)( _cc_out, declarations );
  _cc_out << CS(")");
}

void 
TyvisConcatenationOperator::_publish_cc_bounds( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisConcatenationOperator::_publish_cc_bounds" );
  _cc_out << "nullInfo()";
}

const string &
TyvisConcatenationOperator::_get_cc_operator_name() const {
  static const string operator_name = "savantConcatenate";
  return operator_name;
}

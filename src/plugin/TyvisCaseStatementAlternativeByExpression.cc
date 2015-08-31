
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
#include "TyvisCaseStatementAlternativeByExpression.hh"
#include "TyvisChoice.hh"
#include "TyvisSequentialStatementList.hh"
#include "published_file.hh"
#include "savant/savant.hh"

void
TyvisCaseStatementAlternativeByExpression::_publish_cc( published_file &_cc_out,
							PublishData *declarations ) {
  // XXX this code assumes that each case choice is exactly one value.
  // It will have to be modified for ranges.
    CC_REF( _cc_out, "TyvisCaseStatementAlternativeByExpression::_publish_cc" );
  if(_get_choice()->_get_value()->is_subtype()) {
    //choice is a discrete range
    _cc_out << "else if ( SAVANT_BOOLEAN_TRUE == ";    
    _get_choice()->_publish_cc( _cc_out, declarations );
    _cc_out << ")";
    _cc_out << OS("{");
    _get_sequence_of_statements()->_publish_cc( _cc_out, declarations );
    _cc_out << CS("}");
  }
  else {
    _cc_out << "else if " << OS("( SAVANT_BOOLEAN_TRUE ==");
    _get_current_publish_node()->_publish_cc_rvalue( _cc_out, declarations );
    _cc_out << OS(".vhdlEqual(");
    _get_choice()->_publish_cc( _cc_out, declarations );
    _cc_out << CS(")")
    << CS(")")
    << OS("{");
    _get_sequence_of_statements()->_publish_cc( _cc_out, declarations);
    _cc_out << CS("}");
  }
}

TyvisChoice*
TyvisCaseStatementAlternativeByExpression::_get_choice() {
  return dynamic_cast<TyvisChoice *>(get_choice());
}


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
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisProcedureDeclaration.hh"
#include "TyvisReturnStatement.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/error_func.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>

void 
TyvisReturnStatement::_publish_cc( published_file &_cc_out, PublishData *declarations ) {
  if( _get_allocate_return_value() == TRUE) {
    _publish_cc_resolution_fn_return( _cc_out, declarations );
  } 
  else {
    if (_get_label() != NULL) {
      _get_label()->_publish_cc_lvalue( _cc_out, declarations );
      _cc_out << ": ";
    } // if

    _cc_out << "return ";

    if ( _get_return_expression() != NULL ) {
      ASSERT( _get_return_expression()->is_resolved() );
      _get_return_expression()->_publish_cc_rvalue( _cc_out, declarations );
      _cc_out << ".clone()"; 
    } 
    else {
      ASSERT( dynamic_cast<TyvisProcedureDeclaration *>(get_enclosing_subprogram()) != NULL );
      _cc_out << "NORMAL_RETURN";
    }
    _cc_out << ";" << NL();
  } // else
}

void 
TyvisReturnStatement::_publish_cc_resolution_fn_return( published_file &_cc_out, PublishData *declarations ){
  _cc_out << "return new ";
  _get_return_expression()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << ";" << NL();
}

Tyvis *
TyvisReturnStatement::_get_return_expression() {
  return dynamic_cast<Tyvis *>(get_return_expression());
}

TyvisSubprogramDeclaration*
TyvisReturnStatement::_get_enclosing_subprogram() {
  return dynamic_cast<TyvisSubprogramDeclaration *>(get_enclosing_subprogram());
}

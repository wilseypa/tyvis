
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

#include "TyvisInterfaceList.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisTextLiteral.hh"
#include "published_file.hh"

void
TyvisInterfaceList::_publish_cc_decl( published_file &(_cc_out),
				      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisInterfaceList::_publish_cc_decl" );
  _publish_cc_list( _cc_out, declarations,
		    &Tyvis::_publish_cc_decl,
		    NOT_LAST_ELEMENT,
		    "," );
}

void
TyvisInterfaceList::_publish_cc_decl_subprogram_args( published_file &_cc_out,
						      PublishData *declarations){
  CC_REF( _cc_out, "TyvisInterfaceList::_publish_cc_decl_subprogram_args" );
  _publish_cc_list( _cc_out, declarations,
		    &TyvisInterfaceDeclaration::_publish_cc_decl_subprogram_arg,
		    NOT_LAST_ELEMENT,
		    "," );
}

void
TyvisInterfaceList::_publish_cc_decl_subprogram_arg_vars( published_file &_cc_out,
							  PublishData *declarations ){
  CC_REF( _cc_out, "TyvisInterfaceList::_publish_cc_decl_subprogram_args" );
  _publish_cc_list( _cc_out, declarations,
		    &TyvisInterfaceDeclaration::_publish_cc_subprogram_arg_var,
		    EVEN_LAST_ELEMENT,
		    ";" );
}

void
TyvisInterfaceList::_publish_cc_list( published_file &_cc_out,
				      PublishData *declarations,
				      void (TyvisInterfaceDeclaration::*publish_method)(published_file &,											PublishData *),
				      SeparatorMode separatorMode,
				      const string &separator ){
  CC_REF( _cc_out, "TyvisInterfaceList::_publish_cc_list" );
  TyvisInterfaceDeclaration *current = dynamic_cast<TyvisInterfaceDeclaration *>(first());
  while( current ){
    (current->*publish_method)( _cc_out, declarations );
    current = dynamic_cast<TyvisInterfaceDeclaration *>(successor(current));
    _publish_cc_separator( _cc_out, current != 0, separatorMode, separator );
  }
}

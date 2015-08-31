
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
#include "TyvisDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisNatureDefinition.hh"
#include "TyvisTextLiteral.hh"

#include "savant/savant.hh"
#include "published_file.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include "savant/StandardPackage.hh"
#include <stdlib.h>

void
TyvisNatureDefinition::_publish_cc_type_info_xxx( published_file &_cc_out,
						  PublishData * ){
  if( is_scalar_type() == TRUE ) {
    _cc_out << _get_cc_tyvis_type();
  }
  else {
    if (is_array_type() == TRUE) {
      _cc_out << "ArrayNature";
    }
    else {
      cerr << "Error - TyvisNatureDefinition::_publish_cc_type_name(ostream &) encountered an unknown/unhandled type.\n";
      abort();
    }
  }
}

TyvisTextLiteral *
TyvisNatureDefinition::_get_mangled_declarator(){
  if( _get_declaration() != NULL ){
    return _get_declaration()->_get_mangled_declarator();
  }
  else {
    char *string = "<ANONYMOUS>";
    return dynamic_cast<TyvisTextLiteral *>(TyvisIdentifier::get( string, strlen( string ), get_design_file()->get_class_factory() ));
  }
}

TyvisTypeDefinition *
TyvisNatureDefinition::_get_element_subtype(){
  _report_undefined_fn("_get_element_subtype()");
  return NULL;
}

TyvisDeclaration *
TyvisNatureDefinition::_get_declaration() {
  return dynamic_cast<TyvisDeclaration *>(get_declaration());
}

TyvisTypeDefinition *
TyvisNatureDefinition::_get_final_subtype() {
  _report_undefined_fn("_get_final_subtype()");
  return  NULL;
}

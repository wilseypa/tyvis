
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

#include "TyvisNatureDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "published_file.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include "TyvisTerminalDeclaration.hh"
#include "TyvisNatureDeclaration.hh"
#include "TyvisScalarNatureDefinition.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisArrayNatureDefinition.hh"
#include <iostream>

TyvisNatureDeclaration::TyvisNatureDeclaration() {
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisNatureDeclaration::~TyvisNatureDeclaration() {
  delete get_attributes();
}

void
TyvisNatureDeclaration::_publish_cc_lvalue( published_file &_cc_out, PublishData * ) {
  _cc_out << *_get_mangled_declarator();
}

TyvisTerminalDeclaration*
TyvisNatureDeclaration::_get_reference_terminal() {
  return dynamic_cast<TyvisTerminalDeclaration *>(get_reference_terminal());
}

TyvisAttributeSpecificationList *
TyvisNatureDeclaration::_get_attributes() {
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

TyvisNatureDefinition *
TyvisNatureDeclaration::_get_nature() {
  return dynamic_cast<TyvisNatureDefinition *>(get_nature());
}

TyvisTypeDefinition*
TyvisNatureDeclaration::_get_final_subtype(){
  ASSERT( _get_nature() != NULL );

  return _get_nature()->_get_final_subtype();
}

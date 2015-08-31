
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

#include "TyvisAccessTypeDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisElementDeclaration.hh"
#include "TyvisElementDeclarationList.hh"
#include "TyvisRecordTypeDefinition.hh"
#include "TyvisSimpleName.hh"
#include "TyvisTextLiteral.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>

void
TyvisElementDeclaration::_publish_cc_universal_value( published_file &_cc_out, PublishData * ){

  CC_REF( _cc_out, "TyvisElementDeclaration::_publish_cc_universal_value" );

  ASSERT(_get_declarative_region() != NULL);
  ASSERT(_get_declarative_region()->get_kind() == IIR_RECORD_TYPE_DEFINITION);
  
  _cc_out << _get_field_number();
}

void 
TyvisElementDeclaration::_publish_cc_lvalue( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisElementDeclaration::_publish_cc" );

  _cc_out << *_get_mangled_declarator();
}

void
TyvisElementDeclaration::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisElementDeclaration::_publish_cc_elaborate" );

  _publish_cc_lvalue( _cc_out , declarations );
}

void
TyvisElementDeclaration::_publish_cc_include( published_file &_cc_out ){
  _get_subtype()->_publish_cc_include( _cc_out );
}

int
TyvisElementDeclaration::_get_field_number()
{
  TyvisElementDeclarationList *field_list = (dynamic_cast<TyvisRecordTypeDefinition *>( _get_declarative_region())->_get_element_declarations());
  TyvisElementDeclaration *field = dynamic_cast<TyvisElementDeclaration *>( field_list->first());
  int field_number = 1;
  
  while (field != NULL) {
    if (field == dynamic_cast<TyvisElementDeclaration *>( this) ) {
      break;
    }

    field = dynamic_cast<TyvisElementDeclaration *>( field_list->successor(field));
    field_number++;
  }

  return field_number;
}

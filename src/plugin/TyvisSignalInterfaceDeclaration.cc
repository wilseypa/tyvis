
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

#include "TyvisAttribute.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisConcurrentStatement.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/set.hh"
#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

TyvisSignalInterfaceDeclaration::TyvisSignalInterfaceDeclaration() {
  _driver_addition_flag = FALSE;
}

void
TyvisSignalInterfaceDeclaration::_publish_cc_object_name( published_file &_cc_out,
							  PublishData * ){
  _cc_out << *_get_mangled_declarator();
}

void
TyvisSignalInterfaceDeclaration::_publish_cc_elaborate( published_file &_cc_out,
							PublishData *declarations ){
  CC_REF( _cc_out, "TyvisSignalInterfaceDeclaration::_publish_cc_elaborate" );
    _publish_cc_signal_net_info( _cc_out, DECLARATION, declarations );
}

void
TyvisSignalInterfaceDeclaration::_publish_cc_include( published_file &_cc_out ){
  _publish_cc_include( _cc_out, "tyvis/Signal.hh" );
}

void
TyvisSignalInterfaceDeclaration::_publish_cc_sigdest( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSignalInterfaceDeclaration::_publish_cc_sigdest" );
  
  ASSERT( declarations != NULL );
  ASSERT( get_declarator()->get_kind() == IIR_IDENTIFIER );

  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  _cc_out << "sate.current->"
          << *_get_mangled_declarator();
}

void 
TyvisSignalInterfaceDeclaration::_publish_cc_decl( published_file &(_cc_out), PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSignalInterfaceDeclaration::_publish_cc_decl" );
  
  if(is_implicit_declaration()) {
    _get_attribute_name()->_publish_cc_necessary_decl_in_state( _cc_out );
  }
  else {
    TyvisObjectDeclaration::_publish_cc_decl( _cc_out , declarations );
  }
}

void
TyvisSignalInterfaceDeclaration::_publish_cc_data( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisSignalInterfaceDeclaration::_publish_cc_data" );
  
  _cc_out << "getState()->"
          << *_get_mangled_declarator();
}

void 
TyvisSignalInterfaceDeclaration::_publish_cc_lvalue( published_file &(_cc_out), PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSignalInterfaceDeclaration::_publish_cc_value" );
  ASSERT( get_declarator()->get_kind() == IIR_IDENTIFIER );

  if ((declarations != NULL) && (!declarations->in_collection(this))) {
    declarations->add_declaration(this);
  }
  TyvisDeclaration::_set_scoping_prefix();
  Tyvis::_publish_cc_prefix_string( _cc_out );
  _cc_out << *_get_mangled_declarator();
  TyvisDeclaration::_reset_scoping_prefix();
}

void
TyvisSignalInterfaceDeclaration::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSignalInterfaceDeclaration::_publish_cc_wait_data" );
  
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  _cc_out << "s->"
          << *_get_mangled_declarator();
}

const string
TyvisSignalInterfaceDeclaration::_get_cc_object_type() {
  return "Signal";
}

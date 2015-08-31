
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
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLastValueAttribute.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/StandardPackage.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"

void 
TyvisLastValueAttribute::_publish_cc_rvalue( published_file &_cc_out,
					     PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisLastValueAttribute::_publish_cc_rvalue" );	
  _publish_cc_signal_attribute( _cc_out, "LastValue", declarations );
}

void
TyvisLastValueAttribute::_publish_cc_necessary_decl_in_state( published_file &_cc_out,
							      PublishData *declarations ) {
  _publish_cc_necessary_signal_decl_in_state( _cc_out, LAST_VALUE, "_last_value", declarations );
}

void
TyvisLastValueAttribute::_publish_cc_init_val( published_file &_cc_out, PublishData * ) { 
  _cc_out << "UniversalInteger(0)";
}

void
TyvisLastValueAttribute::_publish_cc_name_in_caps( published_file &_cc_out ) {
  _cc_out << "LAST_VALUE";
}

void
TyvisLastValueAttribute::_publish_cc_attrib_type( published_file &_cc_out ) {
  _cc_out << "function";
}

void
TyvisLastValueAttribute::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  ASSERT(_get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );
  ASSERT(_get_prefix()->is_signal() == TRUE );

  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << "_last_value";
}

void
TyvisLastValueAttribute::_publish_cc_sigtype( published_file &_cc_out ) {
  _cc_out << "I_LAST_VALUE";
}

void
TyvisLastValueAttribute::_publish_cc_state_object_init( published_file &_cc_out,
							PublishData *declarations ) {
  _publish_cc_necessary_signal_state_object_init( _cc_out, LAST_VALUE,
						  "_last_value", declarations );
}

void
TyvisLastValueAttribute::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisLastValueAttribute::_publish_cc_neccessary_decl_init( published_file &_cc_out,
							   PublishData *declarations ) {
  _publish_cc_necessary_signal_init( _cc_out, LAST_VALUE, "_last_value", declarations );
}

void
TyvisLastValueAttribute::_publish_cc_necessary_copying( published_file &_cc_out,
							PublishData *declarations ) {
  _publish_cc_necessary_attribute_copying( _cc_out, LAST_VALUE, "_last_value", declarations);
}

void
TyvisLastValueAttribute::_publish_cc_read_or_write( published_file &_cc_out, 
                                                           const string &fName, 
                                                           const string &sName,
                                                           PublishData *declarations) {
  _publish_cc_attribute_read_or_write( _cc_out, LAST_VALUE, "_last_value", fName, sName, declarations );
}

const string &
TyvisLastValueAttribute::_get_cc_attribute_name() const {
  static const string retval("last_value");
  return retval;
}

TyvisDeclaration *
TyvisLastValueAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
						      TyvisTypeDefinition *decl_type ){ 
  return _build_function_declaration( decl_name, decl_type );
}

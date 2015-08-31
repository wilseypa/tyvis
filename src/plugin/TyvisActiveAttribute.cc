
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
#include "TyvisDesignFile.hh"
#include "TyvisActiveAttribute.hh"
#include "TyvisIdentifier.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "savant/StandardPackage.hh"
#include "published_file.hh"

void 
TyvisActiveAttribute::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisActiveAttribute::_publish_cc" );

  _publish_cc_signal_attribute( _cc_out, "Active", declarations );
}

void
TyvisActiveAttribute::_publish_cc_necessary_decl_in_state( published_file &_cc_out,
							   PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisActiveAttribute::_publish_cc_necessary_decl_in_state" );

  _publish_cc_necessary_signal_decl_in_state( _cc_out, LAST_ACTIVE, "_last_active", declarations);
  _publish_cc_necessary_signal_decl_in_state( _cc_out, ACTIVE, "_active", declarations );
}

void
TyvisActiveAttribute::_publish_cc_init_val( published_file &_cc_out ) { 
  _cc_out << "true";
}

void
TyvisActiveAttribute::_publish_cc_name_in_caps( published_file &_cc_out ) {
  _cc_out << "ACTIVE";
}

void
TyvisActiveAttribute::_publish_cc_attrib_type( published_file &_cc_out ) {
  _cc_out << "function";
}

void
TyvisActiveAttribute::_publish_cc_necessary_decl_init( published_file &_cc_out,
						       PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisActiveAttribute::_publish_cc_necessary_decl_init" );

  ASSERT( dynamic_cast<Tyvis *>(get_prefix()) );
  _publish_cc_necessary_signal_init( _cc_out, LAST_ACTIVE, "_last_active", declarations );
}

void
TyvisActiveAttribute::_publish_cc_elaborate( published_file &_cc_out, PublishData * ){

  CC_REF( _cc_out, "TyvisActiveAttribute::_publish_cc_elaborate" );
  ASSERT( dynamic_cast<Tyvis *>(get_prefix()) );
}

void
TyvisActiveAttribute::_publish_cc_sigtype( published_file &_cc_out ) {
  _cc_out << " I_ACTIVE;\n";
}

void
TyvisActiveAttribute::_publish_cc_state_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisActiveAttribute::_publish_cc_state_object_init" );

  _publish_cc_necessary_signal_state_object_init( _cc_out, LAST_ACTIVE,
						  "_last_active", declarations );
  _publish_cc_necessary_signal_state_object_init( _cc_out, ACTIVE,
						  "_active", declarations, TRUE );
}

void
TyvisActiveAttribute::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisActiveAttribute::_publish_cc_init( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisActiveAttribute::_publish_cc_init" );

  _publish_cc_necessary_signal_init( _cc_out, LAST_ACTIVE, "_last_active", declarations );
  _publish_cc_necessary_signal_init( _cc_out, ACTIVE, "_active", declarations, TRUE );
}

void
TyvisActiveAttribute::_publish_cc_necessary_copying( published_file &_cc_out,
						     PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisActiveAttribute::_publish_cc_necessary_copying" );

  _publish_cc_necessary_attribute_copying( _cc_out, LAST_ACTIVE, "_last_active", declarations);
  _publish_cc_necessary_attribute_copying( _cc_out, ACTIVE, "_active", declarations);
}

void
TyvisActiveAttribute::_publish_cc_read_or_write( published_file &_cc_out,
						 const string &functionName, 
						 const string &streamName,
						 PublishData *declarations) {
  CC_REF( _cc_out, "TyvisActiveAttribute::_publish_cc_read_or_write" );

  _publish_cc_attribute_read_or_write(_cc_out, 
				      LAST_ACTIVE, 
				      "_last_active", 
				      functionName, 
				      streamName,
                                      declarations);

  _publish_cc_attribute_read_or_write( _cc_out, 
				       ACTIVE, 
				       "_active", 
				       functionName, 
				       streamName,
                                       declarations);
}

const string &
TyvisActiveAttribute::_get_cc_attribute_name() const {
  static const string retval("active");
  return retval;
}

TyvisDeclaration *
TyvisActiveAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
						   TyvisTypeDefinition *decl_type ){
  return _build_signal_declaration( decl_name, decl_type );
}

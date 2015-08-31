
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
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisIdentifier.hh"
#include "TyvisPhysicalLiteral.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisQuietAttribute.hh"

#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void
TyvisQuietAttribute::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisQuietAttribute::_publish_cc_rvalue" );	
  _publish_cc_signal_attribute( _cc_out, "Quiet", declarations, TRUE );
}

void
TyvisQuietAttribute::_publish_cc_necessary_decl_in_state( published_file &_cc_out,
							  PublishData *declarations ) {
  _publish_cc_necessary_signal_decl_in_state( _cc_out, QUIET, "_quiet", declarations);
  _publish_cc_necessary_signal_decl_in_state( _cc_out, LAST_ACTIVE, "_last_active", declarations);
}

void
TyvisQuietAttribute::_publish_cc_init_val( published_file &_cc_out ) { 
  _cc_out << "true";
}

void
TyvisQuietAttribute::_publish_cc_name_in_caps( published_file &_cc_out ) {
  _cc_out << "QUIET";
}

void
TyvisQuietAttribute::_publish_cc_attrib_type( published_file &_cc_out ) {
  _cc_out << "signal";
}

void
TyvisQuietAttribute::_publish_cc_necessary_decl_init( published_file &_cc_out,
						      PublishData *declarations ) {
  _publish_cc_necessary_signal_init( _cc_out, QUIET, "_quiet", declarations, TRUE );
  _publish_cc_necessary_signal_init( _cc_out, LAST_ACTIVE, "_last_active", declarations );
}

void
TyvisQuietAttribute::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << "_quiet";
}

void
TyvisQuietAttribute::_publish_cc_sigtype( published_file &_cc_out ) {
  _cc_out << " I_QUIET;\n";
}

void
TyvisQuietAttribute::_publish_cc_type_cast( published_file &_cc_out ) {
  _cc_out << "(Signal<UniversalInteger>* ) ";
}

void 
TyvisQuietAttribute::_publish_cc_state_object_init( published_file &_cc_out,
						    PublishData *declarations ) {
  _publish_cc_necessary_signal_state_object_init( _cc_out, QUIET, "_quiet", declarations, TRUE );
  _publish_cc_necessary_signal_state_object_init( _cc_out, LAST_ACTIVE,
						  "_last_active", declarations );
}

void
TyvisQuietAttribute::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisQuietAttribute::_publish_cc_necessary_copying( published_file &_cc_out,
						    PublishData *declarations ) {
  _publish_cc_necessary_attribute_copying( _cc_out, QUIET, "_quiet", declarations );
  _publish_cc_necessary_attribute_copying( _cc_out, LAST_ACTIVE, "_last_active", declarations );
}

void 
TyvisQuietAttribute::_publish_cc_read_or_write( published_file &_cc_out,
                                                       const string &fName, 
                                                       const string &sName,
                                                       PublishData *declarations) {
  _publish_cc_attribute_read_or_write( _cc_out, QUIET, "_quiet", fName, sName, declarations);
  _publish_cc_attribute_read_or_write( _cc_out, LAST_ACTIVE, "_last_active", fName, sName, declarations);
}

const string &
TyvisQuietAttribute::_get_cc_attribute_name() const {
  static const string retval("quiet");
  return retval;
}

TyvisDeclaration *
TyvisQuietAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
                                                       TyvisTypeDefinition *decl_type ){ 
  return dynamic_cast<TyvisDeclaration*>(_build_signal_declaration( decl_name, decl_type ));
}


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
#include "TyvisStableAttribute.hh"
#include "TyvisIdentifier.hh"
#include "TyvisPhysicalLiteral.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void 
TyvisStableAttribute::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisStableAttribute::_publish_cc_rvalue" );	
  _publish_cc_signal_attribute( _cc_out, "Stable", declarations ,TRUE );
}

void
TyvisStableAttribute::_publish_cc_necessary_decl_in_state( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_necessary_signal_decl_in_state( _cc_out, STABLE, "_stable", declarations);
  _publish_cc_necessary_signal_decl_in_state( _cc_out, LAST_EVENT, "_last_event", declarations);
}

void
TyvisStableAttribute::_publish_cc_init_val( published_file &_cc_out ) { 
  _cc_out << "true";
}

void
TyvisStableAttribute::_publish_cc_name_in_caps( published_file &_cc_out ) {
  _cc_out << "STABLE";
}

void
TyvisStableAttribute::_publish_cc_attrib_type( published_file &_cc_out ) {
  _cc_out << "signal";
}

void
TyvisStableAttribute::_publish_cc_necessary_decl_init( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_necessary_signal_init( _cc_out, STABLE, "_stable", declarations, TRUE );
  _publish_cc_necessary_signal_init( _cc_out, LAST_EVENT, "_last_event", declarations );
}

void
TyvisStableAttribute::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << "_stable";
}

void
TyvisStableAttribute::_publish_cc_sigtype( published_file &_cc_out ) {
  _cc_out << " I_STABLE;\n";
}

void 
TyvisStableAttribute::_publish_cc_state_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_necessary_signal_state_object_init( _cc_out, STABLE, "_stable", declarations, TRUE);
  _publish_cc_necessary_signal_state_object_init( _cc_out, LAST_EVENT, "_last_event", declarations);
}

void
TyvisStableAttribute::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisStableAttribute::_publish_cc_necessary_copying( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_necessary_attribute_copying( _cc_out, STABLE, "_stable", declarations );
  _publish_cc_necessary_attribute_copying( _cc_out, LAST_EVENT, "_last_event", declarations );
}

void 
TyvisStableAttribute::_publish_cc_read_or_write( published_file &_cc_out,
                                                        const string &fName, 
                                                        const string &sName,
                                                        PublishData *declarations){
  _publish_cc_attribute_read_or_write( _cc_out, STABLE, "_stable", fName, sName, declarations);
  _publish_cc_attribute_read_or_write( _cc_out, LAST_EVENT, "_last_event", fName, sName, declarations);
}

const string &
TyvisStableAttribute::_get_cc_attribute_name() const {
  static const string retval("stable");
  return retval;
}

TyvisDeclaration *
TyvisStableAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
                                                        TyvisTypeDefinition *decl_type ){ 
  return _build_signal_declaration( decl_name, decl_type );
}

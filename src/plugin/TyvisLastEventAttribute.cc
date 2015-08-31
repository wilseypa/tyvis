
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
#include "TyvisLastEventAttribute.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisTextLiteral.hh"

#include "savant/StandardPackage.hh"
#include "savant/set.hh"
#include "published_file.hh"

void 
TyvisLastEventAttribute::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisLastEventAttribute::_publish_cc_rvalue" );	
  _publish_cc_signal_attribute( _cc_out, "LastEvent", declarations );
}

void
TyvisLastEventAttribute::_publish_cc_init_val( published_file &_cc_out ) {
  _cc_out << "UniversalInteger(0)";
}

void
TyvisLastEventAttribute::_publish_cc_necessary_decl_in_state( published_file &_cc_out,
							      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisLastEventAttribute::_publish_cc_necessary_decl_in_state" );
  _publish_cc_necessary_decl_in_state_last_event( _cc_out, declarations );
}

void
TyvisLastEventAttribute::_publish_cc_necessary_copying( published_file &_cc_out,
							PublishData *declarations ) {
  _publish_cc_necessary_attribute_copying( _cc_out, LAST_EVENT, "_last_event", declarations );
}

void
TyvisLastEventAttribute::_publish_cc_read_or_write( published_file &_cc_out,
                                                           const string &fName, 
                                                           const string &sName,
                                                           PublishData *declarations) {
  _publish_cc_attribute_read_or_write( _cc_out, LAST_EVENT, "_last_event", fName, sName, declarations );
}

void
TyvisLastEventAttribute::_publish_cc_name_in_caps( published_file &_cc_out ) {
  _cc_out << "LAST_EVENT";
}

void
TyvisLastEventAttribute::_publish_cc_attrib_type( published_file &_cc_out ) {
  _cc_out << "function";
}

void
TyvisLastEventAttribute::_publish_cc_necessary_decl_init( published_file &, PublishData *){ }

void
TyvisLastEventAttribute::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  ASSERT( get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );
  ASSERT(_get_prefix()->is_signal() == TRUE );
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << "_last_event";
}

void
TyvisLastEventAttribute::_publish_cc_sigtype( published_file &_cc_out ) {
  _cc_out << "I_LAST_EVENT";
}

void
TyvisLastEventAttribute::_publish_cc_state_object_init( published_file &_cc_out,
							PublishData *declarations ) {
  _publish_cc_necessary_signal_state_object_init( _cc_out, LAST_EVENT,
						  "_last_event", declarations, TRUE ); 
}

void
TyvisLastEventAttribute::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisLastEventAttribute::_publish_cc_init( published_file &_cc_out, PublishData *declarations ) {

  ASSERT( get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );

  TyvisDeclaration* decl = NULL;
  Tyvis* prefix = _get_prefix();
  TyvisTypeDefinition* type_def = _get_explicit_signal_type();

  ASSERT((prefix->is_signal() == TRUE) ||
	 ( dynamic_cast<TyvisAttribute *>(prefix) != NULL));
  
  if(prefix->is_signal() == TRUE) {
    decl = (dynamic_cast<TyvisDeclaration *>(prefix));
  }
  else {
    decl = (dynamic_cast<TyvisAttribute *>(prefix))->_get_implicit_declaration();      
  }

  if(!prefix->_is_published_attribute_in_initstate(TyvisDeclaration::LAST_EVENT)) {
    _cc_out << *decl->_get_mangled_declarator();
    _cc_out << "_last_event = ";
    if(type_def->is_scalar_type() == TRUE) {
      _cc_out << "PhysicalLiteral( std_standard_timeTypeInfo(), -1 )";
    }
    else if((type_def->is_array_type() == TRUE) || (type_def->is_record_type() == TRUE)) {
      type_def->_publish_cc_type_info_xxx( _cc_out, declarations );
      _cc_out << "_lastevent(ObjectBase::VARIABLE";
      if((type_def->is_array_type() == TRUE) &&
	 (type_def->is_anonymous() == TRUE)){
	_cc_out << "," << NL();
	type_def->_publish_cc_range( _cc_out, declarations );
      }
      _cc_out << "," << NL()
	      << "1," << NL()
	      << "new " << OS("ElementAssociation(Others,")
              << "PhysicalLiteral( std_standard_timeTypeInfo(), -1 )"
	      << CS(");");
    }
    prefix->_add_published_attribute_in_initstate(TyvisDeclaration::LAST_EVENT);
  }
}

const string &
TyvisLastEventAttribute::_get_cc_attribute_name() const {
  static const string retval("last_event");
  return retval;
}

TyvisDeclaration *
TyvisLastEventAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
                                                           TyvisTypeDefinition *decl_type ){ 
  return _build_function_declaration( decl_name, decl_type );
}

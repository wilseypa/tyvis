
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

#include "TyvisDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisEventAttribute.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "TyvisVariableDeclaration.hh"

#include "savant/StandardPackage.hh"
#include "published_file.hh"
#include <sstream>

void
TyvisEventAttribute::_publish_cc_rvalue( published_file &(_cc_out), PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc" );
  
  _publish_cc_signal_attribute( _cc_out, "Event", declarations );
}

void
TyvisEventAttribute::_publish_cc_necessary_decl_in_state( published_file &_cc_out,
							  PublishData *declarations ) {
  ASSERT( _get_prefix() != NULL );
  ASSERT( _get_prefix()->is_resolved() == TRUE );
  ASSERT( _get_prefix()->is_signal() == TRUE ||
	  dynamic_cast<TyvisAttribute *>(_get_prefix()) != NULL );

  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_necessary_decl_in_state" );    
  _publish_cc_necessary_decl_in_state_last_event( _cc_out, declarations );
  if( !_get_prefix()->_is_published_attribute_in_state(TyvisDeclaration::EVENT )){
    CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_necessary_decl_in_state" );
    _cc_out << "Signal " << *_get_prefix()->_get_mangled_declarator() << "_event;" << NL();
    _get_prefix()->_add_published_attribute_in_state(IIR_Declaration::EVENT);
  }
}

void
TyvisEventAttribute::_publish_cc_init_fields_for_signals( published_file &_cc_out ) {
  const string old_current_publish_name = _get_current_publish_name();
  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_init_fields_for_signals" );  

  if (_get_prefix()->_get_subtype()->get_kind() == IIR_RECORD_TYPE_DEFINITION) {
    ostringstream recordName;
    recordName << *(_get_prefix()->_get_mangled_declarator());
    _set_current_publish_name( recordName.str() );
    _get_prefix()->_get_subtype()->_publish_cc_init_fields_for_signals( _cc_out );
  }

  _set_current_publish_name( old_current_publish_name );
}

void
TyvisEventAttribute::_publish_cc_init_val( published_file &_cc_out ) { 
  _cc_out << "true";
}

void
TyvisEventAttribute::_publish_cc_name_in_caps( published_file &_cc_out ) {
  _cc_out << "EVENT";
}

void
TyvisEventAttribute::_publish_cc_attrib_type( published_file &_cc_out ) {
  _cc_out << "function";
}

void
TyvisEventAttribute::_publish_cc_necessary_decl_init( published_file &_cc_out,
						      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_necessary_decl_init" );
  
  ASSERT( _get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );
  ASSERT(_get_prefix()->is_signal() == TRUE );

  _get_prefix()->_publish_cc_declarator( _cc_out, declarations );
  _cc_out << "_last_event = PhysicalType(ObjectBase::VARIABLE, 0, "
	  << "std_standard_timeTypeInfo());\n";
}

void
TyvisEventAttribute::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_elaborate" );
  
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << "_event";
}

void
TyvisEventAttribute::_publish_cc_sigtype( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_sigtype" );
  
  _cc_out << " I_EVENT;\n";
}

void
TyvisEventAttribute::_publish_cc_state_object_init_attr_var( published_file &_cc_out,
                                                             Tyvis::SignalAttribute type,
                                                             PublishData *declarations){

  CC_REF( _cc_out, "_publish_cc_state_object_init_attr_var" );

  string suffix;
  TyvisTypeDefinition *subtype = 0;
  switch( type ){
  case LAST_EVENT:{
    suffix = "_last_event";
    subtype = dynamic_cast<TyvisTypeDefinition *>(get_design_file()->get_standard_package()->get_time_type());
    break;
  }
  case EVENT:{
    suffix = "_event";
    subtype = dynamic_cast<TyvisTypeDefinition *>(get_design_file()->get_standard_package()->get_boolean_type());
    break;
  }
  default:
    abort();
  }

  if( _get_prefix()->_is_published_attribute_in_constructor(type) == FALSE) {
    // We're going to construct a fake variable that will be used to hold
    // the last_event value.  We'll use this for publishing and then
    // destroy it.
    string variable_name( _get_prefix()->_get_mangled_declarator()->convert_to_string() + suffix );
    
    // OK, now publish it.  Publishing hooks it into the cgen_symbol_table so we can't
    // delete it.
    dynamic_cast<TyvisVariableDeclaration *>(build_implicit_variable(variable_name,subtype))->_publish_cc_state_object_init( _cc_out, declarations );
    _get_prefix()->_add_published_attribute_in_constructor(type);
  }
}

void
TyvisEventAttribute::_publish_cc_state_object_init_last_event( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_state_object_init_last_event" );
  _publish_cc_state_object_init_attr_var( _cc_out, LAST_EVENT, declarations );
}

void
TyvisEventAttribute::_publish_cc_state_object_init_event( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_state_object_init_event" );
  _publish_cc_state_object_init_attr_var( _cc_out, EVENT, declarations );
}

void
TyvisEventAttribute::_publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations ) {
  ASSERT( _get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );  
  ASSERT( _get_prefix()->is_signal() == TRUE || dynamic_cast<TyvisAttribute *>(_get_prefix()) != NULL );

  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_state_object_init" );

  _publish_cc_state_object_init_last_event( _cc_out, declarations );
  _cc_out << "," << NL();
  _publish_cc_state_object_init_event( _cc_out, declarations );
  _cc_out << "," << NL();
}

void
TyvisEventAttribute::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisEventAttribute::_publish_cc_init( published_file &_cc_out, PublishData *declarations ) {
  ASSERT( _get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );

  TyvisDeclaration* decl;
  Tyvis* prefix = _get_prefix();

  CC_REF( _cc_out, "TyvisEventAttribute::_publish_cc_init" );
  

  ASSERT((prefix->is_signal() == TRUE) ||
	 (dynamic_cast<TyvisAttribute *>(prefix) != NULL));
  
  if(prefix->is_signal() == TRUE) {
    decl = (dynamic_cast<TyvisDeclaration*>(prefix));
  }
  else {
    decl = (dynamic_cast<TyvisAttribute*>(prefix))->_get_implicit_declaration();      
  }

  if(!prefix->_is_published_attribute_in_initstate(TyvisDeclaration::LAST_EVENT)) {
    _cc_out << *decl->_get_mangled_declarator();
    _cc_out << "_last_event = ";

    if(decl->is_scalar_type() == TRUE) {
      _cc_out << "PhysicalType(ObjectBase::VARIABLE";
      _cc_out << ", UniversalLongLongInteger(-1)";
      _cc_out << "std_standard_timeTypeInfo()); ";    
    }
    else if((decl->is_array_type() == TRUE) || (decl->is_record_type() == TRUE)) {
      decl->_get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
      _cc_out << "_lastevent(ObjectBase::VARIABLE";
      if (decl->is_array_type() == TRUE) {
	decl->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations, TRUE, "_lastevent" );
	_cc_out << ", -1, ";
	decl->_get_subtype()->_publish_cc_range( _cc_out, declarations );
      }
      _cc_out << ", 1, new ElementAssociation(Others, new PhysicalType(ObjectBase::VARIABLE, UniversalLongLongInteger(-1), std_standard_timeTypeInfo()))";
      _cc_out << "); ";    
    }
    _cc_out << "\n";
    prefix->_add_published_attribute_in_initstate(TyvisDeclaration::LAST_EVENT);
  }
  if(!prefix->_is_published_attribute_in_initstate(TyvisDeclaration::EVENT)) {
    _cc_out << *decl->_get_mangled_declarator();
    _cc_out << "_event = ";

    if(decl->is_scalar_type() == TRUE) {
      _cc_out << "SAVANT_BOOLEAN_FALSE;";
    }
    else if((decl->is_array_type() == TRUE) || (decl->is_record_type() == TRUE)) {
      decl->_get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
      _cc_out << "_event(ObjectBase::VARIABLE";
      if(decl->is_array_type()) {
	decl->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations, TRUE, "_event" );
	_cc_out << ", -1, ";
	decl->_get_subtype()->_publish_cc_range( _cc_out, declarations );
      }
      _cc_out << ", 1, new ElementAssociation(Others, SAVANT_BOOLEAN_FALSE)";
      _cc_out << ");";
    }
    _cc_out << NL();
    prefix->_add_published_attribute_in_initstate(TyvisDeclaration::EVENT);
  }
}

void
TyvisEventAttribute::_publish_cc_necessary_copying( published_file & ) {}

void
TyvisEventAttribute::_publish_cc_read_or_write( published_file &, 
                                                const string &, 
                                                const string & ){}

const string &
TyvisEventAttribute::_get_cc_attribute_name() const {
  static const string retval("event");
  return retval;
}

TyvisDeclaration *
TyvisEventAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
						  TyvisTypeDefinition *decl_type ){
  ASSERT( decl_type ==
	  dynamic_cast<TyvisTypeDefinition *>(get_design_file()->get_standard_package()->get_boolean_type()) );
  return _build_function_declaration( decl_name, decl_type );
}

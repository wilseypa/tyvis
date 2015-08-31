
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
#include "TyvisLastActiveAttribute.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisTextLiteral.hh"

#include "savant/StandardPackage.hh"
#include "savant/set.hh"
#include "published_file.hh"

void
TyvisLastActiveAttribute::_publish_cc_rvalue( published_file &_cc_out,
					      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisLastActiveAttribute::_publish_cc_rvalue" );	
  _publish_cc_signal_attribute( _cc_out, "LastActive", declarations );
}

void
TyvisLastActiveAttribute::_publish_cc_init_val( published_file &_cc_out ) {
  ASSERT( get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );
  ASSERT(_get_prefix()->is_signal() == TRUE );
  if((dynamic_cast<TyvisSignalDeclaration *>(get_prefix()))->IIRBase_SignalDeclaration::get_value() == NULL) {
    _cc_out << "UniversalInteger(-1)";
  } else {
    _cc_out << "UniversalInteger(0)";
  }
}

void
TyvisLastActiveAttribute::_publish_cc_necessary_decl_in_state( published_file &_cc_out,
							       PublishData *declarations ) { 
  TyvisDeclaration* decl = NULL;
  Tyvis* prefix = _get_prefix();

  ASSERT((prefix->is_signal() == TRUE) ||
	 ( dynamic_cast<TyvisAttribute *>(prefix) != NULL));
  
  if(prefix->is_signal() == TRUE) {
    decl = (dynamic_cast<TyvisDeclaration*>(prefix));
  }
  else {
    decl = (dynamic_cast<TyvisAttribute*>(prefix))->_get_implicit_declaration();      
  }

  if(!prefix->_is_published_attribute_in_state(TyvisDeclaration::LAST_ACTIVE)) {
    if(decl->is_scalar_type() == TRUE) {
      _cc_out << "  PhysicalType";
    }
    else {
      decl->_get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    }
    _cc_out << " "
            << *decl->_get_mangled_declarator()
            << "_last_active"
            << ";\n";
    prefix->_add_published_attribute_in_state(TyvisDeclaration::LAST_ACTIVE);
  }
}

void
TyvisLastActiveAttribute::_publish_cc_name_in_caps( published_file &_cc_out ) {
  _cc_out << "LAST_ACTIVE";
}

void
TyvisLastActiveAttribute::_publish_cc_attrib_type( published_file &_cc_out ) {
  _cc_out << "time";
}

void
TyvisLastActiveAttribute::_publish_cc_necessary_decl_init( published_file &, PublishData * ){ }

void
TyvisLastActiveAttribute::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  ASSERT( get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );
  ASSERT(_get_prefix()->is_signal() == TRUE );
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << "_last_active";
}

void
TyvisLastActiveAttribute::_publish_cc_sigtype( published_file &_cc_out ) {
  _cc_out << "";
}

void
TyvisLastActiveAttribute::_publish_cc_state_object_init( published_file &_cc_out,
							 PublishData *declarations ) {
  ASSERT( get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );

  TyvisDeclaration* decl;
  Tyvis* prefix = _get_prefix();

  ASSERT((prefix->is_signal() == TRUE) ||
	 ( dynamic_cast<TyvisAttribute *>(prefix) != NULL));
  
  if(prefix->is_signal() == TRUE) {
    decl = (dynamic_cast<TyvisDeclaration*>(prefix));
  }
  else {
    decl = (dynamic_cast<TyvisAttribute*>(prefix))->_get_implicit_declaration();      
  }

  if(!prefix->_is_published_attribute_in_constructor(TyvisDeclaration::LAST_ACTIVE)) {
    decl->_get_mangled_declarator()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << OS("_last_active(ObjectBase::VARIABLE");
    if(decl->is_array_type() == TRUE) {
      _cc_out << "," << NL();
      decl->_get_subtype()->_publish_cc_range( _cc_out, declarations );
    }
    _cc_out << CS(")")
	    << "," << NL();
    prefix->_add_published_attribute_in_constructor(TyvisDeclaration::LAST_ACTIVE);
  }
}

void
TyvisLastActiveAttribute::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisLastActiveAttribute::_publish_cc_init( published_file &_cc_out, PublishData *declarations ) {
  ASSERT( get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );

  TyvisDeclaration* decl;
  Tyvis* prefix = _get_prefix();
  TyvisTypeDefinition* type_def = _get_explicit_signal_type();

  ASSERT((prefix->is_signal() == TRUE) ||
	 ( dynamic_cast<TyvisAttribute *>(prefix) != NULL));
  
  if(prefix->is_signal() == TRUE) {
    decl = (dynamic_cast<TyvisDeclaration*>(prefix));
  }
  else {
    decl = (dynamic_cast<TyvisAttribute*>(prefix))->_get_implicit_declaration();      
  }

  if(!prefix->_is_published_attribute_in_initstate(TyvisDeclaration::LAST_ACTIVE)) {
    _cc_out << *decl->_get_mangled_declarator()
            << "_last_active = ";
    if(type_def->is_scalar_type() == TRUE) {
      _cc_out << "PhysicalLiteral(std_standard_timeTypeInfo(), -1);";
    }
    else if((type_def->is_array_type() == TRUE) || (type_def->is_record_type() == TRUE)) {
      type_def->_publish_cc_type_info_xxx( _cc_out, declarations );
      _cc_out << OS("_lastevent(ObjectBase::VARIABLE");
      if((type_def->is_array_type() == TRUE) &&
	 (type_def->is_anonymous() == TRUE)) {
	_cc_out << "," << NL();
	type_def->_publish_cc_range( _cc_out, declarations );
      }
      _cc_out << "," << NL()
	      << "1," << NL()
	      << OS("new ElementAssociation(")
	      << "Others," << NL()
              << "PhysicalLiteral(std_standard_timeTypeInfo(), -1);"
	      << CS(")");
      _cc_out << CS(");");
    }
    prefix->_add_published_attribute_in_initstate(TyvisDeclaration::LAST_ACTIVE);
  }
}

const string &
TyvisLastActiveAttribute::_get_cc_attribute_name() const {
  static const string retval("last_active");
  return retval;
}

TyvisDeclaration *
TyvisLastActiveAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
                                                            TyvisTypeDefinition *decl_type ){
  return _build_function_declaration( decl_name, decl_type );
}

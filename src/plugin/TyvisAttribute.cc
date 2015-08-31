
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
#include "TyvisAccessTypeDefinition.hh"
#include "TyvisActiveAttribute.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisAttribute.hh"
#include "TyvisAttributeDeclaration.hh"
#include "TyvisBranchQuantityDeclaration.hh"
#include "TyvisConstantDeclaration.hh"
#include "TyvisDelayedAttribute.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDrivingAttribute.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisEventAttribute.hh"
#include "TyvisFreeQuantityDeclaration.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIndexedName.hh"
#include "TyvisIntegerLiteral.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisLastActiveAttribute.hh"
#include "TyvisLastEventAttribute.hh"
#include "TyvisLastValueAttribute.hh"
#include "TyvisLeftAttribute.hh"
#include "TyvisPhysicalLiteral.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "TyvisQuietAttribute.hh"
#include "TyvisRangeTypeDefinition.hh"
#include "TyvisRightAttribute.hh"
#include "TyvisScalarNatureDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisStableAttribute.hh"
#include "TyvisTransactionAttribute.hh"
#include "TyvisTypeDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisVariableInterfaceDeclaration.hh"

#include "savant/savant.hh"
#include "savant/StandardPackage.hh"
#include "savant/error_func.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

using std::abort;

TyvisAttribute::TyvisAttribute() :
  my_attribute_name(0),
  implicit_declaration(0){}

TyvisAttribute::~TyvisAttribute(){
  delete my_attribute_name;
  delete implicit_declaration;
}


void
TyvisAttribute::_publish_cc_necessary_decl_in_state_last_event( published_file &_cc_out,
								PublishData * ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_necessary_decl_in_state_last_event" );

  ASSERT( _get_prefix() != NULL );
  ASSERT( _get_prefix()->is_resolved() == TRUE );
  ASSERT( _get_prefix()->is_signal() == TRUE || 
          dynamic_cast<TyvisAttribute *>(_get_prefix()) != NULL );
  
  if( !_get_prefix()->_is_published_attribute_in_state( TyvisDeclaration::LAST_EVENT )){
    _cc_out << "Signal " << *_get_prefix()->_get_mangled_declarator() << "_last_event;" << NL();
    _get_prefix()->_add_published_attribute_in_state(IIR_Declaration::LAST_EVENT);
  }
}

void
TyvisAttribute::_publish_cc_necessary_decl_in_state( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_necessary_decl_in_state" );
  _report_undefined_fn("_publish_cc_necessary_decl_in_state( _cc_out )");
}

void
TyvisAttribute::_publish_cc_name_in_caps( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_name_in_caps" );
  _report_undefined_fn("_publish_cc_name_in_caps( _cc_out )");
}

void
TyvisAttribute::_publish_cc_attrib_type( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_attrib_type" );
  _report_undefined_fn("_publish_cc_attrib_type( _cc_out )");
}

void
TyvisAttribute::_publish_cc_necessary_decl_init( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_necessary_decl_init" );
  _report_undefined_fn("_publish_cc_necessary_decl_init( _cc_out, declarations )");
}

void
TyvisAttribute::_publish_cc_init_val( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_init_val" );
  _report_undefined_fn("_publish_cc_init_val( _cc_out )");
}

void
TyvisAttribute::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_headers" );
  if( _get_prefix()->is_type() == TRUE || _get_prefix()->is_object() == TRUE ) {
    _get_prefix()->_publish_cc_include( _cc_out );
  }
}

void
TyvisAttribute::_publish_cc_necessary_copying( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_necessary_copying" );
  _report_undefined_fn("_publish_cc_necessary_copying( _cc_out )");
}

void
TyvisAttribute::_publish_cc_necessary_attribute_copying( published_file &_cc_out,
                                                         SignalAttribute,
                                                         const char *object_suffix,
                                                         PublishData *) {
  TyvisDeclaration *decl = _get_actual_signal_declaration();
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_necessary_attribute_copying" );
  _cc_out << "this->"
          << *decl->_get_mangled_declarator()
          << object_suffix << "  = s."
          << *decl->_get_mangled_declarator()
          << object_suffix << ";" << NL();
}

void
TyvisAttribute::_publish_cc_read_or_write( published_file &,
					       const string &, 
					       const string & ) {
  _report_undefined_fn("_publish_cc_necessary_read_or_write(published_file &_cc_out, const char *, const char * )");
}

void
TyvisAttribute::_publish_cc_attribute_read_or_write( published_file &_cc_out,
                                                     SignalAttribute,
                                                     const string &,
                                                     const string &functionName,
                                                     const string &streamName,
                                                     PublishData * ) {
  TyvisDeclaration *decl   = _get_actual_signal_declaration();

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_attribute_read_or_write" );

  _cc_out << decl->_get_mangled_declarator()
          << "." << functionName << "(" << streamName << ");\n";
}

void 
TyvisAttribute::_publish_cc_bounds( published_file &_cc_out, PublishData * ){
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_bounds" );
  _cc_out << "defaultInfo()";
}

void
TyvisAttribute::_publish_cc_sigtype( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_sigtype" );
  _report_undefined_fn("_publish_cc_sigtype( _cc_out )");
}

void
TyvisAttribute::_publish_cc_prefix( published_file &_cc_out,
				    PublishData *declarations ){
  ASSERT(_get_prefix() != NULL);
  ASSERT(_get_prefix()->is_resolved() == TRUE);

  if( _is_type_attribute() && get_prefix()->is_type() ){
    CC_REF( _cc_out, "TyvisAttribute::_publish_cc_prefix" );
    _get_prefix()->_publish_cc_type_info_xxx( _cc_out, declarations );
  }
  else{
    CC_REF( _cc_out, "TyvisAttribute::_publish_cc_prefix" );
    _get_prefix()->_publish_cc_rvalue( _cc_out, declarations );
  }
}

void
TyvisAttribute::_publish_cc_suffix( published_file &_cc_out,
				    PublishData *declarations ){
  _cc_out << "(";
  if( _needs_suffix_published() ){
    if( _get_suffix() ){
      _get_suffix()->_publish_cc_rvalue( _cc_out, declarations );
    }
    else{
      _publish_cc_default_array_suffix( _cc_out );
    }
  }
  _cc_out << ")";
}

bool
TyvisAttribute::_can_publish_static_value(){
  bool retval;
  retval = is_locally_static() && !get_prefix()->is_type();
  return retval;
}

void
TyvisAttribute::_publish_cc_rvalue( published_file &_cc_out,
				    PublishData *declarations ){
  if( _get_implicit_declaration() &&
      !declarations->in_collection(_get_implicit_declaration())) {
    declarations->add_declaration(_get_implicit_declaration());
  }
  
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_rvalue" );
  if( _can_publish_static_value() ){
    _publish_cc_locally_static_value( _cc_out, declarations );
  }
  else{
    _publish_cc_rvalue_dynamic( _cc_out, declarations );
  }
}

void
TyvisAttribute::_publish_cc_rvalue_dynamic( published_file &_cc_out,
					    PublishData *declarations ){
  _publish_cc_prefix( _cc_out, declarations );
  _cc_out << "." << _get_cc_attribute_name();
  _publish_cc_suffix( _cc_out, declarations );
}

void
TyvisAttribute::_publish_cc_signal_attribute( published_file &_cc_out,
                                              const char *attributeString, 
                                              PublishData *declarations,
                                              IIR_Boolean checkSuffixFlag) {

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_signal_attribute" );

  ASSERT( _get_prefix() != NULL );
  ASSERT(_get_prefix()->is_resolved() == TRUE );
  ASSERT(_get_prefix()->is_signal() == TRUE );

  _check_and_add_necessary_decls(declarations);
  
  if(_is_currently_publishing_subprogram() == TRUE) {
    _cc_out << "processPtr->";
  }
  _cc_out << "locate" << attributeString << "Attribute(&(";
  _get_prefix()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << ")";

  _publish_cc_include( _cc_out, "tyvis/VHDLVTime.hh" );  
  if (checkSuffixFlag == TRUE) {
    _cc_out << ", ";
    if(_get_suffix() != NULL) {
      ASSERT(_get_suffix()->get_kind() == IIR_PHYSICAL_LITERAL);
      _cc_out << "VHDLVTime(";
      dynamic_cast<TyvisPhysicalLiteral *>(_get_suffix())->_publish_cc_rvalue( _cc_out, declarations );
      _cc_out << ")";
    } else {
      _cc_out << "VHDLVTime::getVHDLVTimeZero()";
    }
  }
  
  _cc_out << ")";
}

void
TyvisAttribute::_publish_cc_necessary_signal_decl_in_state( published_file &_cc_out,
                                                            SignalAttribute attrib, 
                                                            const char *objectSuffix,
                                                            PublishData *) {
  TyvisDeclaration *decl   = _get_actual_signal_declaration();

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_necessary_signal_decl_in_state" );
  
  if ( decl->_is_published_attribute_in_state(attrib) == FALSE ) {
    if ( decl->is_scalar_type() == TRUE ) {
      _cc_out << _get_cc_signal_attribute_subelement_type( attrib );
    }
    else {
      decl->_get_subtype()->_publish_cc_type_info_xxx( _cc_out, 0 );
    }
    
    _cc_out << " "
            << *decl->_get_mangled_declarator()
            << objectSuffix << ";" << NL();
    
    decl->_add_published_attribute_in_state(attrib);
  }
}

const string  
TyvisAttribute::_get_cc_signal_attribute_subelement_type( SignalAttribute attrib ){
  string retval;
  //  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_signal_attribute_subelement_type" );
  switch (attrib) {
  case EVENT:
  case ACTIVE:
  case STABLE:
  case QUIET:
  case TRANSACTION:
    retval = "EnumerationType";
    break;
  case LAST_EVENT:    
  case LAST_ACTIVE:
    retval = "PhysicalType";
    break;
  case LAST_VALUE:
    abort();
//     retval = _get_actual_signal_declaration()->_get_subtype()->_get_cc_type_info();
    break;
  default:
    cerr << "Error : TyvisAttribute::_publish_cc_signal_attribute"
	 << "_subelement_type(SignalAttribute) - Unhandled case.\n";
    abort();
  }
  return retval;
}

void
TyvisAttribute::_publish_cc_signal_attribute_subelement_type_info( published_file &_cc_out,
                                                                   SignalAttribute attrib,
                                                                   PublishData *declarations){

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_signal_attribute_subelement_type_info" );
  switch (attrib) {
  case EVENT:
  case ACTIVE:
  case STABLE:
  case QUIET:
    _cc_out << ", std_standard_booleanTypeInfo()";
    break;
  case LAST_EVENT:    
  case LAST_ACTIVE:
    _cc_out << ", std_standard_timeTypeInfo()";
    break;
  case LAST_VALUE:
    _get_actual_signal_declaration()->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations );
    break;
  case TRANSACTION:
    _cc_out << ", std_standard_bitTypeInfo()";
    break;
  default:
    cerr << "Error : TyvisAttribute::_publish_cc_signal_attribute"
	 << "_subelement_type(SignalAttribute) - Unhandled case.\n";
  }
}

void
TyvisAttribute::_publish_cc_signal_attribute_subelement_type_suffix( published_file &_cc_out,
                                                                     SignalAttribute attrib) {

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_signal_attribute_subelement_type_suffix" );
  switch (attrib) {
  case LAST_EVENT:
  case LAST_ACTIVE:
    _cc_out << "_lastevent";
    break;
  case EVENT:
  case STABLE:
  case ACTIVE:
  case QUIET:
    _cc_out << "_event";
    break;
  case LAST_VALUE:
  case TRANSACTION:
    break;
  default:
    cerr << "Error : TyvisAttribute::_publish_cc_signal_attribute"
	 << "_subelement_type_suffix(SignalAttribute) - Unhandled case.\n";
  }
}

void
TyvisAttribute::_publish_cc_necessary_signal_state_object_init( published_file &_cc_out,
                                                                SignalAttribute attrib, 
                                                                const char *objectSuffix,
                                                                PublishData *declarations,
                                                                IIR_Boolean implicitSignal) {
  TyvisDeclaration *decl = _get_actual_signal_declaration();
  if( decl->_is_published_attribute_in_constructor(attrib) == FALSE ){
    CC_REF( _cc_out, "TyvisAttribute::_publish_cc_necessary_signal_state_object_init" );
    _cc_out << *decl->_get_mangled_declarator()
            << objectSuffix << "(ObjectBase::";
    
    if (implicitSignal == TRUE) {
      _cc_out << "IMPLICIT_SIGNAL";
    }
    else {
      _cc_out << "VARIABLE";
    }
    ASSERT ( decl->_get_subtype() != NULL );
    ASSERT ( dynamic_cast<TyvisTypeDefinition *>(decl->_get_subtype()) != NULL );
    
    TyvisTypeDefinition *st = decl->_get_subtype();
    
    if ((st->is_array_type() == TRUE) ||
	(st->is_record_type() == TRUE)) {
      st->_publish_cc_object_type_info(_cc_out, declarations, TRUE, objectSuffix);
      _cc_out << ", ";
      st->_publish_cc_resolution_function_id(_cc_out, declarations);
    }
    if (st->is_array_type() == TRUE) {
      if ((st->is_unconstrained_array_type() == TRUE) ||
	  (st->is_anonymous() == TRUE)) {
	_cc_out << ", ";
	st->_publish_cc_range( _cc_out, declarations );
      }
    }
    if (st->is_scalar_type() == TRUE) {
      _publish_cc_signal_attribute_subelement_type_info( _cc_out, attrib, declarations );
    }
    _cc_out << "),\n";    
    decl->_add_published_attribute_in_constructor(attrib);
  }
}

void
TyvisAttribute::_publish_cc_necessary_signal_init( published_file &_cc_out,
                                                   SignalAttribute attrib, 
                                                   const char *objectSuffix, 
                                                   PublishData *declarations,
                                                   IIR_Boolean implicitSignal) {
  TyvisDeclaration *decl = _get_actual_signal_declaration();
  int idx = 0;

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_necessary_signal_init" );
  
  if(decl->_is_published_attribute_in_initstate(attrib) == FALSE) {
    abort();
    _cc_out << *decl->_get_mangled_declarator()
            << objectSuffix << " = ";
    if (decl->is_scalar_type() == TRUE) {
      _cc_out << _get_cc_signal_attribute_subelement_type( attrib )
	      << "(ObjectBase::";
      if (implicitSignal == FALSE) {
	_cc_out << "VARIABLE";
      }
      else {
	_cc_out << "IMPLICIT_SIGNAL";
      }
      _cc_out << ", ";
      _publish_cc_default_initial_universal_value( _cc_out, attrib, declarations );
      _publish_cc_signal_attribute_subelement_type_info( _cc_out, attrib, declarations );
      _cc_out << ");\n";
    }
    else {
      TyvisTypeDefinition *type = decl->_get_subtype();
      do {
	abort();
	idx++;
	type->_publish_cc_type_info_xxx(_cc_out, declarations );
	_publish_cc_signal_attribute_subelement_type_suffix( _cc_out, attrib );
	_cc_out << "(ObjectBase::";
	if (implicitSignal == FALSE) {
	  _cc_out << "VARIABLE";
	}
	else {
	  _cc_out << "IMPLICIT_SIGNAL";
	}
	if ((type->is_array_type() == TRUE) ||
	    (type->is_record_type() == TRUE)) {
	  type->_publish_cc_object_type_info(_cc_out, declarations, TRUE, objectSuffix, TRUE);
	  _cc_out << ", ";
	  type->_publish_cc_resolution_function_id( _cc_out, declarations);
	}
	if (type->is_array_type() == TRUE) {
	  if ((type->is_unconstrained_array_type() == TRUE) ||
	      (type->is_anonymous() == TRUE)) {
	    _cc_out << ", ";
	    type->_publish_cc_range( _cc_out, declarations );
	  }
	  type = type->_get_element_subtype();
	}
	_cc_out << ", 1, new ElementAssociation(Others, new ";
      } while (type->is_array_type() == TRUE);
      
      _cc_out << _get_cc_signal_attribute_subelement_type( attrib )
	      << "(ObjectBase::";
      if (implicitSignal == FALSE) {
	_cc_out << "VARIABLE, ";
      }
      else {
	_cc_out << "IMPLICIT_SIGNAL, ";
      }      
      _publish_cc_default_initial_universal_value( _cc_out, attrib, declarations );
      _cc_out << "))";
      if (decl->is_array_type() == TRUE) {
	while (idx > 1)  {
	  _cc_out << "))";
	  idx--;
	}
      }
      _cc_out << ");\n";
    }
    decl->_add_published_attribute_in_initstate(attrib);
  }
}

void
TyvisAttribute::_publish_cc_default_initial_universal_value( published_file &_cc_out,
                                                             SignalAttribute attrib,
                                                             PublishData *declarations){

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_default_initial_universal_value" );
  switch (attrib) {
  case EVENT:
  case ACTIVE:
    _cc_out << "UniversalInteger(0)";
    break;
  case STABLE:
  case QUIET:
    _cc_out << "UniversalInteger(1)";
    break;
  case LAST_ACTIVE:
  case LAST_EVENT:
    _cc_out << "UniversalLongLongInteger(-1)";
    break;
  case LAST_VALUE: {
    TyvisDeclaration *decl = _get_actual_signal_declaration();
    if (decl->_get_subtype()->is_scalar_type() == TRUE) {
      decl->_get_subtype()->_publish_cc_left( _cc_out, declarations );
    }
    else if (decl->_get_subtype()->is_array_type() == TRUE) {
      decl->_get_subtype()->_get_element_subtype()->_publish_cc_left( _cc_out, declarations );
    }
    break;
  }
  case TRANSACTION: 
    _cc_out << "UniveralInteger(0)";
    break;
  default:
    cerr << "Error : TyvisAttribute::_publish_cc_default_initial_value"
	 << "(SignalAttribute) - Unhandled case.\n";
  }
}

void
TyvisAttribute::_publish_cc_init_fields_for_signals( published_file & ) {}

void
TyvisAttribute::_publish_cc_implicit_signal_state_object_init( published_file &_cc_out, PublishData *declarations ) {
  TyvisTypeDefinition* type_def = _get_explicit_signal_type();
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_implicit_signal_state_object_init" );
  ASSERT(_get_implicit_declaration()->is_signal() == TRUE);
  _cc_out << *_get_implicit_declaration()->_get_mangled_declarator();
  if (get_kind() == IIR_ABOVE_ATTRIBUTE) {
      _cc_out << "(ObjectBase::SIGNAL,std_standard_booleanTypeInfo()";
      if(type_def->is_array_type() == TRUE &&
         type_def->is_anonymous()) {
        _cc_out << ", ";
        type_def->_publish_cc_range( _cc_out, declarations );
      }
    }
  else {
      _cc_out << "(ObjectBase::IMPLICIT_SIGNAL";
      if ((type_def->is_array_type() == TRUE) && 
	  (type_def->is_anonymous() == TRUE)) {
	_cc_out << ", ";
	type_def->_publish_cc_range( _cc_out, declarations );
      }
  }
  _cc_out << "),\n";
}

void
TyvisAttribute::_publish_cc_implicit_signal_declaration( published_file &_cc_out, PublishData *declarations ) {
  TyvisTypeDefinition* type_def = _get_explicit_signal_type();

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_implicit_signal_declaration" );
  if(type_def->is_scalar_type() == TRUE) {
    _cc_out << _get_cc_signal_attribute_subelement_type();
  }
  else {
    type_def->_publish_cc_lvalue( _cc_out, declarations );
    _cc_out << _get_cc_signal_attribute_type_suffix();
  }
  _cc_out << " "
          << *_get_implicit_declaration()->_get_mangled_declarator()
          << ";" << NL();
}

const string
TyvisAttribute::_get_cc_signal_attribute_subelement_type( ) {
  //  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_signal_attribute_subelement_type" );
  string retval;
  if(( dynamic_cast<TyvisQuietAttribute *>(this) ) ||
     ( dynamic_cast<TyvisLastActiveAttribute *>(this) ) ||
     ( dynamic_cast<TyvisStableAttribute *>(this) )) {
    retval = "EnumerationType";
  }
  if( dynamic_cast<TyvisTransactionAttribute *>(this) ) {
    retval = "EnumerationType";
  }
  if( dynamic_cast<TyvisDelayedAttribute *>(this) ) {
    cerr << "Unimplemented Attribute 'DELAYED\n";
    abort();
  }
  return retval;
}

void
TyvisAttribute::_publish_cc_declarator( published_file &_cc_out, PublishData *declarations )
{
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_declarator" );

  //WORKAROUND #######SHOULD BE REMOVED ONCE PARSER FIXES THIS
  if( dynamic_cast<TyvisDeclaration *>(_get_prefix()) ) {
    TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration*>(_get_prefix());
    if(decl->_get_declarative_region() != _get_implicit_declaration()->_get_declarative_region()) {
      if(_get_implicit_declaration()->is_signal() == TRUE) {
	//	cerr << "work around for different declarative regions\nfor a signal and its attribute\n";
	_cc_out.get_stream() << 
	  *(_get_implicit_declaration()->_get_declarative_region()->_get_mangled_declarator());
	_cc_out << "_O";
	_cc_out.get_stream() << *(decl->_get_declarator());
	_publish_cc_signal_attribute_suffix( _cc_out );
	return;
      }
    }
  }

  if (_get_prefix()->get_kind() == IIR_INDEXED_NAME) {
    ASSERT ( _get_prefix()->_get_prefix_declaration() != NULL );
    _get_prefix()->_get_prefix_declaration()->_publish_cc_declarator( _cc_out, declarations );
  }
  else {
    _get_prefix()->_publish_cc_declarator( _cc_out, declarations );
  }
  _publish_cc_signal_attribute_suffix( _cc_out );
}

void
TyvisAttribute::_publish_cc_signal_attribute_suffix( published_file &_cc_out ){
  //  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_signal_attribute_suffix" );
  ASSERT(( _is_signal_attribute() == TRUE ) || 
	 ( is_function_attribute() == TRUE ));
  if ( dynamic_cast<TyvisQuietAttribute *>(this) ) {
    _cc_out << "_quiet";
  }
  if ( dynamic_cast<TyvisEventAttribute *>(this) ) {
    _cc_out << "_event";
  }
  if ( dynamic_cast<TyvisLastEventAttribute *>(this) ) {
    _cc_out << "_last_event";
  }
  if ( dynamic_cast<TyvisActiveAttribute *>(this) ) {
    _cc_out << "_active";
  }
  if ( dynamic_cast<TyvisTransactionAttribute *>(this) ) {
    _cc_out << "_transaction";
  }
  if ( dynamic_cast<TyvisStableAttribute *>(this) ) {
    _cc_out << "_stable";
  }
  if ( dynamic_cast<TyvisLastActiveAttribute *>(this) ) {
    _cc_out << "_last_active";
  }
  if ( dynamic_cast<TyvisLastValueAttribute *>(this) ) {
    _cc_out << "_last_value";
  }
}

const string  
TyvisAttribute::_get_cc_signal_attribute_type_suffix(){
  // Don't insert a ref, this is a suffix
  string retval;
  ASSERT( _is_signal_attribute() );
  if( dynamic_cast<TyvisQuietAttribute *>(this) ) {
    if(_get_explicit_signal_type()->is_scalar_type() == FALSE) {
      retval = "_event";
    }
  }
  if(dynamic_cast<TyvisStableAttribute *>(this)) {
    if(_get_explicit_signal_type()->is_scalar_type() == FALSE) {
      retval = "_event";
    }
  }
  if( dynamic_cast<TyvisTransactionAttribute *>(this) ) {
    if(_get_explicit_signal_type()->is_scalar_type() == FALSE) {
      retval = "_transaction";
    }
  }
  if( dynamic_cast<TyvisEventAttribute *>(this) ) {
    if(_get_explicit_signal_type()->is_scalar_type() == FALSE) {
      retval = "_event";
    }
  }
  if( dynamic_cast<TyvisActiveAttribute *>(this) ) {
    if(_get_explicit_signal_type()->is_scalar_type() == FALSE) {
      retval = "_event";
    }
  }
  if( dynamic_cast<TyvisLastEventAttribute *>(this) ) {
    if(_get_explicit_signal_type()->is_scalar_type() == FALSE) {
      retval = "_lastevent";
    }
  }
  if( dynamic_cast<TyvisLastActiveAttribute *>(this) ) {
    if(_get_explicit_signal_type()->is_scalar_type() == FALSE) {
      retval = "_lastevent";
    }
  }
  if( dynamic_cast<TyvisDrivingAttribute *>(this) ) {
    if(_get_explicit_signal_type()->is_scalar_type() == FALSE) {
      retval = "_event";
    }
  }
  return retval;
}

void
TyvisAttribute::_publish_cc_type_info_xxx( published_file &_cc_out, 
					   PublishData * ){
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_type_info_xxx" );
  TyvisTypeDefinition *type_def = _get_explicit_signal_type();
  ASSERT( type_def != 0 );

  //  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_type_nane" );
  if((type_def->is_array_type() == TRUE) ||
     (type_def->is_record_type() == TRUE)) {
    _cc_out << type_def->_get_cc_type_name() + _get_cc_signal_attribute_type_suffix();
  }
  else {
    _cc_out << _get_cc_signal_attribute_subelement_type();
  }
}

void
TyvisAttribute::_publish_cc_universal_value( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_universal_value" );
  ASSERT(_is_signal_attribute() == FALSE);
  ASSERT(get_kind() != IIR_RANGE_ATTRIBUTE);
  ASSERT(get_kind() != IIR_REVERSE_RANGE_ATTRIBUTE);
  _cc_out << "((";
  _get_prefix()->_publish_cc_universal_type( _cc_out );
  _cc_out << "& )";
  _publish_cc_rvalue( _cc_out, declarations );
  _cc_out << ".readVal())";
}

void
TyvisAttribute::_publish_cc_left( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_left" );
  ASSERT(get_kind() == IIR_RANGE_ATTRIBUTE || get_kind() == IIR_REVERSE_RANGE_ATTRIBUTE);
  ASSERT(_get_prefix() && (_get_prefix()->is_array_type() == TRUE));
  TyvisLeftAttribute *left_attr = new TyvisLeftAttribute();
  copy_location( this, left_attr );
  left_attr->set_prefix(_get_prefix());
  left_attr->set_suffix(_get_suffix());
  left_attr->_publish_cc_value( _cc_out, declarations );
  delete left_attr;
}

void
TyvisAttribute::_publish_cc_right( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_right" );
  ASSERT(this->get_kind() == IIR_RANGE_ATTRIBUTE || this->get_kind() == IIR_REVERSE_RANGE_ATTRIBUTE);
  ASSERT(_get_prefix() && (_get_prefix()->is_array_type() == TRUE));
  TyvisRightAttribute *right_attr = new TyvisRightAttribute();
  copy_location( this, right_attr );
  right_attr->set_prefix(_get_prefix());
  right_attr->set_suffix(_get_suffix());
  right_attr->_publish_cc_value( _cc_out, declarations );
  delete right_attr;
}

void
TyvisAttribute::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_value" );

  ASSERT(_is_signal_attribute() == FALSE);
  _publish_cc_universal_value( _cc_out, declarations );
}

void
TyvisAttribute::_publish_cc_as_for_loop( published_file &_cc_out,
					 PublishData *declarations,
					 const char *forConstant ){
  TyvisScalarTypeDefinition *range = NULL;

  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_as_for_loop" );
  
  if (_get_prefix()->is_array_type() == TRUE) {
    // Okay, extract the index constraint for this guy depending on the
    // suffix
    TyvisArrayTypeDefinition *aType = dynamic_cast<TyvisArrayTypeDefinition *>( _get_prefix()->_get_subtype());
    ASSERT ( aType->_get_resolved_index_subtype() != NULL );
    ASSERT ( aType->_get_resolved_index_subtype()->is_scalar_type() == TRUE );
    range = dynamic_cast<TyvisScalarTypeDefinition *>(aType->_get_resolved_index_subtype());
  }
  else {
    ASSERT ( _get_prefix()->is_scalar_type() == TRUE );
    if (_get_prefix()->is_object() == TRUE) {
      range = dynamic_cast<TyvisScalarTypeDefinition *>( _get_prefix()->_get_subtype());
    }
    else {
      range = dynamic_cast<TyvisScalarTypeDefinition *>(_get_prefix());
    }
  }
  
  ASSERT ( range != NULL );
  
  _cc_out << "for(int " << forConstant << " = ";

  if (range->is_ascending_range() == TRUE) {
    _cc_out << "0";
  }
  else {
    _cc_out << "(";
    range->_get_base_type_left()->_publish_cc_universal_value( _cc_out, declarations );
    _cc_out << " - ";
    range->_get_base_type_right()->_publish_cc_universal_value( _cc_out, declarations );
    _cc_out << ")";
  }
  
  _cc_out << "; (" << forConstant;
  
  if (range->is_ascending_range() == TRUE) {
    _cc_out << " <= ";
  }
  else {
    _cc_out << " >= ";
  }

  if (range->is_ascending_range() == TRUE) {
    _cc_out << "(";
    range->_get_base_type_right()->_publish_cc_universal_value( _cc_out, declarations );
    _cc_out << " - ";
    range->_get_base_type_left()->_publish_cc_universal_value( _cc_out, declarations );
    _cc_out << ")";
  }
  else {
    _cc_out << "0";
  }

  _cc_out << "); ";

  if (range->is_ascending_range() == TRUE) {
    _cc_out << forConstant << "++";
  }
  else {
    _cc_out << forConstant << "--";
  }

  _cc_out << ")  ";
}

void
TyvisAttribute::_publish_cc_static_type_info_fn( published_file &_cc_out,
						 PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisAttribute::_publish_cc_type_info" );

  ASSERT ( _get_prefix() != NULL );
  ASSERT ( _get_prefix()->_get_subtype() != NULL );
  ASSERT ( _get_prefix()->_get_subtype()->is_resolved() == TRUE );
  
  _get_prefix()->_get_subtype()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisAttribute::_publish_cc_locally_static_value( published_file &_cc_out,
						  PublishData *declarations ){
  if( implicit_declaration == 0 ){
    implicit_declaration = _build_implicit_declaration( _get_attribute_name(),
							_get_subtype() );
  }
  implicit_declaration->_publish_cc_locally_static_value( _cc_out, declarations );
}

void
TyvisAttribute::_add_decl_into_declaration_collection(PublishData *declarations) {
  if(_get_prefix() != NULL) {
    _get_prefix()->_add_decl_into_declaration_collection(declarations);
  }
  if(_get_suffix() != NULL) {
    _get_suffix()->_add_decl_into_declaration_collection(declarations);
  }
}

TyvisTextLiteral *
TyvisAttribute::_get_attribute_name(){
  if( my_attribute_name == NULL ){
    my_attribute_name = _build_attribute_name( _get_cc_attribute_name() );
    copy_location( this, my_attribute_name );
  }
  
  return my_attribute_name;
}

TyvisDeclaration*
TyvisAttribute::_get_actual_signal_declaration() {
  ASSERT ( _get_prefix() != NULL );
  ASSERT ( _get_prefix()->is_resolved() == TRUE );
  
  TyvisDeclaration *decl   = NULL;
  Tyvis             *prefix = _get_prefix();
  
  ASSERT ((prefix->is_signal() == TRUE) || ( dynamic_cast<TyvisAttribute *>(prefix) ));
  
  if (prefix->is_signal() == TRUE) {
    if ( dynamic_cast<TyvisName *>(prefix) ) {
      decl = prefix->_get_prefix_declaration();
    }
    else {
      decl = dynamic_cast<TyvisDeclaration *>(prefix);
    }
  }
  else {
    ASSERT ( dynamic_cast<TyvisAttribute *>(prefix) );
    decl = (dynamic_cast<TyvisAttribute *>(prefix))->_get_implicit_declaration();
  }

  ASSERT ( decl != NULL );
  ASSERT ( dynamic_cast<TyvisDeclaration *>(decl) );
  ASSERT ( decl->_get_subtype() != NULL );
  
  return decl;
}

void
TyvisAttribute::_check_and_add_necessary_decls(PublishData *declarations) {
  TyvisDeclaration         *declToCheck = _get_implicit_declaration();

  ASSERT ( _get_prefix() != NULL );

  if (_get_prefix()->get_kind() == IIR_INDEXED_NAME) {
    TyvisIndexedName *idxName = dynamic_cast<TyvisIndexedName *>( _get_prefix());
    ASSERT ( idxName->get_kind() == IIR_INDEXED_NAME );
    ASSERT ( idxName->_get_prefix_declaration() != NULL );
      
    declToCheck = idxName->_get_prefix_declaration()->_get_implicit_declaration_for_attribute( (TyvisAttribute *) this );
  } 

  if ((declToCheck != NULL) && (!declarations->in_collection(declToCheck))) {
    declarations->add_declaration(declToCheck);
  }
}


Tyvis *
TyvisAttribute::_get_suffix( ){
  return dynamic_cast<Tyvis *>(get_suffix());
}

TyvisTypeDefinition*
TyvisAttribute::_get_explicit_signal_type() {
  Tyvis* prefix_decl = _get_prefix();
  ASSERT((prefix_decl->is_signal() == TRUE)  ||
	 (dynamic_cast<TyvisAttribute *>(prefix_decl)));
  if(prefix_decl->is_signal() == TRUE) {
    return (dynamic_cast<TyvisDeclaration*>(prefix_decl))->_get_subtype();
  }
  else {
    return (dynamic_cast<TyvisAttribute*>(prefix_decl))->_get_explicit_signal_type();
  }
}

TyvisTextLiteral *
TyvisAttribute::_build_attribute_name( const string &attribute_name ){
  IIR_TextLiteral *retval =
    TyvisIdentifier::get( attribute_name,
			  get_design_file()->get_class_factory() );
  ASSERT( dynamic_cast<TyvisTextLiteral *>(retval) != 0 );
  return dynamic_cast<TyvisTextLiteral *>(retval);
}

void
TyvisAttribute::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  if(_is_signal_attribute() == TRUE && is_signal() == TRUE) {
    ASSERT(_get_implicit_declaration() != NULL);
    ASSERT(_get_implicit_declaration()->is_resolved() == TRUE);
    //_get_implicit_declaration()->_build_sensitivity_list(sensitivity_list);
    TyvisDesignator *node = dynamic_cast<TyvisDesignator *>(sensitivity_list->first());
    while (node != NULL) {
      ASSERT(node->get_kind() == IIR_DESIGNATOR_EXPLICIT);
      if ((dynamic_cast<TyvisDesignatorExplicit *>(node))->get_name() == this) {
	break;
      }
      node = dynamic_cast<TyvisDesignator *>(sensitivity_list->successor(node));
    }
    if (node == NULL) {
      TyvisDesignatorExplicit *new_designator = new TyvisDesignatorExplicit;
      copy_location( this, new_designator );
      new_designator->set_name(this);
      sensitivity_list->append(new_designator);
    }
  }
  _get_prefix()->_build_sensitivity_list(sensitivity_list);
  if(has_suffix() == TRUE && _get_suffix() != NULL) {
    _get_suffix()->_build_sensitivity_list(sensitivity_list);
  }
}

TyvisConstantDeclaration *
TyvisAttribute::_build_constant_declaration( TyvisTextLiteral *constant_name,
					     TyvisTypeDefinition *constant_type ){
  TyvisConstantDeclaration *retval;
  
  retval = new TyvisConstantDeclaration;
  copy_location( this, retval );
  retval->set_declarator( constant_name );
  retval->set_subtype( constant_type );
  retval->set_is_visible( FALSE );
  retval->set_is_implicit( TRUE );
  return retval;
}

IIR_Boolean 
TyvisAttribute::_is_globally_static_primary(){
  ASSERT( _get_prefix()->is_resolved() == TRUE );
  if( is_value_attribute() == TRUE &&
      get_kind() != IIR_USER_ATTRIBUTE && 
      _get_prefix()->_is_globally_static() == TRUE ){
    return TRUE;
  }
  else if( is_function_attribute() == TRUE &&
	   get_kind() != IIR_USER_ATTRIBUTE &&
	   get_kind() != IIR_EVENT_ATTRIBUTE &&
	   get_kind() != IIR_ACTIVE_ATTRIBUTE &&
	   get_kind() != IIR_LAST_EVENT_ATTRIBUTE &&
	   get_kind() != IIR_LAST_ACTIVE_ATTRIBUTE &&
	   get_kind() != IIR_LAST_VALUE_ATTRIBUTE &&
	   get_kind() != IIR_DRIVING_ATTRIBUTE &&
	   get_kind() != IIR_DRIVING_VALUE_ATTRIBUTE &&
	   ((_get_prefix()->is_subtype() == TRUE && _get_prefix()->_is_globally_static() == TRUE)||
	    ( (_get_prefix()->is_object() == TRUE || 
	       _get_prefix()->get_kind() == IIR_FUNCTION_CALL ) &&  
	      _get_prefix()->_get_subtype()->_is_globally_static() == TRUE ) ) &&
	   ( _get_suffix() == NULL || _get_suffix()->_is_globally_static() == TRUE )){
    return TRUE;
  }
  else{
    return FALSE;
  }
}

void
TyvisAttribute::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  if(_get_prefix()->is_signal() == TRUE) {
    _get_prefix()->_get_list_of_input_signals(list);
  }
}

TyvisDeclaration *
TyvisAttribute::_build_implicit_declaration( TyvisTextLiteral *, TyvisTypeDefinition * ){
  _report_undefined_fn("_get_implicit_declaration( const string &, TyvisTypeDefinition * )");
  return NULL;
}

TyvisDeclaration *
TyvisAttribute::_get_implicit_declaration( ){
  return implicit_declaration;
}

TyvisSignalDeclaration *
TyvisAttribute::_build_signal_declaration( TyvisTextLiteral *signal_name,
					   TyvisTypeDefinition *signal_type ){
  TyvisSignalDeclaration *retval;
  retval = new TyvisSignalDeclaration();
  copy_location( this, retval );
  retval->set_declarator( signal_name );
  retval->set_subtype( signal_type );
  retval->set_declarative_region( _get_current_declarative_region() );
  retval->set_is_visible( FALSE );
  retval->set_is_implicit( TRUE );
  if(get_kind() == IIR_ABOVE_ATTRIBUTE) { 
    copy_location(this, retval); 
//     retval->_add_declaration();
  }
  return retval;
}

TyvisFunctionDeclaration *
TyvisAttribute::_build_function_declaration( TyvisTextLiteral *function_name,
					     TyvisTypeDefinition *return_value,
					     TyvisTypeDefinition *argument_type ){
  TyvisFunctionDeclaration *retval;
  retval = new TyvisFunctionDeclaration;
  copy_location( this, retval );
  retval->set_declarator( function_name );
  retval->set_return_type( return_value );
  retval->set_declarative_region( this );
  retval->set_is_visible( FALSE );
  retval->set_is_implicit( TRUE );
  if( argument_type != NULL ){
    TyvisVariableInterfaceDeclaration *interface_decl = new TyvisVariableInterfaceDeclaration;
    copy_location( this, interface_decl );
    char *interface_name = "X";
    interface_decl->set_declarator(IIRBase_Identifier::get( interface_name, 
							    strlen( interface_name ),
							    get_design_file()->get_class_factory()
							    )
				   );
    interface_decl->set_subtype( argument_type  );
    retval->get_interface_declarations()->append( interface_decl );
    retval->set_is_visible( FALSE );
    retval->set_is_implicit( TRUE );
  }
  return retval;
}

TyvisTypeDeclaration *
TyvisAttribute::_build_type_declaration( TyvisTextLiteral *type_name, 
					 TyvisTypeDefinition *decl_type ){
  TyvisTypeDeclaration *retval = new TyvisTypeDeclaration();
  copy_location( this, retval );
  retval->set_type( decl_type );
  retval->set_declarator( type_name );
  retval->set_declarative_region( _get_current_declarative_region() );
  retval->set_is_visible( FALSE );
  retval->set_is_implicit( TRUE );
  return retval;
}

TyvisFreeQuantityDeclaration *
TyvisAttribute::_build_free_quantity_declaration( TyvisTextLiteral *quantity_name,
						  TyvisTypeDefinition *quantity_type){
  TyvisFreeQuantityDeclaration *retval = new TyvisFreeQuantityDeclaration;
  retval->set_declarator( quantity_name );
  retval->set_value( dynamic_cast<TyvisFreeQuantityDeclaration *>(_get_prefix())->_get_value() );
  retval->set_subtype( quantity_type );
  retval->set_is_implicit( TRUE );
  return retval;
}

TyvisBranchQuantityDeclaration *
TyvisAttribute::_build_branch_quantity_declaration( TyvisTextLiteral *quantity_name,
						    TyvisTypeDefinition *quantity_type ){
  TyvisBranchQuantityDeclaration *retval = new TyvisBranchQuantityDeclaration;
  TyvisBranchQuantityDeclaration *my_decl = dynamic_cast<TyvisBranchQuantityDeclaration *>(get_prefix());
  retval->set_declarator( quantity_name );
  retval->set_across_aspect_expression(my_decl->get_across_aspect_expression());
  retval->set_across_aspect_tolerance(my_decl->get_across_aspect_tolerance());
  retval->set_through_aspect_expression(my_decl->get_through_aspect_expression());
  retval->set_through_aspect_tolerance(my_decl->get_through_aspect_tolerance());
  retval->set_plus_terminal_name( my_decl->get_plus_terminal_name() );
  retval->set_minus_terminal_name( my_decl->get_minus_terminal_name() );
  retval->set_subtype(quantity_type);
  if(my_decl->_is_across_quantity()==TRUE) {
    retval->get_across_aspect_identifier_list()->append(dynamic_cast<IIR_Designator *>(my_decl->get_across_aspect_identifier_list()->first()));
  }
  else {
    retval->get_through_aspect_designator_list()->append(dynamic_cast<IIR_Designator *>(my_decl->get_through_aspect_designator_list()->first()));
  }
  retval->set_is_implicit(TRUE);
  return retval;
}

void
TyvisAttribute::_publish_cc_default_array_suffix( published_file &_cc_out ){
  _cc_out << "UniversalInteger(1)";
}

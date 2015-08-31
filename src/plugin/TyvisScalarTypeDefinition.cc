
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
#include "TyvisEnumerationLiteral.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisFloatingSubtypeDefinition.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisLeftAttribute.hh"
#include "TyvisRangeTypeDefinition.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisTextLiteral.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>
#include "savant/StandardPackage.hh"

string
TyvisScalarTypeDefinition::_get_cc_index_type_info(){
  return _get_cc_type_name() + "IndexTypeInfo()";
}

void 
TyvisScalarTypeDefinition::_publish_cc_left( published_file &_cc_out,
					     PublishData *declarations ) {
  _publish_cc_type_info_xxx( _cc_out, declarations );
  _cc_out << ".left()";
}

void 
TyvisScalarTypeDefinition::_publish_cc_right( published_file &_cc_out,
					      PublishData *declarations ) {
  _publish_cc_type_info_xxx( _cc_out, declarations );
  _cc_out << ".right()";
}

void
TyvisScalarTypeDefinition::_publish_cc_range_method( published_file &_cc_out,
						     const string &type_name ){
  CC_REF( _cc_out, "TyvisScalarTypeDefinition::_publish_cc_range_method" );
  _cc_out << "inline const IntegerTypeInfo &" << NL()
	  << type_name << "Range()" << OS("{")
	  << "static IntegerTypeInfo _range" << OS("(");
  TyvisScalarTypeDefinition::_publish_cc_type_info_args( _cc_out, 0, type_name );
  _cc_out << CS(");")
	  << "return _range;"
	  << CS("}");
}

void
TyvisScalarTypeDefinition::_publish_cc_range( published_file &_cc_out,
					      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisScalarTypeDefinition::_publish_cc_range" );
  TyvisScalarTypeDefinition *to_publish;
  if( get_left() != NULL ){
    to_publish = dynamic_cast<TyvisScalarTypeDefinition *>(this);
  }
  else{
    ASSERT( _get_base_type() != NULL );
    ASSERT( dynamic_cast<TyvisScalarTypeDefinition *>(_get_base_type()) != NULL );
    to_publish = dynamic_cast<TyvisScalarTypeDefinition *>(_get_base_type());
  }
  ASSERT( to_publish->_get_base_type_left() != NULL );
  ASSERT( to_publish->_get_base_type_left()->is_resolved() == TRUE );
  ASSERT( to_publish->_get_direction() != NULL );
  ASSERT( to_publish->_get_direction()->get_kind() == IIR_ENUMERATION_LITERAL );
  ASSERT( to_publish->_get_base_type_right() != NULL );
  ASSERT( to_publish->_get_base_type_right()->is_resolved() == TRUE );
  
  to_publish->_get_base_type_left()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << "," << NL();
  dynamic_cast<TyvisEnumerationLiteral *>(to_publish->get_direction())->_publish_cc_direction( _cc_out );
  _cc_out << "," << NL();
  to_publish->_get_base_type_right()->_publish_cc_rvalue( _cc_out, declarations );
}

void
TyvisScalarTypeDefinition::_publish_cc_init_signal( published_file &_cc_out,
						    PublishData *declarations ) {
  ASSERT( dynamic_cast<TyvisDeclaration *>(_get_current_publish_node()) != NULL );
  if( dynamic_cast<TyvisDeclaration*>(_get_current_publish_node())->is_implicit_declaration() ){
    _publish_cc_init_implicit_signal( _cc_out ); 
  }
  else {
    _publish_cc_init_explicit_signal( _cc_out, declarations ); 
  }
}

void
TyvisScalarTypeDefinition::_publish_cc_init_implicit_signal( published_file &_cc_out ) {
  _cc_out << "ImplicitSignal<";
  _publish_cc_universal_type( _cc_out );
  _cc_out << ">* signal = (ImplicitSignal<"; 
  _publish_cc_universal_type( _cc_out );
  ASSERT( _get_current_publish_name() != "" );
  _cc_out << ">*) (*(ScalarType*)&this->" << _get_current_publish_name() << ")"
	  << ".getObject();" << NL();

  // I expect _current_another_name to be set to the string representation
  // of the signal's netinfo structure when I get here.  It is set in
  // TyvisSignalDeclaration_publish_cc_init_signal( published_file &_cc_out ).
  ASSERT( _get_current_another_name() != "" );

  _cc_out << "signal->type =";
  ASSERT( dynamic_cast<TyvisDeclaration *>(_get_current_publish_node()) != NULL );
  (dynamic_cast<TyvisDeclaration*>(_get_current_publish_node()))->_get_attribute_name()->_publish_cc_sigtype( _cc_out );

  _cc_out << "signal->drvVal = ";
  (dynamic_cast<TyvisDeclaration*>(_get_current_publish_node()))->_get_attribute_name()->_publish_cc_init_val( _cc_out );
  _cc_out << NL();

  _cc_out << "signal->effVal = ";
  (dynamic_cast<TyvisDeclaration*>(_get_current_publish_node()))->_get_attribute_name()->_publish_cc_init_val( _cc_out );
  _cc_out << ";" << NL();
  _cc_out << "signal->name = \"" << _get_current_publish_name() << "\";" << NL();
}

void
TyvisScalarTypeDefinition::_publish_cc_init_explicit_signal( published_file &_cc_out, PublishData *declarations ) {
  // NOTE : The values set in _current_publish_name and _current_another_name
  //        are important. Care must be taken to set these names correctly.
  //        Incase of publishing for RecordTypes, the function expects
  //        _current_publish_name to point to the record name and
  //        _current_another_name to point to the field name.
  //
  //        When publishing scalar types, the variable _current_publish_name
  //        should be set to the scalar variable name while
  //        _current_another_name must be set to NULL

  _cc_out << "Signal<";
  _publish_cc_universal_type( _cc_out );
  _cc_out << ">* signal = (Signal<"; 
  _publish_cc_universal_type( _cc_out );
  _cc_out << ">*) (*(ScalarType*)&this->" << _get_current_publish_name();
  _cc_out << ")"  << ".getObject();" << NL();

  //  _cc_out << "  signal->sensitive = true;\n";
  // XXX not all signals are sensitive; this will be slightly inefficient

  if( _get_current_publish_node()->is_signal()) {
    if( _get_current_publish_node()->get_value() == NULL) {
      _cc_out << "signal->drvVal = ";
      _publish_cc_left( _cc_out, declarations );
      _cc_out << ";" << NL();
      
      _cc_out << "signal->effVal = ";
      _publish_cc_left( _cc_out, declarations );
      _cc_out << ";" << NL();
    }
  }
  _cc_out << "  signal->name = \"" << _get_current_publish_name() << "\";\n";
}

void
TyvisScalarTypeDefinition::_publish_cc_init_last_event( published_file &_cc_out ) {
  _cc_out << _get_current_publish_name() << ";" << NL();
}

void
TyvisScalarTypeDefinition::_publish_cc_class_last_event( published_file & ){}

void 
TyvisScalarTypeDefinition::_publish_cc_class_event( published_file & ){}

void 
TyvisScalarTypeDefinition::_publish_cc_array_info( published_file &_cc_out, PublishData *declarations ){
  _cc_out << OS("ArrayInfo(");
  if( get_left() != NULL) {
    ASSERT( get_right() != NULL);
    ASSERT( get_direction() != NULL);
    _publish_cc_range( _cc_out, declarations );
  }
  else {
    _cc_out << "left1, dirn1, right1";
  }
  _cc_out << CS(")");
}

void
TyvisScalarTypeDefinition::_publish_cc_init_val( published_file &_cc_out,
						 PublishData *declarations ) {
  _publish_cc_left( _cc_out, declarations );
}

void
TyvisScalarTypeDefinition::_publish_cc_signal_value_left( published_file &_cc_out,
							  PublishData *declarations ) {
  if( _get_current_publish_node()->is_signal() ){
    if( _get_current_publish_node()->get_value() == NULL ){
      _publish_cc_left( _cc_out, declarations );
    }
    else {
      dynamic_cast<Tyvis *>(_get_current_publish_node()->get_value())->_publish_cc_universal_value( _cc_out, declarations );
    }
  }
}

void
TyvisScalarTypeDefinition::_publish_cc_decl_type_attributes( published_file & ) {}

void
TyvisScalarTypeDefinition::_publish_cc_define_type_attributes( published_file & ) {}

void 
TyvisScalarTypeDefinition::_publish_cc_range_check( published_file &_cc_out, PublishData *declarations ) {
  if(is_ascending_range()) {
    _cc_out.open_scope( "(val >= (" );
    _publish_cc_universal_type( _cc_out );
    _cc_out.open_scope( "(" );
    _get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out.close_scope( ")" );
    _cc_out.close_scope( ")" );
    _cc_out.open_scope( " && val <= (" );
    _publish_cc_universal_type( _cc_out );
    _cc_out.open_scope( "(" );
    _get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    _cc_out.close_scope( ")" );
    _cc_out.close_scope( ")" );
  }
  else {
    _cc_out.open_scope( "(val <= (" );
    _publish_cc_universal_type( _cc_out );
    _cc_out.open_scope( "(" );
    _get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out.close_scope( ")" );
    _cc_out.close_scope( ")" );
    _cc_out.open_scope( " && val >= (" );
    _publish_cc_universal_type( _cc_out );
    _cc_out.open_scope( "(" );
    _get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    _cc_out.close_scope( ")" );
    _cc_out.close_scope( ")" );
    _cc_out.close_scope( ")" );
  }
}

void 
TyvisScalarTypeDefinition::_publish_cc_decl_destructors( published_file &_cc_out ) {
  _cc_out << "virtual ~";
  _publish_cc_type_info_xxx( _cc_out, 0 );
  _cc_out << "(){}" << NL();
}

void 
TyvisScalarTypeDefinition::_publish_cc_decl_constructors( published_file &_cc_out ) {
  if(is_subtype_decl()) {
    _publish_cc_subtype_constructors( _cc_out );
  } else {
    _publish_cc_type_constructors( _cc_out );
  }
}

void 
TyvisScalarTypeDefinition::_publish_cc_type_constructors( published_file & ){}

void 
TyvisScalarTypeDefinition::_publish_cc_subtype_constructors( published_file & ) {}

void 
TyvisScalarTypeDefinition::_publish_cc_headers( published_file &_cc_out ) {
  if( !_is_kernel_type() ){
    CC_REF( _cc_out, "TyvisScalarTypeDefinition::_publish_cc_headers( published_file &_cc_out )");
    if( this != _get_parent_type() ){
      _get_parent_type()->_publish_cc_headers( _cc_out );
    }
    if( is_subtype() && _get_type_mark() != NULL ){
      _get_type_mark()->_publish_cc_headers( _cc_out );
    }
    //For a type its ranges can be specified in terms of othher types.
    // and that type's attributes. Then in that case those types.hh have to
    //be included in the .cc file
    if( _get_base_type_left() != 0 ){
      _get_base_type_left()->_publish_cc_headers( _cc_out );
    }
    if( _get_base_type_right() != 0 ){
      _get_base_type_right()->_publish_cc_headers( _cc_out );
    }
  }
}

void
TyvisScalarTypeDefinition::_publish_cc_necessary_decl_in_state( published_file & ) {}

void 
TyvisScalarTypeDefinition::_add_decl_into_declaration_collection(PublishData *declarations) {
  if(_get_base_type() != NULL) {
    _get_base_type()->_add_decl_into_declaration_collection(declarations);
  }
  if(_get_base_type_left() != NULL) {
    _get_base_type_left()->_add_decl_into_declaration_collection(declarations);
  }
  if(_get_base_type_right() != NULL) {
    _get_base_type_right()->_add_decl_into_declaration_collection(declarations);
  }
}

TyvisTypeDefinition *
TyvisScalarTypeDefinition::_get_base_type() {
  return dynamic_cast<TyvisTypeDefinition *>(get_base_type());
}

IIR_Boolean
TyvisScalarTypeDefinition::_is_bit_type() {
  IIR_Boolean retval;

  if( this == dynamic_cast<TyvisTypeDefinition *>(get_design_file()->get_standard_package()->get_bit_type()) ){
    retval = TRUE;
  }
  else{
    retval = FALSE;
  }
  return retval;
}

IIR_Boolean
TyvisScalarTypeDefinition::_is_boolean_type() {
  IIR_Boolean retval;

  if( this == dynamic_cast<TyvisEnumerationSubtypeDefinition *>(get_design_file()->get_standard_package()->get_boolean_type()) ){
    retval = TRUE;
  }
  else{
    retval = FALSE;
  }
  return retval;
}

Tyvis*
TyvisScalarTypeDefinition::_get_direction(){
  return dynamic_cast<Tyvis *>(get_direction());
}

Tyvis *
TyvisScalarTypeDefinition::_get_left() {
  return dynamic_cast<Tyvis *>(get_left());
}

Tyvis *
TyvisScalarTypeDefinition::_get_right() {
  return dynamic_cast<Tyvis *>(get_right());
}

IIR_Boolean 
TyvisScalarTypeDefinition::_is_globally_static(){
  if( _get_base_type_left() != NULL && _get_base_type_left()->_is_globally_static() == TRUE &&
      _get_base_type_right() != NULL && _get_base_type_right()->_is_globally_static() == TRUE ){
    return TRUE;
  }
  else{
    return FALSE;
  }
}

void
TyvisScalarTypeDefinition::_publish_cc_type_info_args( published_file &_cc_out,
						       PublishData *declarations,
						       const string & ){
  CC_REF( _cc_out, "TyvisScalarTypeDefinition::_publish_cc_type_info_args" );

  if( _get_left() ){
    _get_left()->_publish_cc_rvalue( _cc_out, declarations );
  }
  else{
    ASSERT( _get_base_type_left() );
    _get_base_type_left()->_publish_cc_rvalue( _cc_out, declarations );
  }
  _cc_out << "," << NL();
  if( _get_direction() ){
    _get_direction()->_publish_cc_direction( _cc_out );
  }
  else{
    ASSERT( _get_base_type_direction() );
    _get_base_type_direction()->_publish_cc_direction( _cc_out );
  }
  _cc_out << ", " << NL();
  if( _get_right() ){
    _get_right()->_publish_cc_rvalue( _cc_out, declarations );
  }
  else{
    ASSERT( _get_base_type_right() );
    _get_base_type_right()->_publish_cc_rvalue( _cc_out, declarations );    
  }
}

string
TyvisScalarTypeDefinition::_get_cc_dynamic_type_info(){
  string retval;

  retval = _get_cc_tyvis_type() + "( "
    + _get_left()->_get_cc_rvalue()
    + ", " 
    + _get_direction()->_get_cc_direction() + ", "
    + _get_right()->_get_cc_rvalue() 
    + " )";

  return retval;
}

bool
TyvisScalarTypeDefinition::_need_to_publish_range(){
  return (get_base_type() &&
	  (dynamic_cast<TyvisScalarTypeDefinition *>(get_base_type())->get_left()!=get_left() ||
	   dynamic_cast<TyvisScalarTypeDefinition *>(get_base_type())->get_right()!=get_right()));
}

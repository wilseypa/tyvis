
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

#include "TyvisAssociationList.hh"
#include "TyvisConcurrentGenerateForStatement.hh"
#include "TyvisConstantDeclaration.hh"
#include "TyvisLabel.hh"
#include "TyvisRangeAttribute.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisSliceName.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void
TyvisSliceName::_publish_cc_lvalue( published_file &(_cc_out), PublishData *declarations ) {
  ASSERT(is_resolved() == TRUE);
  _publish_cc_include( _cc_out, "tyvis/IntegerTypeInfo.hh" );
  _cc_out << "dynamic_cast<LValue &>(";
  _cc_out << "Value(";
  _get_prefix()->_publish_cc_lvalue( _cc_out , declarations );
  _cc_out << ", IntegerTypeInfo(";

  _get_suffix()->_publish_cc_range( _cc_out, declarations );
  _cc_out << ")).readVal()";
  _cc_out << ")"; 
}

void
TyvisSliceName::_publish_cc_rvalue( published_file &(_cc_out), PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSliceName::_publish_cc_rvalue" );	
  ASSERT(is_resolved() == TRUE);
  _publish_cc_include( _cc_out, "tyvis/IntegerTypeInfo.hh" );
  _cc_out << "Value(";
  _get_prefix()->_publish_cc_rvalue( _cc_out , declarations );
  _cc_out << ", IntegerTypeInfo(";

  _get_suffix()->_publish_cc_range( _cc_out, declarations );
  _cc_out << "))";
}

void
TyvisSliceName::_publish_cc_rvalue_noprefix( published_file &(_cc_out), PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSliceName::_publish_cc_rvalue" );	
  ASSERT(is_resolved() == TRUE);
  _publish_cc_include( _cc_out, "tyvis/IntegerTypeInfo.hh" );
  _get_prefix()->_publish_cc_rvalue( _cc_out , declarations );
  _cc_out << ", IntegerTypeInfo(";

  _get_suffix()->_publish_cc_range( _cc_out, declarations );
  _cc_out << "))";
}

void
TyvisSliceName::_publish_cc_object_name( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_lvalue( _cc_out , declarations );
}

void
TyvisSliceName::_publish_cc_slice_name_as_for_loop( published_file &_cc_out, PublishData *declarations ) {
  TyvisScalarTypeDefinition *range = NULL;
  TyvisScalarTypeDefinition *generate_range = NULL;
  TyvisConcurrentGenerateForStatement *generate_for = NULL;

  // Work around for bad aire tree. Once the parser is fixed to give the
  // right iir node .ie. index name, this work around can be removed.
  // The necessary code is already in indexed name.

  if ( dynamic_cast<TyvisScalarTypeDefinition *>(_get_suffix()) == NULL) {
    if ((_get_suffix()->get_kind() == IIR_RANGE_ATTRIBUTE) ||
	(_get_suffix()->get_kind() == IIR_REVERSE_RANGE_ATTRIBUTE)) {
      TyvisAttribute* attr = dynamic_cast<TyvisAttribute *>( get_suffix() );
      attr->_publish_cc_as_for_loop( _cc_out, declarations, "generateConstant" );
    }
    else {
      _cc_out << "for(int generateConstant = ";
      _get_suffix()->_publish_cc_value( _cc_out, declarations );
      _cc_out << "; (generateConstant == ";
      _get_suffix()->_publish_cc_value( _cc_out, declarations );
      _cc_out << "); generateConstant++)\n";
    }
    return;
  }
    
  ASSERT ( _get_prefix()->_is_concurrent_generate_statement() == TRUE );
  ASSERT ( _get_prefix()->get_kind() == IIR_LABEL );
  ASSERT ( (dynamic_cast<TyvisLabel *>( get_prefix() ))->get_statement()->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT );
  ASSERT ( dynamic_cast<TyvisScalarTypeDefinition *>(_get_suffix()) != NULL );
  
  range          = dynamic_cast<TyvisScalarTypeDefinition *>( get_suffix() );
  generate_for   = dynamic_cast<TyvisConcurrentGenerateForStatement *>( (dynamic_cast<TyvisLabel *>( get_prefix() ))->get_statement() );
  generate_range = dynamic_cast<TyvisScalarTypeDefinition *>( generate_for->get_generate_parameter_specification()->get_subtype() );
  
  _cc_out << "for(int generateConstant = ";
  
  if (range->is_ascending_range()) {
    _cc_out << "(";
    range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
    generate_range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << ");";

    _cc_out << "(generateConstant <=";
  }
  else {
    _cc_out << "(";
    generate_range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
    range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << ");";
    
    _cc_out << "(generateConstant >";
  }
  
  if (range->is_ascending_range()) {
    _cc_out << "(";
    
    range->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
    generate_range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << ")";
  }
  else {
    _cc_out << "(";

    generate_range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
    range->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    _cc_out << ")";
  }

  _cc_out << "); generateConstant++)\n";
}

void
TyvisSliceName::_publish_cc_declarator( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_declarator( _cc_out, declarations );
}

void
TyvisSliceName::_publish_cc_name_elaborate( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
}

void
TyvisSliceName::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(is_resolved() == TRUE);
  _get_suffix()->_publish_cc_range( _cc_out, declarations );
}

void
TyvisSliceName::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {
  _cc_out << "ArrayInfo(";
  _publish_cc_range( _cc_out, declarations );
  _cc_out << ")";
}

void
TyvisSliceName::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  _cc_out << "Value(";
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << ", ArrayInfo(";

  _get_suffix()->_publish_cc_range( _cc_out, declarations );
  _cc_out << "))";
}

void
TyvisSliceName::_add_decl_into_declaration_collection(PublishData *declarations) {
  _get_prefix()->_add_decl_into_declaration_collection(declarations);
  _get_suffix()->_add_decl_into_declaration_collection(declarations);
}

void
TyvisSliceName::_set_passed_through_out_port(IIR_Boolean val) {
  ASSERT ( _get_prefix() != NULL );
  _get_prefix()->_set_passed_through_out_port(val);
}

TyvisList *
TyvisSliceName::_get_statement_list(){
  ASSERT( dynamic_cast<TyvisLabel *>(_get_prefix()) != NULL );
  return _get_prefix()->_get_statement_list();
}

IIR_Boolean
TyvisSliceName::_is_concurrent_generate_statement() {
  return _get_prefix()->_is_concurrent_generate_statement();
}

IIR_Mode
TyvisSliceName::_get_mode() {
  return _get_prefix()->_get_mode();
}


TyvisDeclaration*
TyvisSliceName::_get_prefix_declaration() {
  return _get_prefix()->_get_prefix_declaration();
}

Tyvis *
TyvisSliceName::_get_suffix(){
  ASSERT(get_suffix() != NULL);
  return dynamic_cast<Tyvis *>(get_suffix());
}

IIR_Boolean 
TyvisSliceName::_is_globally_static_primary(){
  ASSERT( _get_prefix() != NULL );
  ASSERT( _get_suffix() != NULL );
  ASSERT( is_resolved() == TRUE );

  if( _get_prefix()->_is_globally_static() == TRUE && _get_suffix()->_is_globally_static() ){
    return TRUE;
  }
  else{
    return FALSE;
  }
}

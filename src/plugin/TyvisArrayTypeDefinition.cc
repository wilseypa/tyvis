
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
#include "TyvisArraySubtypeDefinition.hh"
#include "TyvisAttribute.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisEnumerationLiteralList.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisRangeTypeDefinition.hh"
#include "TyvisTextLiteral.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "published_file.hh"
#include <sstream>
#include <clutils/StringUtilities.h>

void 
TyvisArrayTypeDefinition::_publish_cc_left( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_left" );

  ASSERT(get_element_subtype() != NULL);
  _get_element_subtype()->_publish_cc_left( _cc_out, declarations );
}

const string
TyvisArrayTypeDefinition::_get_cc_tyvis_type(){
  return "ArrayTypeInfo";
}

void
TyvisArrayTypeDefinition::_publish_cc_universal_type( published_file &_cc_out ) {
  _publish_cc_include( _cc_out, "tyvis/Aggregate.hh" );
  _cc_out << "Aggregate";
}

void
TyvisArrayTypeDefinition::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_headers" );

  if( get_index_subtype() != 0 ){
    _get_index_subtype()->_publish_cc_headers( _cc_out );
  }
  if( get_element_subtype() != 0 ){
    _get_element_subtype()->_publish_cc_headers( _cc_out );
  }
  
  TyvisTypeDefinition::_publish_cc_headers( _cc_out );
}

void
TyvisArrayTypeDefinition::_publish_cc_constructor_args( published_file &_cc_out,
							PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_constructor_args" );

  //ObjectBase::Objecttype is published in object declaration
  if (is_subtype() == TRUE) {
    if ((_get_base_type()->_get_declaration() == NULL) ||
	(is_anonymous() == TRUE)) {
      _cc_out << ", ";
      _publish_cc_range( _cc_out, declarations );
    }
  }
}

void
TyvisArrayTypeDefinition::_publish_cc_range_args( published_file &_cc_out ) {
  int dimensions = get_num_indexes();
  TyvisTypeDefinition *node = this;

  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_range_args" );
  int i;
  for( i = 1; i <= dimensions; i++) {
    _cc_out << ", int left" << i << ", ArrayInfo::ArrayDirn_t dirn" 
	    << i << ", int right" << i;
    node =  node->_get_element_subtype();
  }
}

void
TyvisArrayTypeDefinition::_publish_cc_constructor_ranges( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_constructor_ranges" );
  abort();
  _publish_cc_type_info_xxx( _cc_out, 0 );
  _cc_out << "(ObjectBase::ObjectType objType, const TypeInfo &tInfo";

  if(is_unconstrained_array_type() == TRUE) {
    _publish_cc_range_args( _cc_out );
  }
}

void
TyvisArrayTypeDefinition::_publish_cc_object_type( published_file &_cc_out,
                                                   IIR_Boolean typeNameFlag, 
                                                   const char *suffix) {
  if (typeNameFlag == TRUE) {
    _publish_cc_type_info_xxx( _cc_out, 0 );
  }

  if (suffix != NULL) {
    _cc_out << suffix;
  }
  
  _cc_out << "(ObjectBase::ObjectType objType, const TypeInfo &tInfo";
}

void
TyvisArrayTypeDefinition::_publish_cc_define_array_type_attributes( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_define_array_type_attributes" );
  // These are defined only for subtypes.
  _report_undefined_fn("_publish_cc_define_array_type_attributes( _cc_out )");
}

void
TyvisArrayTypeDefinition::_publish_cc_init_signal( published_file &_cc_out ) {

  ostringstream tmpname;
  ostringstream tmpanothername;
  ostringstream tmpsuffixname;
  
  const string tmpPtr1 = _get_current_publish_name();
  const string tmpPtr2 = _get_current_another_name();
  const string tmpPtr4 = _get_current_suffix_name();
  Tyvis* tmpPtr3 = _get_current_publish_node();

  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_init_signal" );
  
  _set_index_level( _get_index_level() + 1 );
  _cc_out << "{\n";//begin dummy block for scoping
  _cc_out << "int index" << _get_index_level() << " =0;\n";
  _cc_out << "for(index" << _get_index_level() << "=0;";
  _cc_out << " index";
  _cc_out << _get_index_level() << " < (";
  _cc_out << _get_current_publish_name();
  _cc_out << ").get_number_of_elements(); index" << _get_index_level() << "++) {\n";
  tmpname << _get_current_publish_name() << ".getElement(index" << _get_index_level()
	  << ")";
  tmpanothername << _get_current_another_name() << ".getElement(index"
		 <<_get_index_level()<< ")";

  if( _get_current_suffix_name() != "" ){
    tmpsuffixname << _get_current_suffix_name();
  }
  
  tmpsuffixname << ".getElement(index"
		<< _get_index_level() << ")";
  
  _set_current_publish_name( tmpname.str() );
  _set_current_another_name( tmpanothername.str() );
  _set_current_suffix_name( tmpsuffixname.str() );
  
  _get_element_subtype()->_publish_cc_init_signal( _cc_out );
  
  _cc_out << "}\n";
  _cc_out << "};\n";//end dummy block
  
  _set_current_publish_name( tmpPtr1 );
  _set_current_another_name( tmpPtr2 );
  _set_current_publish_node( tmpPtr3 );
  _set_current_suffix_name( tmpPtr4 );
}

void
TyvisArrayTypeDefinition::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {
  IIR_Int32 num_indexes = get_num_indexes();
  TyvisTypeDefinition *elem_subtype = this;

  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_range" );

  elem_subtype->_get_resolved_index_subtype()->_publish_cc_range( _cc_out, declarations );
  int i;
  for( i = 1; i < num_indexes; i++) {
    _cc_out << "," << NL();
    elem_subtype = elem_subtype->_get_element_subtype();
    ASSERT(elem_subtype != NULL);
    elem_subtype->_get_resolved_index_subtype()->_publish_cc_range( _cc_out, declarations );
  }
}

void 
TyvisArrayTypeDefinition::_publish_cc_array_info( published_file &_cc_out,
						  PublishData *declarations ){
  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_array_info" );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/ArrayInfo.hh" );
  if ((is_unconstrained_array_type() == TRUE) || (get_num_indexes() > 1)) {
    _cc_out << "nullInfo()";
  } 
  else {
    _cc_out << "ArrayInfo(";
    _publish_cc_range( _cc_out, declarations );
    _cc_out << ")";
  }
}

void
TyvisArrayTypeDefinition::_publish_cc_bounds( published_file &_cc_out,
					      PublishData *declarations ){
  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_bounds" );
  _publish_cc_array_info( _cc_out, declarations );
}

void
TyvisArrayTypeDefinition::_publish_cc_necessary_decl_in_state( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_necessary_decl_in_state" );

  _publish_cc_type_info_xxx( _cc_out, 0 );
  _cc_out << "_event ";
  _cc_out << _get_current_publish_name();
  _cc_out << "_event;\n";
  
  _publish_cc_type_info_xxx( _cc_out, 0 );
  _cc_out << "_lastevent  ";
  _cc_out << _get_current_publish_name();
  _cc_out << "_last_event;\n";
}

void 
TyvisArrayTypeDefinition::_publish_cc_type_info_prereq( published_file &_cc_out,
							PublishData *declarations,
							const string &type_name ){
  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_type_info_prereq" );
  if( _get_element_subtype()->is_anonymous()){ 
      //&& _get_element_subtype()->is_locally_static() ){
    CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_type_info_prereq(element)" );
    _get_element_subtype()->_publish_cc_static_type_info_fn( _cc_out,
							     declarations,
							     _get_element_type_name( type_name ));
  }
  if( _get_index_subtype()->is_anonymous() &&
      _get_index_subtype()->is_locally_static() ){
    CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_type_info_prereq(index)" );
    _get_index_subtype()->_publish_cc_static_type_info_fn( _cc_out,
							   declarations,
							   _get_index_type_name( type_name, 0 ));
  }
}

void 
TyvisArrayTypeDefinition::_publish_cc_type_info_args( published_file &_cc_out,
						      PublishData *declarations,
						      const string &type_name ){
  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_type_info_args(element)" );
  _get_element_subtype()->_publish_cc_type_info_xxx( _cc_out,
						     declarations,
						     _get_element_type_name( type_name ) );
  _cc_out << "," << NL();
  CC_REF( _cc_out, "TyvisArrayTypeDefinition::_publish_cc_type_info_args(index)" );
  _get_index_subtype()->_publish_cc_type_info_xxx( _cc_out,
						   declarations,
						   _get_index_type_name( type_name, 0 ) );
  _cc_out << NL();
}

void
TyvisArrayTypeDefinition::_publish_cc_include( published_file &_cc_out ){
  TyvisTypeDefinition::_publish_cc_include( _cc_out );
  _get_element_subtype()->_publish_cc_include( _cc_out );
  _get_index_subtype()->_publish_cc_include( _cc_out );
}

void 
TyvisArrayTypeDefinition::_add_decl_into_declaration_collection(PublishData *declarations) {
  if(_get_base_type() != NULL && _get_base_type()->is_anonymous() == FALSE) {
    _get_base_type()->_add_decl_into_declaration_collection(declarations);
  }
  if(_get_resolved_index_subtype() != NULL) {
    _get_resolved_index_subtype()->_add_decl_into_declaration_collection(declarations);
  }
  if(_get_element_subtype() != NULL) {
    _get_element_subtype()->_add_decl_into_declaration_collection(declarations);
  }
}

TyvisScalarTypeDefinition *
TyvisArrayTypeDefinition::_get_index_subtype(){
  return dynamic_cast<TyvisScalarTypeDefinition *>(get_index_subtype());
}

IIR_Boolean 
TyvisArrayTypeDefinition::_is_single_dimensional_array_type() {
  if(get_num_indexes() == 1) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

IIR_Boolean
TyvisArrayTypeDefinition::_is_element_unconstrained_subtype() {
  TyvisTypeDefinition* element_type = _get_element_type();
  if(element_type->is_array_type() == FALSE) {
    return FALSE;
  }
  else {
    if((element_type->is_anonymous() == TRUE) &&
       (element_type->_get_base_type()->is_unconstrained_array_type() == TRUE)) {
      return TRUE;
    }
  }
  return FALSE;
}

TyvisTypeDefinition*
TyvisArrayTypeDefinition::_get_element_type() {
  TyvisTypeDefinition *current_subtype = _get_element_subtype();
  ASSERT(current_subtype != NULL);
  while((current_subtype->is_element() == FALSE ) &&
	(current_subtype->is_array_type()== TRUE)) {
    current_subtype = current_subtype->_get_element_subtype();
    ASSERT(current_subtype != NULL);
  }
  return current_subtype;
}

IIR_Boolean 
TyvisArrayTypeDefinition::_has_access_type() {
  return _get_element_type()->_has_access_type();
}

TyvisTypeDefinition *
TyvisArrayTypeDefinition::_get_element_subtype(){
  return dynamic_cast<TyvisTypeDefinition *>(get_element_subtype());
}

IIR_Int32
TyvisArrayTypeDefinition::get_num_indexes() {
  IIR_Int32 num_indexes = 1;

  TyvisTypeDefinition *current_subtype = _get_element_subtype();
  ASSERT(current_subtype != NULL);
  while( current_subtype->is_array_type() == TRUE
	 && current_subtype->is_element() == FALSE ){
    num_indexes++;
    current_subtype = current_subtype->_get_element_subtype();
    ASSERT(current_subtype != NULL);
  }

  return num_indexes;
}

TyvisTypeDefinition*
TyvisArrayTypeDefinition::_get_final_subtype() {

  TyvisTypeDefinition* node = _get_element_subtype();
  while ( (node->get_kind() == IIR_ARRAY_TYPE_DEFINITION ||
	   node->get_kind() == IIR_ARRAY_SUBTYPE_DEFINITION) &&
	  node->is_element() == false ){
    node =  node->_get_element_subtype();
  }
  return node;
}

string
TyvisArrayTypeDefinition::_get_element_type_name( const string &type_name ){
  string retval = "";
  if( !_get_element_subtype()->is_anonymous() ){
    retval = _get_element_subtype()->_get_cc_type_name();
  }
  else{
    retval = type_name + "Element";
  }
  return retval;
}

string
TyvisArrayTypeDefinition::_get_index_type_name( const string &type_name, int index_num ){
  return type_name + "Index" + intToString(index_num);
}

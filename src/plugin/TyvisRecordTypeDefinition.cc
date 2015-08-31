
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

#include "TyvisAccessTypeDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisElementDeclaration.hh"
#include "TyvisElementDeclarationList.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisName.hh"
#include "TyvisRecordTypeDefinition.hh"
#include "TyvisTextLiteral.hh"

#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"

TyvisRecordTypeDefinition::TyvisRecordTypeDefinition(){}

TyvisRecordTypeDefinition::~TyvisRecordTypeDefinition(){}

void 
TyvisRecordTypeDefinition::_publish_cc_decl_type_attributes( published_file & ){}

void
TyvisRecordTypeDefinition::_publish_cc_necessary_decl_in_state( published_file &_cc_out ) {
  // The value of _current_publish_name should be set to the variable's
  // name that is being published. The control drops here usually from
  // TyvisEventAttribute
  
  _publish_cc_type_info_xxx( _cc_out, 0 );
  _cc_out << "_event ";
  _cc_out << _get_current_publish_name();
  _cc_out << "_event;" << NL();

  _publish_cc_type_info_xxx( _cc_out, 0 );
  _cc_out << "_last_event ";
  _cc_out << _get_current_publish_name();
  _cc_out << "_last_event;" << NL();
}

const string
TyvisRecordTypeDefinition::_get_cc_tyvis_type(){
  return "RecordTypeInfo";
}

void 
TyvisRecordTypeDefinition::_publish_cc_element_objtypes( published_file &_cc_out, PublishData *declarations ){
  TyvisElementDeclaration* fields = NULL;
  fields = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->first());
  while(fields != NULL) {
    fields->_get_mangled_declarator()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "(objType";
    if (fields->_get_subtype()->_is_kernel_type() == FALSE &&
	fields->_get_subtype()->is_anonymous() == FALSE &&
	fields->_get_subtype()->is_scalar_type() == TRUE){
      fields->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations );
    }
    if (fields->_get_subtype()->is_array_type() == TRUE)  {
      // Argh!...but, only for now...
      _cc_out << ", TypeInfo::getNullTypeInfo()";
      
      if (fields->_get_subtype()->is_anonymous() == TRUE) {
	_cc_out << ", ";
	fields->_get_subtype()->_publish_cc_range( _cc_out, declarations );
      }
    }
    
    _cc_out << ")";
    
    fields = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->successor(fields));
    if(fields != NULL) {
      _cc_out << ",\n";
    }
  }
  _cc_out <<" {\n";
  _cc_out << " numberOfFields = ";
  _cc_out << get_element_declarations()->size() << ";\n";
}

void 
TyvisRecordTypeDefinition::_publish_cc_element_objtypes_event( published_file &_cc_out, PublishData *declarations ){
  TyvisElementDeclaration* fields;
  fields = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->first());
  while(fields != NULL) {
    fields->_get_mangled_declarator()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "(object";
    
    if (fields->_get_subtype()->is_array_type() == TRUE) {
      // Argh!!, but only for now...
      _cc_out << ", ";
      fields->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations, FALSE );
      _cc_out << ", -1";
      
      if (fields->_get_subtype()->is_anonymous() == TRUE) {
	_cc_out << ", ";
	fields->_get_subtype()->_publish_cc_range( _cc_out, declarations );
      }
    }
    
    _cc_out << ")";
    
    fields = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->successor(fields));
    if(fields != NULL) {
      _cc_out << ",\n";
    }
  }
}

int
TyvisRecordTypeDefinition::_get_field_number(Tyvis *searchField) {
  int retval = -1;
  // Obtain the declarator for the field. 

  TyvisTextLiteral* decl = searchField->_get_mangled_declarator();
  ASSERT ( decl != NULL );
  
  int fieldNumber = 0;
  TyvisElementDeclaration *field = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->first());  
  while (field != NULL) {
    if (IIRBase_TextLiteral::cmp(decl, field->_get_mangled_declarator()) == 0) {
      retval = fieldNumber;
      break;
    }
    fieldNumber++;
    field = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->successor(field));
  }

  return retval;
}

IIR_Boolean
TyvisRecordTypeDefinition::_has_access_type() {
  TyvisElementDeclaration* fields = NULL;
  fields = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->first());
  while(fields != NULL) {
    if(fields->_get_subtype()->_has_access_type() == TRUE) {
      return TRUE;;
    }
    fields = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->successor(fields));
  }
  return FALSE;
}

void
TyvisRecordTypeDefinition::_publish_cc_headers( published_file &_cc_out ) {
  TyvisElementDeclaration* fields = NULL;
  fields = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->first());
  while(fields != NULL) {
    fields->_publish_cc_include( _cc_out );
    fields = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->successor(fields));
  }
  //_publish_cc_include( _cc_out, "tyvis/RecordType.hh" );
}

void
TyvisRecordTypeDefinition::_publish_cc_universal_type( published_file &_cc_out ){
  _publish_cc_include( _cc_out, "tyvis/Record.hh" );
  _cc_out << "Record";
}

TyvisScalarTypeDefinition*
TyvisRecordTypeDefinition::_get_index_subtype(){
  return NULL;
}

TyvisTypeDefinition *
TyvisRecordTypeDefinition::_get_new_subtype(){
  TyvisTypeDefinition *retval = new TyvisRecordTypeDefinition();
  copy_location( this, retval );
  return retval;
}

IIR_Boolean 
TyvisRecordTypeDefinition::_is_globally_static(){
  IIR_Boolean retval = TRUE;

  TyvisElementDeclaration *current_decl = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->first());
  while( current_decl != NULL ){
    ASSERT( current_decl->_get_subtype() != NULL );
    if( current_decl->_get_subtype()->_is_globally_static() == FALSE ){
      retval = FALSE;
      break;
    }
    current_decl = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->successor( current_decl ));
  }

  return retval;
}

TyvisElementDeclarationList *
TyvisRecordTypeDefinition::_get_element_declarations() {
  return dynamic_cast<TyvisElementDeclarationList *>(get_element_declarations()); 
}

TyvisFunctionDeclaration* 
TyvisRecordTypeDefinition::_get_resolution_function(){ 
  return dynamic_cast<TyvisFunctionDeclaration*>(get_resolution_function());
}

TyvisTypeDefinition *
TyvisRecordTypeDefinition::_get_element_subtype(int i) {
  return dynamic_cast<TyvisTypeDefinition *>(get_element_subtype(i));
}

string
TyvisRecordTypeDefinition::_get_cc_elements_method( const string &type_name ){
  return type_name + "Elements";
}

void
TyvisRecordTypeDefinition::_publish_cc_type_info_prereq( published_file &_cc_out, PublishData *declarations, const string &type_name ){
  _publish_cc_include( _cc_out, "tyvis/ElementInfo.hh" );

  TyvisElementDeclaration *current = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->first());
  while( current != 0 ){
    if( current->get_subtype()->is_anonymous() ){
      current->_publish_cc_static_type_info_fn( _cc_out, declarations );
    }
    current = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->successor( current ));
  }

  _cc_out << "inline const vector<ElementInfo *> &" << NL()
	  << _get_cc_elements_method( type_name ) << OS("(){")
	  << "static vector<ElementInfo *> _elements;" << NL()
	  << "if( _elements.empty() )" << OS("{");
  current = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->first());
  while( current != 0 ){
    _cc_out << OS("_elements.push_back(")
	    << "new ElementInfo" << OS("(");
    current->_get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "," << NL()
	    << "\"" << *current->_get_declarator() << "\""
	    << CS(")") << CS(");");
    current = dynamic_cast<TyvisElementDeclaration *>(get_element_declarations()->successor( current ));
  }
  _cc_out << CS("}")
	  << "return _elements;"
	  << CS("}");
}

void
TyvisRecordTypeDefinition::_publish_cc_type_info_args( published_file &_cc_out, PublishData *, const string &type_name ){
  CC_REF( _cc_out, "TyvisRecordTypeDefinition::_publish_cc_type_info_args" );
  _cc_out << _get_cc_elements_method( type_name ) << "()";
}

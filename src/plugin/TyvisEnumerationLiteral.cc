
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

#include "TyvisAttributeSpecificationList.hh"
#include "TyvisCharacterLiteral.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisEnumerationLiteralList.hh"
#include "TyvisEnumerationTypeDefinition.hh"
#include "TyvisIdentifier.hh"
#include "TyvisStringLiteral.hh"

#include "savant/set.hh"
#include "published_file.hh"

TyvisEnumerationLiteral::TyvisEnumerationLiteral() {
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisEnumerationLiteral::~TyvisEnumerationLiteral() {
  //Release the list memory
  delete get_attributes();
}

void
TyvisEnumerationLiteral::_publish_cc_rvalue( published_file &_cc_out,
					     PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisEnumerationLiteral::_publish_cc_rvalue" );
  _publish_cc_include( _cc_out, "tyvis/EnumerationLiteral.hh" );
  _cc_out << OS("EnumerationLiteral(");
  if( _get_subtype() != 0 ){
    _get_subtype()->_publish_cc_type_info_xxx(_cc_out, declarations);
    _cc_out << ", " << NL();
  }
  _publish_cc_universal_value( _cc_out, declarations );
  _cc_out << CS(")");
}

void 
TyvisEnumerationLiteral::_publish_cc_universal_value( published_file &_cc_out,
						      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisEnumerationLiteral::_publish_cc_universal_value" );
  _publish_cc_value( _cc_out, declarations );
}

void 
TyvisEnumerationLiteral::_publish_cc_value( published_file &_cc_out,
					    PublishData * ) {
  CC_REF( _cc_out, "TyvisEnumerationLiteral::_publish_cc_value" );
  ASSERT(_get_subtype() != NULL);
  TyvisEnumerationTypeDefinition *enum_subtype 
    = dynamic_cast<TyvisEnumerationTypeDefinition *>(_get_subtype());
  ASSERT( enum_subtype != 0 );
  _cc_out << enum_subtype->_get_enumeration_literals()->_get_ordinal_position( dynamic_cast<TyvisEnumerationLiteral *>(this) );
}

void 
TyvisEnumerationLiteral::_publish_cc_initialization_value( published_file &_cc_out,
							   PublishData *declarations ) {
  _publish_cc_rvalue( _cc_out, declarations );
}

void 
TyvisEnumerationLiteral::_publish_cc_state_object_init( published_file &_cc_out,
							PublishData *declarations ) {
  _cc_out << "new ";
  _publish_cc_rvalue( _cc_out , declarations );
  _cc_out << ",";
}

void
TyvisEnumerationLiteral::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisEnumerationLiteral::_publish_cc_condition( published_file &_cc_out,
						PublishData *declarations ) {
  _publish_cc_rvalue( _cc_out, declarations );
}

void 
TyvisEnumerationLiteral::_publish_cc_wait_data( published_file &_cc_out,
						PublishData *declarations ) {
  _publish_cc_rvalue( _cc_out , declarations );
}

string
TyvisEnumerationLiteral::_get_cc_direction( ){
  string retval;

  if(TyvisTextLiteral::cmp(_get_declarator(), "true") == 0) {
    retval = "ArrayInfo::to";
  } else if(TyvisTextLiteral::cmp(_get_declarator(), "false") == 0) {
    retval = "ArrayInfo::downto";
  } else {
    cerr << "Invalid direction: Neither`to' nor `downto' - literal was " 
	 << _get_declarator()  << endl;
    abort();
  }

  return retval;
}

void 
TyvisEnumerationLiteral::_publish_cc_direction( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisEnumerationLiteral::_publish_cc_direction" );
  _cc_out << _get_cc_direction();
}

void 
TyvisEnumerationLiteral::_publish_cc_headers( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisEnumerationLiteral::_publish_cc_headers" );
  
  ASSERT(_get_subtype() != NULL);
  //  get_subtype()->_publish_cc_headers( _cc_out );
}

void
TyvisEnumerationLiteral::_publish_cc_ams_function( published_file &_cc_out, PublishData *declarations ) {
  if( get_is_currently_publishing_simultaneous_stmt() == TRUE ) {
    _publish_cc_rvalue( _cc_out , declarations );
   }
}

void
TyvisEnumerationLiteral::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisEnumerationLiteral::_publish_cc_bounds" );
  
  _get_subtype()->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisEnumerationLiteral::_publish_cc_data( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_rvalue( _cc_out, declarations );
}

TyvisAttributeSpecificationList *
TyvisEnumerationLiteral::_get_attributes() {
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

TyvisAttributeSpecificationList* 
TyvisEnumerationLiteral::_get_attribute_specification_list() { 
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes()); 
}

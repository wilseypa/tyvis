
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
#include "TyvisIdentifier.hh"
#include "TyvisPhysicalTypeDefinition.hh"
#include "TyvisPhysicalUnit.hh"
#include "TyvisUnitList.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"

TyvisPhysicalUnit::TyvisPhysicalUnit(){
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisPhysicalUnit::~TyvisPhysicalUnit(){
  //Release the list memory
  delete get_attributes();
}

void 
TyvisPhysicalUnit::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisPhysicalUnit::_publish_cc_rvalue" );
  //  _cc_out << *_get_declarator();
  _publish_cc_value( _cc_out, declarations );
}

void 
TyvisPhysicalUnit::_publish_cc_state_object_init( published_file &_cc_out,
						  PublishData *declarations ) {
  _cc_out << "new ";
  _publish_cc_lvalue( _cc_out , declarations );
  _cc_out << ",";
}

void
TyvisPhysicalUnit::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisPhysicalUnit::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {
  _get_multiplier()->_publish_cc_rvalue( _cc_out, declarations );
}

void 
TyvisPhysicalUnit::_publish_cc_universal_value( published_file &_cc_out,
						PublishData *declarations ) {
  _publish_cc_value( _cc_out, declarations );
}

void 
TyvisPhysicalUnit::_publish_cc_unit_declaration( published_file &_cc_out,
						 PublishData *declarations ) {
  _cc_out << OS("UnitDeclaration(")
	  << _get_cc_unit_name() << "," << NL();
  _get_multiplier()->_publish_cc_rvalue( _cc_out, declarations );
  _cc_out << CS(")");
}

string
TyvisPhysicalUnit::_get_cc_unit_name(){
  return string("\"" + _get_declarator()->convert_to_string() + "\"" );
}

void 
TyvisPhysicalUnit::_publish_cc_headers( published_file &_cc_out ) {
  _get_physical_type()->_publish_cc_include( _cc_out );
}

void 
TyvisPhysicalUnit::_publish_cc_initialization_value( published_file &_cc_out,
						     PublishData *declarations ) {
  _cc_out << "1 * ";
  _publish_cc_value( _cc_out, declarations );
}

void
TyvisPhysicalUnit::_publish_cc_object_name( published_file &_cc_out, PublishData * ) {
  _cc_out << *_get_mangled_declarator();
}

TyvisAttributeSpecificationList *
TyvisPhysicalUnit::_get_attributes() {
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

Tyvis *
TyvisPhysicalUnit::_get_multiplier() {
  return dynamic_cast<Tyvis *>(get_multiplier());
}

TyvisPhysicalTypeDefinition *
TyvisPhysicalUnit::_get_physical_type() {
  return dynamic_cast<TyvisPhysicalTypeDefinition *>(get_subtype());
}

TyvisAttributeSpecificationList *
TyvisPhysicalUnit::_get_attribute_specification_list(){ 
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes()); 
}


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
#include "TyvisFloatingSubtypeDefinition.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisPhysicalLiteral.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "TyvisPhysicalUnit.hh"
#include "TyvisTypeDeclaration.hh"
#include "TyvisUnitList.hh"
#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>
#include "savant/set.hh"
#include "savant/StandardPackage.hh"

TyvisPhysicalTypeDefinition::TyvisPhysicalTypeDefinition(){}

TyvisPhysicalTypeDefinition::~TyvisPhysicalTypeDefinition(){}

void 
TyvisPhysicalTypeDefinition::_publish_cc_elaborate( published_file &_cc_out, PublishData * ){
  _cc_out << _get_cc_tyvis_type();
}

void
TyvisPhysicalTypeDefinition::_publish_cc_universal_type( published_file &_cc_out ) {
  _cc_out << "PhysicalLiteral";
}

const string  
TyvisPhysicalTypeDefinition::_get_cc_tyvis_type(){
  return "PhysicalTypeInfo";
}

string
TyvisPhysicalTypeDefinition::_get_cc_units_method(const string &type_name){
  return type_name + "Units";
}


void
TyvisPhysicalTypeDefinition::_publish_cc_type_info_prereq( published_file &_cc_out,
							   PublishData *declarations,
							   const string &type_name ){
  CC_REF( _cc_out, "TyvisPhysicalTypeDefinition::_publish_cc_type_info_prereq" );
  _publish_cc_include( _cc_out, "tyvis/UnitDeclaration.hh" );

  if( _need_to_publish_range() ){
    _publish_cc_range_method( _cc_out, type_name );
  }

  _cc_out << "inline static vector<UnitDeclaration> &" << NL()
	  << _get_cc_units_method( type_name ) << OS("(){")
	  << "static vector<UnitDeclaration> _units;" << NL()
	  << "static bool initialized = false;" << NL()
	  << "if ( !initialized )" << OS("{")
	  << "initialized = true;" << NL();
  if( _get_primary_unit() != 0 ){
    _cc_out << OS("_units.push_back(");
    _get_primary_unit()->_publish_cc_unit_declaration( _cc_out, declarations );
    _cc_out << CS(");");
  }
  TyvisUnitList *list = dynamic_cast<TyvisUnitList *>(get_units());
  TyvisPhysicalUnit *current = dynamic_cast<TyvisPhysicalUnit *>(list->first());
  while( current != 0 ){
    _cc_out << OS("_units.push_back(");
    current->_publish_cc_unit_declaration( _cc_out, declarations );
    current = current = dynamic_cast<TyvisPhysicalUnit *>(list->successor( current ));
    _cc_out << CS(");");
  }
  _cc_out << CS("}")
	  << "return _units;" << NL()
	  << CS("}");
}

void
TyvisPhysicalTypeDefinition::_publish_cc_type_info_args( published_file &_cc_out,
							 PublishData *,
							 const string &type_name ){
  CC_REF( _cc_out, "TyvisPhysicalTypeDefinition::_publish_cc_type_info_args" );

  if( _need_to_publish_range() ){
  // DRH - avoid recursion problems _cc_out << type_name << "Range()," << NL(); 
    _cc_out << type_name << "Range()); //DRH" << NL(); 
  }
  // DRH
  _cc_out << "_ti.addUnits(" ;
  // DRH
  _cc_out << _get_cc_units_method( type_name ) << "()";

 
}

TyvisUnitList *
TyvisPhysicalTypeDefinition::_get_units() {
  return dynamic_cast<TyvisUnitList *>(get_units());
}

TyvisPhysicalUnit *
TyvisPhysicalTypeDefinition::_get_primary_unit() {
  return dynamic_cast<TyvisPhysicalUnit *>(get_primary_unit());
}

bool
TyvisPhysicalTypeDefinition::_is_time() {
  return (IIRBase_Identifier::cmp(_get_declaration()->_get_declarator(), "time") == 0);
}

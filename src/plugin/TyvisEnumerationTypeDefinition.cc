
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
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisEnumerationLiteralList.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisIntegerLiteral32.hh"
#include "TyvisStringLiteral.hh"

#include "savant/StandardPackage.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "published_file.hh"
#include <sstream>
using std::ostringstream;

TyvisEnumerationTypeDefinition::TyvisEnumerationTypeDefinition(){}

TyvisEnumerationTypeDefinition::~TyvisEnumerationTypeDefinition(){}

void
TyvisEnumerationTypeDefinition::_publish_cc_temporary_type_info( published_file &_cc_out,
                                                                 PublishData *declarations,
                                                                 char*, 
                                                                 char*) {
  _publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisEnumerationTypeDefinition::_publish_cc_literals_method( published_file &_cc_out,
							     const string &type_name ){
  _cc_out << "inline static vector<string> &" << NL()
	  << type_name << "Literals()" << OS("{")
	  << "static vector<string> _literals;" << NL()
	  << "static bool initialized = false;" << NL();
  TyvisEnumerationLiteral *current = 
    dynamic_cast<TyvisEnumerationLiteral *>(get_enumeration_literals()->first());
  _cc_out << "if( !initialized )" << OS("{")
	  << "initialized = true;" << NL();
  while( current != 0 ){
    _cc_out << "_literals.push_back(\"";
    current->_get_declarator()->_print_cc( _cc_out.get_stream() );
    _cc_out << "\");" << NL();
    current =
      dynamic_cast<TyvisEnumerationLiteral *>(get_enumeration_literals()->successor( current ));
  }
  _cc_out << CS("}")
	  << "return _literals;"
	  << CS("}");
}

void
TyvisEnumerationTypeDefinition::_publish_cc_type_info_prereq( published_file &_cc_out, 
							      PublishData *,
							      const string &type_name ){
  CC_REF( _cc_out, "TyvisEnumerationTypeDefinition::_publish_cc_type_info_prereq" );
  _publish_cc_literals_method( _cc_out, type_name );
  if( _need_to_publish_range() ){
    _publish_cc_range_method( _cc_out, type_name );
  }
}

void 
TyvisEnumerationTypeDefinition::_publish_cc_type_info_args( published_file &_cc_out,
							    PublishData *,
							    const string &type_name ){
  CC_REF( _cc_out, "TyvisEnumerationTypeDefinition::_publish_cc_type_info_args" );
  if( _need_to_publish_range() ){
    _cc_out << type_name << "Range()," << NL();
  }
  _cc_out << type_name << "Literals()";
}

void 
TyvisEnumerationTypeDefinition::_publish_cc_universal_type( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisEnumerationTypeDefinition::_publish_cc_universal_type" );
  _cc_out << "EnumerationLiteral";
}

const string
TyvisEnumerationTypeDefinition::_get_cc_tyvis_type(){
  //  CC_REF( _cc_out, "TyvisEnumerationTypeDefinition::_publish_cc_tyvis_type" );
  return string("EnumerationTypeInfo");
}

void 
TyvisEnumerationTypeDefinition::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(_get_base_type_left() != NULL);
  ASSERT(_get_direction() != NULL);
  ASSERT(_get_base_type_right() != NULL);

  CC_REF( _cc_out, "TyvisEnumerationTypeDefinition::_publish_cc_range" );

  _get_base_type_left()->_publish_cc_universal_value( _cc_out, declarations );
  _cc_out << ", ";
  _get_direction()->_publish_cc_direction( _cc_out );
  _cc_out << ", ";
  _get_base_type_right()->_publish_cc_universal_value( _cc_out, declarations );
}

TyvisEnumerationLiteralList *
TyvisEnumerationTypeDefinition::_get_enumeration_literals() {
  return dynamic_cast<TyvisEnumerationLiteralList *>(get_enumeration_literals());
}

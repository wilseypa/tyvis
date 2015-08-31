
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
#include "TyvisIntegerLiteral.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisDeclaration.hh"
#include "savant/StandardPackage.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>

void 
TyvisIntegerLiteral::_publish_cc_data( published_file &_cc_out, PublishData * ) {
  _cc_out <<  print_value(0);
}

void 
TyvisIntegerLiteral::_publish_cc_condition( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_universal_value( _cc_out, declarations );
}

void 
TyvisIntegerLiteral::_publish_cc_state_object_init( published_file &_cc_out,
						    PublishData *declarations ) {
  _cc_out << "new ";
  _publish_cc_rvalue( _cc_out , declarations );
}

void
TyvisIntegerLiteral::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisIntegerLiteral::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisIntegerLiteral::_publish_cc_rvalue" );	
  _publish_cc_universal_value( _cc_out, declarations );
}

void 
TyvisIntegerLiteral::_publish_cc_value( published_file &_cc_out, PublishData * ) {
  _cc_out << print_value(0);
}

void 
TyvisIntegerLiteral::_publish_cc_universal_value( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisIntegerLiteral::_publish_cc_universal_value" );
  _cc_out << "UniversalInteger(";
  _publish_cc_value( _cc_out, declarations );
  _cc_out << ")";
}

void 
TyvisIntegerLiteral::_publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_universal_value( _cc_out, declarations );
}

void 
TyvisIntegerLiteral::_publish_cc_elaborate( published_file &_cc_out, PublishData * ){
  _cc_out << print_value(0);
}

void
TyvisIntegerLiteral::_publish_cc_bounds( published_file &_cc_out, PublishData * ) {
  _cc_out << "defaultInfo()";
}

void
TyvisIntegerLiteral::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ){
  _publish_cc_universal_value( _cc_out, declarations );
}

void
TyvisIntegerLiteral::_publish_cc_headers( published_file &_cc_out ) {
  _publish_cc_include( _cc_out, "tyvis/IntegerTypeInfo.hh" );
}

void
TyvisIntegerLiteral::_publish_cc_type_info_xxx( published_file &_cc_out,
						PublishData *declarations ){
  CC_REF( _cc_out, "TyvisIntegerLiteral::_publish_cc_type_info_xxx" );	
  if( _get_subtype() != 0 ){
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  }
  else{
    // Then we'll fall back to integer since that's our Universal Integer
    // type.
    _cc_out << "std_standard_integer";
  }
}

void 
TyvisIntegerLiteral::_publish_cc_ams_function(published_file &_cc_out, PublishData *declarations) {
  _publish_cc_data(_cc_out, declarations);
}


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

#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDeclaration.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisRangeTypeDefinition.hh"
#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>
#include "savant/set.hh"

void 
TyvisRangeTypeDefinition::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisRangeTypeDefinition::_publish_cc_range" );
  ASSERT(get_left() != NULL && get_right() != NULL);
  _get_base_type_left()->_publish_cc_universal_value( _cc_out, declarations );
  _cc_out << "," << NL();
  ASSERT(get_direction()->get_kind() == IIR_ENUMERATION_LITERAL);
  if( is_ascending_range() == TRUE ){
    _cc_out << "ArrayInfo::to," << NL();
  }
  else {
    _cc_out << "ArrayInfo::downto," << NL();
  }
  _get_base_type_right()->_publish_cc_universal_value( _cc_out, declarations );
}

void 
TyvisRangeTypeDefinition::_publish_cc_array_info( published_file &_cc_out, PublishData *declarations ){
  ASSERT(get_left() != NULL && get_right() != NULL);
  _cc_out << "ArrayInfo" << OS("(");
  _publish_cc_range( _cc_out, declarations );
  _cc_out << CS(")");
}

void 
TyvisRangeTypeDefinition::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_array_info( _cc_out, declarations );
}

void 
TyvisRangeTypeDefinition::_publish_cc_universal_type( published_file &_cc_out ) {
  _cc_out << "UniversalInteger";
}

const string
TyvisRangeTypeDefinition::_get_cc_tyvis_type(){
  return "IntegerType";
}

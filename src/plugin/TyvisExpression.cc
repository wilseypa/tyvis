
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

#include "TyvisExpression.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "published_file.hh"

void 
TyvisExpression::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisExpression::_publish_cc_bounds" );
  ASSERT( _get_subtype() != 0 );
  _get_subtype()->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisExpression::_publish_cc_state_object_init( published_file &_cc_out,
						PublishData *declarations ){
  CC_REF( _cc_out, "TyvisExpression::_publish_cc_state_object_init" );  
  _cc_out << "new ";
  _publish_cc_rvalue( _cc_out , declarations );
  _cc_out << ",";
}

void
TyvisExpression::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisExpression::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisExpression::_publish_cc_elaborate" );
  
  _publish_cc_rvalue( _cc_out , declarations );
}

void 
TyvisExpression::_publish_cc_universal_value( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisExpression::_publish_cc_universal_value" );  
  _publish_cc_rvalue( _cc_out, declarations );
  //DRH_cc_out << ".readVal()";
}

void 
TyvisExpression::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisExpression::_publish_cc_value" );
  _publish_cc_universal_value( _cc_out, declarations );
}

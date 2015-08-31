
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

#include "TyvisFloatingPointLiteral64.hh"
#include "TyvisFloatingSubtypeDefinition.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void 
TyvisFloatingPointLiteral64::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  _publish_cc_universal_value( _cc_out, declarations );
}

void
TyvisFloatingPointLiteral64::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_universal_value( _cc_out, declarations );
}

void
TyvisFloatingPointLiteral64::_publish_cc_universal_value( published_file &_cc_out, PublishData * ) {
  _cc_out << "UniversalReal(" << value() << ")";
}

void 
TyvisFloatingPointLiteral64::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_data( _cc_out, declarations );
}

void 
TyvisFloatingPointLiteral64::_publish_cc_data( published_file &_cc_out, PublishData * ) {
  _cc_out << value();
}

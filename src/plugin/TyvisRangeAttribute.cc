
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

#include "TyvisAscendingAttribute.hh"
#include "TyvisDesignFile.hh"
#include "TyvisLeftAttribute.hh"
#include "TyvisIdentifier.hh"
#include "TyvisRangeAttribute.hh"
#include "TyvisRightAttribute.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDeclaration.hh"

#include "savant/StandardPackage.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void 
TyvisRangeAttribute::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(get_prefix() != NULL);

  TyvisLeftAttribute *left_attr = new TyvisLeftAttribute();
  copy_location( this, left_attr );
  left_attr->set_prefix(_get_prefix());
  left_attr->set_suffix(_get_suffix());
  
  TyvisAscendingAttribute *ascending_attr = new TyvisAscendingAttribute();
  copy_location( this, ascending_attr );
  ascending_attr->set_prefix(_get_prefix());
  ascending_attr->set_suffix(_get_suffix());

  TyvisRightAttribute *right_attr = new TyvisRightAttribute();
  copy_location( this, right_attr );
  right_attr->set_prefix(_get_prefix());
  right_attr->set_suffix(_get_suffix());

  left_attr->_publish_cc_value( _cc_out, declarations );
  _cc_out << ", ((";
  ascending_attr->_publish_cc_rvalue( _cc_out , declarations );
  _cc_out << " == SAVANT_BOOLEAN_TRUE)? ArrayInfo::to : ArrayInfo::downto), ";
  right_attr->_publish_cc_value( _cc_out, declarations );

  delete left_attr;
  delete ascending_attr;
  delete right_attr;
  ASSERT(get_prefix() != NULL);
}

const string &
TyvisRangeAttribute::_get_cc_attribute_name() const {
  static const string retval("range");
  return retval;
}

TyvisDeclaration *
TyvisRangeAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
                                                       TyvisTypeDefinition * ){
  // Similar code is in TyvisReverseRangeAttribute 
  TyvisTypeDefinition *prefix_rval = NULL;
  ASSERT(0);
//   TyvisTypeDefinition *prefix_rval = _get_prefix_rval();
  return _build_type_declaration( decl_name, prefix_rval );
}

IIR_Boolean
TyvisRangeAttribute::_is_type_attribute() const {
  TyvisRangeAttribute *mutableThis = const_cast<TyvisRangeAttribute *>(this);
  return mutableThis->_get_prefix() && 
    mutableThis->_get_prefix()->is_type();
}

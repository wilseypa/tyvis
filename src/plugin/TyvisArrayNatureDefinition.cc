
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

#include "savant/IIRBase_TypeDefinition.hh"
#include "Tyvis.hh"
#include "TyvisArrayNatureDefinition.hh"
#include "TyvisArraySubnatureDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisNatureDeclaration.hh"
#include "TyvisRangeTypeDefinition.hh"
#include "TyvisScalarNatureDefinition.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDefinition.hh"
#include "published_file.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include "savant/set.hh"
#include "savant/StandardPackage.hh"

void TyvisArrayNatureDefinition::_publish_cc_lvalue(){}

void TyvisArrayNatureDefinition::_publish_cc_ams(){}

void
TyvisArrayNatureDefinition::_publish_cc_lvalue( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisArrayNatureDefinition::_publish_cc_lvalue");
  _cc_out.get_stream() << "Savant"
		      << *_get_mangled_declarator()
		      << "Type";
}

TyvisScalarTypeDefinition *
TyvisArrayNatureDefinition::_get_index_subtype(){
  return dynamic_cast<TyvisScalarTypeDefinition *>(get_index_subtype());
}
 

TyvisTypeDefinition *
TyvisArrayNatureDefinition::_get_element_subtype(){
  return dynamic_cast<TyvisTypeDefinition *>(get_element_subtype());
}

TyvisTypeDefinition*
TyvisArrayNatureDefinition::_get_final_subtype() {

  TyvisTypeDefinition* node = _get_element_subtype();
  while ( (node->get_kind() == IIR_ARRAY_NATURE_DEFINITION ||
           node->get_kind() == IIR_ARRAY_SUBNATURE_DEFINITION) &&
          node->is_element() == false ){
    node =  node->_get_element_subtype();
  }
  return node;
}

IIR_Int32
TyvisArrayNatureDefinition::get_num_indexes() {
  IIR_Int32 num_indexes = 1;

  TyvisTypeDefinition *current_subtype = _get_element_subtype();
  ASSERT(current_subtype != NULL);
  while( current_subtype->is_array_type() == TRUE
         && current_subtype->is_element() == FALSE ){
    num_indexes++;
    current_subtype = current_subtype->_get_element_subtype();
    ASSERT(current_subtype != NULL);
  }

  return num_indexes;
}

#ifndef TYVIS_ATTRIBUTESPECIFICATION_HH
#define TYVIS_ATTRIBUTESPECIFICATION_HH

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

#include "TyvisDeclaration.hh"
#include "savant/IIRBase_AttributeSpecification.hh"

class TyvisAttributeSpecification : public virtual TyvisDeclaration, public virtual IIRBase_AttributeSpecification {
public:  
  TyvisAttributeSpecification();
  ~TyvisAttributeSpecification();

  void _publish_cc_package_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_package_definition( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_decl( published_file &(_cc_out), PublishData *declarations );
  void _publish_cc_decl_with_constructor( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );

  /**
     Returns the value of the specification as a string.
  */
  const string _get_cc_value_string();

  void _publish_cc_include( published_file & ){}

  IIR_Boolean                   _is_foreign_attribute_specification( );

  // Helper Function
  TyvisDeclaration       *_get_declaration();
  Tyvis                   *_get_value();
  TyvisDesignatorList    *_get_entity_name_list();
protected:

private:
  void _publish_cc_decl_constructor_args( published_file &_cc_out, PublishData *declarations );

};
#endif

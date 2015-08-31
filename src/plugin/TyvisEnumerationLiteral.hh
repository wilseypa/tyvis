#ifndef TYVIS_ENUMERATIONLITERAL_HH
#define TYVIS_ENUMERATIONLITERAL_HH

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
#include "savant/IIRBase_EnumerationLiteral.hh"

class TyvisEnumerationLiteral : public virtual TyvisDeclaration,
				public virtual IIRBase_EnumerationLiteral {
public:
  TyvisEnumerationLiteral();
  ~TyvisEnumerationLiteral();

  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_universal_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_value( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations );

  //While publishing the if and case statements, the enumeration literals
  //must be converted to type of the enumeration literal
  void _publish_cc_condition( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_direction( published_file &_cc_out );
  string _get_cc_direction();

  void _publish_cc_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_wait_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_ams_function( published_file &_cc_out, PublishData *declarations );

  IIR_Boolean _is_static_expression() { return TRUE;}

  void _add_declarations_in_initializations( PublishData * ) {};
  void _add_decl_into_declaration_collection( PublishData * ) {};

  // Helper Functions
  TyvisAttributeSpecificationList*  _get_attribute_specification_list();
  TyvisAttributeSpecificationList * _get_attributes();
protected:

private:

};
#endif

#ifndef TYVIS_COMPONENTDECLARATION_HH
#define TYVIS_COMPONENTDECLARATION_HH

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
#include "savant/IIRBase_ComponentDeclaration.hh"

class TyvisComponentDeclaration : public virtual TyvisDeclaration,
				  public virtual IIRBase_ComponentDeclaration {
public:
  TyvisComponentDeclaration();
  ~TyvisComponentDeclaration();

  /**
     Publish elaboration files.  Needs the fake param to match the
     signature correctly.
  */
  void _publish_cc_binding_name( ostream& outstream );
  void _publish_cc_package_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &, PublishData * );
  void _publish_cc_headerfile( PublishData *declarations );
  void _publish_cc_ccfile( PublishData *declarations );
  void _publish_cc_class( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_destructor( published_file &_cc_out );
  void _publish_cc_instantiate( published_file &_cc_out );
  void _publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_connect( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_getboundentityinfo( published_file &_cc_out );

  using TyvisDeclaration::_publish_cc_include;
  void _publish_cc_include( published_file &_cc_out );

  void _publish_cc_ams_form_characteristic_expressions( published_file &_cc_out );
  void _publish_cc_ams_connect_terminals( published_file &_cc_out, PublishData *declarations );

  // Helper Functions
  TyvisAttributeSpecificationList        *_get_attribute_specification_list();
  TyvisPortList                          *_get_port_list();
  TyvisGenericList                       *_get_local_generic_clause();
  TyvisPortList                          *_get_local_port_clause();
  TyvisGenericList                       *_get_generic_list();
  TyvisPortList                          *_get_entity_port_clause();

protected:

private:

};
#endif

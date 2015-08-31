#ifndef TYVIS_SELECTEDNAME_HH
#define TYVIS_SELECTEDNAME_HH

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

#include "TyvisName.hh"
#include "savant/IIRBase_SelectedName.hh"

class TyvisSelectedName : public virtual TyvisName, public virtual IIRBase_SelectedName {
public:
  void _add_decl_into_declaration_collection(PublishData *declarations);
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_object_name( published_file &_cc_out, PublishData *declarations );
  
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );

  TyvisArchitectureDeclaration *_get_cc_instantiated_architecture();
  TyvisEntityDeclaration *_get_cc_instantiated_entity();

  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_declarator( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );

  savant::set<TyvisDeclaration> *_get_implicit_declarations(){ return _implicit_decls; }
  void _set_implicit_declarations( savant::set<TyvisDeclaration> *id){ _implicit_decls = id; }

  void _set_passed_through_out_port(IIR_Boolean);

  IIR_Boolean _is_globally_static();

  // Helper Functions
  TyvisDeclaration               *_get_package_declaration();
  TyvisPortList                  *_get_port_list();
  TyvisDeclaration               *_get_prefix_declaration();
  Tyvis                           *_get_suffix();
  TyvisGenericList               *_get_generic_list();
protected:

private:
   savant::set<TyvisDeclaration> *_implicit_decls;

};
#endif

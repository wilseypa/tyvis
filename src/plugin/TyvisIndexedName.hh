#ifndef TYVIS_INDEXEDNAME_HH
#define TYVIS_INDEXEDNAME_HH

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
#include "savant/IIRBase_IndexedName.hh"

class TyvisIndexedName : public virtual TyvisName, public virtual IIRBase_IndexedName {
public:
  void _add_decl_into_declaration_collection(PublishData *declarations);
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_range( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_subprogram_arg( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_declarator( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_name_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_object_name( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_wait_data( published_file &_cc_out, PublishData *declarations );
  const string _get_cc_tyvis_type();
  void _publish_cc_index_name_as_for_loop( published_file &_cc_out, PublishData *declarations );
  Tyvis *_get_cc_instantiated_unit();
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );

  IIR_Mode _get_mode();

  IIR_Boolean _is_resolved_signal();

   savant::set<TyvisDeclaration> *_get_implicit_declarations(){ return _implicit_decls; }
  void _set_implicit_declarations(  savant::set<TyvisDeclaration> *id ){ _implicit_decls = id; };

  // Methods needed to check and publish get_attributes() on indexed names 
  IIR_Boolean _is_published_attribute_in_state(SignalAttribute);
  IIR_Boolean _is_published_attribute_in_constructor(SignalAttribute);
  IIR_Boolean _is_published_attribute_in_initstate(SignalAttribute);
  void _add_published_attribute_in_state(SignalAttribute);
  void _add_published_attribute_in_constructor(SignalAttribute);
  void _add_published_attribute_in_initstate(SignalAttribute);

  void _set_passed_through_out_port(IIR_Boolean);

  void _get_list_of_input_signals(  savant::set<Tyvis> *list );  

  IIR_Boolean _is_globally_static_primary();

  Tyvis *_get_declarative_region();

  // Helper Functions
  TyvisDeclaration       *_get_prefix_declaration();
  TyvisDeclaration       *_get_package_declaration();
  TyvisPortList          *_get_port_list();
  Tyvis                   *_get_suffix();
  Tyvis                   *_get_prefix();
  TyvisGenericList       *_get_generic_list();
protected:

private:

  savant::set<TyvisDeclaration> *_implicit_decls;

};
#endif

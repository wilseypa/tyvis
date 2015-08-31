#ifndef TYVIS_ENTITYDECLARATION_HH
#define TYVIS_ENTITYDECLARATION_HH

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

#include "TyvisLibraryUnit.hh"
#include "savant/IIRBase_EntityDeclaration.hh"

class TyvisEntityDeclaration : public virtual TyvisLibraryUnit, public virtual IIRBase_EntityDeclaration {
public:
  TyvisEntityDeclaration();
  ~TyvisEntityDeclaration();

  void _add_decl_into_declaration_collection(PublishData *){}

  void _publish_cc();
  void _publish_cc_elaborate();
  void _publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_binding_name( ostream& outstream );
  void _publish_cc_class( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headerfile( PublishData *declarations );
  void _publish_cc_ccfile(PublishData *declarations);
  void _publish_cc_entity_file_name( ostream &os );
  void _publish_cc_component_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headerfiles_for_cc( published_file &_cc_out );
  void _publish_cc_constructor( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_signal_objects_init( published_file &_cc_out, PublishData *declarations, const IIR_Boolean = TRUE );
  void _publish_cc_include_decls_prefix( ostream &os );
  void _publish_cc_scoping_prefix( ostream &, Tyvis *, Tyvis * );

  /**
     This over-ridden version of this method returns null.  Here we don't
     know what arch was instantiated.
  */
  TyvisArchitectureDeclaration *_get_cc_instantiated_architecture();
  TyvisEntityDeclaration *_get_cc_instantiated_entity();

  // Helper Functions
  TyvisGenericList *                             _get_generic_clause();
  TyvisDeclarationList *                         _get_entity_declarative_part();
  TyvisArchitectureStatementList *               _get_entity_statement_part();
  TyvisPortList *                                _get_port_list();
  TyvisPortList *                                _get_port_clause();
  TyvisGenericList *                             _get_generic_list();
protected:
  TyvisEntityDeclaration*           _get_entity();

private:

};
#endif

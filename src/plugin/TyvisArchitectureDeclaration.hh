#ifndef TYVIS_ARCHITECTUREDECLARATION_HH
#define TYVIS_ARCHITECTUREDECLARATION_HH

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
#include "savant/IIRBase_ArchitectureDeclaration.hh"
// #include "savant/symbol_table.hh"

class TyvisName;
class PublishData;

class TyvisArchitectureDeclaration : public virtual TyvisLibraryUnit,
				     public virtual IIRBase_ArchitectureDeclaration {
public:
  TyvisArchitectureDeclaration();
  ~TyvisArchitectureDeclaration();

  void _publish_cc();
  void _publish_cc_elaborate();
  void _publish_cc_declarations(PublishData *declarations);
  void _publish_cc_makefile_stub();
  void _publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_binding_name( ostream& outstream );
  void _publish_cc_signal_objects_init( published_file &_cc_out, PublishData *declarations, IIR_Boolean = FALSE );
  void _publish_cc_scoping_prefix( ostream &os, Tyvis *, Tyvis * );
  void _publish_cc_implicit_signals( published_file &_cc_out,
				     TyvisDeclarationList *,
				     PublishData *declarations);
  /** Baiscally it publishes the include of the hh file in the cc file. */
  void _publish_cc_include( published_file &_cc_out );
  
  virtual void _publish_cc_main(published_file & ) override;

  /** Publish VHDL-AMS specific header files. */
  void _publish_cc_ams_includes(published_file &);

  /** Function that calls the subclass functions to publish characteristic
      expressions. */
  void _publish_cc_ams_form_characteristic_expressions(published_file &);  

  /** This method gets called in the case of a selected name
      architecture.member.  The architecture doesn't need to be added, so
      this method is stubbed as an empty method here. */
  void _add_decl_into_declaration_collection(PublishData *){}
  
  savant::set<TyvisDeclaration> *_find_declarations( TyvisName * );
  TyvisList *_get_statement_list();

  TyvisArchitectureDeclaration *_get_cc_instantiated_architecture();
  TyvisEntityDeclaration *_get_cc_instantiated_entity();

  string _mangle_declarator();

  // Helper Functions
  TyvisPortList                          *_get_port_list();
  TyvisGenericList                       *_get_generic_list();
  TyvisDeclarationList                   *_get_declaration_list();
  TyvisEntityDeclaration                 *_get_entity();
  TyvisTextLiteral                       *_get_declarator();
  TyvisDeclarationList                   *_get_architecture_declarative_part();
  TyvisArchitectureStatementList         *_get_architecture_statement_part();
protected:

private:
  //@{ These functions are used for generating code that is useful for doing 
  //   dynamic elaboration
  void _publish_cc_class( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headerfile( PublishData *declarations );
  void _publish_cc_ccfile(PublishData *declarations, PublishData *arch_declarations);
  void _publish_cc_constructor( published_file &_cc_out, PublishData *declarations, PublishData *arch_declarations );
  void _publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations, PublishData *arch_declarations );
  void _publish_cc_constructor_with_arguments( published_file &_cc_out, PublishData *declarations, PublishData *arch_declarations );
  void _publish_cc_instantiate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_connect( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headerfiles_for_cc( published_file &_cc_out );
  void _publish_cc_blocks_elaborate( published_file &_cc_out, PublishData *declarations );
  
  void _publish_cc_ams_objects( published_file &, PublishData *collection );
  void _publish_cc_ams_objects_init( published_file &, IIR_Boolean &, PublishData *declarations, PublishData *arch_declarations );
  void _publish_cc_ams_form_global_quantity_list( published_file &, PublishData *declarations );

  void _publish_cc_ams_form_break_set( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_ams_initialize_signals( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_break_signal_info( published_file &_cc_out );
  //@}

  /** This method publishes the necessary foreign process pointer when
      an architecture is foreign. */
  void _publish_cc_foreign_process_pointer( published_file &_cc_out );

  /** Get the design unit name for this design unit. */  
  const string _get_cc_design_unit_name();

  void _publish_cc_include_decls_prefix( ostream &os );

  /** Publishes the instantiate method for a foreign architecture. */
  void _publish_cc_foreign_instantiate( published_file &_cc_out );

  /** Publishes the createNetInfo method for a foreign architecture. */
  void _publish_cc_foreign_createNetInfo( published_file &_cc_out, PublishData *declarations );

  /** Publishes the "allocateModule" C function needed to load this architecture
      as a module. */
  void _publish_cc_allocate_module( published_file &_cc_out );

  /** Publishes the "allocateModule" C function needed to load this architecture
      as a module. */
  void _publish_cc_ams_connect_terminals(published_file &, PublishData *declarations);

  /** Returns true if there is a foreign attribute associated with
     this architecture. */
  bool _is_foreign_architecture();

  /** Returns the foreign attribute associated with this architecture.
     If there is more than one, returns the last one. */
  TyvisAttributeSpecification *_get_foreign_attribute();
   
  IIR_Boolean _is_ams_objects_present;

  PublishData *_arch_declarations;
};
#endif

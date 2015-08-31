#ifndef TYVIS_CONCURRENTGENERATEFORSTATEMENT_HH
#define TYVIS_CONCURRENTGENERATEFORSTATEMENT_HH

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

#include "TyvisConcurrentStatement.hh"
#include "savant/IIRBase_ConcurrentGenerateForStatement.hh"

class TyvisSimpleName;

class TyvisConcurrentGenerateForStatement : public virtual TyvisConcurrentStatement, public virtual IIRBase_ConcurrentGenerateForStatement {
public:
  TyvisConcurrentGenerateForStatement();
  ~TyvisConcurrentGenerateForStatement();

  void _publish_cc( published_file &_cc_out, PublishData *declarations  );
  void _publish_cc_generate_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_declarations( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_class( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headerfile( PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_ccfile(PublishData *declarations);
  void _publish_cc_headerfiles_for_cc( published_file &_cc_out );
  void _publish_cc_constructor( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_signal_objects_init( published_file &_cc_out, 
                                        PublishData *declarations,
					IIR_Boolean first_decl_flag = FALSE );
  void _publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_instantiate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_connect( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_connect_call( published_file &_cc_out, PublishData *declarations ){ _publish_cc_connect_call_generate( _cc_out, declarations ); }
  void _publish_cc_getGenerateConstant( published_file &_cc_out );
  void _publish_cc_resetGenerateConstant( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_body_for_component_createNetInfo( published_file &_cc_out,
						     TyvisArchitectureStatement*,
                                                     PublishData *);
  void _publish_cc_static_members( published_file &_cc_out );
  void _publish_cc_init( published_file &_cc_out, Tyvis *scope, PublishData *declarations );
  void _publish_cc_universal_generate_range( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_generate_for_loop( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_generate_for_loop_with_zero( published_file &_cc_out, PublishData *declarations,  char *variable_name = "i" );
  void _publish_cc_assign_to_loop_constant( published_file &_cc_out,
					    PublishData *declarations ,
					    const char * = "UniversalInteger(i)");

  TyvisLabel *_find_instantiate_label( TyvisSimpleName *to_find );

  virtual TyvisAssociationList *_get_generic_map_aspect(){ return NULL; }

  IIR_Boolean _is_concurrent_generate_statement() { return TRUE; };

  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations ); 

  // Helper Functions
  TyvisDeclarationList           *_get_block_declarative_part();
  TyvisDeclarationList           *_get_declaration_list();
  TyvisList                      *_get_statement_list();
  TyvisConstantDeclaration       *_get_generate_parameter_specification();
  TyvisArchitectureStatementList *_get_concurrent_statement_part();
protected:

private:

};
#endif

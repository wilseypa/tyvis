#ifndef TYVIS_BLOCKSTATEMENT_HH
#define TYVIS_BLOCKSTATEMENT_HH

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
#include "savant/IIRBase_BlockStatement.hh"

class TyvisAssociationElement;
class TyvisLabel;

class TyvisBlockStatement : public virtual TyvisConcurrentStatement, public virtual IIRBase_BlockStatement {
public:
  TyvisBlockStatement();
  ~TyvisBlockStatement();

  void _publish_cc( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );

  /**
     Writes a header with our declarations.
  */
  void _publish_cc_declarations( PublishData *declarations );

  /**
     Writes a header with our declarations.
  */
  void _publish_cc_headerfile( PublishData *declarations );

  /**
     Writes our elab cc file.
  */
  void _publish_cc_ccfile( PublishData *declarations );

  void _publish_cc_class_includes( published_file &_cc_out );
  void _publish_cc_class( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headerfiles_for_cc( published_file &_cc_out );
  void _publish_cc_constructor( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_instantiate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_connect_call( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_connect( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_signal_objects_init( published_file &_cc_out, PublishData *declarations, IIR_Boolean = FALSE );
  void _publish_cc_blocks_elaborate( published_file &_cc_out, PublishData *declarations );

  void _publish_createNetInfo( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_elaborate_add( published_file &_cc_out, TyvisAssociationElement *, TyvisLabel *, PublishData *declarations);

  void _publish_cc_elaborate_addChild( published_file &_cc_out, 
				       TyvisAssociationElement *, 
				       TyvisLabel *,
                                       PublishData *);

  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_include_elab( published_file &_cc_out );

  using TyvisConcurrentStatement::_publish_cc_include;
  void _publish_cc_include( published_file & );

  const string _get_block_unit_name() const;

  // Helper Functions
  TyvisDeclarationList                   *_get_block_declarative_part();
  TyvisGenericList                       *_get_generic_list();
  TyvisPortList                          *_get_port_clause();
  TyvisList                              *_get_statement_list();
  Tyvis                                   *_get_guard_expression();
  TyvisGenericList                       *_get_generic_clause();
  TyvisAssociationList                   *_get_port_map_aspect();
  TyvisArchitectureStatementList         *_get_block_statement_part();
  TyvisAssociationList                   *_get_generic_map_aspect();

  IIR_Boolean                           _is_block_statement() { return TRUE; };

  static Tyvis*                   guard_expr;
protected:

private:

};
#endif

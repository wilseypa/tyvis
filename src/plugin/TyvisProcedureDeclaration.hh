#ifndef TYVIS_PROCEDUREDECLARATION_HH
#define TYVIS_PROCEDUREDECLARATION_HH

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

#include "TyvisSubprogramDeclaration.hh"
#include "savant/IIRBase_ProcedureDeclaration.hh"

class TyvisWaitStatement;

class TyvisProcedureDeclaration : public virtual TyvisSubprogramDeclaration,
				  public virtual IIRBase_ProcedureDeclaration {
public:
  void _publish_cc_wait_init( published_file &_cc_out,
			      dl_list<TyvisWaitStatement> *_wait_stmt_list,
			      PublishData *declarations );

  void _publish_cc_wait_data( published_file &_cc_out,
			      dl_list<TyvisWaitStatement> *_wait_stmt_list,
			      PublishData *declarations );

  void _publish_cc_decl( published_file &_cc_out,
			 PublishData *declarations );

  void _get_signal_source_info(  savant::set<Tyvis> *siginfo );

  string _mangle_declarator();

  // Helper Functions
  Tyvis *                            _get_procedure_name();
  TyvisDeclarationList *            _get_subprogram_declarations();
  TyvisTextLiteral *                _get_declarator();

  IIR_TypeDefinition *get_subtype(){ return 0; }

  virtual void _publish_cc_procedure_stuff( published_file &_cc_out,
					    PublishData *declarations );

private:
  /**
     Returns true if this name needs mangle, false otherwise.
   */
  bool _need_to_mangle();
  /**
     Adds parameters onto the mangled name.
  */
  void _publish_cc_add_parameters( ostream & );
};
#endif

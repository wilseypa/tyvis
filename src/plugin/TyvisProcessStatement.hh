#ifndef TYVIS_PROCESSSTATEMENT_HH
#define TYVIS_PROCESSSTATEMENT_HH

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
#include "TyvisSequentialStatementList.hh"
#include "TyvisDeclarationList.hh"
#include "savant/IIRBase_ProcessStatement.hh"
#include "savant/dl_list.hh"
#include "savant/set.hh"

class TyvisAboveAttribute;
class TyvisWaitStatement;
class PublishData;

class TyvisProcessStatement : public virtual TyvisConcurrentStatement,
			      public virtual IIRBase_ProcessStatement {
public:
  TyvisProcessStatement();
  ~TyvisProcessStatement();

  void _add_declarations_in_initializations(PublishData *declarations);

  void _publish_cc( published_file &_cc_out, PublishData *declarations );
  /** Publish the _decls.hh file for this process. */
  void _publish_cc_decls_header( PublishData *declarations );
  /** Publish the _decls.cc file for this process. */
  void _publish_cc_decls_body( PublishData *declarations ); 
  /** Do some magic wait insertion for above attributes. */
  void _transmute_above_attributes();
  /** Publish the .hh file for this process. */
  void _publish_cc_process_header( PublishData *declarations );
  /** Publish the .cc file for this process. */
  void _publish_cc_process_cc_file( PublishData *declarations );

  void _publish_cc_state(PublishData *declarations);
  void _publish_cc_extern_declarations( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_binding_name( ostream &_cc_out );
  const string _get_cc_binding_name();

  virtual void _publish_createNetInfo( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_instantiate_call( published_file &_cc_out, PublishData *declarations );

#ifdef PROCESS_GRAPH
  void _publish_cc_driver_info( published_file &_cc_out );
#endif

  /** This function publishes all the implicit declarations of the
      declarations visible in the process statement part. */
  void _publish_cc_implicit_declarations( published_file &_cc_out,
					  TyvisDeclaration *decl,
                                          PublishData *declarations );

  void _publish_cc_concurrent_stmt_init( published_file &_cc_out,
					 TyvisDeclarationList *decl_list,
                                         PublishData *declarations );

  /** Publishes an include for this process. */
  virtual void _publish_cc_include( published_file &_cc_out ) { 
    _publish_cc_include( _cc_out, "" ); 
  }

  /** Returns a string holding the process' name wrt to C++ publishing. */
  const string _get_cc_process_class() const;
  const string _get_cc_state_class() const;

  void _publish_cc_ams_break_function_include(published_file &);
  void _publish_cc_ams_breakExpression_function_body(published_file &, PublishData *);
  void _publish_cc_ams_breakExpression_function_prototype(published_file &);

  void _build_wait_list();

  void _get_list_of_input_signals(  savant::set<Tyvis> *list );
  void _get_signal_source_info(  savant::set<Tyvis> *siginfo );

  dl_list<TyvisWaitStatement> _wait_stmt_list;
  savant::set<TyvisAboveAttribute> _above_attribute_set;

  PublishData* get_declaration_collection() { return declaration_collection; }

  // Helper Functions
  TyvisDeclarationList         *get_process_declarative_part();
  TyvisSequentialStatementList *get_process_statement_part();
protected:

private:
  /** The VHDL objects namely the signal and variable has to be specified
      while declaring the type. The type of object namely the signal or
      Variable is specified in the constructor of the state classes that is
      generated. */
  void _publish_cc_state_objects_init( published_file &_cc_out, savant::set<TyvisDeclaration> *, PublishData *declarations );
  /** Publishes the includes that the state class needs. */
  void _publish_cc_headers( published_file &_cc_out );
  /** Publishes the definition of the state class. */
  void _publish_cc_state_class( published_file &_cc_out, PublishData *declarations );
  /** Publishes the constructor for the state class. */
  void _publish_cc_constructor( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_initstate( published_file &_cc_out, PublishData *declarations );
  /** Publishes the methods used to access file objects in the state class. */
  void _publish_cc_state_file_methods( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_cleanstate( published_file &_cc_out );
  void _publish_cc_locatesignal( published_file &_cc_out, PublishData *declarations );
  void _publish_ccprint( published_file &_cc_out );
  void _publish_cc_operator_equalto( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_wait_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_composite_resolved_signal_init( published_file &_cc_out, PublishData *declarations );

  /** Helper for the other _publish_cc_include methods. */
  virtual void _publish_cc_include( published_file &_cc_out, const string &to_insert );

  void _publish_cc_read_or_write( published_file &_cc_out, const string &, const string &, PublishData *declarations );

  /** Publishes an include for this process' state. */
  virtual void _publish_cc_include_state( published_file &_cc_out );

  /** The following function clears the newly set code generation
      get_attributes() in any of the previous process Statements.  code
      generation get_attributes() are the data members in the Tyvis* nodes.
      NEED: The Signals and other objects that are defined in Architecture
      decls are visible throughout. Certain get_attributes() are added to such
      nodes While generating code for process Statements. They should be
      cleared while publishing a new process Statement. */
  void _clear( PublishData *declarations );

  PublishData   *declaration_collection;
};
#endif

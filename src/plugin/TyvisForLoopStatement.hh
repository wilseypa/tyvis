#ifndef TYVIS_FORLOOPSTATEMENT_HH
#define TYVIS_FORLOOPSTATEMENT_HH

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

#include "TyvisSequentialStatement.hh"
#include "savant/IIRBase_ForLoopStatement.hh"

class TyvisSequentialStatementList;
class TyvisConstantDeclaration;
class TyvisAttribute;

class TyvisForLoopStatement : public virtual TyvisSequentialStatement, public virtual IIRBase_ForLoopStatement {
public:
  TyvisForLoopStatement();
  ~TyvisForLoopStatement();

  void _publish_cc( published_file &_cc_out, PublishData *declarations  );
  void _publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_anonymous_type_infos( published_file &_cc_out, PublishData *declarations );
  void _build_wait_list(dl_list<TyvisWaitStatement>* );
  void _build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *);
  void _get_list_of_input_signals( savant::set<Tyvis> *list);
  void _get_signal_source_info( savant::set<Tyvis> *siginfo );

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

  // Helper Functions
  TyvisSequentialStatementList *    _get_sequence_of_statements();
  TyvisConstantDeclaration *        _get_iteration_scheme();
protected:

private:
  void _publish_cc_initialize_iterator( published_file &, PublishData * );
  void _publish_cc_while_loop_begin( published_file &, PublishData * );
  string _get_cc_iterator_name();

};
#endif

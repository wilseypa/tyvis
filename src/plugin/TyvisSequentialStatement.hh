#ifndef TYVIS_SEQUENTIALSTATEMENT_HH
#define TYVIS_SEQUENTIALSTATEMENT_HH

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

#include "TyvisStatement.hh"
#include "savant/IIRBase_SequentialStatement.hh"

template <class type> class dl_list;
class TyvisWaitStatement;
class TyvisProcedureCallStatement;

class TyvisSequentialStatement : public virtual TyvisStatement, public virtual IIRBase_SequentialStatement {
public:
  void _publish_cc_next_label( published_file &_cc_out );
  void _publish_cc_exit_label( published_file &_cc_out );
  virtual void _publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations );

  virtual void _set_process_stmt_label(TyvisLabel *);
  virtual TyvisLabel* _get_process_stmt_label();

  virtual void _build_wait_list(dl_list<TyvisWaitStatement> *);
  virtual void _build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *);

protected:

private:

};
#endif

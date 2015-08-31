#ifndef TYVIS_WAITSTATEMENT_HH
#define TYVIS_WAITSTATEMENT_HH

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
#include "savant/IIRBase_WaitStatement.hh"

class TyvisWaitStatement : public virtual TyvisSequentialStatement, public virtual IIRBase_WaitStatement {
public:
  TyvisWaitStatement();
  ~TyvisWaitStatement();

  // methods and data for c++ code generation
  IIR_Int32 wait_id;
  void _publish_cc( published_file &_cc_out, PublishData *declarations  );
  void _publish_cc_wait_label( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_wait_data( published_file &_cc_out, IIR_Char*, PublishData *declarations );
  void _publish_cc_proc_wait_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_wait_decl( published_file &_cc_out );
  void _publish_cc_procedure_wait( published_file &_cc_out, PublishData *declarations );

  void _build_wait_list(dl_list<TyvisWaitStatement>* );
  void _get_list_of_input_signals(  savant::set<Tyvis> *list );

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

  // Helper Functions
  TyvisDesignatorList *     _get_sensitivity_list();
  Tyvis *                    _get_condition_clause();
  Tyvis *                    _get_timeout_clause();
protected:

private:

};
#endif

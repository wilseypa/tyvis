#ifndef TYVIS_CASESTATEMENTALTERNATIVELIST_HH
#define TYVIS_CASESTATEMENTALTERNATIVELIST_HH

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

#include "TyvisList.hh"
#include "savant/IIRBase_CaseStatementAlternativeList.hh"

class TyvisWaitStatement;

class TyvisCaseStatementAlternativeList : public virtual TyvisList, public virtual IIRBase_CaseStatementAlternativeList {
public:
  void _publish_cc( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_decl_for_loop_iterator( published_file &_cc_out, PublishData *declarations );

  void _build_wait_list(dl_list<TyvisWaitStatement>* );
  void _get_list_of_input_signals(  savant::set<Tyvis> *list );
  void _get_signal_source_info(  savant::set<Tyvis> *siginfo );

protected:

private:

};
#endif

#ifndef TYVIS_CONCURRENTCONDITIONALSIGNALASSIGNMENT_HH
#define TYVIS_CONCURRENTCONDITIONALSIGNALASSIGNMENT_HH

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
#include "savant/IIRBase_ConcurrentConditionalSignalAssignment.hh"

class TyvisSignalDeclaration;
class TyvisConditionalWaveformList;

class TyvisConcurrentConditionalSignalAssignment : public virtual TyvisConcurrentStatement, public virtual IIRBase_ConcurrentConditionalSignalAssignment {
public:
  TyvisConcurrentConditionalSignalAssignment();
  ~TyvisConcurrentConditionalSignalAssignment();

  IIR_Boolean _is_simultaneous_statement();
  TyvisSignalDeclaration *_get_guard_signal();
  void _publish_cc( published_file &_cc_out, PublishData * );
  void _publish_cc_main(published_file& ) override;

  void _build_sensitivity_list(TyvisDesignatorList* sensitivity_list);
  Tyvis *_transmute();

  // Helper Functions
  TyvisConditionalWaveformList   *_get_conditional_waveforms();
  Tyvis                           *_get_reject_time_expression();
  Tyvis                           *_get_target();

protected:
private:
};
#endif

#ifndef TYVIS_SIGNALASSIGNMENTSTATEMENT_HH
#define TYVIS_SIGNALASSIGNMENTSTATEMENT_HH

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
#include "savant/IIRBase_SignalAssignmentStatement.hh"

class TyvisWaitStatement;
class TyvisWaveformList;

class TyvisSignalAssignmentStatement : public virtual TyvisSequentialStatement, public virtual IIRBase_SignalAssignmentStatement {
public:
  TyvisSignalAssignmentStatement();
  ~TyvisSignalAssignmentStatement();

  void _publish_cc( published_file &_cc_out, PublishData *declarations  );

  void _build_wait_list(dl_list<TyvisWaitStatement>* ) {}
  void _get_list_of_input_signals(  savant::set<Tyvis> *list );
  void _get_signal_source_info(  savant::set<Tyvis> *siginfo );

  TyvisWaveformList      *_get_waveform();
  Tyvis                   *_get_reject_time_expression();
  Tyvis                   *_get_target();

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

protected:

private:
  /**
    Publishes the target expression for the signal assignemt statement.
  */
  void _publish_cc_target( published_file &_cc_out, PublishData *declarations );

  /**
     Publishes the time expression that the TyVis wants to see.
   */
  void _publish_cc_time_expression( published_file &_cc_out, 
				    IIR_Boolean first,
                                    PublishData *declarations);

};
#endif

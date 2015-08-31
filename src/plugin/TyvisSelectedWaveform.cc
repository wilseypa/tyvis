
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

#include "TyvisSelectedWaveform.hh"
#include "TyvisList.hh"
#include "TyvisChoiceList.hh"
#include "TyvisChoice.hh"
#include "TyvisWaveformList.hh"
#include "TyvisWaveformElement.hh"
#include "savant/savant.hh"

TyvisSelectedWaveform::TyvisSelectedWaveform() {
  set_waveform(new TyvisWaveformList());
}

TyvisSelectedWaveform::~TyvisSelectedWaveform() {
  //Release the list memory
  delete get_waveform();
}

TyvisWaveformList *
TyvisSelectedWaveform::_get_waveform() {
  return dynamic_cast<TyvisWaveformList *>(get_waveform());
}

Tyvis *
TyvisSelectedWaveform::_get_choice() {
  return dynamic_cast<Tyvis *>(get_choice());
}

void
TyvisSelectedWaveform::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  
  // It would be like:
  // case k is
  //   when false    => NULL;
  //   when true   => NULL;
  //     kk := 5;
  // end case;
  
  // And then assuming that true and false might need to be in the sensitivity list.

  _get_waveform()->_build_sensitivity_list(sensitivity_list);
}

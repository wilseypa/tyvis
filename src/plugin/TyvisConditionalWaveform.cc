// Copyright (c) 2003 The University of Cincinnati.
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

#include "TyvisConditionalWaveform.hh"
#include "TyvisDesignFile.hh"
#include "TyvisWaveformElement.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include <iostream>
#include "TyvisWaveformList.hh"
#include "savant/StandardPackage.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"

TyvisConditionalWaveform::TyvisConditionalWaveform() {
  set_waveform(new TyvisWaveformList());
}

TyvisConditionalWaveform::~TyvisConditionalWaveform(){
  //Release the list memory
  delete get_waveform();
}

Tyvis *
TyvisConditionalWaveform::_get_condition() {
  return dynamic_cast<Tyvis *>(get_condition());
}

TyvisWaveformList *
TyvisConditionalWaveform::_get_waveform(){
  return dynamic_cast<TyvisWaveformList *>(get_waveform());
}

void
TyvisConditionalWaveform::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  if(_get_condition() != NULL) {
    _get_condition()->_build_sensitivity_list(sensitivity_list);
  }
  _get_waveform()->_build_sensitivity_list(sensitivity_list);
}

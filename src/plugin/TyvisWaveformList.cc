
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

#include "TyvisWaveformList.hh"
#include "TyvisWaveformElement.hh"
#include "savant/savant.hh"

void
TyvisWaveformList::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  TyvisWaveformElement* element = dynamic_cast<TyvisWaveformElement *>(first());
  for(; element != NULL; ) {
    element->_build_sensitivity_list(sensitivity_list);
    element = dynamic_cast<TyvisWaveformElement *>(successor(element));
  }  
}

void
TyvisWaveformList::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  TyvisWaveformElement* element = dynamic_cast<TyvisWaveformElement *>(first());
  while( element != NULL ) {
    element->_get_list_of_input_signals(list);
    element = dynamic_cast<TyvisWaveformElement *>(successor(element));
  }
}

void
TyvisWaveformList::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  TyvisWaveformElement* element = dynamic_cast<TyvisWaveformElement *>(first());
  for(; element != NULL; element = dynamic_cast<TyvisWaveformElement *>(successor(element))) {
    element->_build_above_attribute_set(to_build);
  }
}

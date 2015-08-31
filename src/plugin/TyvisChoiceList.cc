
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

#include "TyvisChoiceList.hh"
#include "TyvisChoice.hh"
#include "published_file.hh"
#include "savant/savant.hh"

void
TyvisChoiceList::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ){
  int numberOfChoices = 1;
  TyvisChoice *choice = dynamic_cast<TyvisChoice *>(first());

  CC_REF( _cc_out, "TyvisChoiceList::_publish_cc" );

  _cc_out << OS("else if(")
	  << "SAVANT_BOOLEAN_TRUE == ";
  if(!choice->_get_value()->is_subtype()){
    _get_current_publish_node()->_publish_cc_rvalue( _cc_out, declarations );
    _cc_out << OS(".vhdlEqual(");
  }
  choice->_publish_cc( _cc_out, declarations );
  if(!choice->_get_value()->is_subtype()){
    _cc_out << CS(")");
  }
  choice = dynamic_cast<TyvisChoice *>(successor(choice));
  while(numberOfChoices < size()){
    _cc_out << "||" << OS("(") << NL()
	    << "SAVANT_BOOLEAN_TRUE == " << NL();
    if(!choice->_get_value()->is_subtype()){
      _get_current_publish_node()->_publish_cc_rvalue( _cc_out, declarations );
      _cc_out << OS(".vhdlEqual(");
    }
    choice->_publish_cc( _cc_out, declarations );
    if(!choice->_get_value()->is_subtype()){
      _cc_out << CS(")");
    }
    _cc_out << CS(")");
    numberOfChoices++;
    choice = dynamic_cast<TyvisChoice *>(successor(choice));
  }
  _cc_out << CS(")");
}

void
TyvisChoiceList::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  TyvisChoice* choice = NULL;
  choice = dynamic_cast<TyvisChoice *>(first());
  while(choice != NULL) {
    choice->_build_sensitivity_list(sensitivity_list);
    choice = dynamic_cast<TyvisChoice *>(successor(choice));
  }
}

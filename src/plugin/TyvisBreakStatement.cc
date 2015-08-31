
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

#include "TyvisBreakList.hh"
#include "TyvisBreakStatement.hh"
#include "TyvisIdentifier.hh"
#include "TyvisBreakElement.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisDeclaration.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisLabel.hh"

#include <sstream>
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "published_file.hh"

void
TyvisBreakStatement::_publish_cc_ams() {
}

void
TyvisBreakStatement::_publish_cc( published_file &_cc_out, PublishData *declarations ) {
  string signalname ;
  string stateclass ;
  int condn_present = 0 ;
  int i = 0;
  signalname = _get_process_stmt_label()->_to_string();
  int count = 0 ;
  int len = signalname.size(); 
  char *charlabel = new char[len];  
  strcpy(charlabel,signalname.c_str());
  for(i= len-1 ; i>0 ; i--) {
    if (charlabel[i]== '_') count++;
    if (count == 1) break;
  }
  charlabel[i] = '\0';
  stateclass = string(charlabel);
  if(get_condition() != NULL) {
    _cc_out << "if (SAVANT_BOOLEAN_TRUE.vhdlEqual(" ;
    _get_condition()->_publish_cc_rvalue( _cc_out, declarations );
    _cc_out << ") ){" << NL();
    condn_present = 1;
  }
  _cc_out << "assignSignal((("<< stateclass <<"_state *)getState())->"<< signalname
	  << ", this, SAVANT_BOOLEAN_TRUE, "
	  << " SAVANT_ZERO_TIME, "
	  << "SAVANT_ZERO_TIME, defaultInfo, defaultInfo);"
	  << NL()
	  << "breakflag = 1;  "<< NL() ;
  if(condn_present) {
    _cc_out << "}" << NL();
  }
}


void
TyvisBreakStatement::_set_process_stmt_label(TyvisLabel* new_label) {
  _mangled_process_label = new_label;
}

TyvisLabel*
TyvisBreakStatement::_get_process_stmt_label() {
  return _mangled_process_label;
}

Tyvis *
TyvisBreakStatement::_get_condition() {
  return dynamic_cast<Tyvis *>(get_condition());
}

void
TyvisBreakStatement::_get_list_of_input_signals( savant::set<Tyvis> *list) {
  if ( _get_condition() != NULL) {
    _get_condition()->_get_list_of_input_signals(list);
  }
}

void
TyvisBreakStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  if(_get_condition() != NULL) {
    _get_condition()->_build_above_attribute_set(to_build);
  }
}

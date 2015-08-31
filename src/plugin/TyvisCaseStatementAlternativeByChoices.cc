
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

#include "Tyvis.hh"
#include "TyvisCaseStatementAlternativeByChoices.hh"
#include "TyvisChoiceList.hh"
#include "TyvisChoice.hh"
#include "TyvisSequentialStatementList.hh"
#include "savant/error_func.hh"
#include "savant/set.hh"
#include "published_file.hh"

TyvisCaseStatementAlternativeByChoices::TyvisCaseStatementAlternativeByChoices() {
  set_choices(new TyvisChoiceList());
}

TyvisCaseStatementAlternativeByChoices::~TyvisCaseStatementAlternativeByChoices() {
  //Release the list memory
  delete get_choices();
}

void 
TyvisCaseStatementAlternativeByChoices::_publish_cc( published_file &_cc_out,
						     PublishData *declarations ){

  CC_REF( _cc_out, "TyvisCaseStatementAlternativeByChoices::_publish_cc" );

  // XXX this code assumes that each case choice is exactly one value.
  // It will have to be modified for ranges.
  
  _get_choices()->_publish_cc_rvalue( _cc_out, declarations );

  _cc_out.start_block();
  _get_sequence_of_statements()->_publish_cc( _cc_out, declarations );
  _cc_out.end_block();
}

TyvisChoiceList *
TyvisCaseStatementAlternativeByChoices::_get_choices() {
  return dynamic_cast<TyvisChoiceList *>(get_choices());
}

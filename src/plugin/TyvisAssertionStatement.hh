#ifndef TYVIS_ASSERTIONSTATEMENT_HH
#define TYVIS_ASSERTIONSTATEMENT_HH

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
#include "savant/IIRBase_AssertionStatement.hh"

class TyvisAssertionStatement : public virtual TyvisSequentialStatement, public virtual IIRBase_AssertionStatement {
public:

  void _publish_cc( published_file &_cc_out, PublishData *declarations  );

  Tyvis *_get_assertion_condition();  
  Tyvis *_get_report_expression();  
  Tyvis *_get_severity_expression();

  // Just to keep _report_undefined_scram_fn quiet
  void _get_list_of_input_signals(  savant::set<Tyvis> * );

  /**
     This is an assertion statement.  Returns true.
   */
  IIR_Boolean _is_assert_statement(){ return true; }

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

protected:

private:

};
#endif

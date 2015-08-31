#ifndef TYVIS_BREAKSTATEMENT_HH
#define TYVIS_BREAKSTATEMENT_HH

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

#include "savant/IIRBase_BreakStatement.hh"
#include "TyvisSequentialStatement.hh"

class TyvisBreakStatement : public virtual IIRBase_BreakStatement, public virtual TyvisSequentialStatement {
public:
  /** @name C++ publishing methods.  These methods relate to the publishing
      of C++. */
  void _publish_cc( published_file &, PublishData *declarations );
  void _publish_cc_ams();
 
  /** mangled_process_label is used to name the generated break
      signal. This mangled_process_label is obtained from the process name
      and these functions are used to set and return them respectively. */
  void _set_process_stmt_label(TyvisLabel *);
  TyvisLabel* _get_process_stmt_label();
  void _get_list_of_input_signals( savant::set<Tyvis>* list);

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

  // Helper Functions
  Tyvis              *_get_condition();
protected:

private:
  TyvisLabel *_mangled_process_label;

};
#endif

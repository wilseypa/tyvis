#ifndef TYVIS_CONCURRENTBREAKSTATEMENT_HH
#define TYVIS_CONCURRENTBREAKSTATEMENT_HH

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

#include "savant/IIRBase_ConcurrentBreakStatement.hh"
#include "TyvisSimultaneousStatement.hh"

class TyvisConcurrentBreakStatement : public virtual IIRBase_ConcurrentBreakStatement, public virtual TyvisSimultaneousStatement {
public:
  TyvisConcurrentBreakStatement();
  ~TyvisConcurrentBreakStatement();

  /** C++ publishing methods.  These methods relate to the publishing
      of C++. */
  void _publish_cc();
  void _publish_cc_ams();
    
  /** Creates a process with no sensitivity list and a statement part that
      includes a break statement followed by a wait statement. */
  Tyvis* _transmute();
  
  /** Generates net info for signals in the break statement. */
  void _publish_createNetInfo(char *, published_file &, PublishData *);

  // Helper Functions
  TyvisDesignatorList       *_get_sensitivity_list();
  Tyvis                      *_get_condition();

protected:

private:

};
#endif

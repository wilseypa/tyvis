#ifndef TYVIS_SIMULTANEOUSSTATEMENT_HH
#define TYVIS_SIMULTANEOUSSTATEMENT_HH

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

#include "TyvisArchitectureStatement.hh"
#include "savant/IIRBase_SimultaneousStatement.hh"

class TyvisSimultaneousStatement : public virtual TyvisArchitectureStatement, public virtual IIRBase_SimultaneousStatement {
public:

  virtual void _publish_cc();
  virtual void _publish_cc_ams_function_prototype(published_file &);
  virtual void _publish_cc_ams_function_body(published_file &);
  void _publish_cc_binding_name(ostream &);

  Tyvis* _transmute();

  void _set_enclosing_scope(Tyvis *);
protected:
  Tyvis* enclosingRegion;

private:

};
#endif

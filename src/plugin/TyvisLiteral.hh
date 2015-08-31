#ifndef TYVIS_LITERAL_HH
#define TYVIS_LITERAL_HH

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
#include "savant/IIRBase_Literal.hh"

class TyvisLiteral : public virtual Tyvis, public virtual IIRBase_Literal {
public:
  virtual void _publish_cc_headers( published_file & ) {};
  void _publish_cc_decl_with_constructor_args( published_file &_cc_out,
					       PublishData *declarations );


  // Inferred from the rules in 7.4.1 and 7.4.2 all literals are globally static.
  IIR_Boolean _is_globally_static_primary(){ return TRUE; }
  IIR_Boolean _is_static_expression() { return TRUE;}
  void _get_list_of_input_signals(  savant::set<Tyvis> * ){}
  void _build_sensitivity_list(TyvisDesignatorList*){}

protected:

private:

};
#endif

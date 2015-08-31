#ifndef TYVIS_SIMPLESIMULTANEOUSSTATEMENT_HH
#define TYVIS_SIMPLESIMULTANEOUSSTATEMENT_HH

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

#include "savant/set.hh"
#include "TyvisDeclaration.hh"
#include "TyvisSimultaneousStatement.hh"
#include "savant/IIRBase_SimpleSimultaneousStatement.hh"

class TyvisSimpleSimultaneousStatement : public virtual TyvisSimultaneousStatement, public virtual IIRBase_SimpleSimultaneousStatement {
public:
  TyvisSimpleSimultaneousStatement();

  void _publish_cc( published_file &, PublishData *declarations );

  /** Publish the prototype for the simultaneous equation */
  void _publish_cc_ams_function_prototype(published_file &, PublishData *declarations);

  /** Publish the body of the simultaneous equation */
  void _publish_cc_ams_function_body(published_file &, PublishData *, PublishData *);

  /** Publish the constructors for the simultaneous equations */
  void _publish_cc_characteristic_expressions(TyvisArchitectureStatement::SimultaneousIfPublishingPart,
                                              published_file &,
                                              PublishData *);
  
  /** Publish the implicit reference quantities created for
      T'reference, where T is a terminal */
  void _publish_cc_implicit_reference_equations(published_file &_cc_out, PublishData *declarations);  
  
  /** Publish the implicit contribution quantities created for
      T'contribution, where T is a terminal */
  void _publish_cc_implicit_contribution_equations(published_file &_cc_out, PublishData *declarations);
  
  /** Publish the implicit differential quantities created for
      Q'dot, where Q is a quantity*/
  void _publish_cc_implicit_differential_equations(published_file &_cc_out, PublishData *declarations);
   
  IIR_Int32 _stmt_qty_index;
  IIR_Int32 _stmt_signal_index;
  IIR_Int32 _stmt_node_index;

  savant::set<TyvisDeclaration> _unique_qtys;
  savant::set<TyvisDeclaration> _unique_signals; 
  savant::set<TyvisDeclaration> _unique_generic_constants;

  IIR_Boolean _is_simultaneous_statement();
  IIR_Boolean _is_free_quantity_present();

  // Helper Functions
  Tyvis *_get_left_expression();
  Tyvis *_get_right_expression();
protected:

private:
  void _publish_cc_ams_equation_constructor( SimultaneousIfPublishingPart, 
                                             published_file &,
                                             PublishData *);

  IIR_Boolean _free_quantity_present;
};
#endif

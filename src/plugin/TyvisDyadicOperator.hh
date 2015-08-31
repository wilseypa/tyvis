#ifndef TYVIS_DYADICOPERATOR_HH
#define TYVIS_DYADICOPERATOR_HH

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

#include "TyvisOperator.hh"
#include "savant/IIRBase_DyadicOperator.hh"

class TyvisDyadicOperator : public virtual TyvisOperator, public virtual IIRBase_DyadicOperator {
public:
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_wait_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_ams_function(published_file &, PublishData *declarations);
  void _publish_cc_ams_function_call_in_simult_stmt(published_file &);
  virtual void _publish_cc_ams_operator_name( published_file & );
  
  void _build_reference_quantity_list(dl_list<TyvisReferenceAttribute> *);
  void _set_stmt_signal_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *);

  virtual void _flush_stmt_index();
  IIR_Boolean _contribution_quantity_found();
  IIR_Boolean _reference_quantity_found();
  IIR_Boolean _differential_quantity_found();

  void _build_contribution_quantity_list(dl_list<TyvisContributionAttribute> *);
  void _build_generic_parameter_set( savant::set<TyvisDeclaration> *);
  void _build_differential_quantity_list(dl_list<TyvisDotAttribute> *);
  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);
  void _set_stmt_qty_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *, PublishData *, PublishData *);

  void _get_list_of_input_signals(  savant::set<Tyvis> *list );
  void _build_sensitivity_list(TyvisDesignatorList* sensitivity_list);
  IIR_Boolean _is_static_expression();
  Tyvis  *_transmute();

  virtual void _add_decl_into_declaration_collection(PublishData *declarations);

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

  // Helper Functions
  Tyvis *_get_left_operand();
  Tyvis *_get_right_operand();
  TyvisSubprogramDeclaration *_get_implementation();
  
protected:
  /**
     Does publishing for _publish_cc_{r,l}value.  Publishes:
     left_operand.operator_name( right_operand ) 
     Publishing of left and right operand are handled by the method call
     passed in.
  */
  virtual void _publish_cc_xvalue( published_file &_cc_out,
                                   PublishData *declarations,
                                   void (Tyvis::*publish_function)( published_file &, PublishData * ) );

private:
  IIR_Int32 _stmt_node_index;

};
#endif

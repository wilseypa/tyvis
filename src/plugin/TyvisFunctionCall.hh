#ifndef TYVIS_FUNCTIONCALL_HH
#define TYVIS_FUNCTIONCALL_HH

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

#include "TyvisExpression.hh"
#include "savant/IIRBase_FunctionCall.hh"

class TyvisSubprogramDeclaration;

class TyvisFunctionCall : public virtual TyvisExpression, public virtual IIRBase_FunctionCall {
public:
  TyvisFunctionCall();
  ~TyvisFunctionCall();

  void _publish_cc_as_operator( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate_arg( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file &_cc_out );

  /** This function is implemented recursively in different nodes to
      handle type conversion in procedure calls. The actual can be nest
      function calls and the actual nested with them is extracted and
      published. */
  void _publish_cc_first_objectParameter( published_file &_cc_out, PublishData *declarations );

  /** Publishes the function information appearing in a simultaneous
      statement. */ 
  void _publish_cc_ams_function( published_file &, PublishData * );

  /** Publishes the constructor information for function in simultaneous
      statement. */
  void _publish_cc_ams_function_call_in_simult_stmt(published_file &, PublishData *);

  /** Publishes the math functions and its arguments appearing in a
      simultaneous statement.

      @param math_function_name Math function appearing in the simultaneous statement. 
      @param _cc_out Stream to publish into. */
  void _publish_cc_ams_math_functions( TyvisTextLiteral *math_function_name, published_file &_cc_out, PublishData *declarations );
  const string _get_cc_tyvis_type( );

  IIR_Boolean _is_globally_static_primary();

  /** Sets the index for the equation nodes of the simultaneous stmt. */
  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);
  void _set_stmt_signal_index(IIR_Int32 *index,  savant::set<TyvisDeclaration> *quantity_set);  
  void _flush_stmt_index();

  /** Sets the index for quantities appearing in the parameter list of the
      function.
     
      @param index Index number variable of the quantity.
      @param quantity_set List holding the list of quantities in a
        simultaneous statement. */ 
  void _set_stmt_qty_index(IIR_Int32 *index,  savant::set<TyvisDeclaration> *quantity_set, PublishData *declarations, PublishData *arch_declarations);  
  void _get_list_of_input_signals(  savant::set<Tyvis> *list);

  void _build_sensitivity_list( TyvisDesignatorList* sensitivity_list );

  TyvisSubprogramDeclaration* _get_implementation();
  void _add_decl_into_declaration_collection(PublishData *declarations);
  TyvisAssociationList *_get_parameter_association_list();

  IIR_Mode _get_mode();
protected:

private:
  IIR_Int32 _stmt_node_index;

};
#endif

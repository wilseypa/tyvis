#ifndef TYVIS_AGGREGATE_HH
#define TYVIS_AGGREGATE_HH

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
#include "savant/IIRBase_Aggregate.hh"

class TyvisAggregate : public virtual TyvisExpression, public virtual IIRBase_Aggregate {
public:
  TyvisAggregate();
  ~TyvisAggregate();

  void _add_decl_into_declaration_collection(PublishData *declarations);

  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_element_list( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_range( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_multi_dimensional_range( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_constructor_args( published_file &_cc_out,
				     PublishData *declarations,
				     const string &varArgsClass );

  void _get_list_of_input_signals(  savant::set<Tyvis>* list );
  void _get_signal_source_info(  savant::set<Tyvis> *siginfo );

  IIR_Boolean _is_globally_static_primary();
  void _build_sensitivity_list(TyvisDesignatorList* sensitivity_list);

  TyvisDeclaration* _get_prefix_declaration();
  /** In an aggregate initialization, the associationlist has the list of
      initializations. But there can be many initializations specified by
      TyvisCaseAlternativeByChoices, so this function returns the number of
      elemenst that are explicitly initialized, by considering the number of
      iniatializations by choices as that many number of initializations. */
  IIR_Int32 _get_number_of_element_associations();

  // Helper Functions
  TyvisAssociationList *_get_element_association_list();
protected:

private:
  void _publish_cc_xvalue_begin( published_file &_cc_out );
};
#endif

#ifndef TYVIS_ASSOCIATIONELEMENTBYEXPRESSION_HH
#define TYVIS_ASSOCIATIONELEMENTBYEXPRESSION_HH

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

#include "TyvisAssociationElement.hh"
#include "savant/IIRBase_AssociationElementByExpression.hh"

class TyvisAssociationElementByExpression : public virtual TyvisAssociationElement, public virtual IIRBase_AssociationElementByExpression {
public:
  void _add_decl_into_declaration_collection(PublishData *declarations);
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_subprogram_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_aggregate_iterator( published_file &_cc_out, PublishData *declarations );
  const string _publish_cc_magic_prefix( published_file &_cc_out );
  void _publish_cc_generic_map_aspect( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_generic_map_aspect_for_conc_stmts( published_file &_cc_out, PublishData *declarations );

  /** This function is implemented recursively in different nodes to handle
  type conversion in procedure calls. The actual can be nest function
  calls and the actual nested with them is extracted and published */  
  void _publish_cc_first_objectParameter( published_file &_cc_out, PublishData *declarations );

  void _get_signal_source_info(  savant::set<Tyvis> *siginfo );
  void _build_sensitivity_list(TyvisDesignatorList* sensitivity_list);

  IIR_Boolean _is_globally_static_for_array();
  IIR_Boolean _is_globally_static_for_record();
  IIR_Boolean _is_globally_static_for_function();
  Tyvis* _get_actual();
  
  IIR_Int32 _get_number_of_element_associations();

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

protected:

private:
  /** This function is to get rid of the confusion in the usage of
      _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations )
      Initially, _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations )
      is used to publish in the process's state, but now it is used everywhere. 
      The _publish_state_object_init() calls 
      _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations )
      on its sub-objects. This should stop if the object is not of type 
      TyvisOthersInitialization or TyvisAggregates. 
      _publish_cc_non_aggregate_object_init( published_file &_cc_out ) performs this */
      void _publish_cc_non_aggregate_object_init( published_file &_cc_out, PublishData *declarations, bool left );
      void _publish_cc_state_object_init_work( published_file &_cc_out, PublishData *declarations, bool left );
};
#endif

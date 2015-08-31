#ifndef TYVIS_ASSOCIATIONLIST_HH
#define TYVIS_ASSOCIATIONLIST_HH

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

#include "TyvisList.hh"
#include "savant/IIRBase_AssociationList.hh"

class TyvisAssociationList : public virtual TyvisList, public virtual IIRBase_AssociationList {
public:
  void _add_decl_into_declaration_collection(PublishData *declarations);
  
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_subprogram_arguments( published_file &_cc_out );
  void _publish_cc_for_index( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_generic_map_aspect( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_generic_map_aspect_for_conc_stmts( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_port_map_associations( published_file &_cc_out, PublishData *declarations );
  int  _publish_cc_unconstrained_ports( published_file &_cc_out, PublishData *declarations, IIR_Boolean commaFlag );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_necessary_temporaries_for_TC( published_file &_cc_out, 
						 TyvisInterfaceList *formals_list,
                                                 PublishData *declarations);

  void _publish_cc_with_temporaries_for_TC( published_file &_cc_out, 
					    TyvisInterfaceList *formals_list,
                                            PublishData *declarations);

  void _publish_cc_restore_from_temporaries( published_file &_cc_out, 
					     TyvisInterfaceList *,
                                             PublishData *declarations);

  void _publish_cc_array_index( published_file &_cc_out, PublishData *declarations );
  
  void _get_list_of_input_signals(  savant::set<Tyvis> *list );
  void _get_signal_source_info(  savant::set<Tyvis> *siginfo );
  void _build_sensitivity_list( TyvisDesignatorList* sensitivity_list );
  IIR_Int32 _get_number_of_element_associations();

  void _set_passed_through_out_port(IIR_Boolean); // Parameter ignored!!

  /** IIR_AssociationLists are used for aggregates (both array and record)
      and for paramter lists for functions, among other things.  Each of
      these uses has its own notion of globally static, so we must call the
      right function depending on the context.
  */
  IIR_Boolean  _is_globally_static_for_array();
  IIR_Boolean  _is_globally_static_for_record();
  IIR_Boolean  _is_globally_static_for_function();
  IIR_Boolean  _is_static_expression();			   
  IIR_Boolean  _are_all_scalars();
  IIR_Boolean  _are_any_signals();
  string _get_signal_names();

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

protected:

private:

};
#endif

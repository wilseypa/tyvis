#ifndef TYVIS_ARRAYTYPEDEFINITION_HH
#define TYVIS_ARRAYTYPEDEFINITION_HH

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

#include "TyvisTypeDefinition.hh"
#include "savant/IIRBase_ArrayTypeDefinition.hh"

class TyvisScalarTypeDefinition;

class TyvisArrayTypeDefinition : public virtual TyvisTypeDefinition, public virtual IIRBase_ArrayTypeDefinition {
public:

  void _add_decl_into_declaration_collection(PublishData *declarations);
  void _publish_cc_left( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_init_signal( published_file &_cc_out );
  void _publish_cc_range( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_universal_type( published_file &_cc_out );
  
  void _publish_cc_array_info( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_constructor_args( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_range_args( published_file &_cc_out );
  virtual void _publish_cc_constructor_ranges( published_file &_cc_out );
  virtual void _publish_cc_necessary_decl_in_state( published_file &_cc_out );
  
  virtual void _publish_cc_headers( published_file &_cc_out );
  virtual const string _get_cc_tyvis_type();

  virtual void _publish_cc_define_array_type_attributes( published_file &_cc_out );
  virtual void _publish_cc_type_info_prereq( published_file &_cc_out, PublishData *declarations, const string &type_name );
  virtual void _publish_cc_type_info_args( published_file &_cc_out,
					   PublishData *declarations,
					   const string &type_name );

  using TyvisTypeDefinition::_publish_cc_include;
  void _publish_cc_include( published_file &_cc_out );

  void _publish_cc_decl_operators( published_file &_cc_out );
  void _publish_cc_concatenation_operator( published_file &_cc_out );

  TyvisScalarTypeDefinition *_get_index_subtype();

  IIR_Boolean _is_single_dimensional_array_type();

  //The following function returns TRUE if the element of the
  //array is a restriction of an unconstrained array
  IIR_Boolean _is_element_unconstrained_subtype();

  IIR_Int32 get_num_indexes();
  virtual TyvisTypeDefinition *_get_final_subtype();

  TyvisTypeDefinition *_get_element_subtype();
protected:
  virtual void _publish_cc_object_type( published_file &_cc_out,
					IIR_Boolean typeNameFlag = TRUE, 
					const char* suffix = NULL);

  virtual IIR_Boolean _has_access_type();
private:
  string _get_element_type_name( const string &type_name );
  string _get_index_type_name( const string &type_name, int index );

  TyvisTypeDefinition* _get_element_type();

};
#endif

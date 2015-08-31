#ifndef TYVIS_SCALARTYPEDEFINITION_HH
#define TYVIS_SCALARTYPEDEFINITION_HH

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
#include "savant/IIRBase_ScalarTypeDefinition.hh"

class TyvisScalarTypeDefinition : public virtual TyvisTypeDefinition, public virtual IIRBase_ScalarTypeDefinition {
public:

  void _add_decl_into_declaration_collection(PublishData *declarations);
  virtual void _publish_cc_left( published_file &_cc_out,
				 PublishData *declarations );
  virtual void _publish_cc_right( published_file &_cc_out,
				  PublishData *declarations );
  virtual void _publish_cc_range( published_file &_cc_out,
				  PublishData *declarations );


  virtual void _publish_cc_type_info_args( published_file &_cc_out,
					   PublishData *declarations,
					   const string &type_name );

  /**
     Returns the name of this instance's typeinfo when it is used as the
     index of an array type.
  */
  string _get_cc_index_type_info();
  string _get_cc_dynamic_type_info();

  virtual void _publish_cc_init_implicit_signal( published_file &_cc_out );
  virtual void _publish_cc_init_explicit_signal( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_init_signal( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_array_info( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_signal_value_left( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_init_val( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_decl_constructors( published_file &_cc_out );
  virtual void _publish_cc_type_constructors( published_file &_cc_out );
  virtual void _publish_cc_subtype_constructors( published_file &_cc_out );
  virtual void _publish_cc_decl_destructors( published_file &_cc_out );

  virtual void _publish_cc_headers( published_file &_cc_out );

  /** The following function, publishes the range checking in the 
      generated code. */
  virtual void _publish_cc_range_check( published_file &_cc_out, PublishData *declarations );

  //The following function publishes the declarations 
  //of the type attribute functions in the generated .hh file
  virtual void _publish_cc_decl_type_attributes( published_file &_cc_out );

  // The following function publishes the body 
  //of the Type attribute functions in the generated .cc file
  virtual void _publish_cc_define_type_attributes( published_file &_cc_out );

  // The following function publishes the body 
  //of the individual Type attribute functions in the generated .cc file
  virtual void  _publish_cc_init_last_event( published_file &_cc_out );

  virtual void  _publish_cc_class_last_event( published_file &_cc_out );
  virtual void  _publish_cc_class_event( published_file &_cc_out );

  virtual void  _publish_cc_necessary_decl_in_state( published_file &_cc_out );
  
  TyvisTypeDefinition* _get_base_type();

  IIR_Boolean _is_bit_type();
  IIR_Boolean _is_boolean_type();
  IIR_Boolean _is_globally_static();

  // Helper Functions
  Tyvis                      *_get_left();
  Tyvis                      *_get_right();
  Tyvis                      *_get_direction();
protected:
  /**
     Enumeration and physical types publish range info if they need to.
  */
  virtual void _publish_cc_range_method( published_file &_cc_out,
					 const string &type_name );
  bool _need_to_publish_range();

private:

};
#endif

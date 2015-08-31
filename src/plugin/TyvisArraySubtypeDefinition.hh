#ifndef TYVIS_ARRAYSUBTYPEDEFINITION_HH
#define TYVIS_ARRAYSUBTYPEDEFINITION_HH

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

#include "TyvisArrayTypeDefinition.hh"
#include "savant/IIRBase_ArraySubtypeDefinition.hh"

class TyvisArraySubtypeDefinition : public virtual TyvisArrayTypeDefinition, public virtual IIRBase_ArraySubtypeDefinition {
public:
  void _publish_cc_define_constructors( published_file &_cc_out );
  void _publish_cc_attribute_constructor_arguments( published_file &_cc_out );

  TyvisFunctionDeclaration* _get_resolution_function();
  IIR_Boolean _is_globally_static();
protected:

private:
  void _publish_cc_define_type_attribute_left( published_file &_cc_out );
  void _publish_cc_define_type_attribute_right( published_file &_cc_out );
  void _publish_cc_define_type_attribute_low( published_file &_cc_out );
  void _publish_cc_define_type_attribute_high( published_file &_cc_out );
  void _publish_cc_define_type_attribute_length( published_file &_cc_out );
  void _publish_cc_define_type_attribute_ascending( published_file &_cc_out );

  void _publish_cc_constructor_with_no_value( published_file &_cc_out );
  void _publish_cc_constructor_with_value( published_file &_cc_out );
  void _publish_cc_copy_constructor( published_file &_cc_out );
  void _publish_cc_constructor_with_string( published_file &_cc_out );
  void _publish_cc_constructor_for_alias_init( published_file &_cc_out );
  void _publish_cc_constructor_with_aggregates( published_file &_cc_out );
  void _publish_cc_constructor_with_base_type( published_file &_cc_out );
  void _publish_cc_constructor_with_ranges( published_file &_cc_out );

};
#endif

#ifndef TYVIS_CONSTANTDECLARATION_HH
#define TYVIS_CONSTANTDECLARATION_HH

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

#include "TyvisObjectDeclaration.hh"
#include "savant/IIRBase_ConstantDeclaration.hh"

class TyvisConstantDeclaration : public virtual TyvisObjectDeclaration,
				 public virtual IIRBase_ConstantDeclaration {
public:  
  void _publish_cc_package_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_package_definition( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constant_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_wait_data( published_file &_cc_out, PublishData *declarations);
  void _publish_cc_universal_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_init_package_constant( published_file &_cc_out, PublishData *declarations );
  const string _get_cc_object_type();
  void _publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_decl_with_constructor_args( published_file &_cc_out,
					       PublishData *declarations );
  void _publish_cc_declarator_with_constructor_args( published_file &cc_out,
						     PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out,
				      PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out,
				      PublishData *declarations );

  void _publish_cc_locally_static_value( published_file &_cc_out,
					 PublishData *declarations );

  void _publish_cc_ams_function(published_file &, PublishData *declarations);

  /**
     Here, we mangle the constant's declarator with it's line number to
     make it unique.  This is not required in all the cases, however, we
     have to mangle the iterator in a for loop statement since we could
     have two for loops with the same declarator.
  */
  string _mangle_declarator();

  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);

  IIR_Boolean _is_globally_static_primary();
  IIR_Boolean _is_static_expression();

  void _set_scoping_prefix();

  Tyvis* _get_value();
protected:

private:

};
#endif

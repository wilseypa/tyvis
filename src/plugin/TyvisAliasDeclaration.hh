#ifndef TYVIS_ALIASDECLARATION_HH
#define TYVIS_ALIASDECLARATION_HH

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

#include "TyvisDeclaration.hh"
#include "savant/IIRBase_AliasDeclaration.hh"

class TyvisAliasDeclaration : public virtual TyvisDeclaration, public virtual IIRBase_AliasDeclaration {
public:
  void _publish_cc_package_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_signal_net_info( published_file &_cc_out,
				    cc_publish_mode mode,
				    PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_decl_original_object( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_alias_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate_alias_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate_alias_definition( published_file &_cc_out,
					       PublishData *declarations );
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_args( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_universal_type( published_file &_cc_out );

  void _publish_cc_decl_with_constructor_args( published_file &_cc_out, 
					       PublishData *declarations );

  void _publish_cc_declarator_with_constructor_args( published_file &_cc_out, 
						     PublishData *declarations );

  void _publish_cc_type_info_xxx( published_file &_cc_out,
				  PublishData *declarations );

  IIR_Boolean _is_globally_static_primary();
  void _build_sensitivity_list(TyvisDesignatorList* sensitivity_list);

  TyvisDeclaration *_get_prefix_declaration();
  IIR_Int32 get_num_indexes();
  void _add_declarations_in_initializations( PublishData *collection );
  void _add_decl_into_declaration_collection( PublishData *collection );
  void _add_necessary_decls_into_declaration_collection( PublishData *collection );

  Tyvis *_get_name();

  void _set_driver_addition_flag(IIR_Boolean);

  // Helper functions
  TyvisAttributeSpecificationList* _get_attribute_specification_list(); 
protected:

private:

};
#endif

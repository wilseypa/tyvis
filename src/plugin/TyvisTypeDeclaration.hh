#ifndef TYVIS_TYPEDECLARATION_HH
#define TYVIS_TYPEDECLARATION_HH

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
#include "savant/IIRBase_TypeDeclaration.hh"

class TyvisTypeDeclaration : public virtual TyvisDeclaration, public virtual IIRBase_TypeDeclaration {
public:
  TyvisTypeDeclaration();
  ~TyvisTypeDeclaration();

  void _add_decl_into_declaration_collection(PublishData *declarations);

  void _publish_cc_type_info_xxx( published_file &_cc_out,
				  PublishData *declarations );

  void _publish_cc_package_decl( published_file &_cc_out,
				 PublishData *declarations );

  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_class_name( ostream &os );
  void _publish_cc_include( published_file &_cc_out );
  void _publish_cc_universal_type( published_file &_cc_out );
  void _publish_cc_range( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations );

  IIR_Boolean _is_kernel_type();

  IIR_Int32 get_num_indexes();

  // Helper Functions
  TyvisAttributeSpecificationList        *_get_attribute_specification_list();
  TyvisTypeDefinition                    *_get_type_definition();
protected:

private:

};
#endif

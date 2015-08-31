#ifndef TYVIS_FILEDECLARATION_HH
#define TYVIS_FILEDECLARATION_HH

// Copyright (c) The University of Cincinnati.
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

#include "TyvisObjectDeclaration.hh"
#include "savant/IIRBase_FileDeclaration.hh"

class TyvisFileDeclaration : public virtual TyvisObjectDeclaration,
			     public virtual IIRBase_FileDeclaration {
public:
  void _publish_cc_package_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_package_definition( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_shared_file_decl( published_file &_cc_out,
				     PublishData *declarations,
				     const string &className = "" );
  void _publish_cc_constructor_args( published_file &_cc_out,
				     PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out,
				      PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out,
				      PublishData *declarations );

  using TyvisObjectDeclaration::_publish_cc_include;
  void _publish_cc_include( published_file & );

  IIR_Boolean _is_shared_file();  

  const string _get_cc_object_type();

  // Helper Functions
  Tyvis * _get_file_open_expression();
  Tyvis * _get_file_logical_name();

protected:

private:
  void _publish_cc_get_file_signature( published_file &_cc_out, const string &className );

};
#endif

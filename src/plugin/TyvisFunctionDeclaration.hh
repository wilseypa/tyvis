#ifndef TYVIS_FUNCTIONDECLARATION_HH
#define TYVIS_FUNCTIONDECLARATION_HH

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

#include "TyvisSubprogramDeclaration.hh"
#include "savant/IIRBase_FunctionDeclaration.hh"

class TyvisFunctionDeclaration : public virtual TyvisSubprogramDeclaration,
				 public virtual IIRBase_FunctionDeclaration {
public:  
  void _publish_cc_type_conversion_function( published_file &_cc_out,
					     PublishData *declarations );

  void _publish_cc_type_conversion_function_name( published_file &_cc_out,
						  PublishData *declarations );
  void _publish_cc_binding_name( ostream &os );
  void _publish_cc_wait_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_init_function( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_include( published_file &_cc_out );
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_necessary_copying( published_file &_cc_out );
  void _publish_cc_read_or_write( published_file &_cc_out, const string &, const string & );
  void _publish_cc_decl( published_file &_cc_out, PublishData *declarations );
  string _mangle_declarator();

  IIR_Boolean _is_possible_resolution_function();
  IIR_Boolean _is_possible_type_conversion_function();

  TyvisTypeDefinition *_get_return_type();

  IIR_Int32 get_num_indexes();

  // Helper Functions
  TyvisDeclarationList *_get_subprogram_declarations();
protected:

private:

};
#endif

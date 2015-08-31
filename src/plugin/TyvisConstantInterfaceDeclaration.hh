#ifndef TYVIS_CONSTANTINTERFACEDECLARATION_HH
#define TYVIS_CONSTANTINTERFACEDECLARATION_HH

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

#include "TyvisInterfaceDeclaration.hh"
#include "savant/IIRBase_ConstantInterfaceDeclaration.hh"

class TyvisConstantInterfaceDeclaration : public virtual TyvisInterfaceDeclaration, public virtual IIRBase_ConstantInterfaceDeclaration {
public:
  void _publish_cc_rvalue( published_file &_cc_out,
		           PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out,
			      PublishData *declarations );
  void _publish_cc_elaborate_as_pointers( published_file &_cc_out,
                                          PublishData *declarations,
					  IIR_Boolean publishDefinition = TRUE);
  void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_state_object_init( published_file &, PublishData * ){}
  void _publish_cc_state_l_object_init( published_file &, PublishData * ){}
  const string _get_cc_object_type();
  void _publish_cc_universal_value( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_ams_function(published_file &, PublishData *declarations);
  const string _get_cc_tyvis_type();

  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);

  void _build_generic_parameter_set(savant::set<TyvisDeclaration> *);
  Tyvis * _get_value();

  IIR_Boolean _is_static_expression() { return TRUE; }
protected:

private:
  IIR_Int32 _stmt_node_index;

};
#endif

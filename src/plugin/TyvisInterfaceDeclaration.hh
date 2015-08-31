#ifndef TYVIS_INTERFACEDECLARATION_HH
#define TYVIS_INTERFACEDECLARATION_HH

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
#include "savant/IIRBase_InterfaceDeclaration.hh"

class TyvisInterfaceDeclaration : public virtual TyvisObjectDeclaration,
				  public virtual IIRBase_InterfaceDeclaration {
public:
  void _add_declarations_in_initializations(PublishData *declarations);
  void _add_decl_into_cgen_symbol_table(PublishData *declarations);

  virtual void _publish_cc_decl_subprogram_arg( published_file &_cc_out,
                                                PublishData *declarations);
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_universal_type( published_file &_cc_out );
  void _publish_cc_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_subprogram_arg_var( published_file &_cc_out, PublishData *declarations );

  IIR_Int32 get_num_indexes();

  virtual IIR_Mode _get_mode() { return get_mode(); }

  // Helper Functions
  Tyvis * _get_value();

  string _get_cc_argument_name();

protected:

private:

};
#endif

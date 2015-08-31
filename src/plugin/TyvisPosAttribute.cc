
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

#include "TyvisDesignFile.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisPosAttribute.hh"
#include "savant/StandardPackage.hh"

const string &
TyvisPosAttribute::_get_cc_attribute_name() const {
  static const string retval("pos");
  return retval;
}

TyvisDeclaration *
TyvisPosAttribute::_build_implicit_declaration( TyvisTextLiteral *decl_name,
						TyvisTypeDefinition *decl_type ){
  TyvisTypeDefinition *integer_type = 
    dynamic_cast<TyvisTypeDefinition*>(get_design_file()->get_standard_package()->get_savant_universal_integer());
  return _build_function_declaration( decl_name, decl_type, integer_type );
}


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
#include "TyvisFloatingPointLiteral.hh"
#include "TyvisFloatingSubtypeDefinition.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "savant/StandardPackage.hh"
#include "savant/error_func.hh"
#include "published_file.hh"
#include <sstream>

void 
TyvisFloatingTypeDefinition::_publish_cc_universal_type( published_file &_cc_out ) {
  _cc_out << "UniversalReal";
}

const string
TyvisFloatingTypeDefinition::_get_cc_tyvis_type(){
  return "RealTypeInfo";
}

void 
TyvisFloatingTypeDefinition::_publish_cc_data_members( published_file & ) {}

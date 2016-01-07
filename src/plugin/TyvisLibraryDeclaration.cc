
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

#include "TyvisConfigurationDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisLibraryUnit.hh"
#include "TyvisLibraryUnitList.hh"
#include "TyvisName.hh"
#include "TyvisPackageDeclaration.hh"

#include <FileManager.h>
#include <sstream>
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/StandardPackage.hh"
#include "savant/library_manager.hh"

TyvisLibraryDeclaration::TyvisLibraryDeclaration() {
  set_primary_units(new TyvisLibraryUnitList());
}

TyvisLibraryDeclaration::~TyvisLibraryDeclaration(){}

void 
TyvisLibraryDeclaration::_publish_cc_include_path( ostream &os ){
  os << get_path_to_directory();
}

void 
TyvisLibraryDeclaration::_publish_cc_library_file_name( ostream &os ){
  // We don't need to generate anything for the standard declaration.
  // Also, we don't want to generate anything if we're emitting the
  // Makefile for this library - i.e. we can't link _this_ library's .a
  // file since it doesn't exist until after this method runs.
  if( IIRBase_Identifier::cmp(get_declarator(), "std") != 0 ){
    os << get_path_to_directory() << FileManager::instance()->getDirectorySeparator()
       << *_get_declarator();
    os << library_manager::get_library_suffix() << ".la";
  }
}

string
TyvisLibraryDeclaration::_mangle_declarator(){
  ostringstream mangledDeclarator;
  mangledDeclarator << *(_get_declarator());
  return mangledDeclarator.str();
}

TyvisTextLiteral *
TyvisLibraryDeclaration::_get_declarator() {
  return dynamic_cast<TyvisTextLiteral *>(get_declarator());
}

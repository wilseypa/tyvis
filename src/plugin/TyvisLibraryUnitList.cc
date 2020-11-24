
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
#include "TyvisLibraryUnitList.hh"
#include "TyvisLibraryUnit.hh"
#include "savant/library_manager.hh"
#include <savant/IIR_TextLiteral.hh>

void 
TyvisLibraryUnitList::_publish_cc(){
  TyvisLibraryUnit *lib_unit = dynamic_cast<TyvisLibraryUnit *>(first());

  while (lib_unit != NULL) {
    lib_unit->_publish_cc( );
    lib_unit = dynamic_cast<TyvisLibraryUnit *>(successor(lib_unit));
  }
}

void
TyvisLibraryUnitList::_publish_cc_main( published_file& file_writer ) {
  TyvisLibraryUnit *lib_unit = dynamic_cast<TyvisLibraryUnit *>(first());

  while (lib_unit != NULL) {
    lib_unit->_publish_cc_main( file_writer );
    lib_unit = dynamic_cast<TyvisLibraryUnit *>(successor(lib_unit));
  }
}

void
TyvisLibraryUnitList::_publish_cc_elaborate(){
  TyvisLibraryUnit *unit = dynamic_cast<TyvisLibraryUnit *>(first());

  while (unit != NULL) {
    unit->_publish_cc_elaborate();
    unit = dynamic_cast<TyvisLibraryUnit *>(successor(unit));
  }  
}

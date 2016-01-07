
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
#include "TyvisDesignatorByAll.hh"
#include "TyvisList.hh"
#include "TyvisDeclaration.hh"
#include "TyvisAccessTypeDefinition.hh"
#include "savant/set.hh"
#include "published_file.hh"

void 
TyvisDesignatorByAll::_publish_cc_lvalue( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out , "TyvisDesignatorByAll::_publish_cc_lvalue" );
  _cc_out << "all()";
}

void 
TyvisDesignatorByAll::_publish_cc_designator( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out , "TyvisDesignatorByAll::_publish_cc_designator" );
  _cc_out << "all";
}

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
#include "TyvisFunctionCall.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisSimpleName.hh"
#include "TyvisSubprogramDeclaration.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>

void 
TyvisSimpleName::_publish_cc_lvalue( published_file &, PublishData * ){
  cerr << "TyvisSimpleName::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) called !! ";
  abort();
}

void
TyvisSimpleName::_publish_cc_wait_data( published_file &_cc_out ) {
  _cc_out << "s->";
  _cc_out << (dynamic_cast<TyvisIdentifier*>(get_prefix()));
}

void 
TyvisSimpleName::_publish_cc_data( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_data( _cc_out, declarations );
}

void 
TyvisSimpleName::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {
  _get_prefix()->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisSimpleName::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  _get_prefix()->_publish_cc_elaborate( _cc_out, declarations );
}

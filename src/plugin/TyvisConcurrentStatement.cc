
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

#include "TyvisDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisConcurrentStatement.hh"
#include "TyvisLabel.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "published_file.hh"

void
TyvisConcurrentStatement::_publish_cc_connect_call_generate( published_file &_cc_out, PublishData * ){
  _get_label()->_publish_cc_elaborate( _cc_out.get_stream() );
  _cc_out << "_elab_obj->connect(0, 0);\n"; 
}

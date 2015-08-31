
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

#include "TyvisArraySubtypeDefinition.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisFileTypeDefinition.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisConstantInterfaceDeclaration.hh"
#include "TyvisFloatingSubtypeDefinition.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisProcedureDeclaration.hh"

#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>
#include "savant/StandardPackage.hh"

void
TyvisFileTypeDefinition::_publish_cc_headers( published_file &_cc_out ) {
  _get_type_mark()->_publish_cc_include( _cc_out );
  _publish_cc_include( _cc_out, "tyvis/FileTypeInfo.hh" );
  _publish_cc_include( _cc_out, "tyvis/VHDLProcess.hh" );
}

const string
TyvisFileTypeDefinition::_get_cc_tyvis_type(){
  return "FileTypeInfo";
}

void
TyvisFileTypeDefinition::_publish_cc_data_members( published_file & ) {
  // Do nothing
}

TyvisTypeDefinition *
TyvisFileTypeDefinition::_get_type_mark() {
  return dynamic_cast<TyvisTypeDefinition *>(get_type_mark());
}

void 
TyvisFileTypeDefinition::_publish_cc_type_info_args( published_file &_cc_out,
						     PublishData *declarations,
						     const string & ){
  CC_REF( _cc_out, "TyvisFileTypeDefinition::_publish_cc_type_info_args");
  _get_type_mark()->_publish_cc_type_info_xxx( _cc_out, declarations );
}

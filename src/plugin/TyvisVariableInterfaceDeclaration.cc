
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

#include "TyvisAttributeSpecification.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisIdentifier.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisVariableInterfaceDeclaration.hh"

#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include "PublishData.hh"
#include <sstream>
using std::ostringstream;

void 
TyvisVariableInterfaceDeclaration::_publish_cc_lvalue( published_file &(_cc_out), PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisVariableInterfaceDeclaration::_publish_cc" );
  
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }

  Tyvis::_publish_cc_prefix_string( _cc_out );
  _cc_out << *_get_mangled_declarator();
}

void 
TyvisVariableInterfaceDeclaration::_publish_cc_init( published_file &_cc_out,
						     PublishData * ){
  CC_REF( _cc_out, "TyvisVariableInterfaceDeclaration::_publish_cc_init" );  
}

void 
TyvisVariableInterfaceDeclaration::_publish_cc_data( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisVariableInterfaceDeclaration::_publish_cc_data" );
  
  _cc_out << "getState()->"
          << *_get_mangled_declarator();
}

void 
TyvisVariableInterfaceDeclaration::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisVariableInterfaceDeclaration::_publish_cc_wait_data" );
  
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  _cc_out << "s->"
          << *_get_mangled_declarator();
}

const string
TyvisVariableInterfaceDeclaration::_get_cc_object_type(){
  return "Variable";
}

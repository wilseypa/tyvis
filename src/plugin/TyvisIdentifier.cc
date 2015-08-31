
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
#include "TyvisIdentifier.hh"
#include "TyvisName.hh"

#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include "PublishData.hh"

void 
TyvisIdentifier::_publish_cc_data( published_file &, PublishData * ) {
  //  savant::set<TyvisDeclaration>          *decl_set;
//   TyvisDeclaration                       *decl = NULL;
  
//   ASSERT(declarations != NULL);
  
  ASSERT(0);
  //  decl_set = declarations->find_set( this );
//   switch (decl_set->size()) {
//   case 0:
//     // not in cgen table, find it in global table
//     ASSERT(0);  // This might or might not need to be here
//     decl_set = _get_symbol_table()->find_set( this )->convert_set<TyvisDeclaration>();
//     if (decl_set->size() == 1) {
//       decl = decl_set->getElement();
//     }
//     else {
//       cout << "TyvisIdentifier::_publish_cc_data: I can't resolve myself!"
//   	   << endl;
//       abort();
//     }
//     if (!declarations->in_collection(decl)) {
//       declarations->add_declaration(decl);
//     }
//     break;
//   case 1:
//     decl = decl_set->getElement();
//     break;
//   default:
//     cout << "TyvisIdentifier::_publish_cc_data: symbol multiply defined"
//   	 << endl;
//     abort();
//   }
//   _cc_out << "currentState->" << this;
//   if (decl->get_kind() == IIR_SIGNAL_DECLARATION ||
//       decl->get_kind() == IIR_SIGNAL_INTERFACE_DECLARATION ||
//       decl->get_kind() == IIR_SIMPLE_NAME) {
//   }
//   else if (decl->get_kind() == IIR_VARIABLE_DECLARATION ||
// 	   decl->get_kind() == IIR_VARIABLE_INTERFACE_DECLARATION) {
//   }
}

void 
TyvisIdentifier::_publish_cc_bounds( published_file &, PublishData * ) {
  ASSERT(0);
//   savant::set<TyvisDeclaration>          *decl_set;
//   TyvisDeclaration                       *decl = NULL;
//   ASSERT(declarations != NULL);

//   decl_set = declarations->find_set( this )->convert_set<TyvisDeclaration>();
//   switch (decl_set->size()) {
//   case 0:
//     // not in cgen table, find it in global table
//     ASSERT(0);  // This may or may not be needed
//     decl_set = _get_symbol_table()->find_set( this )->convert_set<TyvisDeclaration>();
//     if (decl_set->size() == 1) {
//       decl = decl_set->getElement();
//     }
//     else {
//       cout << "TyvisIdentifier::_publish_cc_bounds: I can't resolve myself!"
//            << endl;
//       abort();
//     }
//     if (!declarations->in_scope(decl)) {
//       declarations->add_declaration(decl);
//     }
//     break;
//   case 1:
//     decl = decl_set->getElement();
//     break;
//   default:
//     cout << "TyvisIdentifier::_publish_cc_bounds: symbol multiply defined"
//   	 << endl;
//     abort();
//   }
//   decl->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisIdentifier::_publish_cc_object_name( published_file &_cc_out, PublishData * ){
  _cc_out << *this;
}

void
TyvisIdentifier::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  _publish_cc_object_name( _cc_out, declarations );
}


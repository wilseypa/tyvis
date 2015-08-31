// Copyright (c) 2003-2004 The University of Cincinnati.  
// All rights reserved.
// 
// UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
// LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
// 
// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.
// 
// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.
// 
// Authors: Philip A. Wilsey	philip.wilsey@ieee.org
// 	    Dale E. Martin	dmartin@cliftonlabs.com
// 
// --------------------------------------------------------------------------

#include <algorithm>

#include "PublishData.hh"
#include "savant/IIRBase_TextLiteral.hh"

using std::find;

PublishData::PublishData() {
  //  We already know how large it's going to get, so reserve memory
  // in one chunk to enhance performance.
  collection.reserve(IIR_Declaration::LAST_DECLARATION_TYPE);

  // Add the sets
  for (unsigned i = 0; i < IIR_Declaration::LAST_DECLARATION_TYPE; i++){
    collection.push_back( new std::set<IIR_Declaration *> );
  }
}

PublishData::~PublishData() {
  // Erase the sets we allocated
  for (vector<set<IIR_Declaration *> *>::iterator i = collection.begin(); i != collection.end(); i++) {
    delete *i;
  }
}

set<IIR_Declaration*> *
PublishData::get_set( IIR_Declaration::declaration_type type ){
  return collection[type];
}

bool
PublishData::in_collection( IIR_Declaration *to_find ) {
  set<IIR_Declaration*> *decl_set = collection[to_find->get_declaration_type()];
  
  set<IIR_Declaration*>::iterator result = find(decl_set->begin(), decl_set->end(), to_find);

  if (result == decl_set->end())
    return false;
  else
    return true;
}

void
PublishData::add_declaration( IIR_Declaration *new_decl ) {
  collection[new_decl->get_declaration_type()]->insert(new_decl);
}

set<IIR_Declaration*> *
PublishData::find_set( IIR_TextLiteral *name ) {
  set<IIR_Declaration*> *result = new set<IIR_Declaration*>;

  for (vector<set<IIR_Declaration *> *>::iterator cur_set = collection.begin(); cur_set != collection.end(); cur_set++) {
    set<IIR_Declaration*> *decl_set = *cur_set;
    for (set<IIR_Declaration*>::iterator decl = decl_set->begin(); decl != decl_set->end(); decl++) {
      if (IIRBase_TextLiteral::cmp((*decl)->get_declarator(), name) == 0)
        result->insert(*decl);
    }
  }

  return result;
}

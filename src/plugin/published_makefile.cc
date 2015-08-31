// Copyright (c) 2001 Clifton Labs, Inc.
// All rights reserved. 

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING,
// MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.


// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.

// Author: Dale E. Martin          dmartin@cliftonlabs.com

//---------------------------------------------------------------------------

#include "published_makefile.hh"
#include "TyvisLibraryClause.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisTextLiteral.hh"

#include <sstream>
using std::ostringstream;
#include <iostream>
using std::endl;


published_makefile::published_makefile( const string &directory,
					const string &name_base,
					IIR *published_node ) :
  published_file( directory, name_base ),
  my_libraries( new savant::set<TyvisDeclaration> ){ 
  publish_standard_header( published_node );
}

published_makefile::~published_makefile(){
  file_close();
  delete my_libraries;
}

void 
published_makefile::publish_headers( ofstream & ){}

void 
published_makefile::publish_footers( ofstream & ){}


void 
published_makefile::insert_comment( const string &comment_text ){
  get_stream() << "# " << comment_text << std::endl;
}

void
published_makefile::add_library( TyvisDeclaration *library_to_add ){
  ASSERT( library_to_add != 0 );
  my_libraries->add( library_to_add );
}
  

void
published_makefile::add_libraries( TyvisDeclarationList *libraries_to_add ){
  IIR_Declaration *current_context_item =
    dynamic_cast<TyvisDeclaration *>(libraries_to_add->first());
  while( current_context_item != 0 ){
    add_library( dynamic_cast<TyvisDeclaration *>(current_context_item) );
    current_context_item = 
      dynamic_cast<IIR_Declaration *>(libraries_to_add->successor(current_context_item));
  }
  publish_includes_and_libraries();
}

string
published_makefile::get_libraries(){
  ostringstream ldflags_str;
  IIR_Declaration *current_context_item = my_libraries->getElement();
  while( current_context_item != 0 ){
    TyvisDeclaration *current = dynamic_cast<TyvisDeclaration *>(current_context_item);
    if( current && !current->is_work_library() ){
      ldflags_str << " ";
      current->_publish_cc_library_file_name( ldflags_str );
    }
    current_context_item = my_libraries->getNextElement();
  }
  return ldflags_str.str();
}


string
published_makefile::get_includes(){
  ostringstream include_path_str;
  IIR_Declaration *current_context_item = my_libraries->getElement();
  while( current_context_item != 0 ){
    if( dynamic_cast<TyvisLibraryClause *>(current_context_item) || dynamic_cast<TyvisLibraryDeclaration *>(current_context_item)){
      include_path_str << " -I";
      dynamic_cast<TyvisDeclaration *>
	(current_context_item)->_publish_cc_include_path( include_path_str );
    }
    current_context_item = my_libraries->getNextElement();
  }
  return include_path_str.str();
}

void
published_makefile::publish_includes_and_libraries(){
  string include_path = get_includes();
  if( include_path != "" ){
    get_stream() << "LIBRARY_INCLUDES+= " << include_path << "\n\n";
  }
  string ldflags = get_libraries();
  if( ldflags != "" ){
    get_stream() << "LINK_THESE_LIBRARIES += " << ldflags << "\n" << endl;
  }
}

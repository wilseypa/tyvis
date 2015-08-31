#ifndef PUBLISHED_MAKEFILE_HH
#define PUBLISHED_MAKEFILE_HH

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

#include "published_file.hh"
#include <savant/set.hh>

class TyvisDeclaration;
class TyvisDeclarationList;

class published_makefile : public published_file {
public:
  published_makefile( const string &publish_dir,
		      const string &name_base,
		      IIR *published_node );

  ~published_makefile();

  void insert_comment( const string &comment_text );

  //@{
  /** Methods for adding library information to the makefile.  This will
      automatically generate and maintain the -I and -ls necessary.
  */
  void add_library( TyvisDeclaration *library_to_add );
  void add_libraries( TyvisDeclarationList *libraries_to_add );
  //@}

protected:
  virtual void publish_headers( ofstream & );
  virtual void publish_footers( ofstream & );

private:
  void publish_includes_and_libraries();
  string get_libraries();
  string get_includes();

  savant::set<TyvisDeclaration> *my_libraries;
};

#endif

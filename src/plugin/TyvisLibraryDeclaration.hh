#ifndef TYVIS_LIBRARYDECLARATION_HH
#define TYVIS_LIBRARYDECLARATION_HH

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
#include "savant/IIRBase_LibraryDeclaration.hh"

class TyvisLibraryDeclaration : public virtual TyvisDeclaration,
				public virtual IIRBase_LibraryDeclaration {
public:
  TyvisLibraryDeclaration();
  ~TyvisLibraryDeclaration();

  /** Publishes the path into the stream that a C++ compiler needs to
      include in order to get the header files for this library. */
  void _publish_cc_include_path( ostream &os );

  /** Publishes the file name where that should be the result of code
      generation/compilation of this library. Doesn't publish anything
      for the standard library. */
  void _publish_cc_library_file_name( ostream &os );
  string _mangle_declarator();

  // Helper Functions
  TyvisTextLiteral *        _get_declarator();
protected:

private:
};
#endif

#ifndef TYVIS_USECLAUSE_HH
#define TYVIS_USECLAUSE_HH

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
#include "savant/IIRBase_UseClause.hh"

class TyvisName;

class TyvisUseClause : public virtual TyvisDeclaration, public virtual IIRBase_UseClause {
public:
  void _publish_cc_package_name( published_file &_cc_out );
  void _publish_cc_headers( published_file &_cc_out );

  void _publish_cc_include( published_file &_cc_out );

  //@{
  /** These are defined in the TyvisLibraryClause, but these nodes appear
      in context_lists so we need to define them to avoid runtime
      errors. */
  void _publish_cc_include_path( ostream & ){}
  void _publish_cc_library_file_name( ostream & ){}
  //@}

  TyvisDeclaration       *_get_package_declaration();
  TyvisName              *_get_selected_name();
protected:

private:

};
#endif

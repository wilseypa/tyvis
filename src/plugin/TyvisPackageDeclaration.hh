#ifndef TYVIS_PACKAGEDECLARATION_HH
#define TYVIS_PACKAGEDECLARATION_HH

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

#include "TyvisLibraryUnit.hh"
#include "savant/IIRBase_PackageDeclaration.hh"

class TyvisPackageDeclaration : public virtual TyvisLibraryUnit,
				public virtual IIRBase_PackageDeclaration {
public:
  TyvisPackageDeclaration();
  ~TyvisPackageDeclaration();

  void _add_decl_into_declaration_collection(PublishData *){}

  void _publish_cc();
  void _publish_cc_elaborate(){}
  void _publish_cc_header( PublishData *declarations );
  void _publish_cc_cc_file( PublishData *declarations );

  using TyvisDeclaration::_publish_cc_include;
  void _publish_cc_include( published_file &_cc_out );
  void _publish_cc_include_decls_prefix( ostream &os );

  /** Not needed for packages.  */
  void _publish_cc_include_decls( published_file & ){} 

  /** No elaboration is needed for package declarations.  */  
  void _publish_cc_include_elab( published_file & ){}
  void _publish_cc_headers( published_file &_cc_out );

  void _publish_cc_binding_name( ostream &os );

  // Helper Functions
  TyvisDeclaration               *_get_package_declaration();
  TyvisDeclarationList           *_get_package_declarative_part();
  TyvisDeclaration               *_get_prefix_declaration();
protected:

private:

};
#endif

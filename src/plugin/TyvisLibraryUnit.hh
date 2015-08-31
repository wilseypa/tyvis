#ifndef TYVIS_LIBRARYUNIT_HH
#define TYVIS_LIBRARYUNIT_HH

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
#include "savant/IIRBase_LibraryUnit.hh"

class TyvisLibraryUnit : public virtual TyvisDeclaration,
			 public virtual IIRBase_LibraryUnit {
public:
  TyvisLibraryUnit();
  ~TyvisLibraryUnit();

  /**
     Every design unit emits a bit of makefile that the master Makefile can
     pick up.
  */
  virtual void _publish_cc_makefile_stub();

  /** This method builds the main part of a file name for a "_decls"
      file.   */
  virtual void _publish_cc_include_decls_prefix( ostream &os ) = 0;

  /** The following function is used for code generation that does runtime
      elaboration.  This version is used for nodes that generate new
      files. */
  virtual void _publish_cc_elaborate();

  void _publish_cc_include( published_file &_cc_out );

  /** Publish an include that is sufficient for another file to include
      this unit's declarations in the generated code.  There is a
      default implementation here that can be explicitly called, even
      though this method is pure virtual. */
  virtual void _publish_cc_include_decls( published_file &_cc_out );

  /** Publish an include that is sufficient for another file to include
      this unit's elaboration headers in the generated code. There is a
      default implementation here that can be explicitly called, even
      though this method is pure virtual. */
  virtual void _publish_cc_include_elab( published_file &_cc_out );

  /**
     Library units generally publish their own files, and this is an entry
     point for the library unit list to call those functions.
  */
  virtual void _publish_cc( ) = 0;

  /**
     Returns the library declaration that this unit is declared in.  Should
     never be NULL.  
  */
  TyvisLibraryDeclaration *_get_library();

  /**
     Returns the directory that this unit's library directory corresponds
     to.  Should never be NULL.
  */
  const string _get_library_directory();


  TyvisAttributeSpecificationList*       _get_attribute_specification_list();
  virtual TyvisEntityDeclaration*        _get_entity();
  TyvisDeclarationList *                 _get_context_items();
protected:

  PublishData *_declarations;
private:
  /**
     A helper for the other _publish_cc_include methods.
   */
  virtual void _publish_cc_include( published_file &_cc_out, const char * const );

};
#endif

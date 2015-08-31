#ifndef TYVIS_CONFIGURATIONDECLARATION_HH
#define TYVIS_CONFIGURATIONDECLARATION_HH

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
#include "savant/IIRBase_ConfigurationDeclaration.hh"

class TyvisArchitectureDeclaration;
class TyvisBlockConfiguration;

class TyvisConfigurationDeclaration : public virtual TyvisLibraryUnit, public virtual IIRBase_ConfigurationDeclaration {
public:  
  TyvisConfigurationDeclaration();
  ~TyvisConfigurationDeclaration();

  void _publish_cc_elaborate();

  void _publish_cc_headerfile(PublishData *declarations);
  void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_class_includes( published_file &_cc_out );
  void _publish_cc_class( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_object_pointers( published_file &_cc_out );

  void _publish_cc_ccfile(PublishData *declarations);
  void _publish_cc_headerfiles_for_cc( published_file & ){}
  void _publish_cc_constructor( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constructor_with_arguments( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_destructor( published_file &_cc_out );
  void _publish_cc_instantiate( published_file &_cc_out );
  void _publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_connect( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_getboundentityinfo( published_file &_cc_out );

  /**
     Dummy function
  */
  void _publish_cc(){}
  void _publish_cc_binding_name( ostream &os );
  void _publish_cc_include_decls_prefix( ostream &os );

  // Helper Functions
  TyvisArchitectureDeclaration   *_get_configured_architecture();
  TyvisDeclarationList           *_get_configuration_declarative_part();
  TyvisBlockConfiguration        *_get_block_configuration();
  TyvisEntityDeclaration         *_get_entity();
protected:

private:

};
#endif

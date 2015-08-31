#ifndef TYVIS_RECORDTYPEDEFINITION_HH
#define TYVIS_RECORDTYPEDEFINITION_HH

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

#include "TyvisTypeDefinition.hh"
#include "savant/IIRBase_RecordTypeDefinition.hh"

class TyvisRecordTypeDefinition : public virtual TyvisTypeDefinition, public virtual IIRBase_RecordTypeDefinition {
public:
  TyvisRecordTypeDefinition();
  ~TyvisRecordTypeDefinition();

  //The following methods needed for publishing 
  //the record types .hh and .cc files
  virtual const string _get_cc_tyvis_type();
  virtual void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_decl_type_attributes( published_file &_cc_out );
  void _publish_cc_necessary_decl_in_state( published_file &_cc_out );

  void _publish_cc_universal_type( published_file &_cc_out );

  void _publish_cc_element_objtypes( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_element_objtypes_event( published_file &_cc_out, PublishData *declarations );

  /**
     Get the name of the method that will return the list of elements for
     this type.
  */
  static string _get_cc_elements_method( const string &type_name );
  void _publish_cc_type_info_prereq( published_file &_cc_out, PublishData *declarations, const string &type_name );
  void _publish_cc_type_info_args( published_file &_cc_out, PublishData *declarations, const string &type_name );

  int _get_field_number(Tyvis *searchField);

  virtual IIR_Boolean _has_access_type();

  IIR_Boolean _is_globally_static();

  TyvisTypeDefinition *_get_element_subtype(int);

  TyvisScalarTypeDefinition *_get_index_subtype();
  TyvisTypeDefinition *_get_new_subtype();

  // Helper Functions
  TyvisElementDeclarationList    *_get_element_declarations();  
  TyvisFunctionDeclaration       *_get_resolution_function();
protected:

private:

};
#endif

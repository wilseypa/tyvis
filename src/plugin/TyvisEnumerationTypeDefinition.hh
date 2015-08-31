#ifndef TYVIS_ENUMERATIONTYPEDEFINITION_HH
#define TYVIS_ENUMERATIONTYPEDEFINITION_HH

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

#include "TyvisScalarTypeDefinition.hh"
#include "savant/IIRBase_EnumerationTypeDefinition.hh"

class TyvisEnumerationLiteralList;

class TyvisEnumerationTypeDefinition : public virtual TyvisScalarTypeDefinition, public virtual IIRBase_EnumerationTypeDefinition {
public:
  TyvisEnumerationTypeDefinition();
  ~TyvisEnumerationTypeDefinition();

  void _publish_cc_universal_type( published_file &_cc_out );
  const string _get_cc_tyvis_type();
  
  virtual void _publish_cc_range( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_temporary_type_info( published_file &_cc_out,
					PublishData *declarations, char* = NULL, char* = NULL );
  void _publish_cc_type_info_prereq( published_file &_cc_out,
				     PublishData *declarations,
				     const string &type_name );

  void _publish_cc_type_info_args( published_file &_cc_out, 
				   PublishData *declarations,
				   const string &type_name );

  // Helper Functions
  TyvisEnumerationLiteralList *     _get_enumeration_literals();
protected:

private:
  void _publish_cc_literals_method( published_file &_cc_out,
				    const string &type_name );
};
#endif

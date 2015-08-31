#ifndef TYVIS_ACCESSTYPEDEFINITION_HH
#define TYVIS_ACCESSTYPEDEFINITION_HH

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
#include "savant/IIRBase_AccessTypeDefinition.hh"

class TyvisAccessTypeDefinition : public virtual TyvisTypeDefinition, public virtual IIRBase_AccessTypeDefinition {
public:
  // c++ code generation methods
  void _add_decl_into_declaration_collection(PublishData *declarations);
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  virtual const string _get_cc_tyvis_type();
  void _publish_cc_universal_type( published_file &_cc_out );

  virtual void _publish_cc_headers( published_file &_cc_out );

  void _publish_cc_function_all( published_file &_cc_out );

  /** The two functions have to be dummy for Access Types. */
  virtual void _publish_cc_class_last_event( published_file & ){}
  virtual void _publish_cc_class_event( published_file & ){}

  void _publish_cc_static_type_info_fn( published_file &, PublishData * ) {}

  void _publish_cc_type_info_args( published_file &_cc_out,
				   PublishData *,
				   const string & );

  TyvisTypeDefinition* _get_designated_type();

  IIR_Int32 get_num_indexes();

  virtual IIR_Boolean _has_access_type() { return TRUE; }
  
protected:

private:

};
#endif

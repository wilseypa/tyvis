#ifndef TYVIS_DECLARATIONLIST_HH
#define TYVIS_DECLARATIONLIST_HH

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

#include "TyvisList.hh"
#include "savant/IIRBase_DeclarationList.hh"

class TyvisAttributeSpecification;

class TyvisDeclarationList : public virtual TyvisList,
			     public virtual IIRBase_DeclarationList {
public:
  virtual void _publish_cc( published_file &_cc_out, PublishData *declarations  );
  void _publish_cc_decl( published_file &(_cc_out), PublishData *declarations );
  void _publish_cc_package_declarations( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_package_definitions( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_decl_subprograms_and_types( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_prototype( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_constant_declarations( published_file &_cc_out, PublishData *declarations );
  IIR_Boolean  _publish_cc_constant_definitions( published_file &_cc_out, PublishData *declarations, IIR_Boolean = FALSE );  

  /** This method walks through the list of declarations and publishes
      initializations for constants declared here. This method is called
      from process statement only as of now. */
  void _publish_cc_constants( published_file &_cc_out, PublishData *declarations );
  IIR_Boolean _publish_cc_constants_init( published_file &_cc_out, PublishData *declarations, IIR_Boolean first = TRUE );
  void _publish_cc_global_constants_assignments( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_file_objects( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_file_objects_init( published_file &_cc_out, PublishData *declarations );

  /** Returns true if this list contains a foreign attribute specification,
      false otherwise. */
  bool _contains_foreign_attribute();

  /**
     Returns the set of foreign attribute specifications.  Must delete
     return memory.
  */
   savant::set<TyvisAttributeSpecification> *_find_foreign_attributes();
  
protected:

private:

};
#endif

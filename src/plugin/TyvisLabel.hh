#ifndef TYVIS_LABEL_HH
#define TYVIS_LABEL_HH

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
#include "savant/IIRBase_Label.hh"

class TyvisLabel : public virtual TyvisDeclaration, public virtual IIRBase_Label {
public:
  TyvisLabel();
  ~TyvisLabel();

  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( ostream &_cc_out );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_binding_name( ostream& outstream );
  void _publish_cc_include( published_file &_cc_out );

  IIR_Boolean _is_block_statement();
  IIR_Boolean _is_concurrent_generate_statement();

  // Helper Functions
  TyvisAttributeSpecificationList        *_get_attribute_specification_list();
  TyvisStatement                         *_get_statement();
  TyvisList                              *_get_statement_list();
  TyvisAttributeSpecificationList        *_get_attributes();
protected:

private:

};
#endif

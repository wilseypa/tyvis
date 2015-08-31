#ifndef TYVIS_ATTRIBUTEDECLARATION_HH
#define TYVIS_ATTRIBUTEDECLARATION_HH

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
#include "savant/IIRBase_AttributeDeclaration.hh"

class TyvisAttributeDeclaration : public virtual TyvisDeclaration, public virtual IIRBase_AttributeDeclaration {
public:
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file &_cc_out );

  /** Since an attribute declaration doesn't generate a file, there's no
     include we need to get ahold of the declaration.  Of course, if I'm
     wrong, we would call "_publish_cc_include" on our declarative
     region...  */
  void _publish_cc_include( published_file & ){}
  void _add_decl_into_declaration_collection(PublishData *declarations);

  // Helper Functions
  TyvisDeclaration* _get_prefix_declaration();
  TyvisAttribute *_get_attribute();
protected:

private:

};
#endif

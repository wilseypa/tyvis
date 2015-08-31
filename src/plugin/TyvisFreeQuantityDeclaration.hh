#ifndef TYVIS_FREEQUANTITYDECLARATION_HH
#define TYVIS_FREEQUANTITYDECLARATION_HH

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

#include "TyvisQuantityDeclaration.hh"
#include "savant/IIRBase_FreeQuantityDeclaration.hh"
#include <vector>

using std::vector;

class TyvisFreeQuantityDeclaration : public virtual TyvisQuantityDeclaration, public virtual IIRBase_FreeQuantityDeclaration {
public:
  TyvisFreeQuantityDeclaration();

  void _publish_cc_lvalue(published_file &, PublishData *declarations);
  void _publish_cc_decl( published_file &, PublishData *declarations );
  const string _get_cc_object_type();
  void _publish_cc_ams_function(published_file &, PublishData *declarations);
  void _publish_cc_init(published_file &, PublishData *);
  void _publish_cc_init_quantity(published_file &, PublishData *declarations);
  void _publish_cc_tolerances(published_file &);

  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);
  void _set_stmt_qty_index(IIR_Int32 *,  savant::set<IIR_Declaration> *, PublishData *, PublishData *);
  void _flush_stmt_index();

  IIR_Int32 _get_stmt_qty_index();

  // Helper Functions
  Tyvis *_get_value();
protected:
  double ATOL;
  double RTOL;

private:
  IIR_Int32 _stmt_qty_index;
  vector <IIR_Int32> _stmt_node_index;
  IIR_Int32 _qty_occurence_index;

};
#endif

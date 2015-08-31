#ifndef TYVIS_BRANCHQUANTITYDECLARATION_HH
#define TYVIS_BRANCHQUANTITYDECLARATION_HH

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
#include "savant/IIRBase_BranchQuantityDeclaration.hh"
#include <vector>

using std::vector;

class TyvisName;

class TyvisBranchQuantityDeclaration : public virtual TyvisQuantityDeclaration, public virtual IIRBase_BranchQuantityDeclaration {
public:
  TyvisBranchQuantityDeclaration();
  ~TyvisBranchQuantityDeclaration();

  void _publish_cc_lvalue(published_file &, PublishData *);
  void _publish_cc_ams_function(published_file &, PublishData *declarations);
  void _publish_cc_decl( published_file &, PublishData * );
  void _publish_cc_tolerances( published_file &); 

  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);
  void _set_stmt_qty_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *, PublishData *declarations, PublishData *arch_declarations);

  IIR_Int32 _get_stmt_qty_index();
  void _flush_stmt_index();

  // Helper Functions
  Tyvis * _get_across_aspect_expression();
  Tyvis * _get_through_aspect_expression();
  TyvisName* _get_plus_terminal_name();
  TyvisName* _get_minus_terminal_name();
protected:
  IIR_Int32 _stmt_qty_index;

  double RTOL;
  double ATOL;

private:
  void _publish_cc_init_quantity(ostream &);
  vector <IIR_Int32> _stmt_node_index;

};
#endif

#ifndef TYVIS_TYPECONVERSION_HH
#define TYVIS_TYPECONVERSION_HH

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

#include "TyvisExpression.hh"
#include "savant/IIRBase_TypeConversion.hh"

class TyvisTypeConversion : public virtual TyvisExpression, public virtual IIRBase_TypeConversion {
public:
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file &_cc_out );

  /** Publishes the 'pos attribute appearing in a simultaneous
      statement. */
  void _publish_cc_ams_function( published_file &_cc_out, PublishData *declarations );

  /** Sets the equation node values for the simultanesous statement. */
  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);

  void _get_list_of_input_signals(  savant::set<Tyvis> * );
  IIR_Boolean _is_globally_static_primary();
  void _build_sensitivity_list(TyvisDesignatorList* sensitivity_list);

  // Helper Functions
  TyvisTypeDefinition    *_get_type_mark();
  Tyvis                   *_get_expression();
protected:

private:
  IIR_Int32 _stmt_node_index;

};
#endif

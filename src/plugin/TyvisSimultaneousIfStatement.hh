#ifndef TYVIS_SIMULTANEOUSIFSTATEMENT_HH
#define TYVIS_SIMULTANEOUSIFSTATEMENT_HH

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

#include "savant/set.hh"
#include "savant/IIRBase_SimultaneousIfStatement.hh"
#include "TyvisSimultaneousStatement.hh"

class TyvisDeclaration;
class TyvisArchitectureStatementList;
class TyvisSimultaneousElsif;

class TyvisSimultaneousIfStatement : public virtual IIRBase_SimultaneousIfStatement, public virtual TyvisSimultaneousStatement {
public:  
  TyvisSimultaneousIfStatement();
  ~TyvisSimultaneousIfStatement();

  /** @name C++ publishing methods.
      These methods relate to the publishing of C++.*/
  void _publish_cc( published_file &, PublishData *declarations );

  /** Function that publishes the characteristic expression
      corresponding to the condition specified in the If Statement.
      @see TyvisSimpleSimultaneousStatement#_publish_cc_characteristic_expressions */
  void _publish_cc_characteristic_expressions(TyvisArchitectureStatement::SimultaneousIfPublishingPart,
                                              published_file &,
                                              PublishData *);

  /** Function that calls the Simultaneous Statement's 
      _publish_cc_ams_function_prototype for each of the 
      statements in the If part and the else part.*/
  void _publish_cc_ams_function_prototype(published_file &, PublishData *declarations);

  /** Function that calls the Simultaneous Statement's
      _publish_cc_ams_function_body for each of the
      statements in the If part and the else part.*/
  void _publish_cc_ams_function_body(published_file &, PublishData *, PublishData *);

  savant::set<TyvisDeclaration> _unique_qtys;
  savant::set<TyvisDeclaration> _unique_signals;
  savant::set<TyvisDeclaration> _unique_generic_constants;
 
  IIR_Int32 _stmt_qty_index;
  IIR_Int32 _stmt_signal_index;

  /** Returns TRUE if "this" is a simultaneous Statement. */ 
  IIR_Boolean _is_simultaneous_statement();

  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations ); 

  // Helper Functions
  Tyvis                             *_get_condition();
  TyvisArchitectureStatementList    *_get_then_statement_list();
  TyvisSimultaneousElsif            *_get_elsif();
  TyvisArchitectureStatementList    *_get_else_statement_list();
protected:

private:

};
#endif

#ifndef TYVIS_STATEMENT_HH
#define TYVIS_STATEMENT_HH

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

#include "Tyvis.hh"
#include "savant/IIRBase_Statement.hh"

class TyvisLabel;

class TyvisStatement : public virtual Tyvis, public virtual IIRBase_Statement {
public:
  TyvisStatement();
  ~TyvisStatement();

  /**
     Entry into publishing routines.
  */
  virtual void _publish_cc( published_file &_cc_out, PublishData *declarations ) = 0;

  /** All of the assertion-like statements use this method for
      publishing.  This includes assertion statements, report
      statements, and concurrent assertion statements. */
  void _publish_cc_assertion_or_report_statement( published_file &_cc_out, PublishData *declarations );

  /** By default this method publishes code for the expression of an
      assertion statement.  For report statements, it's overridden to print
      "true".  */
  virtual void _publish_cc_assertion_condition( published_file &_cc_out, PublishData *declarations );

  /** If this node returns something non-NULL from
      _get_assertion_condition(), this method will end the conditional that 
      _publish_cc_assertion_condition started. */
  void _publish_cc_assertion_condition_end( published_file &_cc_out );

  /** This method overloads the generic access method
      Tyvis::_get_mangled_declarator() to return an declarator associated with the
      Tyvis hierarchy derived from "this" class.  The declarator associated with a
      Statement is its lablel.  Hence, this method merely returns the mangled
      label associated with this class [obtained by a call to
      TyvisStatement::_get_label()->_get_mangled_declarator].

      @return A TyvisTextLiteral* that points to the mangled declarator
      associated with this class.  The caller must not modify or delete the
      pointer returned by a call to this method virtual
  */
  TyvisTextLiteral* _get_declarator();
  TyvisTextLiteral* _get_mangled_declarator();

  /** This method is defined only in TyvisStatement and is valid for all IIR
      hierarchies derived from TyvisStatement.  This method returns a pointer to
      the mangled label associated with this Tyvis tree.  Mangling of labels is
      "lazy" .ie. a mangled label is obtained (by a call to
      TyvisStatement::_mangle_label()) during the first call to this method (for
      each Tyvis tree) and is saved [in the _mangled_label class member object].
      The saved mangled-label is used in subsequent calls to this method.

      @return A TyvisLabel* that points to a mangled label associated with the
      Tyvis hierarchy derived from "this" node.  The caller must not modify or
      delete the pointer returned by a call to this method.
  */
  virtual TyvisLabel* _get_mangled_label() const;

  virtual void _mangle_label() const;
  virtual void _set_label(TyvisLabel*);

  /** Returns true if this is an TyvisAssertionStatement or an
     TyvisConcurrentAssertionStatement.
  */
  virtual IIR_Boolean _is_assert_statement(){ return false; }

  /**
     Returns the library unit that contains this statement.
  */
  TyvisLibraryUnit *_get_design_unit();

  /** Returns the design unit name that this statement is associated
      with. */
  const string _get_design_unit_name();

  TyvisLabel *_get_label() const;
  virtual Tyvis *_get_case_statement_expression();

  virtual Tyvis *_get_reject_time_expression();
  virtual Tyvis *_get_target();
  virtual Tyvis *_get_assertion_condition(){ return 0; }
  virtual Tyvis *_get_report_expression(){ return 0; }
  virtual Tyvis *_get_severity_expression(){ return 0; }
protected:

private:
  TyvisLabel *_mangled_label;

};
#endif

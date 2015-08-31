#ifndef TYVIS_CONTRIBUTIONATTRIBUTE_HH
#define TYVIS_CONTRIBUTIONATTRIBUTE_HH

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

#include "savant/IIRBase_ContributionAttribute.hh"
#include "TyvisAttribute.hh"

/** The extension base for contribution attribute of terminals
    (T'contribution).  This class describes T'Contribution. For a terminal
    T, the attribute T'Contribution yields a through quantity, whose plus
    terminal is T and whose minus terminal is the reference terminal of
    nature of T. */
class TyvisContributionAttribute : public virtual IIRBase_ContributionAttribute,
				   public virtual TyvisAttribute {
public:  
  /** @name C++ publishing methods.  These methods relate to the publishing
      of C++.*/
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );

 /** Uses the index assigned to each quantity and publishes the quantities
     present in the simultaneous statement. */
  void _publish_cc_ams_function(published_file &, PublishData *declarations);

  /** Adds the implicit quantity declaration to the symbol table
      cgen_arch_sym_tab */
  void _set_stmt_qty_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *, PublishData *, PublishData *);

  const string &_get_cc_attribute_name() const;

  /** Checks if a T'contribution is found in a Simultaneous Statement. If
      so, an implicit quantity has to be created. */
  IIR_Boolean _contribution_quantity_found();

  /** If one or more T'contribution is present in a Simultaneous Statement,
      a free quantity is implicitly created for each and all such
      quantities are appended to a list. */  
  void _build_contribution_quantity_list(dl_list<TyvisContributionAttribute>*);

  TyvisDeclaration *_build_implicit_declaration( TyvisTextLiteral * , TyvisTypeDefinition * );
protected:

private:

};
#endif

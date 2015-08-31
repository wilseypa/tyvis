#ifndef TYVIS_DOTATTRIBUTE_HH
#define TYVIS_DOTATTRIBUTE_HH

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

#include "savant/IIRBase_DotAttribute.hh"
#include "TyvisAttribute.hh"

/** The extension base for Dot attribute of quantities (Q'dot).  For a
    Quantity Q, the attribute Q'Dot yields a quantity of type as Q. */
class TyvisDotAttribute : public virtual IIRBase_DotAttribute, public virtual TyvisAttribute {
public:
  /** Uses the index assigned to each quantity and publishes the quantities
      present in the simultaneous statement. */
  void _publish_cc_ams_function(published_file &, PublishData *declarations);

  /** Sets an index to equation node in the equation. */
  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);

  /** Sets an index to each quantity in the equation. */
  void _set_stmt_qty_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *, PublishData *, PublishData *);

  /** Checks if a Q'dot is found in a Simultaneous Statement. If so, an
      implicit quantity has to be created.*/
  IIR_Boolean _differential_quantity_found();

  /** If one or more Q'dot is present in a Simultaneous Statement, a
      quantity of the same type as Q is implicitly created for each and all
      such quantities are appended to a list.*/
  void _build_differential_quantity_list(dl_list<TyvisDotAttribute>*);

  TyvisDeclaration *_build_implicit_declaration( TyvisTextLiteral * ,TyvisTypeDefinition * );
  const string &_get_cc_attribute_name() const;

protected:

private:

};
#endif

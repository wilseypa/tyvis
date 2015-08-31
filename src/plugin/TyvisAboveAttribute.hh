#ifndef TYVIS_ABOVEATTRIBUTE_HH
#define TYVIS_ABOVEATTRIBUTE_HH

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

#include "savant/IIRBase_AboveAttribute.hh"
#include "TyvisAttribute.hh"

class TyvisAboveAttribute : public virtual IIRBase_AboveAttribute,
			    public virtual TyvisAttribute {
public:
  TyvisAboveAttribute() {}
  ~TyvisAboveAttribute() {}

  /** @name C++ publishing methods.  These methods relate to the publishing
      of C++. */
  void _publish_cc_rvalue(published_file &, PublishData *);
  
  /** Uses the index assigned to each quantity and publishes the quantities
      present in the simultaneous statement. */
  void _publish_cc_ams_function(published_file &, PublishData *declarations);

  void _publish_cc_necessary_decl_in_state(published_file &, PublishData *);
  void _publish_cc_necessary_decl_init(published_file &, PublishData *);
  void _publish_cc_init_val(ostream &);
  void _publish_cc_name_in_caps(ostream &);
  void _publish_cc_attrib_type(ostream &);
  void _publish_cc_sigtype(ostream &);

  TyvisDeclaration *_build_implicit_declaration( TyvisTextLiteral *,
						 TyvisTypeDefinition * );

  const string &_get_cc_attribute_name() const;

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

  /** Sets an index to each signal in the equation. */
  void _set_stmt_signal_index(IIR_Int32 *,  savant::set<TyvisDeclaration>*, PublishData *declarations, PublishData *arch_declarations);
  IIR_Int32 _get_stmt_signal_index();
  void flush_stmt_index();
protected:

private:

};
#endif

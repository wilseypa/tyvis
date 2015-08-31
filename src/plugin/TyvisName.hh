#ifndef TYVIS_NAME_HH
#define TYVIS_NAME_HH

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
#include "savant/IIRBase_Name.hh"

class TyvisName : public virtual Tyvis, public virtual IIRBase_Name {
public:

  void _publish_cc_addChild( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_binding_name( ostream &outstream );
  void _publish_cc_type_info_xxx( published_file &_cc_out,
				  PublishData *declarations );

  virtual void _get_list_of_input_signals(  savant::set<Tyvis> *list );  
  void _get_signal_source_info(  savant::set<Tyvis> *siginfo );
  IIR_Boolean _is_longest_static_prefix();
  IIR_Boolean _is_static_expression();
  void _build_sensitivity_list(TyvisDesignatorList* sensitivity_list);

  Tyvis *_get_prefix();

  virtual Tyvis *_get_suffix() { return NULL; }

  IIR_Boolean _is_composite_resolved_signal();

  virtual TyvisTextLiteral *_get_mangled_declarator();
protected:

private:

};
#endif

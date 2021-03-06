#ifndef TYVIS_ASSOCIATIONELEMENT_HH
#define TYVIS_ASSOCIATIONELEMENT_HH

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

#include "TyvisTuple.hh"
#include "savant/IIRBase_AssociationElement.hh"

class TyvisAssociationElement : public virtual TyvisTuple, public virtual IIRBase_AssociationElement {
public:
  virtual void _publish_cc_subprogram_arguments( published_file &_cc_out );
  virtual void _publish_cc_generic_map_aspect( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_generic_map_aspect_for_conc_stmts( published_file &_cc_out, PublishData *declarations );
  
  virtual IIR_Boolean _is_globally_static_for_array();
  virtual IIR_Boolean _is_globally_static_for_record();
  virtual IIR_Boolean _is_globally_static_for_function();

  Tyvis* _get_formal();
  virtual Tyvis* _get_actual();
  virtual IIR_Int32 _get_number_of_element_associations();
  void _get_list_of_input_signals(  savant::set<Tyvis> *list );

protected:

private:

};
#endif

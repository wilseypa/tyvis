#ifndef TYVIS_QUIETATTRIBUTE_HH
#define TYVIS_QUIETATTRIBUTE_HH

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

#include "TyvisAttribute.hh"
#include "savant/IIRBase_QuietAttribute.hh"

class TyvisQuietAttribute : public virtual TyvisAttribute,
			    public virtual IIRBase_QuietAttribute {
public: 
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_necessary_decl_in_state( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_name_in_caps( published_file &_cc_out );
  void _publish_cc_attrib_type( published_file &_cc_out );
  void _publish_cc_necessary_decl_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_init_val( published_file &_cc_out );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_sigtype( published_file &_cc_out );
  void _publish_cc_type_cast( published_file &_cc_out );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_necessary_copying( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_read_or_write( published_file &_cc_out, 
				  const string &, 
				  const string &,
                                  PublishData *declarations);

  TyvisDeclaration *_build_implicit_declaration( TyvisTextLiteral *, TyvisTypeDefinition * ); 
  const string &_get_cc_attribute_name() const;
  IIR_Boolean _is_signal_attribute(){ return TRUE; }
  virtual IIR_Boolean _needs_suffix_published(){ return TRUE; }

protected:

private:

};
#endif

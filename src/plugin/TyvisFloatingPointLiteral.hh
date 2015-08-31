#ifndef TYVIS_FLOATINGPOINTLITERAL_HH
#define TYVIS_FLOATINGPOINTLITERAL_HH

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

#include "TyvisLiteral.hh"
#include "savant/IIRBase_FloatingPointLiteral.hh"

class TyvisFloatingPointLiteral : public virtual TyvisLiteral, public virtual IIRBase_FloatingPointLiteral {
public:
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_universal_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_ams_function(published_file &, PublishData *declarations);

  void _set_stmt_node_index(IIR_Int32 *, bool, bool&);

  IIR_FP64 _get_value();
protected:

private:
  IIR_Int32 _stmt_node_index;

};
#endif

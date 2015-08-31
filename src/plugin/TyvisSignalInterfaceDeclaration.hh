#ifndef TYVIS_SIGNALINTERFACEDECLARATION_HH
#define TYVIS_SIGNALINTERFACEDECLARATION_HH

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

#include "TyvisInterfaceDeclaration.hh"
#include "savant/IIRBase_SignalInterfaceDeclaration.hh"

class TyvisSignalInterfaceDeclaration : public virtual TyvisInterfaceDeclaration,
					public virtual IIRBase_SignalInterfaceDeclaration {
public:
  TyvisSignalInterfaceDeclaration();

  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  using Tyvis::_publish_cc_include;
  void _publish_cc_include( published_file &_cc_out );
  void _publish_cc_object_name( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_sigdest( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_decl( published_file &(_cc_out), PublishData *declarations );
  void _publish_cc_init( published_file &_cc_out, PublishData *declarations ) { _publish_cc_init_signal( _cc_out, declarations ); }
  void _publish_cc_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_wait_data( published_file &_cc_out, PublishData *declarations );
  const string _get_cc_object_type();

  void _set_passed_through_out_port(IIR_Boolean) {};

  IIR_Boolean _have_added_driver() {
    return _driver_addition_flag;
  }

  void _set_driver_addition_flag(IIR_Boolean val) {
    _driver_addition_flag = val;
  }

protected:

private:
  IIR_Boolean _driver_addition_flag;

};
#endif

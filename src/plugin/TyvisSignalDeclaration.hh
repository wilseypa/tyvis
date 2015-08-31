#ifndef TYVIS_SIGNALDECLARATION_HH
#define TYVIS_SIGNALDECLARATION_HH

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

#include "TyvisObjectDeclaration.hh"
#include "savant/IIRBase_SignalDeclaration.hh"

class TyvisSignalDeclaration : public virtual TyvisObjectDeclaration, public virtual IIRBase_SignalDeclaration {
public:
  TyvisSignalDeclaration();
  
  void _publish_cc_package_decl( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_package_definition( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_value ( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_decl( published_file &(_cc_out), PublishData *declarations );
  void _publish_cc_sigdest( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_wait_data( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );
  const string _get_cc_object_type();
  void _publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations ) { _publish_cc_lvalue( _cc_out , declarations ); }
  void _publish_cc_include(published_file &_cc_out);
  
  virtual void _publish_cc_necessary_copying( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_read_or_write( published_file &_cc_out,
					  const string &, 
					  const string &,
                                          PublishData *declarations);

  void _publish_cc_ams_function(published_file &, PublishData *declarations);
  void _publish_cc_init_for_ams(published_file &);
  void _publish_cc_init_signal_for_ams(published_file &);

  void _set_stmt_signal_index(IIR_Int32 *,  savant::set<IIR_Declaration> *);
  IIR_Int32 _get_stmt_signal_index() {return _stmt_signal_index;}
  void _flush_stmt_index();

  IIR_Boolean _is_passed_through_out_port() {
    return _passed_through_out_port;
  }

  void _set_passed_through_out_port(IIR_Boolean val) {
    _passed_through_out_port = val;
  }

  IIR_Boolean _have_added_driver() {
    return _driver_addition_flag;
  }

  void _set_driver_addition_flag(IIR_Boolean val) {
    _driver_addition_flag = val;
  }

  /** Builds the set of above attributes.*/ 
  void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

  // Helper Functions
  Tyvis                   *_get_value();
  TyvisDeclaration       *_get_signal_decl();

protected:

private:
  IIR_Boolean _passed_through_out_port;
  IIR_Boolean _driver_addition_flag;
  IIR_Int32 _stmt_signal_index;

};
#endif

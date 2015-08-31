
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

#include "TyvisAboveAttribute.hh"
#include "TyvisAttribute.hh"
#include "TyvisConcurrentStatement.hh"
#include "TyvisDesignFile.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/set.hh"
#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

TyvisSignalDeclaration::TyvisSignalDeclaration() {
  _driver_addition_flag = FALSE;
  _passed_through_out_port = FALSE;
  _stmt_signal_index = -1;
}

void
TyvisSignalDeclaration::_publish_cc_package_decl( published_file &_cc_out,
						  PublishData *declarations ){
  _cc_out << "const ";
  _publish_cc_decl_with_constructor_args( _cc_out, declarations );
}

void
TyvisSignalDeclaration::_publish_cc_package_definition( published_file &_cc_out,
							PublishData *declarations ){
  _publish_cc_decl_with_constructor_args( _cc_out, declarations );
}


void 
TyvisSignalDeclaration::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc" );
  
  ASSERT( get_declarator()->get_kind() == IIR_IDENTIFIER );

  // cgen_sym_tab_ptr will be NULL during elaboration, hence this check.
  if( declarations != NULL ){
    if( _get_value() != NULL ){
      _get_value()->_add_decl_into_declaration_collection(declarations);
    }
    if( !declarations->in_collection(this) ){
      declarations->add_declaration(this);
    }
  }
  if( is_implicit_declaration() == true ){
    if( _get_attribute_name() != NULL ){
      _get_attribute_name()->_publish_cc_lvalue( _cc_out , declarations );
      return;
    } 
  }
  TyvisDeclaration::_set_scoping_prefix();
  Tyvis::_publish_cc_prefix_string( _cc_out );
  _cc_out << *_get_mangled_declarator();
  TyvisDeclaration::_reset_scoping_prefix();
}

void
TyvisSignalDeclaration::_publish_cc_value( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_rvalue( _cc_out, declarations );
}

void
TyvisSignalDeclaration::_publish_cc_sigdest( published_file &_cc_out,PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_sigdest" );
  
  ASSERT(declarations != NULL);
  ASSERT(get_declarator()->get_kind() == IIR_IDENTIFIER);

  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  _cc_out << "getState()->"
          << *_get_mangled_declarator();
}

void 
TyvisSignalDeclaration::_publish_cc_decl( published_file &(_cc_out), PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_decl" );
  
  if( _get_attribute_name() != NULL ) {
    // Publish the implict declaration and also other declarations needed
    // for this
    _get_attribute_name()->_publish_cc_necessary_decl_in_state( _cc_out );
  }
  else {
    TyvisObjectDeclaration::_publish_cc_decl( _cc_out , declarations );
  }
}

void
TyvisSignalDeclaration::_publish_cc_init( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_init" );
  
  //Reset the index counter since, a new signal interface declaration
  //is initialized
  _set_index_level( 0 );

  ASSERT(is_implicit_declaration() == FALSE);
  _publish_cc_init_signal( _cc_out, declarations );
}

void
TyvisSignalDeclaration::_publish_cc_data( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_data" );
  
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  _cc_out << "getState()->"
          << *_get_mangled_declarator();
}

void
TyvisSignalDeclaration::_publish_cc_elaborate( published_file &_cc_out, PublishData * ){
  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_elaborate" );
  _cc_out << *_get_mangled_declarator();
}

void
TyvisSignalDeclaration::_publish_cc_wait_data( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_wait_data" );
  
  if (!declarations->in_collection(this)) {
    declarations->add_declaration(this);
  }
  _cc_out << "s->"; 
  _get_mangled_declarator()->_publish_cc_lvalue( _cc_out , declarations );
}

void
TyvisSignalDeclaration::_publish_cc_state_object_init( published_file &_cc_out,
						       PublishData *declarations ) {
  // If this is an implicit signal and it is QUIET attribute, we have to
  // add initialization for LAST_ACTIVE attribute.  Similarly for ACTIVE,
  // STABLE and EVENT attributes.  Before we add, we have to check if
  // LAST_ACTIVE (or LAST_EVENT) attribute is already initialized.  To do
  // this we maintain a boolean array
  // "implemented_attributes_in_constructor" which is set using the method
  // _add_published_attribute_in_constructor(SignalAttribute) and checked
  // using the method
  // _is_published_attribute_in_constructor(SignalAttribute).  The enum
  // SignalAttribute is defined in TyvisDeclaration.hh

  if(is_implicit_declaration()) {
    _get_attribute_name()->_publish_cc_state_object_init( _cc_out, declarations );
  }
  else {
    TyvisObjectDeclaration::_publish_cc_state_object_init( _cc_out, declarations );
  }
}

void
TyvisSignalDeclaration::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

const string
TyvisSignalDeclaration::_get_cc_object_type(){
  string retval;
  if(is_implicit_declaration() == TRUE) {
    retval = "ImplicitSignal";
  } 
  else {
    retval = "Signal";
  }
  return retval;
}

void
TyvisSignalDeclaration::_publish_cc_include(published_file &_cc_out) {
  ASSERT ( _get_subtype() != NULL );
  _get_subtype()->_publish_cc_include(_cc_out);
  Tyvis::_publish_cc_include( _cc_out, "tyvis/Signal.hh" );
}

void
TyvisSignalDeclaration::_publish_cc_necessary_copying( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_necessary_copying" );
  
  if( is_implicit_declaration() ){
    _get_attribute_name()->_publish_cc_necessary_copying( _cc_out );
  }
  else {
    _cc_out << "this->"
            << *_get_mangled_declarator()
            << " = s."
            << *_get_mangled_declarator()
            << ";" << NL();
  }
}

void
TyvisSignalDeclaration::_publish_cc_read_or_write( published_file &_cc_out,
                                                          const string &fName, 
                                                          const string &sName,
                                                          PublishData *){

  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_read_or_write" );
  
  if(is_implicit_declaration() == TRUE) {
    _get_attribute_name()->_publish_cc_read_or_write( _cc_out, fName, sName );
  }
  else {
    _cc_out << fName << "(NULL, " << sName << ", "
            << *_get_mangled_declarator()
            << ");\n";
  }
}

Tyvis*
TyvisSignalDeclaration::_get_value() {
  return dynamic_cast<Tyvis *>(IIRBase_SignalDeclaration::get_value());
}

TyvisDeclaration*
TyvisSignalDeclaration::_get_signal_decl() {
  ASSERT(0);
  return NULL;
//   TyvisAttribute* attached_attribute = _get_attribute_name();
//   savant::set<TyvisDeclaration> *attached_to = attached_attribute->_get_prefix()->_symbol_lookup();

//   if( attached_to == NULL ){
//     ostringstream err;
//     err << (attached_attribute->_get_prefix()) << " undefined";
//     report_error( this, err.str() );
//   }

//   TyvisDeclaration *current_decl = attached_to->getElement();
//   while( current_decl != NULL ){
//     if( current_decl->is_signal() == false ){
//       attached_to->remove( current_decl );
//     }
//     current_decl = attached_to->getNextElement();
//   }

//   TyvisDeclaration *signal_decl = NULL;
//   switch( attached_to->size() ){
//   case 1:{
//     signal_decl = attached_to->getElement();
//     break;
//   }
//   case 0:
//   default:
//     cerr << "Internal error in overload resolution in "
// 	 << "TyvisAttribute::_symbol_lookup() - got "
// 	 << attached_to->size() << " possibilities.\n";
//     abort();
//   }
//   return signal_decl;
}

void
TyvisSignalDeclaration::_flush_stmt_index() {
  // The signal index must be reset after publishing has completed for a
  // simultaneous statement. 
 _stmt_signal_index = -1;
}

void
TyvisSignalDeclaration::_set_stmt_signal_index(IIR_Int32 *index,
                                                       savant::set<IIR_Declaration> *signal_set) {
  if( _get_stmt_signal_index() == -1 ) {
    _stmt_signal_index = *index;
    (*index)++;
  }
  // add ourselves into the set
  signal_set->add(this);
} 

void
TyvisSignalDeclaration::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  if(is_implicit_declaration() == TRUE && _get_attribute_name() != NULL) {
    if(_get_attribute_name()->is_above_attribute() == TRUE) {
      to_build->add(dynamic_cast<TyvisAboveAttribute *>(_get_attribute_name()));
    }
  }
}

void
TyvisSignalDeclaration::_publish_cc_ams_function(published_file &_cc_out, PublishData *) {
  if(_get_currently_publishing_unit() == SIMULTANEOUS_IF) {
   _cc_out << *_get_mangled_declarator();
  }
  else if(_get_currently_publishing_unit() == SIMULTANEOUS_STATEMENT)
    _cc_out << " currentEquation->getSignal(" << _get_stmt_signal_index() << ")";
}

void
TyvisSignalDeclaration::_publish_cc_init_for_ams(published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_init_for_ams" );

  //Reset the index counter since, a new signal interface declaration
  //is initialized
  _set_index_level( 0 );

  ASSERT((is_implicit_declaration() == FALSE)||
         ((_get_attribute_name() != NULL) &&
          _get_attribute_name()->get_kind() == IIR_ABOVE_ATTRIBUTE));

  _publish_cc_init_signal_for_ams( _cc_out );
}

void
TyvisSignalDeclaration::_publish_cc_init_signal_for_ams( published_file &){}

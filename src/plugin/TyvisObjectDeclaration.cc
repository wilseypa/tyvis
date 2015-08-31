
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

#include "TyvisAccessTypeDefinition.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisAttribute.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisConcurrentStatement.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisIdentifier.hh"
#include "TyvisName.hh"
#include "TyvisObjectDeclaration.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisRecordTypeDefinition.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisSubprogramDeclaration.hh"

#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"
#include "published_header_file.hh"

TyvisObjectDeclaration::TyvisObjectDeclaration() {
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisObjectDeclaration::~TyvisObjectDeclaration() {
  //Release the list memory
  delete get_attributes();
}

void 
TyvisObjectDeclaration::_publish_cc_first_objectParameter( published_file &_cc_out,
							   PublishData *declarations ){
  _publish_cc_lvalue( _cc_out , declarations );
}

void 
TyvisObjectDeclaration::_publish_cc_constructor_args( published_file &_cc_out,
						      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_constructor_args" );
  _cc_out << OS("(");
  _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  _cc_out << "," << NL()
	  << "\"" << *_get_declarator() << "\"";
  if( _get_value() != 0 ){
    _cc_out << "," << NL();
    _get_value()->_publish_cc_rvalue( _cc_out, declarations );
  }
  _cc_out << CS(")");
}

void
TyvisObjectDeclaration::_publish_cc_universal_type( published_file &_cc_out ) {
  _get_subtype()->_publish_cc_universal_type( _cc_out );
}

void
TyvisObjectDeclaration::_publish_cc_decl_with_constructor_args( published_file &_cc_out,
								PublishData *declarations ){
  _publish_cc_include( _cc_out );
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_decl_with_constructor_args" );
  _cc_out << _get_cc_object_type() << " ";
  _publish_cc_declarator_with_constructor_args( _cc_out, declarations );
}

void
TyvisObjectDeclaration::_publish_cc_declarator_with_constructor_args( published_file &_cc_out,
								      PublishData *declarations ){
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_with_constructor_args" );
  _publish_cc_lvalue( _cc_out , declarations );
  _publish_cc_constructor_args( _cc_out, declarations );
}

void
TyvisObjectDeclaration::_publish_cc_state_object_init( published_file &_cc_out,
						       PublishData *declarations ){
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_state_object_init" );
  _publish_cc_include( _cc_out );
  _publish_cc_declarator_with_constructor_args( _cc_out, declarations );
  _cc_out << ",";
}

void
TyvisObjectDeclaration::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisObjectDeclaration::_publish_cc_type_and_object_name( published_file &_cc_out,
							  PublishData *declarations ){
  _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  _cc_out << " ";
  _publish_cc_object_name( _cc_out, declarations );
}

void 
TyvisObjectDeclaration::_publish_cc_decl( published_file &_cc_out,
					  PublishData * ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_decl" );
  _publish_cc_include( _cc_out );
  _cc_out << _get_cc_object_type() << " " << *_get_mangled_declarator() << ";" << NL();
}

void
TyvisObjectDeclaration::_publish_cc_range( published_file &_cc_out,
					   PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_range" );

  ASSERT(_get_subtype() != NULL);
  ASSERT(dynamic_cast<TyvisTypeDefinition *>(_get_subtype()) != NULL);
  _get_subtype()->_publish_cc_range( _cc_out, declarations );
}

void
TyvisObjectDeclaration::_publish_cc_bounds( published_file &_cc_out,
					    PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_bounds" );

  ASSERT(_get_subtype() != NULL);
  ASSERT(dynamic_cast<TyvisTypeDefinition *>(_get_subtype()) != NULL);
  _get_subtype()->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisObjectDeclaration::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_headers" );
  _get_subtype()->_publish_cc_headers( _cc_out );
  if( _get_declarative_region() != 0 ){
    _get_declarative_region()->_publish_cc_include( _cc_out );
  }
  if( _get_value() != 0 ){
    _get_value()->_publish_cc_headers( _cc_out );
  }
}

void
TyvisObjectDeclaration::_publish_cc_rvalue( published_file &_cc_out,
					    PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_rvalue" );	
  _publish_cc_lvalue( _cc_out, declarations );
}

void
TyvisObjectDeclaration::_publish_cc_lvalue( published_file &_cc_out,
					    PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_lvalue" );
  if(_get_currently_publishing_unit() == Tyvis::TYPE) {
    _get_value()->_publish_cc_lvalue( _cc_out, declarations );
  } 
  else{
    _cc_out << *_get_mangled_declarator();
  }
}

string
TyvisObjectDeclaration::_get_cc_rvalue(){
  return _get_mangled_declarator()->convert_to_string();
}

void
TyvisObjectDeclaration::_publish_cc_left( published_file &_cc_out,
					  PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_left" );
  _get_subtype()->_publish_cc_left( _cc_out, declarations );
}

void
TyvisObjectDeclaration::_publish_cc_right( published_file &_cc_out,
					   PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_right" );
  _get_subtype()->_publish_cc_right( _cc_out, declarations );
}

void
TyvisObjectDeclaration::_publish_cc_object_base( published_file &,
						 PublishData * ){}

void 
TyvisObjectDeclaration::_add_decl_into_declaration_collection(PublishData *declarations){
  Tyvis *val = dynamic_cast<Tyvis *>(get_value());
  if(val != NULL) {
    val->_add_decl_into_declaration_collection(declarations);
  }
  
  if ((declarations != NULL) && (!declarations->in_collection(this))) {
    declarations->add_declaration(this);
  }
}

TyvisAttributeSpecificationList *
TyvisObjectDeclaration::_get_attributes() {
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

void
TyvisObjectDeclaration::_add_declarations_in_initializations(PublishData *declarations) {
  Tyvis *val = dynamic_cast<Tyvis *>(get_value());
  if(val != NULL) {
    val->_add_decl_into_declaration_collection(declarations);
  }
  if(_get_subtype() != NULL) {
    _get_subtype()->_add_decl_into_declaration_collection(declarations);
  }
}

IIR_Int32 
TyvisObjectDeclaration::get_num_indexes(){
  return _get_subtype()->get_num_indexes();
}

IIR_Boolean 
TyvisObjectDeclaration::_is_globally_static(){
  IIR_Boolean retval = TRUE;

  ASSERT( _get_subtype() != NULL );
  if( _get_subtype()->is_access_type() == TRUE ){
    retval = FALSE;
  }

  return retval;
}

TyvisAttributeSpecificationList *
TyvisObjectDeclaration::_get_attribute_specification_list() { 
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes()); 
}


void
TyvisObjectDeclaration::_publish_cc_include( published_file &_cc_out ){
  if( _get_value() != 0 ){
    _get_value()->_publish_cc_include( _cc_out );
  }
  if( _get_subtype() != 0 ){
    _get_subtype()->_publish_cc_include( _cc_out );
  }
  _publish_cc_include( _cc_out, string("tyvis/") + _get_cc_object_type() + ".hh" );
}

void
TyvisObjectDeclaration::_publish_cc_init_signal( published_file &_cc_out,
						 PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_init_signal" );
  
  //#### yet to handle composite types.  Will be solved eventually
  const string anothername = _get_current_another_name();
  Tyvis *temporaryNode = _get_current_publish_node();
  TyvisDeclaration* tempDecl = NULL;
  // since c_p_n is changed here, it must be reset before returning
  const string procname = _get_current_publish_name();
  ostringstream labelstr;
  ostringstream signalNetinfostr;
  int index = 0;
  IIR_Boolean last_activeAttribRequired = false;
  IIR_Boolean last_eventAttribRequired = false;
  IIR_Boolean last_activeAttribPresent = false;
  IIR_Boolean last_eventAttribPresent = false;
  TyvisDeclaration* implied_declarations = NULL;
  
  //Reset the index counter since, a new signal interface declaration
  //is initialized
  _set_index_level( 0 );

  if( _get_implicit_declarations() != NULL ){
    implied_declarations = _get_implicit_declarations()->getElement();
    while(implied_declarations != NULL) {
      if(declarations->in_collection(implied_declarations)) {
	switch(implied_declarations->get_kind()) {
	case IIR_SIGNAL_DECLARATION:
	case IIR_SIGNAL_INTERFACE_DECLARATION:
	  implied_declarations->set_declarative_region(get_declarative_region());
	  implied_declarations->_publish_cc_init_signal( _cc_out ); 
	  break;
	case IIR_FUNCTION_DECLARATION:
	  //	  implied_declarations->_publish_cc_init_function( _cc_out ); 
	  break;
	default:
	  cerr << "Unknown declaration initialization: "
	       << implied_declarations->get_kind_text() << "\n";
	  abort();
	  break;
	}

	// Count the number of attributes of this signal
	index++;

	switch(implied_declarations->_get_attribute_name()->get_kind()) {
	case IIR_QUIET_ATTRIBUTE:
	case IIR_ACTIVE_ATTRIBUTE:
	  last_activeAttribRequired = true;
	  break;
	case IIR_STABLE_ATTRIBUTE:
	case IIR_EVENT_ATTRIBUTE:
	  last_eventAttribRequired = true;
	  break;
	case IIR_LAST_ACTIVE_ATTRIBUTE:
	    last_activeAttribPresent = true;
	    break;
	case IIR_LAST_EVENT_ATTRIBUTE:
	  last_eventAttribPresent = true;

	default:
	  break;
	}
      }
      implied_declarations = _get_implicit_declarations()->getNextElement();
    }
  }

  // Implementation of the QIUET, STABLE, ACTIVE and EVENT attributes
  // require last_active or last_event attribute.  This will be declared
  // transparent to the user.  This is why index is incremented by one here.
  // CAUTION: However, this must not be done if last_active or last_event is
  // already declared in the process.
  if(last_activeAttribRequired == TRUE &&
     last_activeAttribPresent == FALSE) {
    index++;
  }
  if(last_eventAttribRequired == TRUE &&
     last_eventAttribPresent == FALSE) {
    index++;
  }

  labelstr << *_get_mangled_declarator();
  _set_current_publish_name( labelstr.str() );
  
  if ( is_implicit_declaration() == FALSE ) {
//     ASSERT ( get_declarative_region() != NULL );
//     if( _get_current_publish_node() != NULL && 
// 	dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node()) != NULL ){
//       _get_current_publish_node()->_publish_cc_scoping_prefix( signalNetinfostr,
// 							       _get_declarative_region(), 
// 							       _get_current_publish_node() );
//     }
  }
  else {
    ASSERT ( dynamic_cast<TyvisDeclaration *>(_get_signal_decl()) != NULL );
    tempDecl = (TyvisDeclaration *) _get_signal_decl();
    if( _get_current_publish_node() != NULL && 
	dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node()) != NULL ){
      _get_current_publish_node()->_publish_cc_scoping_prefix( signalNetinfostr,
							       tempDecl->_get_declarative_region(),
							       this );
    }
  }
 
  _set_current_publish_node( this );
  signalNetinfostr << *_get_mangled_declarator();
  _set_current_another_name( signalNetinfostr.str() );
  _cc_out << OS("{");//begin dummy block for scoping
  _cc_out << _get_current_publish_name() << OS(".setElaborationInfo(");
  if((dynamic_cast<TyvisPackageDeclaration *>(_get_declarative_region()) == NULL) &&
     (get_declarative_region()->get_kind() != IIR_PACKAGE_BODY_DECLARATION)) {
    _cc_out << "proc->";
  }
  _cc_out << _get_current_another_name() << CS(");");

  // publish attributes for this signal
  if(index > 0) {
    ASSERT( _get_implicit_declarations() != NULL );
    implied_declarations = _get_implicit_declarations()->getElement();

    while(implied_declarations != NULL) {
      if(declarations->in_collection(implied_declarations)) {
	_cc_out << _get_current_publish_name() << OS(".setAttrib(");
	implied_declarations->_get_attribute_name()->_publish_cc_name_in_caps( _cc_out );
	_cc_out << "," << NL();
	
	if (implied_declarations->_get_attribute_name() != NULL) {
	  implied_declarations->_get_attribute_name()->_publish_cc_declarator( _cc_out, declarations );
	}
	else {
	  _cc_out << *implied_declarations->_get_mangled_declarator();
	}
	_cc_out << CS(");");
      }
      implied_declarations = _get_implicit_declarations()->getNextElement();
    }

    if(last_activeAttribRequired == TRUE && 
       last_activeAttribPresent == FALSE) {
      ASSERT( _get_current_publish_node()->is_signal() == true);
      _cc_out << _get_current_publish_name() << OS(".setAttrib(")
              << "LAST_ACTIVE," << NL()
              << *_get_current_publish_node()->_get_mangled_declarator()
              << "_last_active" << CS(");");
      
      last_activeAttribPresent = TRUE;
    }
    
    if(last_eventAttribRequired == TRUE && 
       last_eventAttribPresent == FALSE) {
      _cc_out << _get_current_publish_name() << OS(".setAttrib(");
      _cc_out << "LAST_EVENT," << NL();
      
      ASSERT( _get_current_publish_node()->is_signal() == true);
      
      // This function takes care of publishing last events
      // properly for the corresponding types

      _cc_out << *_get_current_publish_node()->_get_mangled_declarator();
      _cc_out << "_last_event" << CS(");");

      last_eventAttribPresent = TRUE;
    }
  }

  if ( is_implicit_declaration() == FALSE ){
    _cc_out << _get_current_publish_name() << ".resolveAndUpdate(processPointer);" << NL();
  }
  
  _cc_out << CS("}");//end dummy block
  _set_current_publish_name( procname );
  _set_current_publish_node( temporaryNode );
  _set_current_another_name( anothername );
  if(is_implicit_declaration() == TRUE) {
    _get_attribute_name()->_publish_cc_necessary_decl_init( _cc_out, declarations );
  }
}


void 
TyvisObjectDeclaration::_publish_cc_static_type_info_fn( published_file &_cc_out,
					       PublishData *declarations ){
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_static_type_info_fn" );
  if( _get_subtype() && _get_subtype()->_get_declaration() == this &&
      _get_subtype()->is_locally_static() ){
    _get_subtype()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
}

void
TyvisObjectDeclaration::_publish_cc_signal_net_info( published_file &_cc_out,
                                                     cc_publish_mode mode,
                                                     PublishData *declarations){
  CC_REF( _cc_out, "TyvisObjectDeclaration::_publish_cc_signal_net_info" );
  _get_subtype()->_publish_cc_include( _cc_out );
  if( mode == DECLARATION ){
    Tyvis::_publish_cc_include( _cc_out, "tyvis/SignalNetinfo.hh" );
    if( dynamic_cast<published_header_file *>(&_cc_out) != NULL ) { 
      _cc_out << "SignalNetinfo ";
    }
  }
  _cc_out << *_get_mangled_declarator();
  if( mode == DECLARATION ){
    if( dynamic_cast<published_header_file *>(&_cc_out) != NULL ) { 
      _cc_out << ";" << NL();
    }
  }
  else{
    _cc_out << OS("(");
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "," << NL()
	    << "\"" << *_get_declarator() << "\"" << CS(")");
  }

  if( _get_implicit_declarations() != 0 ){
    TyvisDeclaration *current = _get_implicit_declarations()->getElement();
    while( current != 0 ){
      if( current->is_signal() ){
	CC_REF( _cc_out, "TyvisSignalDeclaration::_publish_cc_signal_net_info" );
	current->_publish_cc_signal_net_info( _cc_out, mode, declarations );
      }
      current = _get_implicit_declarations()->getNextElement();
    }
  }
}
 
const IIR_Char*
TyvisObjectDeclaration::_get_mangling_prefix(){
  return "_";
}

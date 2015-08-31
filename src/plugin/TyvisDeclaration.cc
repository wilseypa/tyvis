
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

#include "TyvisAliasDeclaration.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisAttribute.hh"
#include "TyvisAttributeDeclaration.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisNatureDefinition.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisPackageBodyDeclaration.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisTextLiteral.hh"

#include "savant/error_func.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

TyvisDeclaration::TyvisDeclaration() {
  implicit_declarations = NULL;
  mangledDeclarator = NULL;
  implemented_attributes_in_constructor = new IIR_Boolean[numSignalAttributes];
  implemented_attributes_in_state = new IIR_Boolean[numSignalAttributes];
  implemented_attributes_in_initstate = new IIR_Boolean[numSignalAttributes];
}

TyvisDeclaration::~TyvisDeclaration() {
  if (mangledDeclarator != NULL) {
    delete mangledDeclarator;
  }
}

void
TyvisDeclaration::_publish_cc_addChild( published_file &_cc_out, PublishData *declarations ) {
  ASSERT(is_signal() == TRUE);
  ASSERT( _get_current_publish_name() != "" );

  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_addChild" );
  _publish_cc_include( _cc_out, "tyvis/Signal.hh" );

  if(_is_resolved_signal() == TRUE || _have_added_driver() == FALSE) {
    _cc_out << OS("{")
	    << "Signal *sourceSignal = new Signal";
    _publish_cc_constructor_args( _cc_out, declarations );
    _cc_out << ";" << NL();
    if(_is_composite_resolved_signal() == TRUE) {
      _cc_out << "sourceSignal->setParentCompositeType(sourceSignal)" << NL();
    }

    CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_addChild" );
    if( _get_current_publish_node() != NULL && 
	dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node()) != NULL ){
      _get_current_publish_node()->_publish_cc_scoping_prefix( _cc_out.get_stream(),
							       _get_declarative_region(), 
							       _get_current_publish_node() );
    }
    
    _cc_out << OS("addChild(") << *_get_mangled_declarator() << "," << NL()
	    << "*(sourceSignal)" << _get_current_publish_name();
    _cc_out << CS("");
    //_set_driver_addition_flag(TRUE);
    if(_is_resolved_signal() == TRUE) {
      _cc_out << OS("setResolutionFunctionId(");
      if( _get_current_publish_node() != NULL &&
	  dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node()) ) {
	_get_current_publish_node()->_publish_cc_scoping_prefix( _cc_out.get_stream(),
								 _get_declarative_region(), 
								 _get_current_publish_node() );
      }
      _publish_cc_elaborate( _cc_out, declarations); 
      ASSERT( _get_subtype()->_get_resolution_function() != 0 );
      _cc_out << "," << NL()
	      << *_get_subtype()->_get_resolution_function()->_get_mangled_declarator()
	      << CS(");");
    }
    _cc_out << CS("}");
  }
}

void
TyvisDeclaration::_publish_cc_include_path( ostream & ){
  _report_undefined_fn("TyvisDeclaration::_publish_cc_include_path");
}

void
TyvisDeclaration::_publish_cc_library_file_name( ostream & ){
  _report_undefined_fn("TyvisDeclaration::_publish_cc_library_file_name");
}

void
TyvisDeclaration::_publish_cc_locally_static_value( published_file &,
						    PublishData * ){
  _report_undefined_fn("TyvisDeclaration::_publish_cc_locally_static_value");
}

void
TyvisDeclaration::_publish_cc_locatesignal( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_locatesignal" );
  ASSERT(is_signal() == TRUE);

  _cc_out << "ptr = "
          << *_get_mangled_declarator()
          << ".locateSig(sigId);" << NL()
          << "if( ptr ) return ptr;" << NL();
}

void 
TyvisDeclaration::_publish_cc_declarative_region( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_declarative_region" );

  if(_get_declarative_region() != NULL) {
    Tyvis* my_decl_region = _get_declarative_region();
    switch(my_decl_region->get_kind()) {
    case IIR_FOR_LOOP_STATEMENT:
    case IIR_FUNCTION_DECLARATION:
    case IIR_PROCEDURE_DECLARATION:
    case IIR_PROCESS_STATEMENT:
    case IIR_SENSITIZED_PROCESS_STATEMENT:
    case IIR_ARCHITECTURE_DECLARATION:
    case IIR_ENTITY_DECLARATION:
      // Nothing needs to be done.
      break;
    case IIR_PACKAGE_DECLARATION:
    case IIR_PACKAGE_BODY_DECLARATION:
      _cc_out << dynamic_cast<TyvisPackageDeclaration *>(my_decl_region)->_get_cc_package_name()
	      << "::";
      break;
    default:
      cerr << "TyvisDeclaration::_publish_cc_declarative_region( published_file &_cc_out ): "
	   << my_decl_region->get_kind_text() << " not handled yet." << endl;
      break;
    }
  }
}

void
TyvisDeclaration::_publish_cc_implicit_signal_attributes( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_implicit_signal_attributes" );

  if( implicit_declarations != NULL ){
    TyvisDeclaration* implied_declarations = implicit_declarations->getElement();
    while(implied_declarations != NULL) {
      if(declarations->in_collection(implied_declarations)) {
	implied_declarations->_publish_cc_decl( _cc_out , declarations );
	if(implied_declarations->is_signal() == TRUE) {
	  implied_declarations->_publish_cc_implicit_signal_attributes( _cc_out, declarations );
	}
      }
      implied_declarations = implicit_declarations->getNextElement();
    }
  }
}

void
TyvisDeclaration::_publish_cc_implicit_state_objects_init( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_implicit_state_objects_init" );

  ASSERT(is_signal() == TRUE);
  
  if( implicit_declarations != NULL ){
    TyvisDeclaration* implied_declarations = implicit_declarations->getElement();
    while(implied_declarations != NULL) {
      if(declarations->in_collection(implied_declarations)) {
	implied_declarations->_publish_cc_state_object_init( _cc_out, declarations );
	if(implied_declarations->is_signal() == TRUE) {
	  implied_declarations->_publish_cc_implicit_state_objects_init( _cc_out, declarations );
	}
      }
      implied_declarations = implicit_declarations->getNextElement();
    }
  }
}

void
TyvisDeclaration::_publish_cc_forward_decl( published_file & ){
  _report_undefined_fn( "TyvisDeclaration::_publish_cc_forward_decl" );
}

void
TyvisDeclaration::_publish_cc_implicit_signal_type( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_implicit_signal_type" );

  TyvisAttribute* attribute = _get_attribute_name();
  ASSERT(is_signal() == TRUE);
  attribute->_publish_cc_type_info_xxx( _cc_out, 0 );
}


void
TyvisDeclaration::_publish_cc_object_name( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_object_name" );
  _cc_out << *_get_mangled_declarator();
}

void 
TyvisDeclaration::_publish_cc_declarator( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_declarator" );
  _cc_out << *_get_mangled_declarator();
}

void
TyvisDeclaration::_publish_cc_decl_with_constructor_args( published_file &_cc_out, PublishData *declarations ){
  _publish_cc_decl( _cc_out, declarations );
}

void
TyvisDeclaration::_publish_cc_init_signal( published_file & ) {
  _report_undefined_fn("_publish_cc_init_signal( _cc_out )");
}

void
TyvisDeclaration::_publish_cc_init_function( published_file &, PublishData * ) {
  _report_undefined_fn("_publish_cc_init_function( _cc_out, declarations )");
}

void
TyvisDeclaration::_publish_cc_implicit_signal_attributes_copying( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_implicit_signal_attribute_copying" );
  if( implicit_declarations != NULL ){
    TyvisDeclaration *implicit_declaration = implicit_declarations->getElement();
    while(implicit_declaration != NULL) {
      if( declarations->in_collection( implicit_declaration ) ) {
	implicit_declaration->_publish_cc_necessary_copying( _cc_out );
	if( implicit_declaration->is_signal() == TRUE ){
	  implicit_declaration->_publish_cc_implicit_signal_attributes_copying( _cc_out, declarations );
	}
      }
      implicit_declaration = implicit_declarations->getNextElement();
    }
  }
}

void
TyvisDeclaration::_publish_cc_implicit_signal_attributes_read_or_write( published_file &_cc_out,
                                                                               const string &functionName, 
                                                                               const string &streamName,
                                                                               PublishData *declarations){

  CC_REF( _cc_out, "TyvisDeclaration::_publish_cc_implicit_signal_attributes_read_or_write" );

  if( implicit_declarations != NULL ){
    TyvisDeclaration* implied_declarations = implicit_declarations->getElement();
    while(implied_declarations != NULL) {
      if(declarations->in_collection(implied_declarations)) {
	implied_declarations->_publish_cc_read_or_write( _cc_out, functionName, streamName );
	if(implied_declarations->is_signal() == TRUE) {
	  implied_declarations->_publish_cc_implicit_signal_attributes_read_or_write( _cc_out, functionName, streamName, declarations);
	}
      }
      implied_declarations = implicit_declarations->getNextElement();
    }
  }
}

void
TyvisDeclaration::_publish_cc_necessary_copying( published_file & ){
  _report_undefined_fn("_publish_cc_necessary_copying( _cc_out )");
}

void
TyvisDeclaration::_publish_cc_read_or_write( published_file &, 
                                             const string &, 
                                             const string & ) {
  _report_undefined_fn("_publish_cc_read_or_write( published_file &, const char *, const char *)");
}

void
TyvisDeclaration::_publish_cc_constant_object_init( published_file & ){
  _report_undefined_fn("_publish_cc_constant_object_init( _cc_out )");
}

void
TyvisDeclaration::_publish_cc_signal_net_info( published_file &, cc_publish_mode, PublishData * ){
  _report_undefined_fn("_publish_cc_signal_net_info( published_file &, cc_publish_mode, PublishData * )");
}

string
TyvisDeclaration::_get_cc_package_name(){
  return _get_mangled_declarator()->convert_to_string() + "Pkg";
}

string
TyvisDeclaration::_get_cc_package_body_name(){
  return _get_cc_package_name() + "Body";
}

const IIR_Char*
TyvisDeclaration::_get_mangling_prefix(){
  return "";
}

string
TyvisDeclaration::_mangle_declarator(){
  ostringstream mangledDeclarator;

  if( _get_declarative_region() != NULL ){
    if( _get_declarative_region()->_get_mangled_declarator() != NULL ){
      mangledDeclarator << *_get_declarative_region()->_get_mangled_declarator();
    }
    mangledDeclarator << "_" << _get_mangling_prefix();
  }

  mangledDeclarator << *_get_declarator();

  return mangledDeclarator.str();
}

TyvisTextLiteral *
TyvisDeclaration::_get_mangled_declarator() {
  if( _get_my_mangled_declarator() == NULL ){
    _set_mangled_declarator( _mangle_declarator() );
  }
  return mangledDeclarator;
}

TyvisTextLiteral *
TyvisDeclaration::_get_my_mangled_declarator(){
  return mangledDeclarator;
}

void
TyvisDeclaration::_set_mangled_declarator( const string mangledName ){
  if ( mangledName != "" ) {
    mangledDeclarator = dynamic_cast<TyvisIdentifier *>(TyvisIdentifier::get( mangledName, get_design_file()->get_class_factory() ));
  }
}

TyvisDesignatorList *
TyvisDeclaration::_get_instantiation_list() {
  _report_undefined_fn("_get_instantiation_list()");
  return NULL;
}

void
TyvisDeclaration::_set_scoping_prefix() {
  if( _get_currently_publishing_unit() == Tyvis::PROCEDURE ||
      _get_currently_publishing_unit() == Tyvis::FUNCTION ||
      _get_currently_publishing_unit() == PROCESS ){
    switch (_get_declarative_region()->get_kind()) {
    case IIR_BLOCK_STATEMENT:
    case IIR_ENTITY_DECLARATION:
    case IIR_SENSITIZED_PROCESS_STATEMENT:
    case IIR_PACKAGE_DECLARATION:
    case IIR_ARCHITECTURE_DECLARATION: {
      ostringstream prefix;
      TyvisProcessStatement* process_ptr = _get_current_process_statement();
      if ( process_ptr == NULL ){
	break;
      }
      prefix << "(static_cast<" << *(process_ptr->_get_mangled_label()) << "_state *>(";

      if (_get_currently_publishing_unit() != PROCESS) {
	prefix << "(static_cast<" << *(process_ptr->_get_mangled_label()) << " *>(processPtr)->";
      }
      
      prefix << "getState()))->";
      _set_publish_prefix_string(prefix.str());
      break;
    }
    case IIR_PROCESS_STATEMENT:{
      _set_publish_prefix_string("currentState->");
      break;
    }
    default:
      break;
    }
  }
}

Tyvis *
TyvisDeclaration::_get_value() {
  return dynamic_cast<Tyvis *>(get_value());
}

TyvisTextLiteral *
TyvisDeclaration::_get_declarator() {
  return dynamic_cast<TyvisTextLiteral *>(get_declarator());
}

Tyvis *
TyvisDeclaration::_get_declarative_region() {
  return dynamic_cast<Tyvis *>(get_declarative_region());
}

void
TyvisDeclaration::_add_declarations_in_initializations( PublishData * ) {
  _report_undefined_fn("_add_declarations_in_initializations( PublishData *declarations )");
}

TyvisAttribute *
TyvisDeclaration::_get_attribute_name(){
  return dynamic_cast<TyvisAttribute *>(get_attribute_name());
}

IIR_Int32 
TyvisDeclaration::get_num_indexes(){
  return 0;
}

IIR_Boolean
TyvisDeclaration::_is_published_attribute_in_constructor(SignalAttribute attrib) {
  ASSERT(get_kind() == IIR_SIGNAL_DECLARATION ||
	 get_kind() == IIR_SIGNAL_INTERFACE_DECLARATION);
  ASSERT(implemented_attributes_in_constructor != NULL);
  return implemented_attributes_in_constructor[int(attrib)];
}

void 
TyvisDeclaration::_add_published_attribute_in_constructor(SignalAttribute attrib) {
  ASSERT(get_kind() == IIR_SIGNAL_DECLARATION ||
	 get_kind() == IIR_SIGNAL_INTERFACE_DECLARATION);
  ASSERT(implemented_attributes_in_constructor != NULL &&
	 implemented_attributes_in_constructor[int(attrib)] == FALSE);

  implemented_attributes_in_constructor[int(attrib)] = TRUE;
}

IIR_Boolean
TyvisDeclaration::_is_published_attribute_in_state(SignalAttribute attrib) {
  ASSERT(get_kind() == IIR_SIGNAL_DECLARATION ||
	 get_kind() == IIR_SIGNAL_INTERFACE_DECLARATION);
  ASSERT(implemented_attributes_in_state != NULL);

  return implemented_attributes_in_state[int(attrib)];
}

void 
TyvisDeclaration::_add_published_attribute_in_state(SignalAttribute attrib) {
  ASSERT(get_kind() == IIR_SIGNAL_DECLARATION ||
	 get_kind() == IIR_SIGNAL_INTERFACE_DECLARATION);
  ASSERT(implemented_attributes_in_state != NULL &&
	 implemented_attributes_in_state[int(attrib)] == FALSE);

  implemented_attributes_in_state[int(attrib)] = TRUE;
}

IIR_Boolean
TyvisDeclaration::_is_published_attribute_in_initstate(SignalAttribute attrib) {
  ASSERT(get_kind() == IIR_SIGNAL_DECLARATION ||
	 get_kind() == IIR_SIGNAL_INTERFACE_DECLARATION);
  ASSERT(implemented_attributes_in_initstate != NULL);

  return implemented_attributes_in_initstate[int(attrib)];
}

void 
TyvisDeclaration::_add_published_attribute_in_initstate(SignalAttribute attrib) {
  ASSERT(get_kind() == IIR_SIGNAL_DECLARATION ||
	 get_kind() == IIR_SIGNAL_INTERFACE_DECLARATION);
  ASSERT(implemented_attributes_in_initstate != NULL &&
	 implemented_attributes_in_initstate[int(attrib)] == FALSE);

  implemented_attributes_in_initstate[int(attrib)] = TRUE;
}

TyvisDeclaration *
TyvisDeclaration::_get_prefix_declaration() {
  return this;
}

void
TyvisDeclaration::_build_sensitivity_list(TyvisDesignatorList *sensitivity_list) {
  if(is_signal() == FALSE) {
    return;
  }

  TyvisDesignator *node = dynamic_cast<TyvisDesignator *>(sensitivity_list->first());
  while (node != NULL) {
    ASSERT(node->get_kind() == IIR_DESIGNATOR_EXPLICIT);
    if ((dynamic_cast<TyvisDesignatorExplicit*>(node))->get_name() == this) { break; }
    node = dynamic_cast<TyvisDesignator *>(sensitivity_list->successor(node));
  }
  if (node == NULL) {
    TyvisDesignatorExplicit *new_designator = new TyvisDesignatorExplicit;
    copy_location( this, new_designator );
    new_designator->set_name(this);
    sensitivity_list->append(new_designator);
  }
}

void
TyvisDeclaration::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  if( is_signal() == TRUE && is_implicit_declaration() == FALSE ){
    list->add( this );
    if( get_value() != NULL ){
      dynamic_cast<Tyvis *>(get_value())->_get_list_of_input_signals(list);
    }
  }
}

void
TyvisDeclaration::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  siginfo->add( this );
}

void
TyvisDeclaration::_reset_scoping_prefix() {
  if((_get_currently_publishing_unit() == Tyvis::PROCEDURE) ||
     (_get_currently_publishing_unit() == Tyvis::FUNCTION) ||
     (_get_currently_publishing_unit() == PROCESS)) {
    _set_publish_prefix_string( "" );
  }
}

// This funnction needs work.
//If the type is a composite type, the sub-elements may be resolved
//Subtype. In that case, the signal is considered as a resolved signnal
//This function is yet to take care of such cases
IIR_Boolean 
TyvisDeclaration::_is_resolved_signal() {
  if(_get_subtype()->_get_resolution_function() != NULL) {
    return TRUE;
  }
  else {
    TyvisArrayTypeDefinition *arr_type_def =
      dynamic_cast<TyvisArrayTypeDefinition *>(_get_subtype());
    if( arr_type_def != NULL ) {
      return arr_type_def->_get_element_subtype()->_is_resolved_signal();
    }
    else {
      return FALSE;
    }
  }
}

void
TyvisDeclaration::_set_driver_addition_flag(IIR_Boolean) {
  _report_undefined_fn("_set_driver_addition_flag(IIR_Boolean)");
}

IIR_Boolean
TyvisDeclaration::_is_composite_resolved_signal() {
  if(is_signal() == TRUE) {
    if((_get_subtype()->_get_resolution_function() != NULL) &&
       ((_get_subtype()->is_array_type() == TRUE)|| 
	(_get_subtype()->is_array_type() == TRUE))) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

IIR_Boolean
TyvisDeclaration::_is_signal_in_outerscope() {
  if(is_signal() == TRUE) {
    if((_get_currently_publishing_unit() == Tyvis::PROCEDURE) ||
       (_get_currently_publishing_unit() == Tyvis::FUNCTION)) {
      if((_get_declarative_region()->get_kind() != IIR_PROCEDURE_DECLARATION) &&
	 (_get_declarative_region()->get_kind() != IIR_FUNCTION_DECLARATION)) {
	return TRUE;
      }
    }
  }
  return FALSE;
}

TyvisDeclaration*
TyvisDeclaration::_get_signal_prefix() {
  ASSERT(is_signal() == TRUE);
  ASSERT(is_implicit_declaration() == TRUE);

  TyvisAttribute* attached_attribute = _get_attribute_name();
  Tyvis* retval = NULL;

  ASSERT(attached_attribute != NULL);
  retval = attached_attribute->_get_prefix();

  if (dynamic_cast<TyvisDeclaration *>(retval) == NULL) {
    retval = retval->_get_prefix_declaration();
  }
  
  ASSERT( dynamic_cast<TyvisDeclaration *>(retval) != NULL );

  return (dynamic_cast<TyvisDeclaration*>(retval));
}

TyvisDeclaration*
TyvisDeclaration::_get_signal_decl() {
  _report_undefined_fn("_get_signal_decl()");
  return NULL;
}

TyvisAttributeSpecification*
TyvisDeclaration::_get_attribute_specification(Tyvis* attr){
  ASSERT(attr->_get_prefix_declaration()->get_kind() 
	 == IIR_ATTRIBUTE_DECLARATION);
  TyvisAttributeDeclaration* attr_decl = 
    dynamic_cast<TyvisAttributeDeclaration *>(attr->_get_prefix_declaration());
  TyvisAttributeSpecificationList* attr_spec_list = _get_attribute_specification_list();
  ASSERT(attr_spec_list != NULL);
  TyvisAttributeSpecification* attr_spec = dynamic_cast<TyvisAttributeSpecification *>(attr_spec_list->first());

  while(attr_spec != NULL) {
    if(attr_spec->_get_declaration() == attr_decl) {
      return attr_spec;
    }
    attr_spec = dynamic_cast<TyvisAttributeSpecification *>(attr_spec_list->successor(attr_spec));
  }
  return NULL;
}

void
TyvisDeclaration::_clear() {
  for (int i = 0; i < numSignalAttributes;  i++) {
    implemented_attributes_in_constructor[i] = FALSE;
    implemented_attributes_in_state[i] = FALSE;
    implemented_attributes_in_initstate[i] = FALSE;
  }
}

TyvisDeclaration*
TyvisDeclaration::_get_implicit_declaration_for_attribute ( TyvisAttribute *findAttrib ) {
  // Walk the list of implicit declarations and find if there is a function
  // declaration whose attribute is == findAttrib. If so return that
  // function declaration. Else return NULL. This function is called from
  // IIRScram_Attribute::_check_and_add_necessary_decls() to handle attributes
  // on indexed names.
    
  if( implicit_declarations != NULL ){
    TyvisDeclaration* implied_declarations = implicit_declarations->getElement();
    while(implied_declarations != NULL) {
      if (implied_declarations->_get_attribute_name()->get_kind() == findAttrib->get_kind()) {
	return implied_declarations;
      }
      
      implied_declarations = implicit_declarations->getNextElement();
    }
  }

  return NULL;
}  

 savant::set<TyvisDeclaration> *
TyvisDeclaration::_get_implicit_declarations(){
  return implicit_declarations;
}


void 
TyvisDeclaration::_set_implicit_declarations(  savant::set<TyvisDeclaration> *new_set ){
  ASSERT( new_set != NULL );
  implicit_declarations = new_set;
}

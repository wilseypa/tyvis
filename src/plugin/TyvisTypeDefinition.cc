
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

#include "TyvisScalarTypeDefinition.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisPackageBodyDeclaration.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisDesignFile.hh"

#include "savant/StandardPackage.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>

#include <vector>

class TyvisTypeDefinition::name_manager::impl {
public:
  impl(){}
  ~impl(){}

  bool contains_name( const string &check_for ) const {
    bool retval = false;
    for( unsigned int i = 0; i < my_names.size(); i++ ){
      if( my_names[i] == check_for ){
	retval = true;
	break;
      }
    }
    return retval;
  }
  void add_name( const string &to_add ){
    my_names.push_back( to_add );
  }
private:
  vector<string> my_names;
};

TyvisTypeDefinition::name_manager::name_manager() : 
  _impl( new TyvisTypeDefinition::name_manager::impl() ){}

TyvisTypeDefinition::name_manager::~name_manager(){
  delete _impl;
}

bool
TyvisTypeDefinition::name_manager::contains_name( const string &check_for ) const {
  return _impl->contains_name( check_for );
}

void
TyvisTypeDefinition::name_manager::add_name( const string &to_add ) {
  _impl->add_name( to_add );
}

TyvisTypeDefinition::TyvisTypeDefinition(){}

TyvisTypeDefinition::~TyvisTypeDefinition(){}


void
TyvisTypeDefinition::_publish_cc_left( published_file &, PublishData * ) {
  _report_undefined_fn("TyvisTypeDefinition::_publish_cc_left( _cc_out, declarations )");  
}

void
TyvisTypeDefinition::_publish_cc_bounds( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_bounds" );
  _cc_out << "defaultInfo()";
}

void
TyvisTypeDefinition::_publish_cc_right( published_file &, PublishData * ) {
  _report_undefined_fn("TyvisTypeDefinition::_publish_cc_right( _cc_out, declarations )");  
}

void
TyvisTypeDefinition::_publish_cc_array_info( published_file & ) {
  _report_undefined_fn("TyvisTypeDefinition::_publish_cc_array_info( _cc_out )");
}

void
TyvisTypeDefinition::_publish_cc_parent_type_name( published_file &_cc_out ) {
    CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_parent_type_name" );
  if( _get_parent_type() != this ){
    _get_parent_type()->_publish_cc_type_info_xxx( _cc_out, 0 );
  }
  else{
    _publish_cc_kernel_type( _cc_out );
  }
}

void
TyvisTypeDefinition::_publish_cc_decl_predefined_procedures_prototypes( published_file & ){
  _report_undefined_fn("TyvisTypeDefinition::_publish_cc_decl_predefined_procedures_prototypes( _cc_out )");
}

void 
TyvisTypeDefinition::_publish_cc_elaborate( published_file &_cc_out,
					    PublishData *declarations ){

  CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_elaborate" );

  if( is_scalar_type() == TRUE && _is_kernel_type() == FALSE ){
    _publish_cc_kernel_type( _cc_out );
  }
  else {
    _publish_cc_type_info_xxx( _cc_out, declarations );
  }
}

void
TyvisTypeDefinition::_publish_cc_subelement_type( published_file & ) {
  _report_undefined_fn("TyvisTypeDefinition::_publish_cc_subelement_type( _cc_out )");  
}

void
TyvisTypeDefinition::_publish_cc_anonymous_type_name( ostream & ){
  _report_undefined_fn("TyvisTypeDefinition::_publish_cc_anonymous_type_name( ostream & )");  
}

void
TyvisTypeDefinition::_publish_cc_base_type_name( published_file &_cc_out ) {
  if( is_subtype_decl() == TRUE ){
    if( is_enumeration_type() == TRUE ){
      _get_base_type()->_publish_cc_type_info_xxx( _cc_out, 0 );
    }
    else {
      ASSERT(_get_type_mark() != NULL);
      _get_type_mark()->_publish_cc_type_info_xxx( _cc_out, 0 );
    }
  }
  else if(_get_base_type() != NULL && _get_base_type()->_get_declaration() != NULL) {
    _get_base_type()->_publish_cc_type_info_xxx( _cc_out, 0 );
  }
  else {
    _publish_cc_type_info_xxx( _cc_out, 0 );
  }
}

void 
TyvisTypeDefinition::_publish_cc_type_string( published_file &_cc_out ){
  _publish_cc_type_info_xxx( _cc_out, 0 );
}

const string
TyvisTypeDefinition::_get_cc_type_name(){
  return _get_mangled_declarator()->convert_to_string();
}

void
TyvisTypeDefinition::_publish_cc_type_info_xxx( published_file &_cc_out,
						PublishData *declarations ){
  CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_type_info_xxx" );
  _publish_cc_type_info_xxx( _cc_out, declarations, _get_cc_type_name() );
}

void
TyvisTypeDefinition::_publish_cc_type_info_xxx_static( published_file &_cc_out,
						       PublishData *,
						       const string &type_name ){
  CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_type_info_xxx_static" );
  _cc_out << (type_name + "TypeInfo()");
}

void
TyvisTypeDefinition::_publish_cc_type_info_xxx_dynamic( published_file &_cc_out,
							PublishData *declarations,
							const string &type_name ){
  CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_type_info_xxx_dynamic" );
  _cc_out << _get_cc_tyvis_type() << OS("(");
  _publish_cc_type_info_args( _cc_out,
			      declarations,
			      type_name );
  _cc_out << CS(")");
}

void
TyvisTypeDefinition::_publish_cc_type_info_xxx( published_file &_cc_out,
						PublishData *declarations,
						const string &type_name ){
//DNS: There mst be a better way of finding out if we should be using dynamic method!
  if( is_locally_static() && type_name.substr(0,4) != "left"){ //DNS : this is bogus
    _publish_cc_type_info_xxx_static( _cc_out, declarations, type_name );
  }
  else{
    _publish_cc_type_info_xxx_dynamic( _cc_out, declarations, type_name );
  }
}

void
TyvisTypeDefinition::_publish_cc_init_signal( published_file & ) {
  _report_undefined_fn("TyvisTypeDefinition::_publish_cc_init_signal( _cc_out )");  
}

void
TyvisTypeDefinition:: _publish_cc_init_quantity(published_file &, PublishData *) {
  _report_undefined_fn("_publish_cc_init_quantity(_cc_out, declarations)");
}

void
TyvisTypeDefinition::_publish_cc_decl_type_attributes( published_file & ) {
  _report_undefined_fn("TyvisTypeDefinition::_publish_cc_decl_type_attributes( _cc_out )");  
}

void
TyvisTypeDefinition::_publish_cc_necessary_decl_in_state( published_file & ){
  // Dummy function. 
}

void
TyvisTypeDefinition::_publish_cc_init_fields_for_signals( published_file & ) {
  // Dummy function. 
}

void
TyvisTypeDefinition::_publish_cc_resolution_function_id( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_resolution_function_id" );
  
  if( _get_resolution_function() != NULL ){
    _cc_out << OS("_savant_entity_elab::addResolutionFn(")
            << *_get_resolution_function()->_get_mangled_declarator()
            << CS(")");
  }
  else{
    _cc_out << "-1";
  }
}

void
TyvisTypeDefinition::_publish_cc_temporary_type_info( published_file &,
                                                      PublishData *,
                                                      char *, 
                                                      char *) {
}

void
TyvisTypeDefinition::_publish_cc_constructor_args( published_file &, PublishData *) {}

void
TyvisTypeDefinition::_publish_cc_init_val( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_init_val( published_file &_cc_out )");
}


void
TyvisTypeDefinition::_publish_cc_type_info_scoping_prefix( published_file &_cc_out,
                                                           PublishData *,
                                                           const bool commaFlag ){
  Tyvis *_old_publish_node = _get_current_publish_node();
  IIR_Boolean prefix     = FALSE;
  
  if (is_anonymous() == FALSE) {
    TyvisDeclaration *typeDecl = _get_declaration();
    ASSERT ( typeDecl != NULL );
    ASSERT ( typeDecl->_get_declarative_region() != NULL );

    if( _get_current_publish_node() == NULL ){
      _set_current_publish_node( typeDecl->_get_declarative_region() );
    }
    
    // Determine the correct scope in which the type_info has been declared...
    if ( _get_elaboration_class() != NULL ){
      if (commaFlag == TRUE) {
	_cc_out << ", ";
      }
      
      // If we are publishing in the state of a process or a process we
      // need to add the enclosing scope pointer ourselves...
      
      if( _get_currently_publishing_unit() == PROCESS ){
	ASSERT ( _get_current_process_statement() != NULL );
	_cc_out << "((" 
		<< _get_current_process_statement()->_get_declarative_region()->_get_cc_elaboration_class_name()
		<< " *) getProc())->";
	prefix = TRUE;
      }
      if (_get_currently_publishing_unit() == PROCESS_STATE) {
	if (typeDecl->_get_declarative_region()->get_kind() ==
	    IIR_PROCESS_STATEMENT) {
	  // Okay...this declaration is in the process's declarative region
	  // so we don't need a scoping prefix etc...
	  _cc_out << "processPtr";
	  
	  prefix = TRUE;
	}
	else {
	  _cc_out << "(("
		  << _get_current_publish_node()->_get_cc_elaboration_class_name()
		  << " *)processPtr->getProc())->";
	  prefix = TRUE;
	}
      }
      
      if ((_get_currently_publishing_unit() == PROCEDURE) ||
	  (_get_currently_publishing_unit() == FUNCTION)) {
	if (typeDecl->_get_declarative_region()->get_kind() == IIR_PROCESS_STATEMENT) {
	  _cc_out << "(("
                  << *typeDecl->_get_declarative_region()->_get_mangled_declarator()
                  << " *) processPtr)->";
	  prefix = TRUE;
	}
	else {
	  // Okay.. have to publish necessary scope..
	  // The type declaration could be in any one of the enclosing scopes..
	  // like say entity declaration/ architecture declaration etc...

	  // Hack...this will work only for one level...and not more..
	  // HAVE TO FIX THIS...

	  if( typeDecl->_get_declarative_region() == _get_current_publish_node() ||
	      (_get_current_publish_node() != NULL && 
	       dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node()) != NULL )) {
	    // Okay...
	    _cc_out << "((";
	    _get_current_publish_node()->_publish_cc_class_name( _cc_out.get_stream() );
	    _cc_out << " *) processPtr->getProc())->";
	    prefix = TRUE;
	  }
	  else {
	    cerr << "Error - TyvisTypeDefinition.cc::"
		 << "_publish_cc_type_info_scoping_prefix(bool) - unhandled"
		 << "case in publishing scoping prefix for object defined in "
		 << "subprogram but type is declared in outer scope.\n";
	    _cc_out << "NULL";
	    prefix = TRUE;
	  }
	}
      }
      
      if( typeDecl->_get_declarative_region() == _get_current_publish_node() ){
	if( _get_currently_publishing_unit() == TYPE ){
	  if (prefix == FALSE) {
	    _cc_out << "enclosingScope->";
	  }
	}
	else {
	  if (prefix == FALSE) {
	    _cc_out << "this->";
	  }
	}
      }
      else {
	Tyvis *curr_node = _get_current_publish_node();
	if( _get_currently_publishing_unit() == PROCESS &&
	    curr_node->get_kind() != IIR_PROCESS_STATEMENT &&
	    _get_current_process_statement() != NULL ){
	  curr_node = _get_current_process_statement();
	}

	ASSERT ( curr_node != NULL );
	ostringstream os;
	curr_node->_publish_cc_scoping_prefix( os,
					       typeDecl->_get_declarative_region(),
					       curr_node );
	string scopingPrefix = os.str();
	if( scopingPrefix.length() == 0 ){
	  if( prefix == FALSE ){
	    // Nothing dumped..so just print this...
	    _cc_out << "this->";
	  }
	}
	else {
	  _cc_out << scopingPrefix;
	}
      }
    }
    else {
      if (typeDecl->_get_declarative_region()->get_kind() ==
	  IIR_PROCESS_STATEMENT) {
	if (_get_currently_publishing_unit() == PROCESS_STATE) {
	  _cc_out << "processPtr->";
	}
      }
    }
  }
  _set_current_publish_node( _old_publish_node );
}

void
TyvisTypeDefinition::_publish_cc_object_type_info( published_file &_cc_out,
                                                   PublishData *declarations,
                                                   const IIR_Boolean, 
                                                   const char *,
                                                   const IIR_Boolean ){
  _publish_cc_type_info_xxx( _cc_out, declarations );
}

void
TyvisTypeDefinition::_publish_cc_type_info_args( published_file &,
						 PublishData *,
						 const string & ){
  _report_undefined_fn("_publish_cc_type_info_args");
}

void
TyvisTypeDefinition::_publish_cc_static_type_info_fn( published_file &_cc_out, 
						      PublishData *declarations,
						      const string &type_name ){
  CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_static_type_info_fn" );
  string publish_type;
  if( type_name == "" ){
    publish_type = _get_cc_type_name();
  }
  else{
    publish_type = type_name;
  }

  if( !_names.contains_name( publish_type ) ){
    _names.add_name( publish_type );
    if( get_line_number() >= 0 ){
      _cc_out << "// This TypeInfo was published for a " << get_kind_text();
      _cc_out << " found at " << *_get_file_name() << ":" << get_line_number() << NL();
    }

    if( is_locally_static() ){
      // Sometimes the prereq needs a forward reference to the type method.
      _cc_out << "const " << _get_cc_tyvis_type() << " &" << NL();
      _publish_cc_type_info_xxx( _cc_out, declarations, publish_type );
      _cc_out << ";" << NL();
    }// Publish unconstrained array type defs if assigned a name-> 
    else if( is_unconstrained_array_type() && !is_anonymous() ){
      _cc_out << "const " << _get_cc_tyvis_type() << " &" << NL();
      _cc_out << _get_cc_type_name() << "TypeInfo();" << NL();
      _cc_out << ";" << NL();
    }//<-

    _publish_cc_headers( _cc_out );
    _publish_cc_include( _cc_out );      
    _publish_cc_type_info_prereq( _cc_out, declarations, publish_type );
    
    if( is_locally_static() ){
      _cc_out << "inline const " << _get_cc_tyvis_type() << " &" << NL();
      _publish_cc_type_info_xxx( _cc_out, declarations, publish_type );
      _cc_out << OS("{")
	      << "static " << _get_cc_tyvis_type() << " _ti(";
      _publish_cc_type_info_args( _cc_out, declarations, publish_type );
      _cc_out << ");" << NL()
	      << "return _ti;"
	      << CS("}");
    }// Publish unconstrained array type defs if assigned a name->
    else if( is_unconstrained_array_type() && !is_anonymous()){
      _cc_out << "inline const " << _get_cc_tyvis_type() << " &" << NL();
      _cc_out << _get_cc_type_name() << "TypeInfo()";
      _cc_out << OS("{")
	      << "static " << _get_cc_tyvis_type() << " _ti(";
      _publish_cc_type_info_args( _cc_out, declarations, publish_type );
      _cc_out << ");" << NL()
	      << "return _ti;"
	      << CS("}");
    }// <-
  }
}

void 
TyvisTypeDefinition::_publish_cc_include( published_file &_cc_out ){
  if( _get_declaration() && 
      _get_declaration()->_get_declarative_region() &&
      dynamic_cast<TyvisLibraryUnit *>(_get_declaration()->_get_declarative_region()) ){
    _get_declaration()->_get_declarative_region()->_publish_cc_include( _cc_out );
  }
  else if ( get_base_type() ){
    ASSERT( get_base_type() != this );
    _get_base_type()->_publish_cc_include( _cc_out );
  }
  _publish_cc_include( _cc_out, string("tyvis/") + _get_cc_tyvis_type() + ".hh" );
}

void 
TyvisTypeDefinition::_publish_cc_headers( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisTypeDefinition::_publish_cc_include" );
  if(_is_kernel_type() == FALSE) {
    if (is_anonymous() == FALSE) {
      ostringstream file_name_stream;
      IIR_Boolean flag = FALSE;
      
      ASSERT ( _get_declaration() != NULL );
      ASSERT ( _get_declaration()->_get_declarative_region() != NULL );
      Tyvis *declRegn = _get_declaration()->_get_declarative_region();

      if( declRegn == _get_current_publish_node() ){
	return;
      }
      
      if( dynamic_cast<TyvisConcurrentStatement *>(declRegn) ||
	  declRegn->get_kind() == IIR_PROCESS_STATEMENT ){
	declRegn->_publish_cc_class_name(file_name_stream);
	flag = TRUE;
      }
      else {
	if ( dynamic_cast<TyvisPackageDeclaration *>(declRegn) ||
	     dynamic_cast<TyvisPackageBodyDeclaration *>(declRegn) ){
	  if ( _get_currently_publishing_unit() != Tyvis::PACKAGE_BODY &&
	       _get_currently_publishing_unit() != Tyvis::PACKAGE_PUB ){
	    declRegn->_publish_cc_binding_name( file_name_stream );
	    flag = TRUE;
	  }
	}
      }
      
      if (flag == TRUE) {
	// Some data has been published...
	file_name_stream << ".hh";
	_publish_cc_include( _cc_out, file_name_stream.str());
      }
    }
    else {
      if (_get_base_type() != NULL) {
	_get_base_type()->_publish_cc_include(_cc_out);
      }
    }
  }
}

void
TyvisTypeDefinition::_add_decl_into_declaration_collection(PublishData *) {
  // Dummy function to keep _report_undefined_fn quiet.
}

TyvisTextLiteral *
TyvisTypeDefinition::_get_mangled_declarator(){
  TyvisTextLiteral *retval = 0;

  if( _get_declaration() != NULL ){
    retval = _get_declaration()->_get_mangled_declarator();
  }
  else if( _get_base_type() != 0 ){
    retval = _get_base_type()->_get_mangled_declarator(); 
  } 
  else {
    char *string = "<ANONYMOUS>";
    retval = dynamic_cast<TyvisTextLiteral *>(TyvisIdentifier::get( string, strlen( string ), get_design_file()->get_class_factory() ));
  }
  return retval;
}

TyvisTypeDefinition * 
TyvisTypeDefinition::_get_type_mark() {
  return dynamic_cast<TyvisTypeDefinition *>(get_type_mark());
}

TyvisTypeDefinition*
TyvisTypeDefinition::_get_base_type(){
  return dynamic_cast<TyvisTypeDefinition *>(get_base_type());
}

TyvisTypeDefinition*
TyvisTypeDefinition::_get_resolved_base_type(){
  return dynamic_cast<TyvisTypeDefinition *>(get_resolved_base_type());
}

IIR_Boolean
TyvisTypeDefinition::_is_kernel_type() {
  if( (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "integer") == 0)        ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "boolean") == 0)        ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "severity_level") == 0) ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "positive") == 0)       ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "character") == 0)      ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "real") == 0)           ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "time") == 0)           ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "natural") == 0)        ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "bit") == 0)            ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "side") == 0)           ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "width") == 0)          ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "delay_length") == 0)   ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "file_open_kind") == 0) ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "bit_vector") == 0)     ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "string") == 0)         ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "text") == 0)           ||
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "line") == 0)           ||  
      (IIRBase_TextLiteral::cmp(_get_mangled_declarator(), "file_open_status") == 0)
      ) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

IIR_Boolean 
TyvisTypeDefinition::_is_resolved_type(){
  if (_get_resolution_function() != NULL) {
    return TRUE;
  }
  return FALSE;
}

IIR_Boolean
TyvisTypeDefinition::_is_anonymous(){
  if (_get_declaration())
    return FALSE;
  return IIRBase_TypeDefinition::is_anonymous();
}

TyvisScalarTypeDefinition *
TyvisTypeDefinition::_get_index_subtype(){
  _report_undefined_fn("TyvisTypeDefinition::_get_index_subtype()");  
  return NULL;
}

TyvisScalarTypeDefinition *
TyvisTypeDefinition::_get_resolved_index_subtype(){
  return dynamic_cast<TyvisScalarTypeDefinition *>(get_resolved_index_subtype());
}

Tyvis*
TyvisTypeDefinition::_get_base_type_left() {
  return dynamic_cast<Tyvis *>(get_base_type_left());
}

Tyvis*
TyvisTypeDefinition::_get_base_type_direction() {
  return dynamic_cast<Tyvis *>(get_base_type_direction());
}

Tyvis*
TyvisTypeDefinition::_get_base_type_right() {
  return dynamic_cast<Tyvis *>(get_base_type_right());
}

TyvisTypeDefinition *
TyvisTypeDefinition::_get_element_subtype(){
  _report_undefined_fn("TyvisTypeDefinition::_get_element_subtype()");  
  return NULL;
}

IIR_Int32
TyvisTypeDefinition::get_num_indexes(){
  // This is over-ridden in TyvisArrayTypeDefinition...
  return 0;
}

Tyvis*
TyvisTypeDefinition::_get_direction(){
  _report_undefined_fn("TyvisTypeDefinition::_get_direction()");  
  return  NULL;
}

TyvisDeclaration *
TyvisTypeDefinition::_get_declaration() {
  return dynamic_cast<TyvisDeclaration *>(get_declaration());
}

TyvisTypeDefinition *
TyvisTypeDefinition::_get_bottom_base_type(){
  if( _get_base_type() == NULL ){
    return dynamic_cast<TyvisTypeDefinition *>(this);
  }
  else{
    return _get_base_type()->_get_bottom_base_type();
  }
}

TyvisTypeDefinition *
TyvisTypeDefinition::_get_parent_type(){
  TyvisTypeDefinition *retval = const_cast<TyvisTypeDefinition *>(this);

  if( is_subtype() && !is_enumeration_type() && _get_type_mark() ){
    retval = _get_type_mark();
  }
  else if( _get_base_type() != NULL && _get_base_type()->is_anonymous() == FALSE ) {
    retval = _get_base_type();
  }
  
  return retval;
}

TyvisFunctionDeclaration *
TyvisTypeDefinition::_get_resolution_function( ){
  return NULL; 
}

Tyvis*
TyvisTypeDefinition::_get_elaboration_class() {
  TyvisDeclaration *decl = _get_declaration();

  if (decl == NULL) {
    // An anonymous type definition...well, there has to be a base type
    // somewhere that has a decl!! we need to sope there... otherwise
    // this guy has no elaboration class. Could be the 2nd dimension of a
    // 2 dimensional array or something like that...

    if (_get_resolved_base_type() != NULL) {
      return _get_resolved_base_type()->_get_elaboration_class();
    }
    
    if (_get_type_mark() != NULL) {
      return _get_type_mark()->_get_elaboration_class();
    }
    
    return NULL;
  }
  
  Tyvis* declRgn = decl->_get_declarative_region();
  ASSERT ( declRgn != NULL );
  
  if ((dynamic_cast<TyvisSubprogramDeclaration *>(declRgn)) ||
      (declRgn->get_kind() == IIR_PROCESS_STATEMENT) ||
      (dynamic_cast<TyvisPackageDeclaration *>(declRgn)) ||
      (dynamic_cast<TyvisPackageBodyDeclaration *>(declRgn))) {
    return NULL;
  }

  return declRgn;
}

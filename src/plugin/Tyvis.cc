
// Copyright (c) The University of Cincinnati.
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

#include "TyvisAccessTypeDefinition.hh"
#include "TyvisAliasDeclaration.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisArchitectureStatement.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisBlockStatement.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisConfigurationDeclaration.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisConstantDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisElementDeclaration.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisGenericList.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisPortList.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisRangeTypeDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "published_file.hh"
#include "PublishData.hh"
#include "savant/error_func.hh"
#include "savant/savant.hh"
#include "savant/include_manager.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include <clutils/StringUtilities.h>
#include <cstring>
#include <set>
#include <sstream>
using std::ostringstream;

Tyvis::Tyvis() {
  _currently_publishing_simultaneous_stmt = FALSE;
  _currently_publishing_simultaneous_if_stmt = FALSE;
}

void
Tyvis::_publish_cc_variable_lvalue( published_file &_cc_out,
				    PublishData *declarations ){
  CC_REF( _cc_out, "Tyvis::_publish_cc_variable_lvalue" );
  _publish_cc_lvalue( _cc_out, declarations );
}

void
Tyvis::_publish_cc_signal_lvalue( published_file &_cc_out,
				  PublishData *declarations ){
  CC_REF( _cc_out, "Tyvis::_publish_cc_signal_lvalue" );
  _publish_cc_lvalue( _cc_out, declarations );
}

void
Tyvis::_publish_cc_lvalue( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations )");
}

string
Tyvis::_get_cc_rvalue(){
  _report_undefined_fn("_get_cc_rvalue()");
  return "";
}

void
Tyvis::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "Tyvis::_publish_cc_rvalue" );
  _publish_cc_lvalue( _cc_out, declarations );
}

void 
Tyvis::_publish_cc_prefix_string( published_file &_cc_out ) {
  if( _get_publish_prefix_string() != "" ) {
    _cc_out << _get_publish_prefix_string();
  }
}

void
Tyvis::_publish_cc_scoping_prefix( ostream &, Tyvis *, Tyvis * ){
  _report_undefined_fn("_publish_cc_scoping_prefix( ostream&, Tyvis *, Tyvis * )");
}

void 
Tyvis::_publish_cc_condition( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_condition( _cc_out, declarations )");
}

void 
Tyvis::_publish_cc_name_elaborate( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_name_elaborate( _cc_out, declarations )");
}

void 
Tyvis::_publish_cc_init( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_init( _cc_out, declarations )");
}

void
Tyvis::_publish_cc_init_for_ams(published_file & ){
  _report_undefined_fn("_publish_cc_init_for_ams( _cc_out )");
}

const string
Tyvis::_get_cc_object_type(){
  _report_undefined_fn("_get_cc_object_type()");
  return "";
}

void 
Tyvis::_publish_cc_object_name( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_object_name( _cc_out, declarations )");
}

void 
Tyvis::_publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_rvalue( _cc_out, declarations );
}

void
Tyvis::_publish_cc_universal_type( published_file & ){
  _report_undefined_fn("_publish_cc_universal_type( _cc_out )");  
}

void
Tyvis::_publish_cc_kernel_type( published_file &pf ) {
  _publish_cc_kernel_type( pf.get_stream() );
}

void
Tyvis::_publish_cc_kernel_type( ostream &os ) {
  os << _get_cc_tyvis_type();
}

const string
Tyvis::_get_cc_tyvis_type() {
  if( _get_subtype() != 0 ){
    return _get_subtype()->_get_cc_tyvis_type();
  }
  else{
    _report_undefined_fn("_get_cc_tyvis_type()");
    return "";
  }
}

void
Tyvis::_publish_cc_first_objectParameter( published_file &, PublishData * ){
  ASSERT ( is_object() == TRUE );
  _report_undefined_fn("_publish_cc_first_objectParameter( published_file &,  PublishData * )");
}

void 
Tyvis::_publish_cc_wait_data( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_wait_data( _cc_out )");
}

void
Tyvis::_publish_cc_data( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_data( _cc_out, declarations )");
}

void 
Tyvis::_publish_cc_decl( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_decl( _cc_out , declarations )");
}

void 
Tyvis::_publish_cc_decl_with_constructor_args( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_decl_with_constructor_args( _cc_out , declarations )");
}

void
Tyvis::_publish_cc_bounds( published_file &, PublishData * ) {
  _report_undefined_fn("_publish_cc_bounds( _cc_out, declarations )");
}

void
Tyvis::_publish_cc_range( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_range( _cc_out, declarations )");
}

void 
Tyvis::_publish_cc_static_type_info_fn( published_file &, PublishData * ){} 

void
Tyvis::_publish_cc_package_decl( published_file &,
				 PublishData * ){}

void
Tyvis::_publish_cc_package_definition( published_file &,
				       PublishData * ){}

void
Tyvis::_publish_cc_value( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_value( _cc_out, declarations )");
}

void
Tyvis::_publish_cc_universal_value( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_universal_value( _cc_out, declarations )");
}

void
Tyvis::_publish_cc_binding_name( ostream & ){
  _report_undefined_fn("_publish_cc_binding_name( _cc_out )");
}

void
Tyvis::_publish_cc_class_name( ostream& os ){
  // Publishes the name of the C++ class for this object if any...
  _publish_cc_binding_name(os);
  
  if ((get_kind() == IIR_ARCHITECTURE_DECLARATION) ||
      (get_kind() == IIR_ENTITY_DECLARATION) ||
      ( dynamic_cast<TyvisConcurrentStatement *>(this) != NULL && 
        get_kind() != IIR_PROCESS_STATEMENT )) {
    os << "_elab";
  }
}

void
Tyvis::_publish_cc_declarator( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_declarator( _cc_out, declarations )");
}

void
Tyvis::_publish_cc_type_info_xxx( published_file &_cc_out,
				  PublishData *declarations ){
  CC_REF( _cc_out, "Tyvis::_publish_cc_type_info_xxx" );
  if( _get_subtype() != 0 ){
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  }
  else{
    cerr << "Tyvis::_get_cc_type_info() called on " << get_kind_text() 
	 << ", with a null subtype - aborting!" << endl;
    abort();
  }
}

void
Tyvis::_publish_cc_for_index( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_for_index( _cc_out, declarations )");
}

void
Tyvis::_publish_cc_elaborate( published_file &, PublishData * ){}

void 
Tyvis::_publish_cc_subprogram_arg( published_file & ) {
  _report_undefined_fn("_publish_cc_subprogram_arg( _cc_out )");
}

void 
Tyvis::_publish_cc_constructor_args( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_constructor_args( _cc_out, declarations )");
}

void 
Tyvis::_publish_cc_sigdest( published_file & ){
  _report_undefined_fn("_publish_cc_sigdest( _cc_out )");
}

void
Tyvis::_publish_cc_headers( published_file & ){
  _report_undefined_fn("_publish_cc_headers( _cc_out )");
}

void 
Tyvis::_publish_cc_headerfiles_for_cc_default( published_file &_cc_out ){
  ostringstream file_name_stream;
  _publish_cc_binding_name( file_name_stream );
  if( get_kind() != IIR_PROCESS_STATEMENT ){
    file_name_stream << "_elab";
  }
  file_name_stream << ".hh";
  _publish_cc_include( _cc_out, file_name_stream.str() );
}

void
Tyvis::_publish_cc_direction( published_file & ){
  _report_undefined_fn("_publish_cc_direction( _cc_out )");
}

string
Tyvis::_get_cc_direction(){
  _report_undefined_fn("_get_cc_direction()");
  return "";
}

void
Tyvis::_publish_cc_addChild( published_file &, PublishData * ) {
  _report_undefined_fn("_publish_cc_addChild( _cc_out, declarations )");
}  

void
Tyvis::_publish_cc_class_includes( published_file &_cc_out, 
				      TyvisArchitectureStatementList* conc_stmt_list) {
  ASSERT(conc_stmt_list != NULL);
  ASSERT( dynamic_cast<TyvisConcurrentStatement *>(this) != NULL ||
	  is_entity_declaration() == TRUE ||
	  is_component_declaration() == TRUE ||
	  get_kind() == IIR_ARCHITECTURE_DECLARATION );
	  
  TyvisArchitectureStatement* conc_stmt = dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->first());
  while( conc_stmt != NULL ){
    _cc_out << "class ";
    if( conc_stmt->get_kind() == IIR_PROCESS_STATEMENT ){
      if( conc_stmt->_get_mangled_label() != NULL ){
	_cc_out.get_stream() << *(conc_stmt->_get_mangled_label());
      }
      else {
	_cc_out << "ANON_PROCESS" << conc_stmt;
      }
      _cc_out << ";" << NL();
    }
    else {
      _cc_out << conc_stmt->_get_cc_elaboration_class_name() << ";" << NL();
    }
    conc_stmt = dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->successor(conc_stmt));
  }
}

void
Tyvis::_publish_cc_signals( published_file &_cc_out,
                            TyvisDeclarationList* decl_list,
                            PublishData *declarations) {
  CC_REF( _cc_out, "Tyvis::_publish_cc_signals" );
  ASSERT(decl_list != NULL);
  ASSERT( dynamic_cast<TyvisConcurrentStatement *>(this) != NULL ||
	  is_entity_declaration() == TRUE ||
	  is_component_declaration() == TRUE ||
	  get_kind() == IIR_ARCHITECTURE_DECLARATION );
  
  TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration *>(decl_list->first());
  while (decl != NULL) {
    if(decl->is_signal() == TRUE) {
      decl->_publish_cc_signal_net_info( _cc_out, DECLARATION, declarations );
    }
    else if(decl->get_kind() == IIR_ALIAS_DECLARATION) {
      if(decl->is_signal() == TRUE) {
	(dynamic_cast<TyvisAliasDeclaration*>(decl))->_publish_cc_elaborate_alias_definition( _cc_out, declarations );
      }
    }
    decl = dynamic_cast<TyvisDeclaration *>(decl_list->successor(decl));    
  }
}

void
Tyvis::_publish_cc_object_pointers( published_file &_cc_out, 
                                    TyvisArchitectureStatementList* conc_stmt_list,
                                    PublishData *){
  CC_REF( _cc_out, "Tyvis::_publish_cc_object_pointers" );

  TyvisArchitectureStatement *conc_stmt = dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->first());
  while (conc_stmt != NULL) {
    if ( conc_stmt->get_kind() == IIR_PROCESS_STATEMENT ) {
      TyvisProcessStatement *process_statement = dynamic_cast<TyvisProcessStatement *>(conc_stmt);
      const string process_name = process_statement->_get_cc_process_class();
      // If the concurrent statement is a concurrent generate for
      // statement, then we need to instantiate multiple process. So we
      // declare a process_class ** array so that process_class[i] contains
      // the process instantiated seperately and not deleted in the
      // destructor. Because the processes are deleted in the main.cc of
      // the generate code.
      _cc_out << process_name;
      if ( get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT ){
	_cc_out << " **";
      }
      else {
	_cc_out << " *";
      }
      _cc_out << process_name << "_elab_obj;" << NL();
    }
    else if( conc_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT ||
	     conc_stmt->_is_block_statement() ||
	     conc_stmt->_is_concurrent_generate_statement() ){
      _cc_out << conc_stmt->_get_cc_elaboration_class_name() << " *";
      conc_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out.get_stream() );
      _cc_out << "_elab_obj;" << NL();
    }
    else if( conc_stmt->get_kind() == IIR_SIMPLE_SIMULTANEOUS_STATEMENT ||
	     conc_stmt->get_kind() == IIR_SIMULTANEOUS_IF_STATEMENT ) {
      // No action required
    }
    else {
      ostringstream err;
      err << "ERROR! TyvisBlockStatement::_publish_cc_object_pointers( _cc_out ):"
	  << " unknown concurrent statement type |" 
	  << conc_stmt->get_kind_text() << "| in arch";
      report_error( this, err.str() );
    }
    conc_stmt = dynamic_cast<TyvisConcurrentStatement *>(conc_stmt_list->successor(conc_stmt));
  }
}

void
Tyvis::_publish_cc_object_pointers_init( published_file &_cc_out,
                                               TyvisArchitectureStatementList* conc_stmt_list,
                                               TyvisDeclarationList* decl_list,
                                               PublishData *declarations) {

  // Publish pointers from entity statement part
  //  entitydecl->_publish_cc_object_pointers_init( _cc_out, declarations );
  // Publish pointers from architecture statement part
  TyvisArchitectureStatement* conc_stmt =  
          dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->first());

  while(conc_stmt != NULL) {
    conc_stmt->_publish_cc_concurrent_stmt_init( _cc_out, decl_list, declarations );
    conc_stmt = dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->successor(conc_stmt));
  }
}

void
Tyvis::_publish_cc_destructor( published_file &_cc_out, 
                                     TyvisArchitectureStatementList* conc_stmt_list) {
  
  TyvisArchitectureStatement* conc_stmt = dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->first());
  
  _cc_out << _get_cc_elaboration_class_name() << "::~"
	  << _get_cc_elaboration_class_name() << "(){\n";
  while (conc_stmt != NULL) {
    switch(conc_stmt->get_kind()) {
    case IIR_PROCESS_STATEMENT:
      if(get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT) {
	_cc_out << "delete ";
	_cc_out << " [] ";
	
	if (conc_stmt->_get_mangled_label() != NULL) {
	  _cc_out.get_stream() << *(conc_stmt->_get_mangled_label());
	}
	else {
	  _cc_out << "ANON_PROCESS" << conc_stmt;
	}
	_cc_out << "_elab_obj;" << NL();
      }
      break;
      
    case IIR_COMPONENT_INSTANTIATION_STATEMENT:
    case IIR_BLOCK_STATEMENT:
    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
      _cc_out << "delete ";
      if( get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT ){
	_cc_out << " [] ";
      }
      conc_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out.get_stream() );
      _cc_out << "_elab_obj;" << NL();
      break;
    case IIR_SIMPLE_SIMULTANEOUS_STATEMENT:
    case IIR_SIMULTANEOUS_IF_STATEMENT:
      // No need to do anything for this case
      break;
    default:
      cerr << "ERROR!!!!! _publish_cc_destructor( _cc_out ): unknown conc_statement "
	   << "type |" << conc_stmt->get_kind_text() << "| in arch" << endl;
    }
    
    conc_stmt = dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->successor(conc_stmt));
  }
  _cc_out << "}\n";
}

void
Tyvis::_publish_cc_anonymous_drivers( published_file &_cc_out, TyvisDeclarationList* decl_list, PublishData *declarations ){
  const string old_current_publish_name = _get_current_publish_name();
  _set_current_publish_name( ", SourceBase::ANONYMOUS_PROCESS_ID);\n" );
  
  CC_REF( _cc_out, "Tyvis::_publish_cc_anonymous_drivers" );

  TyvisDeclaration *current_decl = dynamic_cast<TyvisDeclaration *>(decl_list->first());
  while( current_decl != NULL ){
    switch( current_decl->get_kind() ){
    case IIR_SIGNAL_DECLARATION:{
      if ( dynamic_cast<TyvisSignalDeclaration *>(current_decl)->get_value() &&
	   !dynamic_cast<TyvisSignalDeclaration *>(current_decl)->_is_passed_through_out_port() ){
	current_decl->_publish_cc_addChild( _cc_out, declarations );
      }
      break;
    }
    case IIR_SIGNAL_INTERFACE_DECLARATION:{
      if ( dynamic_cast<TyvisSignalInterfaceDeclaration *>(current_decl)->get_value() ){
	current_decl->_publish_cc_addChild( _cc_out, declarations );
      }
      break;
    }
    default:
      break;
    }
    current_decl = dynamic_cast<TyvisDeclaration *>(decl_list->successor(current_decl));
  }

  _set_current_publish_name( old_current_publish_name );
}

void
Tyvis::_publish_cc_copy_generics_to_globals( published_file &_cc_out, PublishData *declarations ) {
  ASSERT ( dynamic_cast<TyvisConcurrentStatement *>(this) != NULL ||
	   get_kind() == IIR_ENTITY_DECLARATION );
  if (_get_generic_list() != NULL && _get_generic_list()->first() != NULL) {
    _cc_out << "void" << NL()
	    << _get_cc_elaboration_class_name()
	    << OS("::copyGenericsToGlobals(){");
    _get_generic_list()->_publish_cc_copy_generics_to_globals( _cc_out, declarations );
    _cc_out << CS("}");
  }
}

void
Tyvis::_publish_cc_reference_info( published_file &_cc_out, const char *method_info, int line_number ){
  if (gen_cc_ref == TRUE) {
    string info( method_info + string("(") + intToString( line_number ) + ") on ");
    if( get_file_name() == NULL ){
      info += "NULL";
    }
    else{
      info += _get_file_name()->_to_string();
    }
    info += "(" + intToString( get_line_number() ) +")";
    _cc_out.insert_comment( info );
  }
}

void
Tyvis::_publish_cc_dump( published_file &_cc_out ){

  CC_REF( _cc_out, "Tyvis::_publish_cc_dump" );;

  _cc_out.get_stream() << "// Kind = " << get_kind_text() << endl;
}

void 
Tyvis::_publish_cc_include( published_file & ){
  // By default we're going to do nothing...
} 

void 
Tyvis::_publish_cc_include_decls( published_file & ){
  _report_undefined_fn( "_publish_cc_include_decls" );
} 

void 
Tyvis::_publish_cc_include_elab( published_file & ){
  _report_undefined_fn( "_publish_cc_include_elab" );
} 

void 
Tyvis::_publish_cc_include( published_file &os, 
                            const string &file_to_include,
                            bool system_include ){
  //  _publish_cc_include( os, file_to_include, os.get_file_name( os ), system_include );
  ASSERT( file_to_include != "" );
  ASSERT( file_to_include != ".hh" );
  if( file_to_include == "_savant_entity_elab.hh" ){
    os.add_include( string("tyvis/") + file_to_include, true );
  }
  else{
    os.add_include( file_to_include, system_include );
  }
}

void
Tyvis::_publish_cc_ams_function(published_file &, PublishData *) {
  _report_undefined_fn("_publish_cc_ams_function()");
}

const string
Tyvis::_get_cc_elaboration_class_name(){
  string retval;

  ostringstream binding_name_stream;
  _publish_cc_binding_name( binding_name_stream );
  retval += binding_name_stream.str();
  retval += "_elab";
  
  return retval;
}

TyvisArchitectureDeclaration *
Tyvis::_get_cc_instantiated_architecture(){
  _report_undefined_fn("_get_cc_instantiated_architecture()");
  return 0;
}

TyvisEntityDeclaration *
Tyvis::_get_cc_instantiated_entity(){
  _report_undefined_fn("_get_cc_instantiated_entity()");
  return 0;
}

void
Tyvis::_publish_cc_savantnow_process_ptr( published_file &_cc_out ){
  CC_REF( _cc_out, "Tyvis::_publish_cc_savant_now_process_ptr" );
  _publish_cc_savantnow( _cc_out, "processPtr->" );
}

void
Tyvis::_publish_cc_savantnow_no_process_ptr( published_file &_cc_out ){
  CC_REF( _cc_out, "Tyvis::_publish_cc_savant_now_no_process_ptr" );
  _publish_cc_savantnow( _cc_out, "" );
}

void
Tyvis::_publish_cc_array_index( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "Tyvis::_publish_cc_array_index" );
  _publish_cc_rvalue( _cc_out, declarations );
}

void
Tyvis::_publish_cc_savantnow( published_file &_cc_out, const string &prefix ){
  _cc_out << "#ifdef savantnow" << NL()
	  << "#undef savantnow" << NL()
	  << "#define savantnow PhysicalLiteral(std_standard_timeTypeInfo(), "
	  <<  prefix << "getTimeNow().getMajor() )" << NL()
	  << "#endif" << NL();
}

TyvisTextLiteral *
Tyvis::_get_mangled_declarator() {
  _report_undefined_fn("_get_mangled_declarator()");
  return NULL;
}

void
Tyvis::_publish_make_reference_info( published_file &_cc_out,
					char *method_info, int line_number ){
  if( gen_cc_ref == true ){
    _cc_out << "\n# " << method_info << "(" << line_number << ") on ";
    if( get_file_name() == NULL ){
      _cc_out << "NULL";
    }
    else{
      _cc_out.get_stream() << _get_file_name();
    }
    _cc_out << "(" << get_line_number() << ")\n";
  }
}


TyvisTextLiteral *
Tyvis::_get_declarator() {
  cout << get_kind_text() << "::_get_declarator() called" << endl;

  ASSERT( dynamic_cast<TyvisDeclaration *>(this) != NULL || 
          get_kind() == IIR_SIMPLE_NAME);
  _report_undefined_fn("_get_declarator()");
  return NULL;
}

TyvisList *
Tyvis::_get_statement_list(){
  _report_undefined_fn("_get_statement_list()");

  return NULL;
}

Tyvis * 
Tyvis::_transmute() {
  _report_undefined_fn("_transmute()");
  return NULL;
}

void 
Tyvis::_add_decl_into_declaration_collection(PublishData *) {
  if( is_literal() == FALSE && 
      dynamic_cast<TyvisElementDeclaration *>(this) == NULL &&
      get_kind() != IIR_PHYSICAL_UNIT && 
      get_kind() != IIR_PHYSICAL_LITERAL &&
      get_kind() != IIR_TYPE_CONVERSION ){
    _report_undefined_fn("_add_decl_into_declaration_collection(declarations)");
  }
}

void 
Tyvis::_report_undefined_fn(char *fn_name) {
  string err( string(fn_name) + " not defined for Tyvis node: " + get_implementation_class() );
  report_error( this, err );
  abort();
}

TyvisTypeDefinition *
Tyvis::_get_subtype() {
  IIR_TypeDefinition *subtype = get_subtype();
  return dynamic_cast<TyvisTypeDefinition *>(subtype);
}

static TyvisTypeDefinition *_aggregate_iterator_subtype = NULL;
TyvisTypeDefinition *_get_aggregate_iterator_subtype(){
  return _aggregate_iterator_subtype;
}
void _set_aggregate_iterator_subtype( TyvisTypeDefinition *new_type ){
  _aggregate_iterator_subtype = new_type;
}

static int _aggregate_iterator_counter = 0;
int _get_aggregate_iterator_counter(){
  return _aggregate_iterator_counter;
}
string &_get_current_configuration_name(){
  static string _current_configuration_name = "";
  return _current_configuration_name;
}

void _set_current_configuration_name( string new_configuration_name ){
  _get_current_configuration_name() = new_configuration_name;
}

string &_get_current_architecture_name(){
  static string _current_architecture_name = "";
  return _current_architecture_name;
}

void _set_current_architecture_name( string new_name ){
  _get_current_architecture_name() = new_name;
}

string &_get_current_entity_name(){
  static string _current_entity_name = "";
  return _current_entity_name;
}
void _set_current_entity_name( string new_name ){
  _get_current_entity_name() = new_name;
}

static IIR_Int32 _number_of_processes = 0;
IIR_Int32 _get_number_of_processes(){
  return _number_of_processes;
}
void _set_number_of_processes( IIR_Int32 new_number ){
  _number_of_processes = new_number;
}

string &_get_current_suffix_name(){
  static string _current_suffix_name = "";
  return _current_suffix_name;
}

void _set_current_suffix_name( string new_name ){
  _get_current_suffix_name() = new_name;
}

static IIR_Int32 _index_level = 0;
IIR_Int32 _get_index_level(){
  return _index_level;
}

void _set_index_level( IIR_Int32 new_level ){
  _index_level = new_level;
}

static TyvisProcessStatement* _current_process_statement = NULL;
TyvisProcessStatement *_get_current_process_statement(){
  return _current_process_statement;
}
void _set_current_process_statement( TyvisProcessStatement *new_stmt ){
  _current_process_statement = new_stmt;
}

string &_get_current_another_name(){
  static string _current_another_name = "";
  return _current_another_name;
}

void _set_current_another_name( string new_name ){
  _get_current_another_name() = new_name;
}

static IIR_Boolean _allocate_return_value = FALSE;
IIR_Boolean _get_allocate_return_value(){
  return _allocate_return_value;
}
void _set_allocate_return_value( IIR_Boolean new_value ){
  _allocate_return_value = new_value;
}

string &_get_publish_prefix_string(){
  static string _publish_prefix_string = "";
  return _publish_prefix_string;
}
void _set_publish_prefix_string( string new_string ){
  _get_publish_prefix_string() = new_string;
}

static IIR::PublishedUnit _currently_publishing_unit = IIR::NONE_PARSING;
IIR::PublishedUnit _get_currently_publishing_unit(){
  return _currently_publishing_unit;
}

void _set_currently_publishing_unit( IIR::PublishedUnit new_unit ){
  _currently_publishing_unit = new_unit;
}

static IIR_Boolean _publish_object_without_prefix = FALSE; 
IIR_Boolean _get_publish_object_without_prefix(){
  return _publish_object_without_prefix;
}
void _set_publish_object_without_prefix( IIR_Boolean new_value ){
  _publish_object_without_prefix = new_value;
}

// Following are global variables.  We're giving them file static scope so
// as to not be accessed directly.  Eventually these should all disappear.
string &_get_current_publish_name(){
  static string _current_publish_name = "";
  return _current_publish_name;
}
void _set_current_publish_name( string new_name ){
  _get_current_publish_name() = new_name;
}

void _set_aggregate_iterator_counter( int new_counter ){
  _aggregate_iterator_counter = new_counter;
}

static Tyvis *_current_publish_node = NULL;
Tyvis *_get_current_publish_node(){
  return _current_publish_node;
}

void _set_current_publish_node( Tyvis *new_publish_node ){
  _current_publish_node = new_publish_node;
}

string &_get_current_elab_name(){
  static string _current_elab_name = "";
  return _current_elab_name;
}
void _set_current_elab_name( string new_name ){
  _get_current_elab_name() = new_name;
}

IIR_Boolean 
Tyvis::_is_resolved_type() {
  return FALSE;
}

IIR_Boolean
Tyvis::_is_currently_publishing_generate_for() {
  return (_currently_publishing_unit == GENERATE_FOR);
}

IIR_Boolean
Tyvis::_is_currently_publishing_subprogram() {
  return (_currently_publishing_unit == FUNCTION ||
	  _currently_publishing_unit == PROCEDURE);
}

void
Tyvis::_group_component_instantiations(TyvisArchitectureStatementList* conc_stmt_list, int blockSize) {
  // This method groups (blockSize) number of component instantiations into
  // a single implicit block. This was done to reduce the size of the published
  // individual elaboration class. Just a neat trick to get things going -DJ
  int componentInstantiationCount               = conc_stmt_list->_get_number_of_component_instantiations();
  int componentCounter                          = 0;
  TyvisBlockStatement *newBlock             = NULL;
  TyvisArchitectureStatement* conc_stmt     = 
    dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->first());
  TyvisArchitectureStatement* next_stmt     = NULL;
  
  while (componentInstantiationCount > blockSize) {
    componentCounter = 0;
    newBlock         = new TyvisBlockStatement();
    copy_location( this, newBlock );
    newBlock->set_declarative_region( this );
    
    while ((componentCounter < blockSize) && (conc_stmt != NULL))  {
      next_stmt = dynamic_cast<TyvisArchitectureStatement *>(conc_stmt_list->successor(conc_stmt));
      if (conc_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
	conc_stmt_list->remove(conc_stmt);
	
	newBlock->get_block_statement_part()->append(conc_stmt);
	conc_stmt->set_declarative_region(newBlock);
	
	componentCounter++;
	componentInstantiationCount--;
	
	ASSERT ( conc_stmt_list->_get_number_of_component_instantiations() ==
		 componentInstantiationCount );
      }
      
      conc_stmt = next_stmt;
    }

    conc_stmt_list->append(newBlock);
  }
}

Tyvis *
Tyvis::_get_declarative_region(){
  _report_undefined_fn("_get_declarative_region");  
  return 0;
}

void
Tyvis::_publish_cc_addToFanOut( published_file &_cc_out,
				TyvisProcessStatement *process,
				PublishData *declarations ) {
  CC_REF( _cc_out, "Tyvis::_publish_cc_addToFanOut" );
  _get_declarative_region()->_publish_cc_scoping_prefix( _cc_out.get_stream(),
							 _get_declarative_region(),
							 _get_declarative_region() );
  _publish_cc_object_name( _cc_out, declarations );
  _cc_out << ".addToFanOut( "
	  << process->_get_cc_process_class() + "_elab_obj"
	  << " );" << NL();
}

const string 
Tyvis::_to_string(){
  string retval = "";

  ostringstream stream;
  print( stream );
  retval += stream.str();

  return retval;
}

IIR_Mode
Tyvis::_get_mode() {
  _report_undefined_fn("_get_mode()");
  return IIR_UNKNOWN_MODE;
}

TyvisGenericList *
Tyvis::_get_generic_list(){
  _report_undefined_fn("_get_generic_list()");
  return NULL;
}

TyvisLibraryUnit *
Tyvis::_get_entity_aspect(){
  _report_undefined_fn("_get_entity_aspect()");
  return NULL;
}

TyvisDeclarationList*
Tyvis::_get_declaration_list() {
  _report_undefined_fn("_get_declaration_list()");
  return NULL;
}

IIR_Boolean
Tyvis::_is_published_attribute_in_state(SignalAttribute) {
  _report_undefined_fn("_is_published_attribute_in_state()");
  return FALSE;
}

void 
Tyvis::_add_published_attribute_in_state(SignalAttribute) {
  _report_undefined_fn("_add_published_attribute_in_state()");
}

IIR_Boolean
Tyvis::_is_published_attribute_in_constructor(SignalAttribute) {
  _report_undefined_fn("_is_published_attribute_in_constructor()");
  return FALSE;
}

void 
Tyvis::_add_published_attribute_in_constructor(SignalAttribute) {
  _report_undefined_fn("_add_published_attribute_in_constructor()");
}

IIR_Boolean
Tyvis::_is_published_attribute_in_initstate(SignalAttribute) {
  _report_undefined_fn("_is_published_attribute_in_initstate()");
  return FALSE;
}

void 
Tyvis::_add_published_attribute_in_initstate(SignalAttribute) {
  _report_undefined_fn("_add_published_attribute_in_initstate()");
}

TyvisDeclaration* 
Tyvis::_get_prefix_declaration() {
  _report_undefined_fn("_get_prefix_declaration()");
  return NULL;
}

Tyvis *
Tyvis::_get_component_name( ){
  _report_undefined_fn("_get_component_name()");
  return NULL;
}

TyvisLibraryDeclaration *
Tyvis::_get_work_library(){
  ASSERT( get_design_file() != 0 );
  return dynamic_cast<TyvisDesignFile *>(get_design_file())->_get_work_library();
}

void 
Tyvis::_build_sensitivity_list(TyvisDesignatorList*) {
  if( get_kind() != IIR_PHYSICAL_LITERAL &&
      get_kind() != IIR_ALLOCATOR ){
    _report_undefined_fn("_build_sensitivity_list()");
  }
}

void
Tyvis::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *) {
  // do Nothing.
}

IIR_Boolean
Tyvis::_is_longest_static_prefix() { 
  return _is_static_expression();
}

IIR_Boolean 
Tyvis::_is_globally_static_primary(){
  // If no overrided version of this got called, we're going to fall back
  // to calling "is_locally_static".  See 7.4.2 in the '93 LRM for more
  // info.
  return is_locally_static();
}

void 
Tyvis::_get_list_of_input_signals(  savant::set<Tyvis> * ){
  _report_undefined_fn("_get_list_of_input_signals()");
}

void 
Tyvis::_get_signal_source_info(  savant::set<Tyvis> * ){
  _report_undefined_fn("_get_signal_source_info()");
}

void
Tyvis::_set_passed_through_out_port(IIR_Boolean) {
  _report_undefined_fn("_set_passed_through_out_port(IIR_Boolean)");
}

TyvisPortList *
Tyvis::_get_port_list(){
  _report_undefined_fn("_get_port_list()");
  return NULL;
}

TyvisFunctionDeclaration*
Tyvis::_get_resolution_function(){
  return NULL;
}

void
Tyvis::_set_stmt_node_index(IIR_Int32 *, bool, bool&) {
  // No need to do anything
  return;
}

void 
Tyvis::_set_stmt_qty_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *, PublishData *, PublishData *) {
  // No need to do anything
  return;
}

IIR_Int32
Tyvis::_get_stmt_qty_index() {
  return -1;
}

TyvisDeclaration* 
Tyvis::_get_package_declaration() {
  return NULL;
}

TyvisIdentifier *
Tyvis::_get_file_name() {
  return dynamic_cast<TyvisIdentifier *>(get_file_name());
}

void
Tyvis::_flush_stmt_index() {
  // No need to do anything
  return;
}

IIR_Boolean
Tyvis::_reference_quantity_found() {
  return FALSE;
}

IIR_Boolean
Tyvis::_contribution_quantity_found() {
  return FALSE;
}

IIR_Boolean
Tyvis::_differential_quantity_found() {
  return FALSE;
}

void
Tyvis::_build_contribution_quantity_list(dl_list<TyvisContributionAttribute> *) {
  // do nothing
}

void
Tyvis::_build_differential_quantity_list(dl_list<TyvisDotAttribute> *) {
  // do nothing
}

void
Tyvis::_copy_symbols_defined_in_enclosing_scope(PublishData *current, PublishData *outer) {
  TyvisDeclaration *decl;

  std::set<IIR_Declaration *> *decl_set = current->get_set(TyvisDeclaration::CONSTANT);
  for (std::set<IIR_Declaration *>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ((decl->_get_declarative_region() != this) && (!outer->in_collection(decl))){
      outer->add_declaration(decl);
    }
  }
  
  decl_set = current->get_set(TyvisDeclaration::SIGNAL);
  for (std::set<IIR_Declaration *>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ((decl->_get_declarative_region() != this) && (!outer->in_collection(decl))){
      outer->add_declaration(decl);
    }
  }

  decl_set = current->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  for (std::set<IIR_Declaration *>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ((decl->_get_declarative_region() != this) && (!outer->in_collection(decl))){
      outer->add_declaration(decl);
    }
  }
  
  decl_set = current->get_set(TyvisDeclaration::VARIABLE);
  for (std::set<IIR_Declaration *>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ((decl->_get_declarative_region() != this) && (!outer->in_collection(decl))){
      outer->add_declaration(decl);
    }
  }
  
  decl_set = current->get_set(TyvisDeclaration::ALIAS);
  for (std::set<IIR_Declaration *>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ((decl->_get_declarative_region() != this) && (!outer->in_collection(decl))){
      outer->add_declaration(decl);
    }
  }
  
  decl_set = current->get_set(TyvisDeclaration::ATTRIBUTE);
  for (std::set<IIR_Declaration *>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ((decl->_get_declarative_region() != this) && (!outer->in_collection(decl))){
      outer->add_declaration(decl);
    }
  }

  decl_set = current->get_set(TyvisDeclaration::S_FILE);
  for (std::set<IIR_Declaration *>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ((decl->_get_declarative_region() != this) && (!outer->in_collection(decl))){
      outer->add_declaration(decl);
    }
  }
}

TyvisAttributeSpecification*
Tyvis::_get_attribute_specification(Tyvis*) {
  _report_undefined_fn("_get_attribute_specification()");
  return NULL;
}

IIR_Int32
Tyvis::_get_stmt_signal_index() {
  return -1;
}

void 
Tyvis::_set_stmt_signal_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *) {
  // No need to do anything
  return;
}


TyvisDeclaration *
Tyvis::_find_in_implicit_list( const string to_find ){
  TyvisDeclaration *current_decl;
  
  if( _get_implicit_declarations() != NULL ){
    current_decl = _get_implicit_declarations()->getElement();
    while( current_decl != NULL ){
      if( IIRBase_TextLiteral::cmp( current_decl->_get_declarator(), const_cast<char *>(to_find.c_str()) ) ==0){
        return current_decl;
      }
      current_decl = _get_implicit_declarations()->getNextElement();
    }
  }

  return NULL;
}

 savant::set<TyvisDeclaration> *
Tyvis::_get_implicit_declarations( ){
  _report_undefined_fn("get_implicit_declarations( )");
  return NULL;
}

void 
Tyvis::_set_implicit_declarations(  savant::set<TyvisDeclaration> * ){
  _report_undefined_fn("set_implicit_declarations(  savant::set<TyvisDeclaration> * )"); 
}

void
Tyvis::set_is_currently_publishing_simultaneous_if_stmt(bool newValue) { 
  _currently_publishing_simultaneous_if_stmt = newValue;
}

void 
Tyvis::set_is_currently_publishing_simultaneous_stmt(bool newValue) {
  _currently_publishing_simultaneous_stmt = newValue;
}

IIR_Boolean
Tyvis::get_is_currently_publishing_simultaneous_if_stmt() {
  return _currently_publishing_simultaneous_if_stmt;
}

IIR_Boolean 
Tyvis::get_is_currently_publishing_simultaneous_stmt() { 
  return _currently_publishing_simultaneous_stmt;
}

Tyvis *
Tyvis::_get_current_declarative_region(){
  ASSERT(0);
  return NULL;
}

TyvisAttributeSpecificationList * 
Tyvis::_get_attribute_specification_list( ){
  return NULL;
}

void
Tyvis::_build_generic_parameter_set( savant::set<TyvisDeclaration> *) {
  // do nothing
}

void
Tyvis::_build_reference_quantity_list(dl_list<TyvisReferenceAttribute> *) {
  // do nothing
}

void
Tyvis::_publish_cc_subprogram_call_name( published_file &, PublishData * ){
  _report_undefined_fn("_publish_cc_subprogram_call_name");
}

Tyvis *
Tyvis::_get_value() {
  return dynamic_cast<Tyvis *>(get_value());
}

published_file &
operator<<(published_file &pf, IIR &is ) {
  is.print( pf.get_stream() );
  return pf;
}

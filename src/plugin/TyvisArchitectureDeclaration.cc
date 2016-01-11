
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

#include "TyvisArchitectureDeclaration.hh"
#include "TyvisArchitectureStatement.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisAliasDeclaration.hh"
#include "TyvisAssociationElement.hh"
#include "TyvisAttribute.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisBlockStatement.hh"
#include "TyvisBranchQuantityDeclaration.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisConcurrentGenerateIfStatement.hh"
#include "TyvisConcurrentGenerateForStatement.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisDeclaration.hh"
#include "TyvisDesignator.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisFileDeclaration.hh"
#include "TyvisFreeQuantityDeclaration.hh"
#include "TyvisGenericList.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisLabel.hh"
#include "TyvisName.hh"
#include "TyvisPortList.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisUseClause.hh"
#include "TyvisWaitStatement.hh"

#include "PublishData.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include <sstream>
#include "savant/language_processing_control.hh"
#include <cstdlib> // for rand()

extern language_processing_control *lang_proc;

TyvisArchitectureDeclaration::TyvisArchitectureDeclaration()
{
  _arch_declarations = new PublishData();
  set_architecture_declarative_part(new TyvisDeclarationList());
  set_architecture_statement_part(new TyvisArchitectureStatementList());
}

TyvisArchitectureDeclaration::~TyvisArchitectureDeclaration()
{
  delete _arch_declarations;
  delete get_architecture_declarative_part();
  delete get_architecture_statement_part();
}

void 
TyvisArchitectureDeclaration::_publish_cc(){
  Tyvis* temp = _get_current_publish_node();
  const string old_current_publish_name = _get_current_publish_name();

  _set_currently_publishing_unit(ARCHITECTURE_DECL);
  _set_current_publish_node( this );
  _set_current_entity_name( _get_entity()->_get_mangled_declarator()->convert_to_string() );
  _set_current_architecture_name( _get_mangled_declarator()->convert_to_string() );

  cerr << "Publishing design unit: " << _get_current_entity_name()
       << "(" << _get_current_architecture_name() << ")\n";

  _set_current_publish_name( "" );

  // Check and group component instantiation statements for large designs
  _group_component_instantiations(_get_architecture_statement_part(), 300);
  
  _publish_cc_declarations( _declarations );
  _publish_cc_makefile_stub();

  _set_current_publish_name( old_current_publish_name );
  _set_current_publish_node( temp );
  
  _set_currently_publishing_unit(NONE);
}

TyvisArchitectureDeclaration *
TyvisArchitectureDeclaration::_get_cc_instantiated_architecture(){
  return this;
}

TyvisEntityDeclaration *
TyvisArchitectureDeclaration::_get_cc_instantiated_entity(){
  return _get_entity();
}

void 
TyvisArchitectureDeclaration::_publish_cc_declarations(PublishData *declarations){
  // Publish the header file
  published_header_file header_file( _get_library_directory(), 
				     _get_cc_design_unit_name(),
				     this );
  CC_REF( header_file, "TyvisArchitectureDeclaration::_publish_cc_declarations" );
  _publish_cc_include_decls( header_file );
  _get_context_items()->_publish_cc( header_file, declarations );
  Tyvis::_publish_cc_include( header_file, "tyvis/STDTypes.hh" );

  CC_REF( header_file, "TyvisArchitectureDeclaration::_publish_cc_declarations" );
  _get_architecture_declarative_part()->_publish_cc( header_file, declarations );
  _get_architecture_statement_part()->_publish_cc_ams_function_prototype(header_file);

  // Publish the body
  published_cc_file cc_file( _get_library_directory(), 
			     _get_cc_design_unit_name(),
			     this );
  CC_REF( cc_file, "TyvisArchitectureDeclaration::_publish_cc_declarations" );
  _publish_cc_include_elab( cc_file );
  _publish_cc_include_decls( cc_file );
  CC_REF( cc_file, "TyvisArchitectureDeclaration::_publish_cc_declarations" );
  Tyvis::_publish_cc_include( cc_file, "tyvis/VHDLProcess.hh" );
  CC_REF( cc_file, "TyvisArchitectureDeclaration::_publish_cc_declarations" );
  _get_architecture_declarative_part()->_publish_cc_decl( cc_file, declarations );
  CC_REF( cc_file, "TyvisArchitectureDeclaration::_publish_cc_declarations" );
  _get_architecture_statement_part()->_publish_cc( cc_file, declarations );
  _get_architecture_statement_part()->_publish_cc_ams_function_body(cc_file);
  TyvisConcurrentStatement *conc_stmt = dynamic_cast <TyvisConcurrentStatement *>
    (get_architecture_statement_part()->first());
  while(conc_stmt != NULL) {
    if(conc_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      (dynamic_cast <TyvisProcessStatement *>(conc_stmt))->
        _publish_cc_ams_breakExpression_function_prototype(header_file);
    }
    conc_stmt =  dynamic_cast <TyvisConcurrentStatement *>
      (get_architecture_statement_part()->successor(conc_stmt));
  }
  conc_stmt = dynamic_cast <TyvisConcurrentStatement *>
    (get_architecture_statement_part()->first());
  while(conc_stmt != NULL) {
    if(conc_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      (dynamic_cast <TyvisProcessStatement *>(conc_stmt))->
        _publish_cc_ams_break_function_include(cc_file);
      dynamic_cast<TyvisProcessStatement*>(conc_stmt)->
        _publish_cc_ams_breakExpression_function_body(cc_file, declarations);
    }
    conc_stmt =  dynamic_cast <TyvisConcurrentStatement *>
      (get_architecture_statement_part()->successor(conc_stmt));
  }
}

void
TyvisArchitectureDeclaration::_publish_cc_makefile_stub(){
  ASSERT( _get_entity() != 0 );
  // Dump the entity's Makefile stub
  _get_entity()->_publish_cc_makefile_stub();
  // Dump the architecture's Makefile stub
  TyvisLibraryUnit::_publish_cc_makefile_stub();
}  

void
TyvisArchitectureDeclaration::_publish_cc_elaborate( ){
  Tyvis* temp_node                        = _get_current_publish_node();
  const string old_current_publish_name         = _get_current_publish_name();
  PublishedUnit old_publish_unit                = _get_currently_publishing_unit();

  _set_current_publish_node( this );
  _set_current_publish_name( "" );
  _set_currently_publishing_unit(ARCHITECTURE_DECL);
  _set_current_configuration_name( "" );

  _set_current_entity_name( _get_entity()->_get_mangled_declarator()->convert_to_string() );
  _set_current_architecture_name( _get_mangled_declarator()->convert_to_string() );
  
  _publish_cc_headerfile( _declarations );
  _publish_cc_ccfile( _declarations, _arch_declarations );

  _set_current_publish_node( temp_node );
  _set_current_publish_name( old_current_publish_name );
  _set_currently_publishing_unit(old_publish_unit);
}

void
TyvisArchitectureDeclaration::_publish_cc_blocks_elaborate( published_file &_cc_out,
							    PublishData *declarations ) {
  TyvisArchitectureStatement* arch_stmt = NULL;
  const string temp = _get_current_publish_name();
 
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_blocks_elaborate" );

  _set_current_publish_name( "" );
  
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_architecture_statement_part()->first());
  while (arch_stmt != NULL) {
    if (arch_stmt->get_kind() == IIR_BLOCK_STATEMENT |
        arch_stmt->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT |
        arch_stmt->get_kind() == IIR_CONCURRENT_GENERATE_IF_STATEMENT) {
      arch_stmt->_publish_cc_elaborate( _cc_out, declarations );
    }
    
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_architecture_statement_part()->successor(arch_stmt));
  }
  _set_current_publish_name( temp );
}

void
TyvisArchitectureDeclaration::_publish_cc_headerfile( PublishData *declarations ){
  // Publish the .hh file
  published_header_file header_file( _get_library_directory(), 
				     _get_cc_elaboration_class_name(),
				     this );
  CC_REF( header_file, "TyvisArchitectureDeclaration::_publish_cc_headerfile" );
  Tyvis::_publish_cc_include( header_file, "tyvis/STDTypes.hh" );
  
  if( lang_proc->processing_vhdl_ams() ){
    _publish_cc_ams_includes( header_file );
  }

  // Request the Hierarchy.hh for our hierarchy stuff
  Tyvis::_publish_cc_include( header_file, "tyvis/Hierarchy.hh" );

  // Request AMSType.hh if VHDL-AMS constructs are invoked.
  if( lang_proc->processing_vhdl_ams() ) {
    Tyvis::_publish_cc_include( header_file, "tyvis/AMSType.hh" );
  }

  // Just include the decls file which has all the required includes
  // _publish_cc_include_decls( header_file );
  Tyvis::_publish_cc_include( header_file, _get_cc_design_unit_name() + ".hh" );
  _publish_cc_class_includes( header_file, _get_architecture_statement_part() );
  _publish_cc_class( header_file, declarations );
}

void
TyvisArchitectureDeclaration::_publish_cc_class( published_file &_cc_out,
						 PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_class" );

  _get_entity()->_publish_cc_include_elab( _cc_out );
  _cc_out << "class " << _get_cc_elaboration_class_name() 
	  << " : public "
	  << OS( _get_entity()->_get_cc_elaboration_class_name()
		 + "{" )
	  << "public:" << NL();

  // Publish the constructor
  _cc_out << _get_cc_elaboration_class_name() << "(";
  if(_get_entity()->_get_port_clause()->_publish_cc_unconstrained_ports( _cc_out,
								      declarations,
								      TRUE, FALSE, FALSE))
     _cc_out << ",";
  _cc_out << "const std::string& name"
          << ");" << NL();

  if(_get_entity()->get_generic_clause()->size() > 0) {
    _cc_out << _get_cc_elaboration_class_name() << "(" << NL();
    _get_entity()->_get_generic_list()->_publish_generic_parameter_list( _cc_out, declarations );
    _get_entity()->_get_port_clause()->_publish_cc_unconstrained_ports( _cc_out,
									declarations,
									TRUE, FALSE, TRUE );
    _cc_out << " );" << NL();
  }

  // The destructor.
  _cc_out << "~" << _get_cc_elaboration_class_name() << "();\n" << NL()
	       << "virtual std::vector<std::shared_ptr<warped::Event>> receiveEvent( const warped::Event& ) override;" << NL()
	       << "virtual std::vector<std::shared_ptr<warped::Event>> initializeLP() override;" << NL()
	  << "virtual std::vector<std::shared_ptr<warped::Event>> assignSignal( const std::string name, const int value, unsigned int delay, unsigned int timestamp ) override;" << NL();

  if( lang_proc->processing_vhdl_ams() ){
    _publish_cc_ams_objects( _cc_out, declarations );
    _cc_out << "void formCharacteristicExpressions();\n";
    _cc_out << "void initializeSignals();\n";
    _cc_out << "void connectTerminals(int, ...);\n";
  }
  
  _get_architecture_declarative_part()->_publish_cc_constant_declarations( _cc_out, declarations );
  
  //Publish the signal info data members
  _publish_cc_signals( _cc_out, _get_architecture_declarative_part(), declarations );
  
  if( lang_proc->processing_vhdl_ams() ){
    _publish_cc_break_signal_info(_cc_out);
  }

  // Since the implicit declarations for the signals declared in the port
  // clause in the entity is NULL. But get_entity()->port_clause in the
  // architecture declaration gives implicit declarations for the signals
  // in the port clause of the entity also. we are going to publish the
  // implicit declarations for those signals here in the architecture
  _publish_cc_implicit_signals( _cc_out, _get_entity()->_get_port_clause(), declarations );

  // If we're a foreign arch, we'll publish that.  Otherwise we'll publish
  // process pointers.
  if( _is_foreign_architecture() ){
    _publish_cc_foreign_process_pointer( _cc_out );
  }
  else{
    // Publish the pointers to processes and objects used in this architecture
    // TODO: Here I find all the object of the subhierarchy that I need
    //_publish_cc_object_pointers( _cc_out, _get_architecture_statement_part(), declarations );
  }

  // Publish file objects.
  _get_architecture_declarative_part()->_publish_cc_file_objects( _cc_out, declarations );

  _cc_out << CS("};");

  // Publish the necessary type info structures that have been declared
  // within the scope of this architecture here...
  // Publish any constants declaraed here so that the other declarations
  // can use them without any trouble.
  _get_architecture_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisArchitectureDeclaration::_publish_cc_ccfile(PublishData *declarations, PublishData *arch_declarations){ 
  // Publish the .cc file
  published_cc_file cc_file( _get_library_directory(), 
			     _get_cc_elaboration_class_name(),
			     this );
  CC_REF( cc_file, "TyvisArchitectureDeclaration::_publish_cc_ccfile" );
  Tyvis::_publish_cc_include( cc_file, "tyvis/SourceBase.hh" );
  Tyvis::_publish_cc_include( cc_file, "cstdarg", true );
  _publish_cc_include_elab( cc_file );

  if( lang_proc->processing_vhdl_ams() ) {
    cc_file << "extern list <VHDLType *> globalFreeQuantityList;" <<NL();
  }

  _publish_cc_headerfiles_for_cc( cc_file );
  _publish_cc_constructor( cc_file, declarations, arch_declarations );
  _publish_cc_destructor( cc_file, _get_architecture_statement_part() );
  _publish_cc_instantiate( cc_file, declarations );
  _publish_cc_createNetInfo( cc_file, declarations );
  _publish_cc_connect( cc_file,declarations );
  _publish_cc_blocks_elaborate( cc_file, declarations );
  _publish_cc_allocate_module( cc_file );
  if( lang_proc->processing_vhdl_ams() ) {
    _publish_cc_ams_form_characteristic_expressions( cc_file );
    _publish_cc_ams_initialize_signals( cc_file, declarations );
    _publish_cc_ams_form_break_set(cc_file, declarations);
    _publish_cc_ams_connect_terminals( cc_file, declarations );
  }
}

void
TyvisArchitectureDeclaration::_publish_cc_headerfiles_for_cc( published_file &_cc_out ) {
  // Publish the header files for the binding architectures specified
  // in configuration specification.

  // Publish the forward references for the entity statement part
  TyvisArchitectureStatement *arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_architecture_statement_part()->first());
  while (arch_stmt != NULL) {
    // Currently the process name doesn't include its entity and architecture name. 
    switch ( arch_stmt->get_kind() ){
    case IIR_PROCESS_STATEMENT:
      arch_stmt->_publish_cc_include( _cc_out );
      break;
    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
      arch_stmt->_publish_cc_headerfiles_for_cc_default( _cc_out );
      break;
    case IIR_BLOCK_STATEMENT:
      arch_stmt->_publish_cc_include_elab( _cc_out );
      break;
    case IIR_COMPONENT_INSTANTIATION_STATEMENT:
      TyvisComponentInstantiationStatement *as_component_instantiation_stmt;
      as_component_instantiation_stmt = dynamic_cast<TyvisComponentInstantiationStatement *>(arch_stmt);
      TyvisAssociationElement *asselem;

      asselem = dynamic_cast<TyvisAssociationElement *>(as_component_instantiation_stmt->get_port_map_aspect()->first());
      while( asselem != NULL ){
	if (asselem->_get_actual() != NULL) {
	  ASSERT( asselem->get_formal() != NULL );
	  // Okay. The actual is not open
	  if ((asselem->get_formal()->get_kind() == IIR_FUNCTION_CALL) ||
	      (asselem->_get_actual()->get_kind() == IIR_FUNCTION_CALL)) {
	    Tyvis::_publish_cc_include( _cc_out, "tyvis/TypeConvert.hh" );
	    // We don't need to publish this multiple times.
	    break;
	  }
	}
	asselem = dynamic_cast<TyvisAssociationElement *>(as_component_instantiation_stmt->get_port_map_aspect()->successor(asselem));
      }
      break;
    default:
      // Nothing to be done for other statements
      break;
    }
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_architecture_statement_part()->successor(arch_stmt));
  }

  // Publish the header files for the processes defined in the entity
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(_get_entity()->get_entity_statement_part()->first());
  while (arch_stmt != NULL) {
    if (arch_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      // What about processes with no labels?  Also, if there is going to
      // be one more "get_kind" checked, go to a case statment.  Actually,
      // it's not even possible to have a process in the statements of an
      // entity, is it?  Check the LRM.
      arch_stmt->_get_mangled_label()->_publish_cc_include( _cc_out );
    }
    else {
      cerr << "TyvisArchitectureDeclaration_publish_cc_headefiles_for_cc( published_file &_cc_out ) - "
	   << "Un-handled concurrent statement (" << arch_stmt->get_kind()
	   << ") in entity\n";
    }

    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(_get_entity()->get_entity_statement_part()->successor(arch_stmt));
  }

  // Include the architecture decls header file
  _publish_cc_include_decls( _cc_out );
}

void
TyvisArchitectureDeclaration::_publish_cc_constructor( published_file &_cc_out, PublishData *declarations, PublishData *arch_declarations ) {

  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_constructor" );
 
  _publish_cc_constructor_with_no_arguments( _cc_out, declarations, arch_declarations );
  if (get_entity()->get_generic_clause()->size() > 0) {
    _publish_cc_constructor_with_arguments( _cc_out, declarations, arch_declarations );
  }
}

void
TyvisArchitectureDeclaration::_publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations, PublishData *arch_declarations ) {
  IIR_Boolean firstDeclFlag         = TRUE;
  int noOfUnconstrainedPorts        = 0;
  
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_constructor_with_no_arguments" );
 
  _cc_out << _get_cc_elaboration_class_name() << "::"
	  << _get_cc_elaboration_class_name() << OS("( ");
  noOfUnconstrainedPorts = 
    _get_entity()->_get_port_clause()->_publish_cc_unconstrained_ports( _cc_out, declarations, TRUE, TRUE, FALSE);
  _cc_out << CS(")");
  
  if (noOfUnconstrainedPorts > 0) {
    // Have to pass the parameters to the entity .ie. parent class...
    _cc_out << ":" << NL();
    _cc_out << OS(_get_entity()->_get_cc_elaboration_class_name() + "(");
    _get_entity()->_get_port_clause()->_publish_cc_unconstrained_ports( _cc_out, declarations, FALSE, TRUE, FALSE );
    _cc_out << CS(")");
  }
  
  _publish_cc_ams_objects_init(_cc_out, firstDeclFlag, declarations, arch_declarations);

  // Initialize all the type info structures first here...
  firstDeclFlag = _get_architecture_declarative_part()->_publish_cc_constant_definitions( _cc_out, declarations, firstDeclFlag);
  _publish_cc_signal_objects_init( _cc_out, declarations, firstDeclFlag );
  _cc_out << OS("{");
  _publish_cc_ams_form_global_quantity_list( _cc_out, declarations );
  _cc_out.insert_comment( "all of the global constants..." );
  _get_architecture_declarative_part()->_publish_cc_global_constants_assignments( _cc_out, declarations );
  _cc_out.insert_comment( "all of the file objects..." );
  _get_architecture_declarative_part()->_publish_cc_file_objects_init( _cc_out, declarations );
  _cc_out.insert_comment( "all of the object pointers..." );
  _publish_cc_object_pointers_init( _cc_out, declarations );
  _cc_out << CS("}");
}

void
TyvisArchitectureDeclaration::_publish_cc_constructor_with_arguments(published_file &_cc_out, PublishData *declarations, PublishData *arch_declarations){
  int numGenericClause = _get_entity()->get_generic_clause()->size();
  IIR_Boolean firstDeclFlag         = TRUE;
  
  CC_REF( _cc_out, 
		"TyvisArchitectureDeclaration::_publish_cc_constructor_with_arguments" );

  _cc_out << _get_cc_elaboration_class_name() << "::" << _get_cc_elaboration_class_name() 
	  << OS("(");
  _get_entity()->_get_generic_list()->_publish_generic_parameter_list( _cc_out, declarations );
  _get_entity()->_get_port_clause()->_publish_cc_unconstrained_ports( _cc_out, declarations, TRUE, TRUE, TRUE );
  _cc_out << CS(")");
  
  if(numGenericClause > 0) {
    CC_REF( _cc_out, 
		  "TyvisArchitectureDeclaration::_publish_cc_constructor_with_arguments" );
    _cc_out << ":" << NL()
	    << _get_entity()->_get_cc_elaboration_class_name() << OS("(");
    _get_entity()->_get_generic_list()->_publish_generic_parameters_notypes( _cc_out, declarations );
    _get_entity()->_get_port_clause()->_publish_cc_unconstrained_ports( _cc_out, declarations, FALSE, TRUE, TRUE);
    _cc_out << CS(")");
    firstDeclFlag = FALSE;
  }

  bool tempFlag = true;
  tempFlag = _get_architecture_declarative_part()->_publish_cc_constant_definitions( _cc_out, declarations, firstDeclFlag);
  firstDeclFlag = tempFlag && firstDeclFlag;

  _publish_cc_ams_objects_init(_cc_out, firstDeclFlag, declarations, arch_declarations);
  _publish_cc_signal_objects_init( _cc_out, declarations,firstDeclFlag );
  
  _cc_out << OS("{");
  _publish_cc_ams_form_global_quantity_list( _cc_out, declarations );
  _get_architecture_declarative_part()->_publish_cc_global_constants_assignments( _cc_out, declarations );
  _get_architecture_declarative_part()->_publish_cc_file_objects_init( _cc_out, declarations );
  _publish_cc_object_pointers_init( _cc_out, declarations );
  _cc_out << CS("}");
}

void
TyvisArchitectureDeclaration::_publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_object_pointer_init" );
 
  // we are setting the _current_publish_node to the architecture declaration
  // so that if during the _publish_cc_object_pointers_init for architecture declaration
  // the node needs to get the archicture declaration. it can use the _current
  // _publish_node variable.
  
  Tyvis *tempNode = _get_current_publish_node();
  _get_entity()->_publish_cc_object_pointers_init( _cc_out, declarations );

  _set_current_publish_node( this );
  Tyvis::_publish_cc_object_pointers_init( _cc_out, 
                                                 _get_architecture_statement_part(), 
                                                 _get_architecture_declarative_part(),
                                                 declarations);
  _set_current_publish_node( tempNode );
}

void
TyvisArchitectureDeclaration::_publish_cc_instantiate( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_instantiate" );
  _cc_out << "std::vector<std::shared_ptr<warped::Event>>" << NL()
	       << OS( _get_cc_elaboration_class_name() + "::initializeLP(){")
          << "std::cout << name_ << \": initialization\" << std::endl;" << NL()
          << "std::vector<std::shared_ptr<warped::Event>> response_events;" << NL()
          << "return response_events;"
          << CS("}\n");
}

void
TyvisArchitectureDeclaration::_publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_createNetInfo" );
  // AssignSignal method
  _cc_out << "std::vector<std::shared_ptr<warped::Event>>" << NL()
	       << OS( _get_cc_elaboration_class_name() + "::AssignSignal( const std::string name, const int value, const unsigned int delay, const unsigned int timestamp){")
          << "std::vector<std::shared_ptr<warped::Event>> response_events;" << NL()
          << "assert( signals_.find(name) != signals_.end() );" << NL()
          << OS("if ( delay == 0 ) {")
          << "signals_[name] = value;" << NL()
          << OS("if ( hierarchy.find(name) != hierarchy.end() ) {")
          << OS("for ( auto it = hierarchy.find(name)->second.begin(); it != hierarchy.find(name)->second.end(); it++) {")
          << "std::cout << name_ << \": send a message to \" << (*it).first << \" because the signal \" << (*it).second << \" has changed\" std::endl;" << NL()
          << "response_events.emplace_back(new SigEvent { (*it).first, value, (*it).second, timestamp });"
          << CS("}")
          << CS("}")
          << CS("}")
          << OS("else {")
          << "response_events.emplace_back(new SigEvent { name_, value, name, timestamp + delay });"
          << CS("}")
          << "return response_events;"
          << CS("}\n");

  // AssignSignal method
  _cc_out << "std::vector<std::shared_ptr<warped::Event>>" << NL()
	       << OS( _get_cc_elaboration_class_name() + "::receiveEvent( const warped::Event& event ){")
          << "std::cout << name_ << \": received a message from \" << event.sender_name_" << NL()
          << "\" at time \" << std::to_string(event.timestamp()) << \".\" << std::endl;" << NL()
          << "const SigEvent sign_event = static_cast<const SigEvent&>(event);" << NL()
          << "std::vector<std::shared_ptr<warped::Event>> response_events = AssignSignal(sign_event.signalName(), sign_event.Value(), 0, sign_event.timestamp());" << NL()
          << "return response_events;"
          << CS("}");
}

void
TyvisArchitectureDeclaration::_publish_cc_connect( published_file &_cc_out,
						   PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_connect" );

}

void
TyvisArchitectureDeclaration::_publish_cc_binding_name(ostream& outstream){
  outstream << _get_cc_design_unit_name();
}

void
TyvisArchitectureDeclaration::_publish_cc_signal_objects_init( published_file &_cc_out,
                                                                      PublishData *declarations,
                                                                      IIR_Boolean firstDeclFlag) {
  TyvisDeclaration* decl;
  bool first = 0;

  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_signal_objects_init" );
  
  decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->first());
  while (decl != NULL) {
    if(decl->get_kind() == IIR_SIGNAL_DECLARATION) {
      if( first == 0 && firstDeclFlag == TRUE ){
	_cc_out << ":" << NL();
	first = 1;
      }
      else {
	_cc_out << "," << NL();
      }
      decl->_publish_cc_signal_net_info( _cc_out, DEFINITION, declarations );
    }
    decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->successor(decl));    
  }
  
  // Publish the initializations for the implicit_declarations of the signals
  // in the port clause of the entity declartion.
  TyvisDeclarationList *port_clause_list = (_get_entity()->_get_port_clause());
  decl = dynamic_cast<TyvisDeclaration *>(port_clause_list->first());
  while ( decl != NULL ) {
    TyvisDeclaration *nextdecl = dynamic_cast<TyvisDeclaration *>(port_clause_list->successor(decl));
    if( decl->_get_implicit_declarations() != NULL
	&& decl->_get_implicit_declarations()->size() != 0) {
      TyvisDeclaration *imp_decl = decl->_get_implicit_declarations()->getElement();
      while(imp_decl != NULL) {
	if(imp_decl->is_signal() == TRUE) {
	  if( first == 0 && firstDeclFlag == TRUE ){
	    _cc_out << ":" << NL();
	    first = 1;
	  }
	  else {
	    _cc_out << "," << NL();
	  }
	  if( dynamic_cast<TyvisObjectDeclaration *>(imp_decl) != 0 ){
	    dynamic_cast<TyvisObjectDeclaration *>(imp_decl)->_publish_cc_declarator_with_constructor_args( _cc_out, declarations );
	  }
	}
	imp_decl = decl->_get_implicit_declarations()->getNextElement();
      }
    }
    decl = nextdecl;
  }
  TyvisConcurrentStatement *conc_stmt = dynamic_cast <TyvisConcurrentStatement *>
    (get_architecture_statement_part()->first());
  while (conc_stmt != NULL) {
    if (conc_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      TyvisProcessStatement *temp_proc = dynamic_cast <TyvisProcessStatement *>(conc_stmt);
      for (TyvisSequentialStatement* seqstmt = dynamic_cast<TyvisSequentialStatement *>(temp_proc->get_process_statement_part()->first());
          seqstmt != NULL;
          seqstmt = dynamic_cast<TyvisSequentialStatement *>(temp_proc->get_process_statement_part()->successor(seqstmt)) ) {
        ostringstream breakstr;
        if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
          ASSERT(seqstmt->_get_process_stmt_label() != NULL);
          breakstr <<dynamic_cast <TyvisProcessStatement *>(conc_stmt)
	    ->_get_cc_process_class() << "_breakSignal" ;
          string breakstring = breakstr.str();
          _cc_out << ",\n" << breakstring << "(new EnumerationType "
		  <<"(ObjectBase::SIGNAL,UniversalInteger(0), "
		  <<"std_standard_booleanTypeInfo()))" ;
          _cc_out << ",\n" << breakstring 
		  << "_info(ObjectBase::SIGNAL_NETINFO)" ;
        }
      }
    }
    conc_stmt =  dynamic_cast <TyvisConcurrentStatement *>
      (get_architecture_statement_part()->successor(conc_stmt));
  }
}

void
TyvisArchitectureDeclaration::_publish_cc_scoping_prefix( ostream &, Tyvis *, Tyvis * ){
  // Nothing to be done
}

void
TyvisArchitectureDeclaration::_publish_cc_implicit_signals( published_file &_cc_out,
                                                                   TyvisDeclarationList *decl_list,
                                                                   PublishData *declarations){
  ASSERT(decl_list != NULL);
  ASSERT((dynamic_cast<TyvisArchitectureDeclaration *>(this) != NULL) ||
	 (is_entity_declaration() == TRUE) ||
	 (is_component_declaration() == TRUE) ||
	 (get_kind() == IIR_ARCHITECTURE_DECLARATION));
  
  TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration *>(decl_list->first());

  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_implicit_signals" );

  while (decl != NULL) {
    TyvisDeclaration *nextdecl = dynamic_cast<TyvisDeclaration *>(decl_list->successor(decl));
    if(decl->is_signal() == TRUE) {
      //If the Signals has implicit signals
      //They are also needed for elaboration info
      if( decl->_get_implicit_declarations() != NULL &&
	  decl->_get_implicit_declarations()->size() != 0) {
	TyvisDeclaration* imp_decl = decl->_get_implicit_declarations()->getElement();
	while(imp_decl != NULL) {
	  if(imp_decl->is_signal() == TRUE) {
	    imp_decl->_publish_cc_implicit_signal_type( _cc_out );
	    _cc_out << " ";
	    if( dynamic_cast<TyvisObjectDeclaration *>(imp_decl) != 0 ){
	      dynamic_cast<TyvisObjectDeclaration *>(imp_decl)->_publish_cc_declarator_with_constructor_args( _cc_out, declarations );
	    }
	    _cc_out << ";" << NL();
	  }
	  imp_decl = decl->_get_implicit_declarations()->getNextElement();
	}
      }
    }
    else if(decl->get_kind() == IIR_ALIAS_DECLARATION) {
      if(decl->is_signal() == TRUE) {
	(dynamic_cast<TyvisAliasDeclaration*>(decl))->_publish_cc_elaborate_alias_definition( _cc_out, declarations );
      }
    }
    decl = nextdecl;
  }
}

void
TyvisArchitectureDeclaration::_publish_cc_include(published_file &_cc_out) {
  const string include_file_name = _get_cc_elaboration_class_name() + ".hh";
  Tyvis::_publish_cc_include( _cc_out, include_file_name );
}

const string
TyvisArchitectureDeclaration::_get_cc_design_unit_name(){
  return _get_mangled_declarator()->convert_to_string();
}

void
TyvisArchitectureDeclaration::_publish_cc_include_decls_prefix( ostream &os ){
  os << *_get_entity()->_get_mangled_declarator();
}

void 
TyvisArchitectureDeclaration::_publish_cc_foreign_process_pointer( published_file &_cc_out ){
  Tyvis::_publish_cc_include( _cc_out, "tyvis/ForeignProcess.hh" );
  _cc_out << "ForeignProcess *foreignProcess;" << NL();
}

void
TyvisArchitectureDeclaration::_publish_cc_foreign_instantiate( published_file &_cc_out ){
  // We need to call the foreign process creation method with the string from the 
  // attribute spec.  First we'll get that string.
  _cc_out << OS("foreignProcess = ForeignProcess::instantiate(")
	  << _get_foreign_attribute()->_get_cc_value_string() 
	  << "," << NL() 
	  << "\"" << _get_cc_elaboration_class_name() << "\"," << NL()
	  << "this"
	  << CS(");");
}

void
TyvisArchitectureDeclaration::_publish_cc_foreign_createNetInfo( published_file &_cc_out,
								 PublishData *declarations ){
  string old_publish_name = _get_current_publish_name();
  _set_current_publish_name( ", foreignProcess );" );

  // We need to do:
  // Add( signals, foreignProcess )
  TyvisInterfaceDeclaration *currentSignal =
    dynamic_cast<TyvisInterfaceDeclaration *>(get_entity()->get_port_clause()->first());
  while( currentSignal != 0 ){
    // Add the "Add" call...
    currentSignal->_publish_cc_object_name( _cc_out, declarations );
    _cc_out << ".addToFanOut( foreignProcess );";
    
    // Add the "addChild" call...
    currentSignal->_publish_cc_addChild( _cc_out, declarations );

    currentSignal = dynamic_cast<TyvisInterfaceDeclaration *>(_get_entity()->_get_port_clause()->successor( currentSignal ));
  }

  _set_current_publish_name( old_publish_name );
}

void
TyvisArchitectureDeclaration::_publish_cc_ams_includes(published_file &header_file) {

  CC_REF( header_file, "TyvisArchitectureDeclaration::_publish_cc_ams_includes" );

    header_file << "\n// VHDL-AMS include files. \n";
    header_file << "#include \"adouble.h\"\n";
    header_file << "#include \"adutils.h\"\n";
    header_file << "#include \"Quantity.hh\"\n";
    header_file << "#include \"Terminal.hh\"\n";
    header_file << "#include \"RealType.hh\"\n";
    header_file << "#include \"FreeEquation.hh\"\n";
    header_file << "#include \"DifferentialEquation.hh\"\n";
    header_file << "#include \"BranchEquation.hh\"\n";
    header_file << "\n";
}

void
TyvisArchitectureDeclaration::_publish_cc_ams_objects( published_file &_cc_out,
						       PublishData *declarations ) {
  TyvisDeclaration *decl = NULL;
  decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->first());
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_ams_objects" );
  while(decl != NULL) {
    decl->_publish_cc_decl( _cc_out, declarations );
    decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->successor(decl));
  }

  std::set<IIR_Declaration *> *decl_set = declarations->get_set(TyvisDeclaration::QUANTITY);  
  std::set<IIR_Declaration*>::iterator iter = decl_set->begin();
  while( iter != decl_set->end() ){
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->is_implicit_declaration() == TRUE) {
      decl->_publish_cc_decl( _cc_out, declarations );
    }
    iter++;
  }
}

void
TyvisArchitectureDeclaration::_publish_cc_ams_objects_init( published_file &_cc_out, IIR_Boolean &firstDeclFlag, PublishData *declarations,  PublishData *  ) {
  TyvisDeclaration* decl = NULL;
  int first = 0;
  decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->first());
  while(decl != NULL) {
  switch(decl->get_kind()) {
   case IIR_FREE_QUANTITY_DECLARATION: {
      TyvisFreeQuantityDeclaration *free_qty = dynamic_cast<TyvisFreeQuantityDeclaration *>(decl);
       if((first == 0) && (firstDeclFlag == TRUE)) {
         _cc_out << ":" << NL();
         first = 1;
         _is_ams_objects_present = TRUE;
         firstDeclFlag = FALSE;
       }
      else {
        _cc_out << "," << NL();
      }
      decl->_publish_cc_lvalue(_cc_out, declarations);
      _cc_out << "(ObjectBase::QUANTITY, ";
      _cc_out << "\"";
      decl->_publish_cc_lvalue(_cc_out, declarations);
      _cc_out << "\", ";
      free_qty->_publish_cc_tolerances(_cc_out);
      _cc_out << "FREE, NULL, NULL";
      if(free_qty->_get_value() != NULL) {
        _cc_out << ", ";
        free_qty->_get_value()->_publish_cc_ams_function(_cc_out, declarations);
      }
      _cc_out << ")";
    }
      break;
    case IIR_TERMINAL_DECLARATION: {
      if ((first == 0) && (firstDeclFlag == TRUE)) {
	_cc_out << ":" << NL();
	first = 1;
	_is_ams_objects_present = TRUE;
	firstDeclFlag = FALSE ;
      }
      else {
	_cc_out << "," << NL();
      }
      decl->_publish_cc_lvalue(_cc_out, declarations);
      _cc_out << "( ObjectBase::TERMINAL ";
      _cc_out << ",\"";
      decl->_publish_cc_lvalue(_cc_out, declarations);
      _cc_out << "\")";
    }
      break;
    case IIR_BRANCH_QUANTITY_DECLARATION: {
       if((first == 0) && (firstDeclFlag == TRUE)) {
          _cc_out << ":" << NL();
          first = 1;
         _is_ams_objects_present = TRUE;
         firstDeclFlag = FALSE;
       }
       else {
         _cc_out << "," << NL();
       }
       decl->_publish_cc_lvalue(_cc_out, declarations);
       _cc_out << "(ObjectBase::QUANTITY, ";
       _cc_out << "\"";
       decl->_publish_cc_lvalue(_cc_out, declarations);
       _cc_out << "\", ";
       TyvisBranchQuantityDeclaration *branch_qty = dynamic_cast<TyvisBranchQuantityDeclaration*>(decl);
       if(decl->is_implicit_declaration()==FALSE) {
	 branch_qty->_publish_cc_tolerances(_cc_out);
       }
       if(decl->is_implicit_declaration()==TRUE) {
	 _cc_out << "IMPLICIT,NULL, NULL)";
       }   
       else {
	 branch_qty->_publish_cc_tolerances(_cc_out);
	 if (branch_qty->_is_across_quantity()) {
	   _cc_out << "ACROSS, ";
	 } 
	 else {
	   _cc_out << "THROUGH," ;
	 }
	 TyvisDeclaration *plus_terminal = dynamic_cast<TyvisDeclaration *>(branch_qty->get_plus_terminal_name());
	 TyvisDeclaration *minus_terminal = dynamic_cast<TyvisDeclaration *>(branch_qty->get_minus_terminal_name());
	 _cc_out << "&";
	 plus_terminal->_publish_cc_lvalue(_cc_out, declarations);
	 _cc_out << ", ";
	 _cc_out << "&";
	 minus_terminal->_publish_cc_lvalue(_cc_out, declarations);
	 if(branch_qty->_is_across_quantity()==TRUE &&
	    branch_qty->_get_across_aspect_expression() != NULL ) {
	   _cc_out << ", ";
	   branch_qty->_get_across_aspect_expression()->_publish_cc_ams_function(_cc_out, declarations);
	 }
	 else {
	   if(branch_qty->_is_across_quantity() == FALSE &&
              branch_qty->_get_through_aspect_expression() != NULL ) {
             _cc_out << ", ";
             branch_qty->_get_through_aspect_expression()->_publish_cc_ams_function(_cc_out, declarations);
	   }
	 }
	 _cc_out << ")";
       }
    }
      break ;
  default:
    break;
  }
  decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->successor(decl));
  }

  std::set<IIR_Declaration *> *decl_set = declarations->get_set(TyvisDeclaration::QUANTITY);  
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->is_implicit_declaration() == TRUE) {
      if((first == 0) && (firstDeclFlag == 1)) {
	_cc_out << ":" << NL();
	first = 1;
	_is_ams_objects_present = TRUE;
      }
      else {
	_cc_out << "," << NL();
      }
      decl->_publish_cc_lvalue(_cc_out, declarations);
      _cc_out << "(ObjectBase::QUANTITY, \"";
      decl->_publish_cc_lvalue(_cc_out, declarations );
      if (decl->get_kind() == IIR_FREE_QUANTITY_DECLARATION) {
	_cc_out << "\", IMPLICIT , NULL, NULL)";
      }
      else {   
	_cc_out << "\", IMPLICIT ";
      }
      if (decl->get_kind() == IIR_BRANCH_QUANTITY_DECLARATION) {
	TyvisBranchQuantityDeclaration *branch_qty = dynamic_cast<TyvisBranchQuantityDeclaration*>(decl);
	TyvisDeclaration *plus_terminal = dynamic_cast<TyvisDeclaration *>(branch_qty->get_plus_terminal_name()) ;
	TyvisDeclaration *minus_terminal = dynamic_cast<TyvisDeclaration *>(branch_qty->get_minus_terminal_name()) ;
	_cc_out << ", &" ;
	plus_terminal->_publish_cc_lvalue(_cc_out, declarations );
	_cc_out << ", &";
	minus_terminal->_publish_cc_lvalue(_cc_out, declarations );
	_cc_out << ")";
      }
    }
  }
  _cc_out << NL();
}

void
TyvisArchitectureDeclaration::_publish_cc_ams_form_global_quantity_list(published_file &_cc_out, PublishData *declarations) {
  TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->first());
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_ams_form_global_quantity_list" );
  while ( decl != NULL) {
    if (decl->get_kind() == IIR_FREE_QUANTITY_DECLARATION ) {
      _cc_out << "globalFreeQuantityList.push_back(&(";
      decl->_publish_cc_lvalue( _cc_out, declarations);
      _cc_out << "));" << NL();
    }
    decl =  dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->successor(decl));
  }
}

void
TyvisArchitectureDeclaration::_publish_cc_allocate_module( published_file &_cc_out ){
}

void
TyvisArchitectureDeclaration::_publish_cc_break_signal_info(published_file &_cc_out) {
  TyvisConcurrentStatement* conc_stmt;
  
  conc_stmt = dynamic_cast<TyvisConcurrentStatement *>(get_architecture_statement_part()->first());
  while (conc_stmt != NULL) {
    if (conc_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      TyvisProcessStatement *temp_proc = dynamic_cast <TyvisProcessStatement *>(conc_stmt);
      for (TyvisSequentialStatement* seql_stmt =
	     dynamic_cast<TyvisSequentialStatement *>(temp_proc->get_process_statement_part()->first()) ;
	   seql_stmt != NULL;
	   seql_stmt = dynamic_cast<TyvisSequentialStatement *>(temp_proc->get_process_statement_part()->successor(seql_stmt))) {
        ostringstream breakstr;
        if (seql_stmt->get_kind() == IIR_BREAK_STATEMENT) {
          breakstr << dynamic_cast <TyvisProcessStatement *>(conc_stmt)->_get_cc_process_class() << "_breakSignal" ;
          string breakstring = breakstr.str();
          _cc_out << "EnumerationType *" << breakstring << ";" << NL();
          _cc_out << "EnumerationType " << breakstring << "_info;" << NL();
        }
      }
    }
    conc_stmt = dynamic_cast<TyvisConcurrentStatement *>(get_architecture_statement_part()->successor(conc_stmt));
  }
}

void
TyvisArchitectureDeclaration::_publish_cc_ams_form_break_set(published_file &_cc_out, PublishData *declarations) {
  
  // _currently_publishing_break_info = TRUE;
  TyvisConcurrentStatement *conc_stmt = NULL;
  _cc_out << "\nvoid\n";
  _publish_cc_binding_name(_cc_out.get_stream());
  _cc_out << "_elab::formBreakSet() {\n";
  _cc_out << "  list<breakElement *>breakElementList;"<<NL();
  _cc_out << "  breakElement *current_element;" << NL();
  _cc_out << "  breakSet *current_set;" << NL() ;
  _cc_out << "  signalDS *breakSignal;" << NL() << NL();
  conc_stmt = dynamic_cast<TyvisConcurrentStatement *>(get_architecture_statement_part()->first());
  while (conc_stmt != NULL) {
    if (conc_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      TyvisProcessStatement *temp_proc = dynamic_cast <TyvisProcessStatement *>(conc_stmt);
      for (TyvisSequentialStatement* seql_stmt =
	     dynamic_cast<TyvisSequentialStatement *>(temp_proc->get_process_statement_part()->first()) ;
	   seql_stmt != NULL;
	   seql_stmt = dynamic_cast<TyvisSequentialStatement *>(temp_proc->get_process_statement_part()->successor(seql_stmt))) {
        if (seql_stmt->get_kind() == IIR_BREAK_STATEMENT) {
          seql_stmt->_publish_cc( _cc_out, declarations );
        }
      }
    }
    conc_stmt = dynamic_cast<TyvisConcurrentStatement *>(get_architecture_statement_part()->successor(conc_stmt));
  }
  _cc_out << "\n}" << NL();
  //  _currently_publishing_break_info = FALSE;
}

void
TyvisArchitectureDeclaration::_publish_cc_ams_initialize_signals(published_file &_cc_out, PublishData *declarations ) {
  TyvisConcurrentStatement* conc_stmt = NULL;
  TyvisDeclaration *decl;
  savant::set<TyvisDeclaration> quantity_set;
  
  _cc_out << "void\n";
  _publish_cc_binding_name(_cc_out.get_stream());
  _cc_out << "_elab::initializeSignals() {\n";
  //  _currently_publishing_simultaneous_stmt  = TRUE;
  decl = dynamic_cast<TyvisDeclaration *>(*(declarations->get_set(TyvisDeclaration::SIGNAL)->begin()));
  //  _currently_publishing_simultaneous_stmt  = FALSE;
  TyvisDeclaration *current_quantity_decl = NULL;
  conc_stmt = dynamic_cast <TyvisConcurrentStatement *>
    (get_architecture_statement_part()->first());
  while (conc_stmt != NULL) {
    if (conc_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      TyvisProcessStatement *temp_proc = dynamic_cast <TyvisProcessStatement *>(conc_stmt);
      for (TyvisSequentialStatement* seqstmt = dynamic_cast<TyvisSequentialStatement *>(temp_proc->get_process_statement_part()->first());
	   seqstmt != NULL;
	   seqstmt = dynamic_cast<TyvisSequentialStatement *>(temp_proc->get_process_statement_part()->successor(seqstmt))) {
        ostringstream breakstr;
        const string design_unit_name = _get_cc_design_unit_name();
        if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
	  Tyvis::_publish_cc_include( _cc_out, "tyvis/SignalNetinfo.hh" );
          ASSERT(seqstmt->_get_process_stmt_label() != NULL);
          breakstr <<dynamic_cast <TyvisProcessStatement *>(conc_stmt)->
	    _get_cc_process_class() << "_breakSignal" ;
          string breakstring = breakstr.str();
          _cc_out << "{" << NL();
          _cc_out << "Signal<UniversalInteger> *signal = " << NL();
          _cc_out << "(Signal<UniversalInteger>*) ((ScalarType*)this->" ;
          _cc_out << breakstring << ")->object;" << NL();
          _cc_out << "SignalNetinfo *signal_info = " << NL();
          _cc_out << "(SignalNetinfo*) (*(ScalarType*)&"
                  << breakstring << "_info).object;" << NL();
          _cc_out << "signal->fanDest = signal_info->obj_ids;" << NL();
          _cc_out << "signal->fanout = signal_info->fanout;" << NL();
          _cc_out << "signal->id = signal_info->id;" << NL();
          _cc_out << "signal->type = EXPLICIT;" << NL();
          _cc_out << "signal->sensitive = true;" << NL();
          _cc_out << "signal->numAttributes = 0;" << NL();
          _cc_out << "signal->attributeList = NULL;" << NL();
          _cc_out << "signal->source = signal_info->source;" << NL();
          _cc_out << "signal->name = \"" << breakstring << "\";" << NL();
          _cc_out << "}" << NL();
        }
      }
      TyvisProcessStatement *current_process_stmt = dynamic_cast <TyvisProcessStatement *> (conc_stmt);
      std::set<IIR_Declaration*> *decl_set = current_process_stmt->get_declaration_collection()->get_set(TyvisDeclaration::QUANTITY);
      for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
        current_quantity_decl = dynamic_cast<TyvisDeclaration*>(*iter);
        quantity_set.add(dynamic_cast<TyvisDeclaration*>(current_quantity_decl));
      }
    }
    conc_stmt = dynamic_cast <TyvisConcurrentStatement *>
      (get_architecture_statement_part()->successor(conc_stmt));
  }
  current_quantity_decl = quantity_set.getElement();
  while (current_quantity_decl != NULL) {
    current_quantity_decl->_publish_cc_init(_cc_out, declarations);
    current_quantity_decl = quantity_set.getNextElement();
  }
  _cc_out << "}\n";
}

void
TyvisArchitectureDeclaration::_publish_cc_ams_connect_terminals(published_file &_cc_out, PublishData *declarations) {
  
  CC_REF( _cc_out, "TyvisArchitectureDeclaration::_publish_cc_ams_connect_terminals" );
  
  TyvisArchitectureStatement *arch_stmt = NULL;
  TyvisInterfaceDeclaration *portelement = NULL;
  TyvisDeclaration *decl = NULL;
  TyvisBranchQuantityDeclaration *branch_qty = NULL;
  Tyvis *plus_terminal = NULL;
  Tyvis *minus_terminal = NULL;
  IIR_Boolean firstDeclFlag = TRUE;
  
  // to assign terminal index ...
  int terminal_index = 0;
  
  _cc_out << "void\n";
  this->_publish_cc_binding_name(_cc_out.get_stream()); 
  _cc_out << "_elab::connectTerminals(int numberOfTerminals, ...) {\n";
  _cc_out << "va_list ap;\n";
  _cc_out << "terminalInfo = (AMSType**)new "
          << "char[numberOfTerminals * sizeof(AMSType*)];\n";
  _cc_out << "va_start(ap, numberOfTerminals);\n";
  _cc_out << "for(int i=0; i <numberOfTerminals; i++) {\n";
  _cc_out << "  terminalInfo[i] = va_arg(ap, AMSType*);\n";
  _cc_out << "}\n";
  
  // adding code for mangling the names of the quantities inside component
  // thus a quantity vr inside a component will be renamed as vr_<label>
  // where this label is obtained from the component instantiation
  // statement from that component instantiation. This will enable us to
  // associate quantities in the output with their components.

  _cc_out << "char *labelInfo = va_arg(ap, char *);\n";
  
  _cc_out << "va_end(ap);\n";
  
  _cc_out << "if (numberOfTerminals > 0 ) { \n";
  
  portelement = dynamic_cast<TyvisInterfaceDeclaration *>(_get_entity()->get_port_clause()->first());
  
  for (; portelement != NULL;
      portelement = dynamic_cast<TyvisInterfaceDeclaration *>(_get_entity()->get_port_clause()->successor(portelement) )) {
  
    switch (portelement->get_kind()) {
    case IIR_TERMINAL_INTERFACE_DECLARATION: {
      _cc_out << "  ";
      _cc_out << "setTerminalInfo(";
      portelement->_publish_cc_lvalue(_cc_out, declarations);
      _cc_out << " , *(terminalInfo[" << terminal_index << "] ));\n";
      terminal_index++;
    }
      break;
    default:
      break;
    } 
  }
  
  // now we have to publish such that all the quantities which had
  // interface terminals as their plus/minus terminals, get their
  // elaboration information properly ...
  for (decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->first());
       decl != NULL;
       decl = dynamic_cast<TyvisDeclaration *>(get_architecture_declarative_part()->successor(decl))) {
    branch_qty = dynamic_cast<TyvisBranchQuantityDeclaration*>(decl);
    if (branch_qty != NULL) {
      plus_terminal = branch_qty->_get_plus_terminal_name();
      minus_terminal = branch_qty->_get_minus_terminal_name();
      
      if (plus_terminal->is_interface() == TRUE) {
        // code for name mangling for internal quantities of components.
        if (firstDeclFlag == TRUE) {
          _cc_out << "  char *temp = new char [strlen(\"";
          branch_qty->_publish_cc_lvalue(_cc_out, declarations);
          _cc_out << "_\") + strlen(labelInfo)];\n";
          firstDeclFlag = FALSE;
        }
        else {
          _cc_out << "  temp = new char [strlen(\"";
          branch_qty->_publish_cc_lvalue(_cc_out, declarations);
          _cc_out << "_\") + strlen(labelInfo)];\n";
        }
        _cc_out << "  strcpy(temp, \"";
        branch_qty->_publish_cc_lvalue(_cc_out, declarations);
        _cc_out << "_\");\n";
        _cc_out << "  strcat(temp, labelInfo);\n";
        _cc_out << "  setPlusTerminal(&(";
        branch_qty->_publish_cc_lvalue(_cc_out, declarations);
        _cc_out << "), &";
        plus_terminal->_publish_cc_lvalue(_cc_out, declarations);
        _cc_out << ", temp);" << NL();
        _cc_out << "   delete [] temp;\n";
      }
      if (minus_terminal->is_interface() == TRUE) {
        _cc_out << "  setMinusTerminal(&(";
        branch_qty->_publish_cc_lvalue(_cc_out, declarations);
        _cc_out << "), &";
        minus_terminal->_publish_cc_lvalue(_cc_out, declarations);
        _cc_out << ");" << NL();
      }
    }
  }
  _cc_out << "}\n";
  
  //Publishing output connections for  architecture statement part
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_architecture_statement_part()->first());
  while (arch_stmt != NULL) {
    ostringstream objectname;
    if (arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
      objectname << *(arch_stmt->_get_mangled_label()) << "_elab_obj" << ends;
      (dynamic_cast<TyvisComponentInstantiationStatement*>(arch_stmt))->_publish_connect_terminals( _cc_out, declarations );
    }
    else {
      //No need to do anything for other type of statements
    }
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_architecture_statement_part()->successor(arch_stmt));
  }
  _cc_out << "}\n";
}

string
TyvisArchitectureDeclaration::_mangle_declarator(){
  string mangled_declarator_tmp;

  if( _get_entity() != NULL ){
    mangled_declarator_tmp = _get_entity()->_get_mangled_declarator()->convert_to_string();
  }
  mangled_declarator_tmp += string("_") + _get_mangling_prefix();
  mangled_declarator_tmp += get_declarator()->convert_to_string();

  return mangled_declarator_tmp;
}

void TyvisArchitectureDeclaration::_publish_cc_ams_form_characteristic_expressions(published_file &_cc_out) {
  TyvisArchitectureStatement* arch_stmt;
  
  CC_REF(_cc_out , "TyvisArchitectureDeclaration::_publish_cc_ams_form_characteristic_expressions");
  
  _cc_out << "void" << NL();
  _publish_cc_binding_name(_cc_out.get_stream());
  _cc_out << "_elab::formCharacteristicExpressions() {" << NL();
  _cc_out << "component *equation = NULL;" << NL();
  _cc_out << "component **parent_equation = NULL;" << NL();
  _cc_out << "//signalDS *equationSignal = NULL;" << NL() << NL();
  _cc_out << "//initializeSignals();" << NL();
  
  // Publish the characteristic expressions for simultaneous statements
  dynamic_cast<TyvisArchitectureStatementList *>(get_architecture_statement_part())->_publish_cc_characteristic_expressions(_cc_out);
  
  // Publishing characteristic expressions in the components instantiated
  // in this architecture, if any...
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_architecture_statement_part()->first());
  while ( arch_stmt != NULL ) {
    ostringstream objectname;
    if (arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
      objectname << *(arch_stmt->_get_mangled_label()) << "_elab_obj" << ends;
      _set_current_elab_name( objectname.str() );
      (dynamic_cast<TyvisComponentInstantiationStatement*>(arch_stmt))->_publish_form_characteristic_expressions(_cc_out);
    }   
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_architecture_statement_part()->successor(arch_stmt));
  }
  _cc_out << "}" << NL();
}


bool 
TyvisArchitectureDeclaration::_is_foreign_architecture(){
  return _get_architecture_declarative_part()->_contains_foreign_attribute();
}


TyvisAttributeSpecification *
TyvisArchitectureDeclaration::_get_foreign_attribute(){
  TyvisAttributeSpecification *retval = 0;
   savant::set<TyvisAttributeSpecification> *foreign_attributes = 
    _get_architecture_declarative_part()->_find_foreign_attributes();
  if( foreign_attributes != 0 ){
    TyvisAttributeSpecification *last_element = 0;
    TyvisAttributeSpecification *current = foreign_attributes->getElement();
    while( current != 0 ){
      last_element = current;
      current = foreign_attributes->getNextElement();
    }
    retval = last_element;
  }

  return retval;
}

TyvisList *
TyvisArchitectureDeclaration::_get_statement_list(){
  return dynamic_cast<TyvisList *>(get_architecture_statement_part());
}

TyvisDeclarationList *
TyvisArchitectureDeclaration::_get_architecture_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>(get_architecture_declarative_part());
}

TyvisArchitectureStatementList *
TyvisArchitectureDeclaration::_get_architecture_statement_part() {
  return dynamic_cast<TyvisArchitectureStatementList *>(get_architecture_statement_part());
}

TyvisEntityDeclaration*
TyvisArchitectureDeclaration::_get_entity() {
  return dynamic_cast<TyvisEntityDeclaration *>(get_entity());
}

TyvisTextLiteral *
TyvisArchitectureDeclaration::_get_declarator() {
  return dynamic_cast<TyvisTextLiteral *>(get_declarator());
}

TyvisGenericList *
TyvisArchitectureDeclaration::_get_generic_list(){
  ASSERT( _get_entity() != NULL );
  return _get_entity()->_get_generic_list();
}

TyvisDeclarationList*
TyvisArchitectureDeclaration::_get_declaration_list() {
  return dynamic_cast<TyvisDeclarationList *>(get_architecture_declarative_part());
}

TyvisPortList *
TyvisArchitectureDeclaration::_get_port_list(){
  ASSERT( _get_entity() != NULL );
  return _get_entity()->_get_port_list();
}

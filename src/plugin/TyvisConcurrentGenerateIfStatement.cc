
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

#include "TyvisConcurrentGenerateIfStatement.hh"
#include "TyvisAliasDeclaration.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisAssociationList.hh"
#include "TyvisBlockStatement.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDesignator.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisLibraryUnit.hh"
#include "TyvisPortList.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSimpleName.hh"

#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "published_cc_file.hh"
#include "published_header_file.hh"
#include <sstream>

TyvisConcurrentGenerateIfStatement::TyvisConcurrentGenerateIfStatement(){
  set_block_declarative_part(new TyvisDeclarationList());
  set_concurrent_statement_part(new TyvisArchitectureStatementList());
}

TyvisConcurrentGenerateIfStatement::~TyvisConcurrentGenerateIfStatement(){
  //Release the list memory
  delete get_block_declarative_part();
  delete get_concurrent_statement_part();
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_generate_elaborate( published_file &_cc_out, PublishData *declarations ){
  TyvisArchitectureStatement *arch_stmt;
  const string temp = _get_current_architecture_name();
  const string old_current_name = _get_current_publish_name();

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_generate_elaborate" );

  ostringstream newName;

  _set_current_publish_name( "SG" );
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());
  
  while (arch_stmt != NULL) {
    // Need to check if generate statements can have other concurrent
    // statements that will need elaboration. If so that check should
    // be added to the following if condition.
    
    switch (arch_stmt->get_kind())  {
    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
    case IIR_BLOCK_STATEMENT:
      arch_stmt->_publish_cc_elaborate( _cc_out, declarations );
      break;

    case IIR_PROCESS_STATEMENT:
    case IIR_COMPONENT_INSTANTIATION_STATEMENT:
      // Prevent the error message from being printed for process statementso
      break;
      
    default:
      cerr << "ERROR! TyvisConcurrentGenerateIfStatement::"
	   << "_publish_cc_generate_elaborate( _cc_out, declarations ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "|" << endl;
      break;
    }
    
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor(arch_stmt));
  }

  _set_current_architecture_name( temp );
  _set_current_publish_name( old_current_name );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_declarations( PublishData *declarations ){
  string generate_unit_name;
  ostringstream ent_arch_generate;

  ent_arch_generate << _get_current_entity_name() << "_"
		    << _get_current_architecture_name();
  _publish_cc_enclosing_stmt_to_architecture_path(ent_arch_generate);
  ent_arch_generate << "_" << *(_get_mangled_label());
  generate_unit_name = ent_arch_generate.str();

  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     generate_unit_name,
				     this );
  CC_REF( header_file, "TyvisConcurrentGenerateIfStatement::_publish_cc_declarations" );

  Tyvis::_publish_cc_include( header_file, _get_design_unit_name() + ".hh" );

  _get_block_declarative_part()->_publish_cc( header_file, declarations );

  published_cc_file cc_file( _get_work_library()->get_path_to_directory(), 
			     generate_unit_name,
			     this );
  CC_REF( cc_file, "TyvisConcurrentGenerateIfStatement::_publish_cc_declarations" );

  cc_file << "#include \"" << generate_unit_name << ".hh\"\n\n";

  _get_block_declarative_part()->_publish_cc_decl( cc_file, declarations );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_class( published_file &_cc_out, PublishData *declarations ) {
  _publish_cc_class_generate_stmt( _cc_out, declarations );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_headerfile(PublishData *declarations){
  ostringstream filename;
  _publish_cc_class_name(filename);
  string filename_ptr = filename.str();
  
  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     filename_ptr,
				     this );
  
  CC_REF( header_file, "TyvisConcurrentGenerateIfStatement::_publish_cc_headerfile" );

  Tyvis::_publish_cc_include( header_file, _get_design_unit_name() + ".hh" );
  Tyvis::_publish_cc_include( header_file, "tyvis/Hierarchy.hh" );

  //  _publish_cc_headers( header_file );
  header_file << "\n";
  _publish_cc_class_includes( header_file, _get_concurrent_statement_part() );
  header_file << "\n";
  _publish_cc_class( header_file, declarations );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_elaborate" );

  _publish_cc_headerfile( declarations );
  _publish_cc_ccfile( declarations );
  _publish_cc_generate_elaborate( _cc_out, declarations );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_ccfile( PublishData *declarations ){
  ostringstream filename;

  _publish_cc_class_name(filename);
  published_cc_file cc_file( _get_work_library()->get_path_to_directory(), 
			     filename.str(),
			     this );
  CC_REF( cc_file, "TyvisConcurrentGenerateIfStatement::_publish_cc_ccfile" );

  cc_file << "#include \"" << filename.str() << ".hh\"\n\n";
  _publish_cc_headerfiles_for_cc( cc_file );

  _publish_cc_constructor( cc_file, declarations );
  _publish_cc_destructor( cc_file, _get_concurrent_statement_part() );
  _publish_cc_instantiate( cc_file, declarations );
  _publish_cc_createNetInfo( cc_file, declarations );
  _publish_cc_connect( cc_file, declarations );

}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_headerfiles_for_cc( published_file &_cc_out ){

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_headerfiles_for_cc" );

  _publish_cc_headerfiles_for_cc_generate_statement( _cc_out );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_constructor( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_constructor" );

  _publish_cc_constructor_with_no_arguments( _cc_out, declarations );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations )
{

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_constructor_with_no_arguments" );
  _cc_out << _get_cc_elaboration_class_name() << "::"
	  << _get_cc_elaboration_class_name() << "(";

  _cc_out << _get_current_publish_name() << _get_current_entity_name() << "_"
   	  << _get_current_architecture_name();
  _publish_cc_enclosing_stmt_to_architecture_path( _cc_out.get_stream() );

  _cc_out << "_elab* outerScope)  {\n";

  _cc_out << "  enclosingScope = outerScope;\n";

  _publish_cc_generate_condition( _cc_out, declarations );
  _cc_out << "{\n";
  _publish_cc_object_pointers_init( _cc_out, declarations );
  _cc_out << "}\n}\n";
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_signal_objects_init( published_file &_cc_out,
                                                                            PublishData *declarations,
                                                                            IIR_Boolean firstDeclFlag){
  TyvisDeclaration *decl;
  int first = 0;

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_signal_objects_init" );

  decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->first());

  while (decl != NULL) {
    if (decl->get_kind() == IIR_SIGNAL_DECLARATION) {
      if ((first == 0) && (firstDeclFlag == TRUE)) {
	_cc_out << ":\n";
	first = 1;
      }
      else {
	_cc_out << ",\n";
      }
      
      decl->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "(ObjectBase::SIGNAL_NETINFO";

      if (((decl->_get_subtype()->is_array_type() == TRUE) ||
	   (decl->_get_subtype()->is_record_type() ==  TRUE)) &&
	  (decl->_get_subtype()->is_access_type() == FALSE)) {
	decl->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations );
	_cc_out << ", ";
	decl->_get_subtype()->_publish_cc_resolution_function_id( _cc_out, declarations );
	
	if (decl->_get_subtype()->is_anonymous() == TRUE) {
	  _cc_out << ", ";
	  decl->_get_subtype()->_publish_cc_range( _cc_out, declarations );
	}
      }
      
      _cc_out <<  ")";
      
      if( decl->_get_implicit_declarations() != NULL &&
	  decl->_get_implicit_declarations()->size() != 0 ) {
	TyvisDeclaration* imp_decl = decl->_get_implicit_declarations()->getElement();
	while (imp_decl != NULL) {
	  if (imp_decl->get_kind() == IIR_SIGNAL_DECLARATION) {
	    _cc_out << ",\n";
	    imp_decl->_publish_cc_elaborate( _cc_out, declarations );
	    _cc_out << "(ObjectBase::SIGNAL_NETINFO";

	    if (((imp_decl->_get_subtype()->is_array_type() == TRUE) ||
		 (imp_decl->_get_subtype()->is_record_type() ==  TRUE)) &&
		(imp_decl->_get_subtype()->is_access_type() ==  FALSE)) {
	      imp_decl->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations );
	      _cc_out << ", ";
	      imp_decl->_get_subtype()->_publish_cc_resolution_function_id( _cc_out, declarations );
	      
	      if (imp_decl->_get_subtype()->is_anonymous() == TRUE) {
		_cc_out << ", ";
		imp_decl->_get_subtype()->_publish_cc_range( _cc_out, declarations );
	      }
	    }
	    
	    _cc_out << ")";
	  }
	  imp_decl = decl->_get_implicit_declarations()->getNextElement();
	}
      }
    }
    else if (decl->get_kind() == IIR_ALIAS_DECLARATION) {
      if ((dynamic_cast<TyvisAliasDeclaration *>(decl))->_get_name()->is_signal()) {
	if ((first == 0) && (firstDeclFlag == 1)) {
	  _cc_out << ":\n";
	  first = 1;
	}
	else {
	  _cc_out << ",\n";
	}
	(dynamic_cast<TyvisAliasDeclaration *>(decl))->_publish_cc_elaborate_alias_init( _cc_out, declarations );
      }
    }
    
    decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->successor(decl));
  }
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations ){
  const string                          temp                    = _get_current_architecture_name();
  const string                          old_current_name        = _get_current_publish_name();
  ostringstream                         newName;
  TyvisArchitectureStatement        *arch_stmt              = NULL;
  IIR_Boolean                           found                   = FALSE;
  int                                   wanted_instantiation    = 1;
  TyvisConfigurationSpecification   *config_spec_decl       = NULL;
  Tyvis                              *tempNode               = NULL;
  TyvisDeclaration                  *decl                   = NULL;

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_object_pointers_init" );

  _set_current_publish_name( "SG" );
  
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());

  while (arch_stmt != NULL) {
    switch(arch_stmt->get_kind()) {
    case IIR_PROCESS_STATEMENT:
      if (arch_stmt->_get_mangled_label() != NULL) {
	_cc_out << *(arch_stmt->_get_mangled_label());
      }
      else {
	_cc_out << "ANON_PROCESS" << arch_stmt;
      }
      _cc_out << "_elab_obj = NULL;\n";
      break;

    case IIR_COMPONENT_INSTANTIATION_STATEMENT:
      TyvisComponentDeclaration *componentName;
      componentName =
        dynamic_cast<TyvisComponentDeclaration *>(arch_stmt->_get_instantiated_unit());
      ASSERT(componentName->get_kind() == IIR_COMPONENT_DECLARATION);

      _cc_out << *(arch_stmt->_get_mangled_label())
	      << "_elab_obj = new "
	      << componentName->_get_cc_elaboration_class_name()
	      << ";\n";

      arch_stmt->_get_mangled_label()->_get_mangled_declarator()->_publish_cc_lvalue( _cc_out, declarations );
      _cc_out << "_elab_obj->boundedEntity = ";

      while ((decl != NULL) && (found == FALSE)) {
	if (decl->get_kind() == IIR_CONFIGURATION_SPECIFICATION) {
	  config_spec_decl = dynamic_cast<TyvisConfigurationSpecification *>(decl);
	  TyvisTextLiteral *local_componentName = componentName->_get_mangled_declarator();
	  ASSERT( dynamic_cast<TyvisDeclaration *>(config_spec_decl->_get_component_name()) );
	  TyvisTextLiteral *binding_componentName = (dynamic_cast<TyvisDeclaration *>(config_spec_decl->get_component_name()))->_get_mangled_declarator();

	  if (0 == IIRBase_TextLiteral::cmp(local_componentName, binding_componentName)) {
	    TyvisDesignator *designator = dynamic_cast<TyvisDesignator *>(config_spec_decl->get_instantiation_list()->first());

	    while ((designator != NULL) && (found == FALSE)) {
	      switch(designator->get_kind()) {
	      case IIR_DESIGNATOR_EXPLICIT:
		ASSERT( dynamic_cast<TyvisDeclaration *>(designator->_get_name()) );

		wanted_instantiation = IIRBase_TextLiteral::cmp((dynamic_cast<TyvisDeclaration *>(designator->_get_name()))->_get_declarator(), arch_stmt->_get_label()->_get_declarator());
		if (wanted_instantiation == 0) {
		  found = TRUE;
		  config_spec_decl = (dynamic_cast<TyvisConfigurationSpecification *>(decl));
		}
		break;

	      case IIR_DESIGNATOR_BY_ALL:
		found = TRUE;
		config_spec_decl = (dynamic_cast<TyvisConfigurationSpecification *>(decl));
		break;

	      default:
		break;
	      }

	      designator = dynamic_cast<TyvisDesignator *>(config_spec_decl->get_instantiation_list()->successor(designator));
	    }
	  }
	}

	decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->successor(decl));
      }

      if (found == FALSE) {
	_cc_out << " NULL;\n";
      }
      else {
	_cc_out << " new ";
	if (config_spec_decl->get_entity_aspect() != NULL) {
	  _cc_out << config_spec_decl->_get_entity_aspect()->_get_cc_elaboration_class_name()
		  << "(";
	  tempNode = _get_current_publish_node();
	  _set_current_publish_node( arch_stmt );
	  config_spec_decl->_get_generic_map_aspect()->_publish_cc_lvalue( _cc_out, declarations );
	  _cc_out << ");\n";
	  config_spec_decl->_publish_cc_port_map_aspect( _cc_out, declarations );
	  _set_current_publish_node( tempNode );
	}
	  
	found = FALSE;
	wanted_instantiation = 1;
      }
      break;

    case IIR_BLOCK_STATEMENT:
      arch_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "_elab_obj = new "
	      << _get_cc_elaboration_class_name() << "(this";
      
      if ( (dynamic_cast<TyvisBlockStatement *>(arch_stmt))->get_generic_map_aspect()->size() > 0) {
	_cc_out << ", \n";
	tempNode = _get_current_publish_node();
	_set_current_publish_node( arch_stmt );
	arch_stmt->_get_generic_map_aspect()->_publish_cc_generic_map_aspect_for_conc_stmts( _cc_out, declarations );
	_set_current_publish_node( tempNode );
      }
      
      _cc_out << ");\n";
      break;

    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
      arch_stmt->_get_label()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "_elab_obj = new "
	      << _get_cc_elaboration_class_name()
	      << "(this);\n";
      break;

    default:
      cerr << "ERROR! TyvisConcurrentGenerateIfStatement::"
	   << "_publish_cc_object_pointers_init( _cc_out, declarations ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "|" << endl;
      break;
    }

    found = FALSE;
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor(arch_stmt));
  }

  _set_current_architecture_name( temp );
  _set_current_publish_name( old_current_name );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_generate_condition( published_file &_cc_out, PublishData *declarations )
{

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_generate_condition" );

  _cc_out << "  if (";
  _get_if_condition()->_publish_cc_lvalue( _cc_out, declarations );
  _cc_out << " == SAVANT_BOOLEAN_TRUE)  \n";
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_instantiate( published_file &_cc_out, PublishData *declarations )
{
  TyvisArchitectureStatement *arch_stmt = NULL;

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_instantiate" );
  
  _cc_out << "void\n" << _get_cc_elaboration_class_name() << "::instantiate(Hierarchy * hier)  {\n";

  _publish_cc_generate_condition( _cc_out, declarations );
  _cc_out << "{\n";

  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());
  while (arch_stmt != NULL) {
    arch_stmt->_publish_cc_instantiate_call( _cc_out, declarations );
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor(arch_stmt));
  }
  _cc_out << "\ncreateNetInfo();\n";
  _cc_out << "  }\n";
  _cc_out << "}\n";  
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_body_for_component_createNetInfo( published_file &_cc_out,
                                                                                         TyvisArchitectureStatement *arch_stmt,
                                                                                         PublishData *declarations){
  ostringstream labelname;
  ostringstream objectname;
  int wanted_instantiation = 1;
  IIR_Boolean found = false;
  TyvisEntityDeclaration* bindingEntity = NULL;
  int noofelements = 0;

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_body_for_component_createNetInfo" );
  
  labelname << *(arch_stmt->_get_mangled_label()) << "_elab_obj->";
  _set_current_elab_name( labelname.str() );

  Tyvis* componentName = arch_stmt->_get_instantiated_unit();      
  if(componentName->get_kind() == IIR_COMPONENT_DECLARATION) {
    TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->first());
    while( decl != NULL ){
      if( decl->get_kind() == IIR_CONFIGURATION_SPECIFICATION ){
	TyvisTextLiteral* local_componentName = componentName->_get_declarator();
	TyvisTextLiteral* binding_componentName = (dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_component_name()->_get_declarator();
	if(IIRBase_TextLiteral::cmp(local_componentName, binding_componentName) == 0) {
	  TyvisDesignator* designator = dynamic_cast<TyvisDesignator *>(decl->_get_instantiation_list()->first());
	  while(designator != NULL) {
	    switch(designator->get_kind()) {
	    case IIR_DESIGNATOR_EXPLICIT:
	      wanted_instantiation = IIRBase_TextLiteral::cmp((dynamic_cast<TyvisDeclaration *>(designator->_get_name()))->_get_declarator(), arch_stmt->_get_mangled_label()->_get_declarator());
	      if(wanted_instantiation == 0) {
		found = TRUE;
		if((dynamic_cast<TyvisConfigurationSpecification*>(decl))->get_port_map_aspect()->size() == 0) {
		  _set_current_publish_node( componentName );
		  objectname << "(( ";
		  TyvisLibraryUnit* lib_unit = decl->_get_entity_aspect(); 
		  if(lib_unit != NULL) {
		    lib_unit->_publish_cc_binding_name(objectname);
		    bindingEntity = lib_unit->_get_entity();
		  }
		  objectname << "_elab*) " << *(arch_stmt->_get_mangled_label());
		  objectname << "_elab_obj->boundedEntity)->";
		  _set_current_publish_name( objectname.str() );
		  dynamic_cast<TyvisPortList *>(bindingEntity->get_port_clause())->_publish_cc_port_map_aspect( _cc_out, declarations );
		}
		else {
		  _set_current_publish_node( componentName );
		  objectname << "(( ";
		  TyvisLibraryUnit* lib_unit = decl->_get_entity_aspect(); 
		  if( lib_unit != NULL ){
		    lib_unit->_publish_cc_binding_name(objectname);
		    bindingEntity = lib_unit->_get_entity();
		  }
		  objectname << "_elab*) " << *(arch_stmt->_get_mangled_label());
		  objectname << "_elab_obj->boundedEntity)->";
		  _set_current_publish_name( objectname.str() );
		  noofelements = (dynamic_cast<TyvisConfigurationSpecification*>(decl))->get_port_map_aspect()->size();
		  if(noofelements != 0) {
		    (dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_port_map_aspect()->_publish_cc_elaborate( _cc_out, declarations );
		  }
		  else {
		    TyvisLibraryUnit* binding_unit = (dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_entity_aspect();
		    if(binding_unit!= NULL) {
		      dynamic_cast<TyvisPortList *>(binding_unit->_get_entity()->get_port_clause())->_publish_cc_port_map_aspect( _cc_out, declarations );
		    }
		  }
		}
	      }
	      break;
	    case IIR_DESIGNATOR_BY_ALL:
	    case IIR_DESIGNATOR_BY_OTHERS:
	      found = TRUE;
	      if((dynamic_cast<TyvisConfigurationSpecification*>(decl))->get_port_map_aspect()->size() == 0) {
		_set_current_publish_node( componentName );
		objectname << "(( ";
		TyvisLibraryUnit *lib_unit = decl->_get_entity_aspect(); 
		if(lib_unit != NULL) {
		  lib_unit->_publish_cc_binding_name(objectname);
		  bindingEntity = lib_unit->_get_entity();
		}
		objectname << "_elab*) " << *(arch_stmt->_get_mangled_label());
		objectname << "_elab_obj->boundedEntity)->";
		_set_current_publish_name( objectname.str() );
		dynamic_cast<TyvisPortList *>(bindingEntity->get_port_clause())->_publish_cc_port_map_aspect( _cc_out, declarations );
	      }
	      else {
		_set_current_publish_node( componentName );
		objectname << "(( ";
		TyvisLibraryUnit *lib_unit = decl->_get_entity_aspect();
		if(lib_unit != NULL) {
		  lib_unit->_publish_cc_binding_name(objectname);
		  bindingEntity = lib_unit->_get_entity();
		}
		objectname << "_elab*) " << *(arch_stmt->_get_mangled_label());
		objectname << "_elab_obj->boundedEntity)->";
		_set_current_publish_name( objectname.str() );
		noofelements = (dynamic_cast<TyvisConfigurationSpecification*>(decl))->get_port_map_aspect()->size();
		if(noofelements != 0) {
		  (dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_port_map_aspect()->_publish_cc_elaborate( _cc_out, declarations );
		}
		else {
		  TyvisLibraryUnit* binding_unit = decl->_get_entity_aspect();
		  if( binding_unit!= NULL ){
		    dynamic_cast<TyvisPortList *>(binding_unit->_get_entity()->get_port_clause())->_publish_cc_port_map_aspect( _cc_out, declarations );
		  }
		}
	      }
	      break;

	    default:
	      break;
	    }
	    if( found == TRUE ){
	      designator = NULL;
	    }
	    else {
	      designator = dynamic_cast<TyvisDesignator *>(decl->_get_instantiation_list()->successor(designator));
	    }
	  }
	}
      }
      if(found == TRUE) {
	decl = NULL;
      }
      else {
	decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->successor(decl));
      }
    }
  }
  
  objectname << *(arch_stmt->_get_mangled_label()) << "_elab_obj";
  _set_current_elab_name( objectname.str() );
  arch_stmt->_publish_createNetInfo( _cc_out, declarations );
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations ) {
  TyvisArchitectureStatement *arch_stmt;
  IIR_Boolean found = false;
  int wanted_instantiation = 1;
  const string tmp = _get_current_elab_name();
  const string tmp2 = _get_current_publish_name();
  Tyvis* tmpNode   = _get_current_publish_node();
  PublishedUnit old_publish_unit = _get_currently_publishing_unit();

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_createNetInfo" );

  _set_currently_publishing_unit(Tyvis::GENERATE_IF);
 
  _cc_out << "void\n"
	  << _get_cc_elaboration_class_name()
	  << "::createNetInfo(){\n";

  _publish_cc_generate_condition( _cc_out, declarations );
  _cc_out << "{\n";
  
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());
  while (arch_stmt != NULL) {
    ostringstream objectname;
    ostringstream labelname;
    _set_current_publish_name( "" );
    _set_current_publish_node( this );
    
    if (arch_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      if(arch_stmt->_get_mangled_label() != NULL) {
	objectname << *(arch_stmt->_get_mangled_label());
      }
      else {
	objectname << "ANON_PROCESS" << arch_stmt;
      }
      objectname << "_elab_obj";
      _set_current_elab_name( objectname.str() );
      arch_stmt->_publish_createNetInfo( _cc_out, declarations );
    }
    else if ((arch_stmt->get_kind() == IIR_BLOCK_STATEMENT) ||
	     (arch_stmt->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT) || (arch_stmt->get_kind() == IIR_CONCURRENT_GENERATE_IF_STATEMENT)) {
    }
    else if (arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
      _publish_cc_body_for_component_createNetInfo( _cc_out, arch_stmt, declarations );
    }
    else {
      cerr << "ERROR! TyvisConcurrentGenerateIfStatement::"
	   << "_publish_cc_createNetInfo( _cc_out, declarations ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "|" << endl;
    }
    _set_current_elab_name( "" );
    found = FALSE;
    wanted_instantiation = 1;
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor(arch_stmt));
  }
  _cc_out << "}\n}\n\n";
  _set_current_elab_name( tmp );
  _set_current_publish_name( tmp2 );
  _set_current_publish_node( tmpNode );
  _set_currently_publishing_unit(old_publish_unit);
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_connect( published_file &_cc_out,
							 PublishData *declarations ){

  CC_REF( _cc_out, "TyvisConcurrentGenerateIfStatement::_publish_cc_connect" );
  Tyvis::_publish_cc_include( _cc_out, "cstdarg", true );
  _cc_out << "void" << NL()
	  << _get_cc_elaboration_class_name()
	  << OS("::connect( int inputsignals, int outputsignals, ... ){") << NL()
	  << "int noOfSignals = inputsignals + outputsignals;" << NL()
	  << "va_list ap;" << NL();
  _publish_cc_generate_condition( _cc_out, declarations );
  _cc_out << OS("{") << NL()
	  << "va_start(ap, outputsignals);" << NL()
	  << OS("for( int i = 0; i < noOfSignals; i++ ){") << NL()
	  << "addToFanOut( va_arg(ap, RValue*) );" << NL()
	  << CS("}") << NL()
	  << "va_end(ap);" << NL();

  TyvisArchitectureStatement *arch_stmt =
    dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());
  while( arch_stmt != NULL ){
    ostringstream objectname;
    if (arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
      objectname << *(arch_stmt->_get_mangled_label()) << "_elab_obj";
      _set_current_elab_name( objectname.str() );
      dynamic_cast<TyvisComponentInstantiationStatement*>(arch_stmt)->_publish_connect( _cc_out, declarations );
    }
    else if (arch_stmt->_is_concurrent_generate_statement() == TRUE) {
      arch_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "_elab_obj->connect(0, 0);" << NL();
    }
    else if (arch_stmt->_is_block_statement() == TRUE) {
      dynamic_cast<TyvisBlockStatement *>(arch_stmt)->_publish_cc_connect( _cc_out, declarations );
    }
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor(arch_stmt));
  }
  _cc_out << CS("}") << CS("}") << NL();
}

TyvisDeclarationList*
TyvisConcurrentGenerateIfStatement::_get_declaration_list() {
  return dynamic_cast<TyvisDeclarationList *>(get_block_declarative_part());
}

TyvisList *
TyvisConcurrentGenerateIfStatement::_get_statement_list() { 
  return dynamic_cast<TyvisList *>(get_concurrent_statement_part()); 
}

Tyvis *
TyvisConcurrentGenerateIfStatement::_get_if_condition() {
  return dynamic_cast<Tyvis *>(get_if_condition());
}

TyvisDeclarationList *
TyvisConcurrentGenerateIfStatement::_get_block_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>(get_block_declarative_part());
}

TyvisArchitectureStatementList *
TyvisConcurrentGenerateIfStatement::_get_concurrent_statement_part() {
  return dynamic_cast<TyvisArchitectureStatementList *>(get_concurrent_statement_part());
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc( published_file &, PublishData *declarations ) {
  ostringstream ent_arch_generate_if;
  string generate_for_unit_name;
  Tyvis *temp                   = _get_current_publish_node();
  const string old_architecture_name = _get_current_architecture_name();
  const string old_current_name      = _get_current_publish_name();
  ostringstream arch_name;

  PublishedUnit saved_publishing_unit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(GENERATE_IF);
  _set_current_publish_node( this );
  
  ent_arch_generate_if << _get_current_entity_name()
			<< "_" << _get_current_architecture_name();
  _publish_cc_enclosing_stmt_to_architecture_path(ent_arch_generate_if);
  ent_arch_generate_if << "_" <<*_get_mangled_label() << "_waits";
  
  generate_for_unit_name = ent_arch_generate_if.str();
  _publish_cc_declarations( declarations );

  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     generate_for_unit_name,
				     this );
  CC_REF( header_file, "TyvisConcurrentGenerateIfStatement::_publish_cc" );
  header_file << "#define " << generate_for_unit_name << "_WAITS_HH\n" << NL();
  header_file << "class State;\n\n";

  published_cc_file cc_file( _get_work_library()->get_path_to_directory(), 
			     generate_for_unit_name,
			     this );
  CC_REF( cc_file, "TyvisConcurrentGenerateIfStatement::_publish_cc" );

  _set_current_publish_name( "SG" );
  _get_concurrent_statement_part()->_publish_cc( cc_file, declarations );

  _set_current_publish_name( old_current_name );
  _set_current_architecture_name( old_architecture_name );
  
  _set_current_publish_node( temp );

  _set_currently_publishing_unit(saved_publishing_unit);
}

void
TyvisConcurrentGenerateIfStatement::_publish_cc_static_type_info_fn( published_file &_cc_out,
							   PublishData *declarations ){
  _get_block_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_declaration_list()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_statement_list()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_if_condition()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_concurrent_statement_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

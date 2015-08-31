
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

#include "TyvisArchitectureStatementList.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisGenericList.hh"
#include "TyvisPortList.hh"
#include "TyvisAssociationList.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisAssociationElement.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisConstantInterfaceDeclaration.hh"
#include "TyvisComponentConfiguration.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisAliasDeclaration.hh"
#include "TyvisBlockStatement.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisDesignator.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisIntegerLiteral.hh"
#include "TyvisConcurrentGenerateForStatement.hh"
#include "TyvisConcurrentGenerateIfStatement.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisIndexedName.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisDesignFile.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include <sstream>

Tyvis*
TyvisBlockStatement::guard_expr = NULL;

TyvisBlockStatement::TyvisBlockStatement() {
  set_generic_clause(new TyvisGenericList());
  set_generic_map_aspect(new TyvisAssociationList());
  set_port_clause(new TyvisPortList());
  set_port_map_aspect(new TyvisAssociationList());
  set_block_declarative_part(new TyvisDeclarationList());
  set_block_statement_part(new TyvisArchitectureStatementList());
}

TyvisBlockStatement::~TyvisBlockStatement() {
  //Release the list memory
  delete get_generic_clause();
  delete get_generic_map_aspect();
  delete get_port_clause();
  delete get_port_map_aspect();
  delete get_block_declarative_part();
  delete get_block_statement_part();
}

void
TyvisBlockStatement::_publish_cc_blocks_elaborate( published_file &_cc_out, PublishData *declarations ){
  TyvisArchitectureStatement *arch_stmt;
  const string temp = _get_current_architecture_name();
  const string old_current_name = _get_current_publish_name();

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_blocks_elaborate" );
  
  _set_current_publish_name( "" );
  
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->first());
  
  while (arch_stmt != NULL) {
    if (arch_stmt->get_kind() != IIR_COMPONENT_INSTANTIATION_STATEMENT &&
	arch_stmt->get_kind() != IIR_PROCESS_STATEMENT) {
      arch_stmt->_publish_cc_elaborate( _cc_out, declarations );
    }
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->successor(arch_stmt));
  }
  _set_current_architecture_name( temp );
  _set_current_publish_name( old_current_name );
}

const string
TyvisBlockStatement::_get_block_unit_name() const {
  ostringstream rstream;
  rstream << *_get_mangled_label();
  return rstream.str();
}

void 
TyvisBlockStatement::_publish_cc_include( published_file &_cc_out ){
  Tyvis::_publish_cc_include( _cc_out, _get_block_unit_name() + ".hh" );
}

void
TyvisBlockStatement::_publish_cc_declarations( PublishData *declarations ) {
  const string &block_unit_name = _get_block_unit_name();

  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     block_unit_name,
				     this );
  CC_REF( header_file, "TyvisBlockStatement::_publish_cc_declarations" );
  _publish_cc_include_elab( header_file );
  // If this block has an implicit guard signal then add it to the list
  if (get_implicit_guard_signal() != NULL) {
    get_block_declarative_part()->append(get_implicit_guard_signal());
  }

  _get_generic_clause()->_publish_cc_elaborate_as_pointers( header_file, declarations, FALSE );
  _get_block_declarative_part()->_publish_cc( header_file, declarations );
  
  published_cc_file cc_file( _get_work_library()->get_path_to_directory(), 
			     block_unit_name,
			     this );
  CC_REF( cc_file, "TyvisBlockStatement::_publish_cc_declarations" );
  cc_file << "#include \"" << block_unit_name << ".hh\""
	  << NL() << NL();

  _get_generic_clause()->_publish_cc_elaborate_as_pointers( cc_file, declarations, TRUE );
  _get_block_declarative_part()->_publish_cc_decl( cc_file, declarations );
}

void
TyvisBlockStatement::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  Tyvis* old_publish_node = _get_current_publish_node();

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_elaborate" );

  _set_current_publish_node( this );
  
  _publish_cc_headerfile( declarations );
  _publish_cc_ccfile( declarations );

  _publish_cc_blocks_elaborate( _cc_out, declarations );

  _set_current_publish_node( old_publish_node );
}

void
TyvisBlockStatement::_publish_cc_headerfile(PublishData *declarations){
  const string filename = _get_cc_elaboration_class_name();

  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     filename,
				     this );
  CC_REF( header_file, "TyvisBlockStatement::_publish_cc_headerfile" );
  
  Tyvis::_publish_cc_include( header_file, "tyvis/_savant_entity_elab.hh" );

//   header_file << "#include \"";
//   header_file << _get_current_publish_name() << _get_current_entity_name() << "_"
// 	  << _get_current_architecture_name() ;
//   _publish_cc_enclosing_stmt_to_architecture_path( header_file );
//   header_file << "_elab.hh\"\n";
  // Include the declarations file 
//   header_file << "#include \"";
//   header_file << _get_current_entity_name() << "_" << _get_current_architecture_name();
//   _publish_cc_enclosing_stmt_to_architecture_path( header_file );
//   header_file << "_" << *(_get_mangled_label());
//   header_file << ".hh\"\n";

  // Request the Hierarchy.hh for our hierarchy stuff
  Tyvis::_publish_cc_include( header_file, "tyvis/Hierarchy.hh" );

  header_file << "\n";
  _publish_cc_class_includes( header_file );
  header_file << "\n";
  _publish_cc_class( header_file, declarations );
}

void
TyvisBlockStatement::_publish_cc_class_includes( published_file &_cc_out ) {
  const string temp = _get_current_architecture_name();
  const string old_current_name = _get_current_publish_name();

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_class_includes" );
  
  _set_current_publish_name( "" );
  _cc_out << "class " << _get_cc_elaboration_class_name() << ";" << NL();

  Tyvis::_publish_cc_class_includes( _cc_out, _get_block_statement_part() );

  _set_current_architecture_name( temp );
  _set_current_publish_name( old_current_name );
}

void
TyvisBlockStatement::_publish_cc_class( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_class" );
  
  _cc_out << "class " << _get_design_unit_name() << ";" << NL()
          << "class " << _get_cc_elaboration_class_name()
	  << " : public _savant_entity_elab"<< OS("{")
	  << "\npublic:" << NL()
	  << _get_cc_elaboration_class_name() << "( _savant_entity_elab * );" << NL()
	  << "_savant_entity_elab *getArchitecture();" << NL(); 
  
  if (_get_generic_clause()->size() > 0) {
    _cc_out << _get_cc_elaboration_class_name()
	    << "(";
    _cc_out << _get_current_publish_name() << _get_design_unit_name();
    _publish_cc_enclosing_stmt_to_architecture_path( _cc_out.get_stream() );
    _cc_out  << " *enclosingScope, " << NL();
    _get_generic_clause()->_publish_generic_parameter_list( _cc_out, declarations );
    _cc_out << ");" << NL();
  }

  _cc_out << "~" << _get_cc_elaboration_class_name() << "();" << NL();

  _cc_out << "void instantiate(Hierarchy * hier, const string parent_base, const char *local_name);" << NL();
  _cc_out << "std::string _base;" << NL();
  _cc_out << "void createNetInfo();" << NL();
  _cc_out << "void connect(int, int, ...);\n" << NL();
  
  _publish_cc_signals( _cc_out, _get_block_declarative_part(), declarations );
  _get_generic_clause()->_publish_cc_elaborate( _cc_out, declarations );
  _get_port_clause()->_publish_cc_elaborate( _cc_out, declarations );
  
  _publish_cc_object_pointers( _cc_out, _get_block_statement_part(), declarations );

  if (_get_generic_clause()->first() != NULL) {
    _cc_out << "void copyGenericsToGlobals();" << NL();
  }
  _cc_out << "\nprivate:" << NL()
	  << "_savant_entity_elab *enclosingScope;" << NL()
	  << CS("};");
  
  _get_block_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_block_statement_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisBlockStatement::_publish_cc_ccfile( PublishData *declarations ){
  const string filename = _get_cc_elaboration_class_name();
  published_cc_file cc_file( _get_work_library()->get_path_to_directory(),
			     filename,
			     this );
  CC_REF( cc_file, "TyvisBlockStatement::_publish_cc_ccfile" );

  _publish_cc_include_elab( cc_file );
  _publish_cc_headerfiles_for_cc( cc_file );
  cc_file << "extern VHDLKernel *proc_array[];" << NL();

  _publish_cc_constructor( cc_file, declarations );
  _publish_cc_destructor( cc_file, _get_block_statement_part() );
  if (_get_generic_clause()->first() != NULL) {
    _publish_cc_copy_generics_to_globals( cc_file, declarations );
  }
  _publish_cc_instantiate( cc_file, declarations );
  _publish_cc_createNetInfo( cc_file, declarations );
  _publish_cc_connect( cc_file, declarations );
  //_publish_cc_partition( cc_file );
}

void
TyvisBlockStatement::_publish_cc_headerfiles_for_cc( published_file &_cc_out ){
  const string temp = _get_current_architecture_name();
  const string old_current_name = _get_current_publish_name();

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_headerfiles_for_cc" );

  _set_current_publish_name( "" );

  _publish_cc_headerfiles_for_cc_default( _cc_out );

  TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->first());
  while(decl != NULL) {
    if(decl->get_kind() == IIR_CONFIGURATION_SPECIFICATION) {
      if (decl->_get_entity_aspect() != NULL) {
	decl->_get_entity_aspect()->_publish_cc_headerfiles_for_cc_default( _cc_out );
      }
    }
    // Nothing for other nodes.
    decl =  dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->successor(decl));
  }

  TyvisArchitectureStatement *arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->first());
  while (arch_stmt != NULL) {
    switch( arch_stmt->get_kind() ){
    case IIR_PROCESS_STATEMENT:
    case IIR_BLOCK_STATEMENT:
    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
      arch_stmt->_publish_cc_headerfiles_for_cc_default( _cc_out );
      break;

    case IIR_COMPONENT_INSTANTIATION_STATEMENT:
      arch_stmt->_publish_cc_headerfiles_for_cc_default( _cc_out );
      if (dynamic_cast<TyvisComponentInstantiationStatement *>(arch_stmt)->_get_configuration() != NULL) {
	(dynamic_cast<TyvisComponentInstantiationStatement *>(arch_stmt))->_get_configuration()->_publish_cc_headers( _cc_out );
      }
      break;
      
    default:
      // do nothing.
      ;
    }

    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->successor( arch_stmt ));
  }
  
  _set_current_architecture_name( temp );
  _set_current_publish_name( old_current_name );
}

void
TyvisBlockStatement::_publish_cc_constructor( published_file &_cc_out,
					      PublishData *declarations ){

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_constructor" );

  _publish_cc_constructor_with_no_arguments( _cc_out, declarations );
  if (get_generic_clause()->size() > 0) {
    _publish_cc_constructor_with_arguments( _cc_out, declarations );
  }
}

void
TyvisBlockStatement::_publish_cc_constructor_with_no_arguments( published_file &_cc_out,
								PublishData *declarations ){
  int           numGenericClause        = get_generic_clause()->size();
  int           numPortClause           = get_port_clause()->size();
  IIR_Boolean   isFirstInitializer      = true;
  
  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_constructor_with_no_arguments" );
  
  _cc_out << _get_cc_elaboration_class_name () << "::" 
	  << _get_cc_elaboration_class_name() << "( _savant_entity_elab *outerScope )";
  
  if(numGenericClause > 0) {
    _cc_out << ":" << NL();
    _get_generic_clause()->_publish_generic_init( _cc_out, declarations );
    isFirstInitializer = false;
  }
  if(numPortClause > 0) {
    if( isFirstInitializer ){
      _cc_out << ":" << NL();
      isFirstInitializer = false;
    }
    else{
      _cc_out << ",";
    }
    _get_port_clause()->_publish_cc_port_init( _cc_out, declarations );
  } else if( numPortClause >  0 ){
    _cc_out << ":" << NL();
    _get_port_clause()->_publish_cc_port_init( _cc_out, declarations );
  }
  
  _publish_cc_signal_objects_init( _cc_out, declarations, isFirstInitializer );
  _cc_out << OS("{");
  _cc_out << "enclosingScope = outerScope;" << NL();

  // before newing the elab guys down the heirarchy copy generics of this
  // block to the generic pointers in the block_decls file
  if (numGenericClause > 0) {
    _cc_out << "copyGenericsToGlobals();" << NL();
  }
  _publish_cc_object_pointers_init( _cc_out,
				    _get_block_statement_part(),
				    _get_block_declarative_part(),
                                    declarations);

  _cc_out << CS("}");

  _get_block_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisBlockStatement::_publish_cc_constructor_with_arguments( published_file &_cc_out, PublishData *declarations ){
  int           numGenericClause        = _get_generic_clause()->size();
  int           numPortClause           = _get_port_clause()->size();
  IIR_Boolean   isFirstInitializer      = true;
  
  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_constructor_with_arguments" );
  _cc_out << _get_cc_elaboration_class_name() << "::" << _get_cc_elaboration_class_name() 
	  << OS("(")
  	  << _get_current_publish_name() << _get_design_unit_name();
  _publish_cc_enclosing_stmt_to_architecture_path( _cc_out.get_stream() );
  _cc_out << " *outerScope,";
    
  _get_generic_clause()->_publish_generic_parameter_list( _cc_out, declarations );
  _cc_out << CS(")");
  
  if(numGenericClause > 0) {
      _cc_out << ":" << NL();    
    _get_generic_clause()->_publish_generic_init_by_arguments( _cc_out, declarations );
    isFirstInitializer = false;
  }
  if(numPortClause > 0) {
    if( isFirstInitializer ){
      _cc_out << ":" << NL();
    }
    else {
      _cc_out << "," << NL();
    }
    _get_port_clause()->_publish_cc_port_init( _cc_out, declarations );
    isFirstInitializer = false;
  }
  
  _publish_cc_signal_objects_init( _cc_out, declarations, TRUE );
  _cc_out << OS("{");
  _cc_out << "enclosingScope = outerScope;" << NL();
  _publish_cc_object_pointers_init( _cc_out, _get_block_statement_part(), _get_block_declarative_part(), declarations );
  _cc_out << CS("}");

  _get_block_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisBlockStatement::_publish_cc_signal_objects_init( published_file &_cc_out,
                                                      PublishData *declarations,
                                                      IIR_Boolean isFirstInitializer ){
  int                   first                   = 0;
  TyvisDeclaration*     decl                    = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->first());
  int                   numGenericClause        = _get_generic_clause()->size();
  int                   numPortClause           = _get_port_clause()->size();

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_signal_objects_init" );
  
  while (decl != NULL) {
    if(decl->get_kind() == IIR_SIGNAL_DECLARATION) {
      if((first == 0) &&  isFirstInitializer && (numGenericClause == 0)
	 && (numPortClause == 0)) {
	_cc_out << ":" << NL();
	first = 1;
      }
      else {
	_cc_out << "," << NL();
      }
      decl->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "("; 
      if (((decl->_get_subtype()->is_array_type() == TRUE) ||
	   (decl->_get_subtype()->is_record_type() ==  TRUE)) &&
	  (decl->_get_subtype()->is_access_type() == FALSE)) {
	decl->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations );
	_cc_out << ", ";
	_cc_out << "\"" << *_get_declarator() << "\"";
      }
      
      _cc_out << ")";
      //If the Signals has implicit signals
      //They are also needed for elaboration info
      if( decl->_get_implicit_declarations() != NULL && 
	  decl->_get_implicit_declarations()->size() != 0) {
	TyvisDeclaration* imp_decl = decl->_get_implicit_declarations()->getElement();
	while(imp_decl != NULL) {
	  if(imp_decl->get_kind() == IIR_SIGNAL_DECLARATION) {
	    _cc_out << "," << NL();
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
    else if(decl->get_kind() == IIR_ALIAS_DECLARATION) {
      if((dynamic_cast<TyvisAliasDeclaration*>(decl))->_get_name()->is_signal()) {
	if((first == 0) && isFirstInitializer && (numGenericClause == 0)
	   && (numPortClause == 0)) {
	  _cc_out << ":" << NL();
	  first = 1;
	}
	else {
	  _cc_out << "," << NL();
	}
	(dynamic_cast<TyvisAliasDeclaration*>(decl))->_publish_cc_elaborate_alias_init( _cc_out, declarations );
      }
    }
    decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->successor(decl));    
  }
}

void
TyvisBlockStatement::_publish_cc_instantiate( published_file &_cc_out,
					      PublishData *declarations ){
  TyvisArchitectureStatement *arch_stmt = NULL;

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_instantiate" );

  _cc_out << "_savant_entity_elab *" << NL() << _get_cc_elaboration_class_name() 
    	  << "::getArchitecture(){ return enclosingScope; }" << NL();

  _cc_out << "void" << NL() << _get_cc_elaboration_class_name() 
    	  << "::instantiate(Hierarchy * hier, const string parent_base, const char *local_name)  {\n";
  _cc_out << "_base = hier->addFrame(parent_base, local_name);\n";

  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->first());
  while (arch_stmt != NULL) {
    arch_stmt->_publish_cc_instantiate_call( _cc_out, declarations );
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->successor(arch_stmt));
  }
  _cc_out << "createNetInfo();" << NL();
  _cc_out << "}\n";
}

void
TyvisBlockStatement::_publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations )
{
  TyvisArchitectureStatement *arch_stmt = NULL;
  IIR_Boolean found = false;
  int wanted_instantiation = 1;
  const string tmp  = _get_current_elab_name();
  const string tmp2 = _get_current_publish_name();
  Tyvis* tmpNode   = _get_current_publish_node();
  PublishedUnit oldunit = _get_currently_publishing_unit();

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_createNetInfo" );
  
  _cc_out << "void" << NL()
	  << _get_cc_elaboration_class_name() <<"::createNetInfo() {" << NL();

  _set_currently_publishing_unit(BLOCK);
  _set_current_publish_node( this );
  
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->first());
  while (arch_stmt!=NULL) {
    _set_current_publish_node( this );
    if (arch_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      ostringstream objectname;
      if(arch_stmt->_get_mangled_label() != NULL) {
	objectname << *(arch_stmt->_get_mangled_label()->_get_mangled_declarator());
      }
      else {
	objectname << "ANON_PROCESS" << arch_stmt;
      }
      objectname << "_elab_obj";
      _set_current_elab_name( objectname.str() );
      arch_stmt->_publish_createNetInfo( _cc_out, declarations );
      _set_current_elab_name( "" );
    }
    else if ((arch_stmt->_is_block_statement() == TRUE) ||
	     (arch_stmt->_is_concurrent_generate_statement() == TRUE )){
    }
    else if ( arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT ){
      arch_stmt->_publish_createNetInfo( _cc_out, declarations );
    }
    else {
      cerr << "ERROR! TyvisBlockStatement::"
	   << "_publish_cc_createNetInfo( _cc_out, declarations ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "| in arch" << endl;
    }
    found = FALSE;
    wanted_instantiation = 1;
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->successor(arch_stmt));
  }
  _cc_out << "}\n\n";

  _set_currently_publishing_unit(oldunit);
  
  _set_current_elab_name( tmp );
  _set_current_publish_name( tmp2 );
  _set_current_publish_node( tmpNode );
}

void
TyvisBlockStatement::_publish_cc_connect( published_file &_cc_out, PublishData *declarations ){
  TyvisAssociationElement* actual_clause = NULL;

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_connect" );
  _publish_cc_include( _cc_out, "cstdarg", true );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/SignalNetinfo.hh" );

  _cc_out << "void" << NL()
	  << _get_cc_elaboration_class_name()
	  << OS("::connect(int inputsignals, int outputsignals, ...) {")
	  << "int NoofSignals = inputsignals + outputsignals;" << NL()
	  << "va_list ap;" << NL()
	  << "va_start(ap, outputsignals);" << NL()
	  << "for(int i=0; i <NoofSignals; i++) {" << NL()
	  << "  addToFanOut( va_arg(ap, RValue*) );" << NL()
	  << "}" << NL()
	  << "va_end(ap);" << NL();
  
  //Pass on the output connection  inforamtion to its output signals
  TyvisInterfaceDeclaration* portelement = NULL;
  IIR_Mode mode;
  int index = 0;

  _cc_out << OS("if(inputsignals > 0){");
  portelement = dynamic_cast<TyvisInterfaceDeclaration *>(get_port_clause()->first());
  actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
  for(; portelement != NULL; ) {
    mode = portelement->get_mode();
    if( mode == IIR_IN_MODE ) {
      if (!(actual_clause->_get_actual()->_is_static_expression())){
	_cc_out << "setSourceInfo(";
	portelement->_publish_cc_elaborate( _cc_out, declarations ); 
	_cc_out << "," << NL() 
		<< "*(fanOutInfo["
		<< index << "]));" << NL();
	index++;
      }
    }
    portelement = dynamic_cast<TyvisInterfaceDeclaration *>(get_port_clause()->successor(portelement));
    if (get_port_map_aspect()->successor(actual_clause)!=NULL){
      actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
    }
  }
  _cc_out << CS("}");
  _cc_out << OS("if( outputsignals > 0 ){");
  portelement = dynamic_cast<TyvisInterfaceDeclaration *>(get_port_clause()->first());
  for(; portelement != NULL; ) {
    mode = portelement->get_mode();
    if(mode == IIR_INOUT_MODE || mode == IIR_OUT_MODE) {
      if (!(actual_clause->_get_actual()->_is_static_expression())){
	_cc_out << OS("Add(");
	portelement->_publish_cc_elaborate( _cc_out, declarations ); 
	_cc_out << "," << NL() 
		<< "*(fanOutInfo["<< index << "])" << CS(");") << NL();
	if (mode != IIR_OUT_MODE){
	  _cc_out << OS("setSourceInfo(");
	  portelement->_publish_cc_elaborate( _cc_out, declarations ); 
	  _cc_out << "," << NL()
		  << "*(fanOutInfo[" << index << "])" << CS(");");
	}
	index++;
      }
    }
    portelement = dynamic_cast<TyvisInterfaceDeclaration *>(get_port_clause()->successor(portelement));
  }
  _cc_out << CS("}");

  TyvisArchitectureStatement *arch_stmt;
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->first());
  while (arch_stmt != NULL) {
    arch_stmt->_publish_cc_connect_call( _cc_out, declarations );
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_block_statement_part()->successor(arch_stmt));
  }
  _cc_out << CS("}");
}

void
TyvisBlockStatement::_publish_cc_connect_call( published_file &_cc_out, PublishData *declarations ){
  TyvisAssociationElement*       actual_clause         = NULL;
  int                                   noofinputsignals      = 0;
  int                                   noofoutputsignals     = 0;
  IIR_Boolean                           first                 = false;
  TyvisFunctionCall              *downTypeConversionFn = NULL;
  TyvisDeclaration               *tempDeclaration      = NULL;
  TyvisLabel                     *label                = _get_label();
  int                                   literal_count         = 0;

  _get_label()->_publish_cc_elaborate( _cc_out.get_stream() );

  if (_get_currently_publishing_unit() == GENERATE_FOR) {
    _cc_out << "_elab_obj[i - generateMin].connect(";
  }
  else {
    _cc_out << "_elab_obj->connect(";
  }

  actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
  
  if (actual_clause == NULL) {
    _cc_out << "0, 0);" << NL();
    return;
  }

  while(actual_clause != NULL) {
    ASSERT(actual_clause->_get_formal() != NULL);
    switch(actual_clause->_get_formal()->_get_mode()) {
    case IIR_IN_MODE:
      noofinputsignals++;
      first = true;
      break;
    case IIR_OUT_MODE:
    case IIR_INOUT_MODE:
      noofoutputsignals++;
      first = true;
      break;
    default:
      // do nothing
      break;
    }
    if (actual_clause->_get_actual()->_is_static_expression() == TRUE){ 
        literal_count++;
    }
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
  }
  
   if (literal_count == noofinputsignals + noofoutputsignals) {
    _cc_out << "0, 0);" << NL();
    return;
  }

  actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());

  while(actual_clause != NULL) {
    ASSERT(actual_clause->_get_formal() != NULL);
    switch(actual_clause->_get_formal()->_get_mode()) {
    case IIR_IN_MODE:
      if (actual_clause->_get_actual()->_is_static_expression()){
	  noofinputsignals--;
      }
      first = true;
      break;
    case IIR_OUT_MODE:
    case IIR_INOUT_MODE:
      if (actual_clause->_get_actual()->_is_static_expression()){
	  noofoutputsignals--;
      }
      first = true;
      break;
    default:
      // do nothing
      break;
    }
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
  }
  
  _cc_out << noofinputsignals << ", " << noofoutputsignals;

  if(first == true) {
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());  
    while(actual_clause != NULL) {
      if (actual_clause->_get_actual()->_is_static_expression() == FALSE){
	if( actual_clause->_get_formal()->_get_mode() == IIR_IN_MODE ){
	  _cc_out << ", &";
	  if(actual_clause == NULL) {
	    ASSERT(actual_clause != NULL);
	  }
	  ASSERT(actual_clause->_get_actual() != NULL);
	  if (actual_clause->_get_actual()->get_kind() ==IIR_FUNCTION_CALL) {
	    dynamic_cast<TyvisFunctionCall *>(actual_clause->get_actual())->_publish_cc_elaborate_arg( _cc_out, declarations );
	  }
	  else {
	    actual_clause->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
	  }
	}
       }
       actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
     }
    
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());  
    for(;actual_clause != NULL; ) {
      switch(actual_clause->_get_formal()->_get_mode()) {
      case IIR_OUT_MODE:
      case IIR_INOUT_MODE:
	_cc_out << ", ";
	_cc_out << "&";
	if (actual_clause->_get_actual()->get_kind() == IIR_FUNCTION_CALL) {
	  dynamic_cast<TyvisFunctionCall *>(actual_clause->_get_actual())->_publish_cc_elaborate_arg( _cc_out, declarations );
	}
	else {
	  if (dynamic_cast<TyvisDeclaration *>(actual_clause->_get_actual()) != NULL) {
	    tempDeclaration = dynamic_cast<TyvisDeclaration *>(actual_clause->_get_actual());
	    ASSERT (tempDeclaration != NULL );
	    _publish_cc_scoping_prefix( _cc_out.get_stream(), 
					tempDeclaration->_get_declarative_region(),
					_get_declarative_region() );
	  }
	  else if (actual_clause->_get_actual()->get_kind() == IIR_INDEXED_NAME) {
	    TyvisIndexedName *tempIdxName = dynamic_cast<TyvisIndexedName *>(actual_clause->_get_actual());
	    ASSERT ( tempIdxName->get_kind() == IIR_INDEXED_NAME );
	    if (dynamic_cast<TyvisDeclaration *>(tempIdxName->get_prefix()) != NULL) {
	      tempDeclaration = dynamic_cast<TyvisDeclaration *>(tempIdxName->get_prefix());

	      ASSERT ( tempDeclaration != NULL );
	      _publish_cc_scoping_prefix( _cc_out.get_stream(),
					  tempDeclaration->_get_declarative_region(),
					  _get_declarative_region());
	    }
	  }
	  
	  actual_clause->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
	}
	break;
      default:
	// do nothing;
	break;
      }
      actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
    }
  }
  _cc_out << ");" << NL();
  
  actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());  
  while(actual_clause != NULL) {
    switch(actual_clause->_get_formal()->_get_mode()) {
    case IIR_IN_MODE:
    case IIR_INOUT_MODE:
      ASSERT(actual_clause->_get_actual() != NULL);
      if(actual_clause->_get_actual()->get_kind() == IIR_FUNCTION_CALL) {
	downTypeConversionFn = dynamic_cast<TyvisFunctionCall *>(actual_clause->_get_actual());
	_cc_out << "addDownTypeConversionFn(";
	if(actual_clause->get_formal()->get_kind() == IIR_FUNCTION_CALL) {
	  label->_publish_cc_elaborate( _cc_out.get_stream() );
	  _cc_out << "_elab_obj->";
	  dynamic_cast<TyvisFunctionCall *>(actual_clause->get_formal())->_publish_cc_elaborate_arg( _cc_out, declarations );
	}
	else {
	  label->_publish_cc_elaborate( _cc_out.get_stream() );
	  
	  if (_get_currently_publishing_unit() == GENERATE_FOR) {
	    _cc_out << "_elab_obj[i - generateMin].";
	  }
	  else {
	    _cc_out << "_elab_obj->";
	  }
	  actual_clause->_get_formal()->_publish_cc_elaborate( _cc_out, declarations );
	}
	_cc_out << ", ";
	dynamic_cast<TyvisFunctionCall *>(actual_clause->_get_actual())->_publish_cc_elaborate_arg( _cc_out, declarations );
	_cc_out << ", ";
	downTypeConversionFn->_get_implementation()->_publish_cc_type_conversion_function_name( _cc_out, declarations );
	_cc_out << ");" << NL();
      }
      break;
    default:
      // do nothing
      break;
    }
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
  }
}

void 
TyvisBlockStatement::_publish_cc_elaborate_add( published_file &_cc_out,
                                                       TyvisAssociationElement *actual_clause,
                                                       TyvisLabel *label,
                                                       PublishData *declarations) {
  Tyvis* decl = actual_clause->_get_formal();
  TyvisFunctionCall *upTypeConversionFn = NULL;
  TyvisFunctionCall *downTypeConversionFn = NULL;
  TyvisDeclaration* tempDeclaration = NULL;

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_elaborate_Add" );
    
  if (actual_clause->_get_actual()->get_kind()!=IIR_FUNCTION_CALL &&
      actual_clause->_get_formal()->get_kind()!=IIR_FUNCTION_CALL){
    _cc_out << "Add(";
    ASSERT(actual_clause->_get_actual() != NULL);
    if(actual_clause->_get_actual()->get_kind() == IIR_FUNCTION_CALL) {
      downTypeConversionFn = dynamic_cast<TyvisFunctionCall *>(actual_clause->_get_actual());
      downTypeConversionFn->_publish_cc_elaborate_arg( _cc_out, declarations );
    } else {
      if(dynamic_cast<TyvisDeclaration *>(actual_clause->_get_actual()) && 
	 actual_clause->_get_actual()->_is_static_expression() == FALSE) {
	tempDeclaration = dynamic_cast<TyvisDeclaration *> (actual_clause->_get_actual());
        ASSERT( tempDeclaration != NULL );

	_publish_cc_scoping_prefix( _cc_out.get_stream(), tempDeclaration->_get_declarative_region(), _get_declarative_region( ));
      }
      else if(actual_clause->_get_actual()->is_name() == TRUE) {
	tempDeclaration = dynamic_cast<TyvisDeclaration *> (actual_clause->_get_actual()->_get_prefix_declaration());
	ASSERT ( tempDeclaration != NULL );
	_publish_cc_scoping_prefix( _cc_out.get_stream(), 
				    tempDeclaration->_get_declarative_region(),
				    _get_declarative_region());
      }
      else if (actual_clause->_get_actual()->_is_static_expression() == TRUE) {
	_cc_out << "*(new ";
	actual_clause->_get_actual()->_get_subtype()->_publish_cc_type_string( _cc_out );
	_cc_out << "(ObjectBase::SIGNAL_NETINFO, ";
	actual_clause->_get_actual()->_publish_cc_value( _cc_out, declarations );
	actual_clause->_get_actual()->_get_subtype()->_publish_cc_object_type_info( _cc_out, declarations );
	_cc_out << "))";
      }
      else{
	cerr << "Error : TyvisBlockStatement::_publish_cc_elaborate_addChild( published_file &_cc_out, PublishData *declarations ) - declaration kind not handled yet.\n";
      }
      
      if (actual_clause->_get_actual()->_is_static_expression() == FALSE){
	actual_clause->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
      }
      
      _cc_out << ", ";
      label->_publish_cc_elaborate( _cc_out.get_stream() );
      
      if (_get_currently_publishing_unit() == GENERATE_FOR) {
	_cc_out << "_elab_obj[i - generateMin].";
      }
      else {
	_cc_out << "_elab_obj->";
      }
      
      if(decl->get_kind() == IIR_FUNCTION_CALL) {
	upTypeConversionFn = dynamic_cast<TyvisFunctionCall *>(decl);
	upTypeConversionFn->_publish_cc_elaborate_arg( _cc_out, declarations );
      } else {
	decl->_publish_cc_elaborate( _cc_out, declarations );
      }    
      _cc_out << ");\n";
      
      if(upTypeConversionFn != NULL) {
	_cc_out << "setUpConversionFunctionId(";
	label->_publish_cc_elaborate( _cc_out.get_stream() );
	
	if (_get_currently_publishing_unit() == GENERATE_FOR) {
	  _cc_out << "_elab_obj[i - generateMin].";
	}
	else {
	  _cc_out << "_elab_obj->";
	}
	
	upTypeConversionFn->_publish_cc_elaborate_arg( _cc_out, declarations );
	_cc_out << ", ";
	upTypeConversionFn->_get_implementation()->_publish_cc_type_conversion_function_name( _cc_out, declarations );
	_cc_out << ");\n";
      }
    }
  }
}

void 
TyvisBlockStatement::_publish_cc_elaborate_addChild( published_file &_cc_out,
                                                            TyvisAssociationElement *actual_clause,
                                                            TyvisLabel *label,
                                                            PublishData *declarations) {
  Tyvis*             decl            = actual_clause->_get_formal();
  TyvisDeclaration* tempDeclaration = NULL;

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_elaborate_addChild" );
 
  if (actual_clause->_get_actual()->get_kind() != IIR_FUNCTION_CALL &&
      actual_clause->get_formal()->get_kind() != IIR_FUNCTION_CALL){
     _cc_out << "addChild(";
  if(actual_clause->_get_actual()->get_kind() == IIR_FUNCTION_CALL) {
    //((TyvisFunctionCall *)actual_clause->_get_actual())->_publish_cc_elaborate_arg( _cc_out, declarations );
  } else {
    if( dynamic_cast<TyvisDeclaration *>(actual_clause->_get_actual()) ) {
      tempDeclaration = dynamic_cast<TyvisDeclaration *>(actual_clause->_get_actual());
      ASSERT ( tempDeclaration != NULL );
      _publish_cc_scoping_prefix( _cc_out.get_stream(), 
				  tempDeclaration->_get_declarative_region(),
				  _get_declarative_region());
    }
    else if(actual_clause->_get_actual()->is_name() == TRUE) {
      tempDeclaration = (TyvisDeclaration *) actual_clause->_get_actual()->_get_prefix_declaration();
      ASSERT ( tempDeclaration != NULL );
      _publish_cc_scoping_prefix( _cc_out.get_stream(), 
				  tempDeclaration->_get_declarative_region(),
				  _get_declarative_region());
    }
    else if (dynamic_cast<TyvisLiteral *>(actual_clause->_get_actual())){
             
      cerr << "Error : TyvisBlockStatement::_publish_cc_elaborate_addChid( published_file &_cc_out ) - declaration kind not handled yet.\n";
    }
    actual_clause->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
  }
    
  _cc_out << ", ";
  label->_publish_cc_elaborate( _cc_out.get_stream() );

  if (_get_currently_publishing_unit() == GENERATE_FOR) {
    _cc_out << "_elab_obj[i - generateMin].";
  }
  else {
    _cc_out << "_elab_obj->";
  }
  
  if(decl->get_kind() == IIR_FUNCTION_CALL) {
    //upTypeConversionFn = (TyvisFunctionCall *)decl;
    //upTypeConversionFn->_publish_cc_elaborate_arg( _cc_out, declarations );
  } else {
    if ( dynamic_cast<TyvisDeclaration *>(decl) ) {
      tempDeclaration = dynamic_cast<TyvisDeclaration *>(decl);
      ASSERT ( tempDeclaration != NULL );
    }
    else if ( decl->is_name() == TRUE ) {
      tempDeclaration = dynamic_cast<TyvisDeclaration *>(decl->_get_prefix_declaration());
      ASSERT ( tempDeclaration != NULL );
      _publish_cc_scoping_prefix( _cc_out.get_stream(), 
				  tempDeclaration->_get_declarative_region(),
				  _get_declarative_region());
    }
    else {
      cerr << "Error : TyvisBlockStatement::_publish_cc_elaborate_addChid( published_file &_cc_out ) - declaration kind not handled yet.\n";
    }
    
    decl->_publish_cc_elaborate( _cc_out, declarations );
  }    
  _cc_out << ");\n";
  }
}

void
TyvisBlockStatement::_publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc_type_info" );

  _get_block_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_block_statement_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisBlockStatement::_publish_cc_include_elab( published_file &_cc_out ){
  const string filename = _get_cc_elaboration_class_name() + ".hh";
  Tyvis::_publish_cc_include( _cc_out, filename );
}

void
TyvisBlockStatement::_publish_cc( published_file &_cc_out, PublishData *declarations ) {
  Tyvis *temp = _get_current_publish_node();
  const string old_architecture_name = _get_current_architecture_name();
  const string old_current_name = _get_current_publish_name();
  Tyvis *old_guard_expr;

  CC_REF( _cc_out, "TyvisBlockStatement::_publish_cc" );

  PublishedUnit _saved_publishing_unit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(BLOCK);
  _set_current_publish_name( "" );
  _set_current_publish_node( this );

  _publish_cc_declarations( declarations );
  old_guard_expr = guard_expr;
  // Set the static variable so that it can be used in the process using guards
  guard_expr = _get_guard_expression();
  _get_block_statement_part()->_publish_cc( _cc_out, declarations );
  guard_expr = old_guard_expr;

  _set_current_publish_name( old_current_name );
  _set_current_architecture_name( old_architecture_name );
  _set_current_publish_node( temp );
  _set_currently_publishing_unit(_saved_publishing_unit);
}

void
TyvisBlockStatement::_publish_createNetInfo( published_file &_cc_out, PublishData *declarations ){    
  TyvisAssociationElement* actual_clause = NULL;
  Tyvis* formal = NULL;
  TyvisLabel *label = _get_mangled_label();

  actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
  if(actual_clause == NULL){
    return;
  }
  
  while(actual_clause != NULL) {
    formal = actual_clause->_get_formal();
    ASSERT(formal != NULL);
    ASSERT(formal->is_resolved() == TRUE);
    switch(formal->_get_mode()) {
    case IIR_IN_MODE:
      _publish_cc_elaborate_add( _cc_out, actual_clause, label, declarations);
      break;
    case IIR_OUT_MODE:
      // Code for adding the resolution information tree
      _publish_cc_elaborate_addChild( _cc_out, actual_clause, label, declarations );
      break;
    case IIR_INOUT_MODE:
      _publish_cc_elaborate_add( _cc_out, actual_clause, label, declarations );
      // Code for adding the resolution information tree
      _publish_cc_elaborate_addChild( _cc_out, actual_clause, label, declarations );
      break;
    default:
      cerr << "Buffer and linkage mode not yet supported in "
	   << "Component instantiation statements\n";
      break;
    }
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
  }
}

TyvisList *
TyvisBlockStatement::_get_statement_list(){
  return dynamic_cast<TyvisList *>(get_block_statement_part());
}

TyvisAssociationList *
TyvisBlockStatement::_get_generic_map_aspect() {
    return dynamic_cast<TyvisAssociationList *>(get_generic_map_aspect());
}

TyvisAssociationList *
TyvisBlockStatement::_get_port_map_aspect() {
    return dynamic_cast<TyvisAssociationList *>(get_port_map_aspect());
}

Tyvis *
TyvisBlockStatement::_get_guard_expression() {
  return dynamic_cast<Tyvis *>(get_guard_expression());
}

TyvisGenericList *
TyvisBlockStatement::_get_generic_clause() {
  return dynamic_cast<TyvisGenericList *>(get_generic_clause());
}

TyvisArchitectureStatementList *
TyvisBlockStatement::_get_block_statement_part() {
  return dynamic_cast<TyvisArchitectureStatementList *>(get_block_statement_part());
}

TyvisGenericList *
TyvisBlockStatement::_get_generic_list(){
  return dynamic_cast<TyvisGenericList *>(get_generic_clause());
}

TyvisPortList *
TyvisBlockStatement::_get_port_clause() {
  return dynamic_cast<TyvisPortList *>(get_port_clause());
}

TyvisDeclarationList *
TyvisBlockStatement::_get_block_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>(get_block_declarative_part());
}

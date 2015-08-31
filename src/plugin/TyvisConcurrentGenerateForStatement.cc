
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

#include "TyvisConcurrentGenerateForStatement.hh"
#include "TyvisAliasDeclaration.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisAssociationList.hh"
#include "TyvisBlockStatement.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisConstantDeclaration.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignator.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisPortList.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSimpleName.hh"

#include "savant/set.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include <sstream>

TyvisConcurrentGenerateForStatement::TyvisConcurrentGenerateForStatement(){
  set_block_declarative_part(new TyvisDeclarationList());
  set_concurrent_statement_part(new TyvisArchitectureStatementList());
}

TyvisConcurrentGenerateForStatement::~TyvisConcurrentGenerateForStatement(){
  //Release the list memory
  delete get_block_declarative_part();
  delete get_concurrent_statement_part();
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_generate_elaborate( published_file &_cc_out, PublishData *declarations ){
  TyvisArchitectureStatement *arch_stmt = NULL;
  const string temp = _get_current_architecture_name();
  const string old_current_name = _get_current_publish_name();
  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_generate_elaborate" );
  _set_current_publish_name( "SG" );

  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());

  while (arch_stmt != NULL) {
    // Need to check if generate statements can have other concurrent
    // statements that will need elaboration. If so that check should
    // be added to the following if condition.

    switch (arch_stmt->get_kind()) {
    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_BLOCK_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
      arch_stmt->_publish_cc_elaborate( _cc_out, declarations );
      break;

    case IIR_PROCESS_STATEMENT:
    case IIR_COMPONENT_INSTANTIATION_STATEMENT:
      // Prevent the error message from popping up.
      break;
      
    default:
      cerr << "ERROR! TyvisConcurrentGenerateForStatement::"
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
TyvisConcurrentGenerateForStatement::_publish_cc_init( published_file &_cc_out,
						       Tyvis *scope, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_init" );
  _get_generate_parameter_specification()->_publish_cc_lvalue( _cc_out, declarations );
  _cc_out << ".updateVal(";
  _publish_cc_scoping_prefix( _cc_out.get_stream(), scope, this );
  _cc_out << "getGenerateConstant(1));" << NL();
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_universal_generate_range( published_file &_cc_out, PublishData *declarations )
{

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_universal_generate_range" );

  _cc_out << "(";
  
  if (_get_generate_parameter_specification()->_get_subtype()->is_ascending_range()) {
    (dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype()))->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
    (dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype()))->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " + 1";
  }
  else {
    (dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype()))->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
    (dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype()))->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " + 1";
  }
  
  _cc_out << ")";
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_assign_to_loop_constant( published_file &_cc_out,
                                                                          PublishData *declarations,
                                                                          const char *localIterator){

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_assign_to_loop_constant" );

  _cc_out << "Variable::assign(";
  _get_generate_parameter_specification()->_publish_cc_lvalue( _cc_out, declarations );
  _cc_out << ", "
	  << localIterator
	  << ");" << NL();
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_generate_for_loop( published_file &_cc_out, PublishData *declarations )
{
  TyvisScalarTypeDefinition *range = NULL;

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_generate_for_loop" );

  range = dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype());
  ASSERT(range != NULL);

  _cc_out << "  for(i = ";
  range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
  _cc_out << "; (i ";

  if (range->is_ascending_range() == TRUE) {
    _cc_out << " <= ";
  }
  else {
    _cc_out << " >= ";
  }

  range->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );
  _cc_out << "); ";

  if (range->is_ascending_range() == TRUE) {
    _cc_out << "i++";
  }
  else {
    _cc_out << "i--";
  }

  _cc_out << ")  ";
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_declarations( published_file &cc_out,
							       PublishData *declarations ) {
  ostringstream ent_arch_generate;

  ent_arch_generate << _get_current_entity_name() << "_"
		    << _get_current_architecture_name();
  _publish_cc_enclosing_stmt_to_architecture_path(ent_arch_generate);
  ent_arch_generate << "_" << *(_get_mangled_label());

  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     ent_arch_generate.str(),
				     this );
  CC_REF( header_file, "TyvisConcurrentGenerateForStatement::_publish_cc_declarations" );
  

  Tyvis::_publish_cc_include( header_file, _get_design_unit_name() + ".hh" );
  
  _get_block_declarative_part()->_publish_cc( header_file, declarations );
  
  CC_REF( header_file, "TyvisConcurrentGenerateForStatement::_publish_cc_declarations" );

  header_file << "extern Variable ";
  _get_generate_parameter_specification()->_publish_cc_lvalue( header_file, declarations );
  header_file << ";" << NL();
  _get_generate_parameter_specification()->_publish_cc_static_type_info_fn( header_file, declarations );
  
  published_cc_file cc_file( _get_work_library()->get_path_to_directory(), 
			     ent_arch_generate.str(),
			     this );
  CC_REF( cc_file, "TyvisConcurrentGenerateForStatement::_publish_cc_declarations" );

  cc_file << "#include \"" << ent_arch_generate.str() << ".hh\"\n\n";
  
  _get_block_declarative_part()->_publish_cc_decl( cc_file, declarations );

  cc_file << "Variable ";
   _get_generate_parameter_specification()->_publish_cc_lvalue( cc_file, declarations );
  cc_file << "(";
  _get_generate_parameter_specification()->_publish_cc_type_info_xxx( cc_file, declarations );
  cc_file << ", string(\"\"));" << NL();

}

void
TyvisConcurrentGenerateForStatement::_publish_cc_class( published_file &_cc_out,
							PublishData *declarations ) {
  _publish_cc_class_generate_stmt( _cc_out, declarations );
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_headerfile(PublishData *declarations) {
  ostringstream filename;

  _publish_cc_class_name(filename);
  
  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     filename.str(),
				     this );
  CC_REF( header_file, "TyvisConcurrentGenerateForStatement::_publish_cc_headerfile" );

  Tyvis::_publish_cc_include( header_file, "tyvis/_savant_entity_elab.hh" );
  Tyvis::_publish_cc_include( header_file, "tyvis/Hierarchy.hh" );
  Tyvis::_publish_cc_include( header_file, _get_design_unit_name() + ".hh" );

  //  _publish_cc_headers( header_file );
  header_file << "\n";
  _publish_cc_class_includes( header_file, _get_concurrent_statement_part() );
  header_file << "\n";
  _publish_cc_class( header_file, declarations );
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  PublishedUnit saved_publishing_unit = _get_currently_publishing_unit();

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_elaborate" );

  _set_currently_publishing_unit(GENERATE_FOR);
  _publish_cc_headerfile( declarations );
  _publish_cc_ccfile( declarations );
  _publish_cc_generate_elaborate( _cc_out, declarations );
  _set_currently_publishing_unit(saved_publishing_unit);
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_ccfile(PublishData *declarations){
  ostringstream filename;

  _publish_cc_class_name(filename);

  published_cc_file cc_file( _get_work_library()->get_path_to_directory(), 
			     filename.str(),
			     this );
  CC_REF( cc_file, "TyvisConcurrentGenerateForStatement::_publish_cc_ccfile" );

  cc_file << "#include \"" << filename.str() << ".hh\"\n\n";
  _publish_cc_headerfiles_for_cc( cc_file );

  ostringstream ent_arch_generate;

  ent_arch_generate << _get_current_entity_name() << "_"
		    << _get_current_architecture_name();
  _publish_cc_enclosing_stmt_to_architecture_path(ent_arch_generate);
  ent_arch_generate << "_" << *(_get_mangled_label());
  cc_file << "#include \"" << ent_arch_generate.str() << ".hh\"\n";

  _publish_cc_static_members( cc_file );
  _publish_cc_constructor( cc_file, declarations );
  _publish_cc_destructor( cc_file, _get_concurrent_statement_part() );
  _publish_cc_instantiate( cc_file, declarations );
  _publish_cc_createNetInfo( cc_file, declarations );
  _publish_cc_connect( cc_file, declarations );
  _publish_cc_getGenerateConstant( cc_file );
  _publish_cc_resetGenerateConstant( cc_file, declarations );
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_static_members( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_static_members" );

  _cc_out << "int\n"
	  << _get_cc_elaboration_class_name() 
	  << "::generateConstant = 0;\n\n"

	  << "ArrayInfo::ArrayDirn_t\n"
	  << _get_cc_elaboration_class_name()
	  << "::generateDirection = ";
  if( _get_generate_parameter_specification()->is_ascending_range() ){
    _cc_out << "ArrayInfo::to";
  }
  else {
    _cc_out << "ArrayInfo::downto";
  }
  _cc_out << ";\n\n";
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_getGenerateConstant( published_file &_cc_out ){
  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_getGenerateConstant" );

  _cc_out << "Value" << NL()
	  << _get_cc_elaboration_class_name()
	  << "::getGenerateConstant( int flag )" << OS("{")
	  << "UniversalInteger returnValue(generateConstant);" << NL()
	  << "if( flag ) generateConstant +="
	  << "((generateDirection == ArrayInfo::to) ? 1 : -1);" << NL()
	  << "return returnValue;" 
	  << CS("}");
}
  
void
TyvisConcurrentGenerateForStatement::_publish_cc_resetGenerateConstant( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_resetGenerateConstant" );

  _cc_out << "void "
	  << _get_cc_elaboration_class_name()
	  << "::resetGenerateConstant()  {\n"
	  << "  generateConstant = ";
  (dynamic_cast<TyvisScalarTypeDefinition *>( _get_generate_parameter_specification()->_get_subtype()))->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
  _cc_out << ";\n}\n\n";
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_headerfiles_for_cc( published_file &_cc_out ){

  CC_REF( _cc_out, 
		"TyvisConcurrentGenerateForStatement::_publish_cc_headerfiles_for_cc" );

  _publish_cc_headerfiles_for_cc_generate_statement( _cc_out );
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_constructor( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_constructor" );

  _publish_cc_constructor_with_no_arguments( _cc_out, declarations );
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations )
{

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_constructor_with_no_arguments" );
  _cc_out << _get_cc_elaboration_class_name() << "::"
	  << _get_cc_elaboration_class_name() << "(";

  ASSERT ( _get_declarative_region() != NULL );
  _get_declarative_region()->_publish_cc_class_name( _cc_out.get_stream() );
  _cc_out << "* outerScope)  {\n";
  _cc_out << "  generateConstant = ";
  (dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype()))->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
  _cc_out << ";\n";
  _cc_out << "  enclosingScope = outerScope;\n";
  
  _publish_cc_object_pointers_init( _cc_out, declarations );
  _cc_out << "}\n";
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_signal_objects_init( published_file &_cc_out,
                                                                             PublishData *declarations,
                                                                             IIR_Boolean first_decl_flag ){
  TyvisDeclaration *decl;
  int first = 0;

  CC_REF( _cc_out, 
		"TyvisConcurrentGenerateForStatement::_publish_cc_signal_objects_init" );

  decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->first());

  while (decl != NULL) {
    if (decl->get_kind() == IIR_SIGNAL_DECLARATION) {
      if ( first == 0 && first_decl_flag == TRUE ){
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
      
      if( decl->_get_implicit_declarations() != NULL
	  && decl->_get_implicit_declarations()->size() != 0 ) {
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
	if( first == 0 && first_decl_flag == 1 ){
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
TyvisConcurrentGenerateForStatement::_publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations ){
  const string temp = _get_current_architecture_name();
  const string old_current_name = _get_current_publish_name();

  TyvisArchitectureStatement *arch_stmt;
  IIR_Boolean found = FALSE;
  Tyvis* tmpNode = NULL;

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_object_pointers_init" );

  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());

  while (arch_stmt != NULL) {
    switch(arch_stmt->get_kind()) {
    case IIR_PROCESS_STATEMENT:
      if (arch_stmt->_get_mangled_label() != NULL) {
	_cc_out.get_stream() << *(arch_stmt->_get_mangled_label());
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

      _cc_out.get_stream() << *(arch_stmt->_get_mangled_label());
      _cc_out << "_elab_obj = new "
	      << arch_stmt->_get_cc_elaboration_class_name()
	      << "[";
      _publish_cc_universal_generate_range( _cc_out, declarations );
      _cc_out << "]";
      _cc_out << ";\n";

      _cc_out << "for(int i = 0; (i < ";
      _publish_cc_universal_generate_range( _cc_out, declarations );
      _cc_out << "); i++)  {\n";

      tmpNode = _get_current_publish_node();
      _set_current_publish_node( this );     
      arch_stmt->_publish_cc_concurrent_stmt_init( _cc_out, dynamic_cast<TyvisDeclarationList *>(get_block_declarative_part()), declarations );
      _set_current_publish_node( tmpNode );
      _cc_out << "}\n";
      
      break;

    case IIR_BLOCK_STATEMENT:      {
      _set_current_publish_name( "SG" );

      arch_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "_elab_obj = new "
	      << arch_stmt->_get_cc_elaboration_class_name() << "[";
      _publish_cc_universal_generate_range( _cc_out, declarations );
      _cc_out << "](this";
      
      if ((dynamic_cast<TyvisBlockStatement *>(arch_stmt))->get_generic_map_aspect()->size() > 0) {
	_cc_out << ", \n";
	tmpNode = _get_current_publish_node();
	_set_current_publish_node( arch_stmt );
	dynamic_cast<TyvisAssociationList *>((dynamic_cast<TyvisBlockStatement *>(arch_stmt))->get_generic_map_aspect())->_publish_cc_generic_map_aspect_for_conc_stmts( _cc_out, declarations );
	_set_current_publish_node( tmpNode );
      }
      
      _cc_out << ");\n";
      
      _set_current_architecture_name( temp );
      _set_current_publish_name( old_current_name );
      }
      break;
    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:{
	_set_current_publish_name( "SG" );
      
      arch_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "_elab_obj = new "
	      << arch_stmt->_get_cc_elaboration_class_name() << "[";
      _publish_cc_universal_generate_range( _cc_out, declarations );
      _cc_out << "](this);\n";

      _set_current_architecture_name( temp );
      _set_current_publish_name( old_current_name );
      }
    break;

    default:
      cerr << "ERROR! TyvisConcurrentGenerateForStatement::"
	   << "_publish_cc_object_pointers_init( _cc_out, declarations ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "|" << endl;
      break;
    }

    found = FALSE;
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor(arch_stmt));
  }
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_instantiate( published_file &_cc_out, PublishData *declarations )
{
  TyvisArchitectureStatement *arch_stmt = NULL;

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_instantiate" );
  
  _cc_out << "void\n"
	  << _get_cc_elaboration_class_name()
          << OS("::instantiate(Hierarchy * hier, const string parent_base, const char *local_name)  {");
  _cc_out << "_base = hier->addFrame(parent_base, local_name);" << NL();


  _cc_out << "int generateMin = ";

  TyvisScalarTypeDefinition *range =
    dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype());

  if ( range->is_ascending_range() == TRUE )
    range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
  else
    range->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );

  _cc_out << ";" << NL();
  _cc_out << "int i;\n";
  
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());
  while (arch_stmt != NULL) {
    if (arch_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      ostringstream objectname;
      if (arch_stmt->_get_mangled_label() != NULL) {
	objectname << *(arch_stmt->_get_mangled_label());
      }
      else {
	objectname << "ANON_PROCESS" << arch_stmt;
      }
      _cc_out << objectname.str() << "_elab_obj = new " << objectname.str();
      _cc_out << " *[";
      _publish_cc_universal_generate_range( _cc_out, declarations );
      _cc_out << "]";
      _cc_out << ";\n";
      
      _cc_out << "for(i = 0; (i < ";
      _publish_cc_universal_generate_range( _cc_out, declarations );
      _cc_out << OS("); i++)  {");
      _cc_out << "    " << objectname.str() << "_elab_obj[i] = new ";
      _cc_out << objectname.str() << "(this);" ;
      _cc_out << CS("}");
      
      _set_number_of_processes( _get_number_of_processes() + 1 );
    }
    else if ((arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT)||
	     (arch_stmt->_is_concurrent_generate_statement() == TRUE) ||
	     (arch_stmt->_is_block_statement() == TRUE)) {
      _publish_cc_generate_for_loop( _cc_out, declarations );
      _cc_out << OS("{");
      _publish_cc_include( _cc_out, "sstream", true );
      _cc_out << "ostringstream l_name;" << NL();
      _cc_out << "l_name << \"gen_\" << i;" << NL();
//      _publish_cc_assign_to_loop_constant( _cc_out, declarations );
      (arch_stmt->_get_mangled_label())->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "_elab_obj[i - generateMin].instantiate(hier, _base, l_name.str().c_str()" << " );";
      _cc_out << CS("}");
    }
    else {
      cerr << "ERROR! TyvisConcurrentGenerateForStatement::"
	   << "_publish_cc_instantiate( _cc_out ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "|" << endl;
    }
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor(arch_stmt));
  }
  _cc_out << "createNetInfo();";
  _cc_out << CS("}\n");
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_body_for_component_createNetInfo( published_file &_cc_out,
                                                                                          TyvisArchitectureStatement *arch_stmt,
                                                                                          PublishData *declarations){
  ostringstream labelname;
  ostringstream objectname;
  int wanted_instantiation = 1;
  IIR_Boolean found = false;
  TyvisEntityDeclaration* bindingEntity = NULL;
  int noofelements = 0;

  CC_REF( _cc_out, 
		"TyvisConcurrentGenerateForStatement::_publish_cc_body_for_component_createNetInfo" );
  
  labelname << *(arch_stmt->_get_mangled_label()) << "_elab_obj[i].";
  _set_current_elab_name( labelname.str() );
  Tyvis* componentName = arch_stmt->_get_instantiated_unit();
      
  if(componentName->get_kind() == IIR_COMPONENT_DECLARATION) {
    TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration *>(get_block_declarative_part()->first());
    while(decl != NULL) {
      if(decl->get_kind() == IIR_CONFIGURATION_SPECIFICATION) {
	TyvisTextLiteral* local_componentName = componentName->_get_mangled_declarator();
	TyvisTextLiteral* binding_componentName = (dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_component_name()->_get_mangled_declarator();
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
		  TyvisLibraryUnit* lib_unit =(dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_entity_aspect(); 
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
		  TyvisLibraryUnit* lib_unit = (dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_entity_aspect(); 
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
		    TyvisLibraryUnit* binding_unit =(dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_entity_aspect();
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
		TyvisLibraryUnit* lib_unit =(dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_entity_aspect(); 
		if(lib_unit != NULL) {
		  lib_unit->_publish_cc_binding_name(objectname);
		  bindingEntity = lib_unit->_get_entity();
		}
		objectname << "_elab*) " << (arch_stmt->_get_mangled_label());
		objectname << "_elab_obj->boundedEntity)->";
		_set_current_publish_name( objectname.str() );
		dynamic_cast<TyvisPortList *>(bindingEntity->get_port_clause())->_publish_cc_port_map_aspect( _cc_out, declarations );
	      }
	      else {
		_set_current_publish_node( componentName );
		objectname << "(( ";
		TyvisLibraryUnit* lib_unit =(dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_entity_aspect();
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
		  TyvisLibraryUnit* binding_unit =(dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_entity_aspect();
		  if(binding_unit!= NULL) {
		    dynamic_cast<TyvisPortList *>(binding_unit->_get_entity()->get_port_clause())->_publish_cc_port_map_aspect( _cc_out, declarations );
		  }
		}
	      }
	      break;

	    default:
	      break;
	    }
	    if(found == TRUE) {
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
TyvisConcurrentGenerateForStatement::_publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations ){
  TyvisArchitectureStatement *arch_stmt = NULL;
  IIR_Boolean found = false;
  int wanted_instantiation = 1;
  const string tmp  = _get_current_elab_name();
  const string tmp2 = _get_current_publish_name();
  Tyvis* tmpNode   = _get_current_publish_node();
  PublishedUnit old_publishing_unit = _get_currently_publishing_unit();

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_createNetInfo" );

  _set_currently_publishing_unit(GENERATE_FOR);
  
  _cc_out << "void\n"
	  << _get_cc_elaboration_class_name()
	  << "::createNetInfo() {\n"
	  << "  int generateMin = ";

  TyvisScalarTypeDefinition *range =
    dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype());

  if ( range->is_ascending_range() == TRUE )
    range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
  else
    range->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );

  _cc_out << ";\n\n";
  _cc_out << "int i;\n";
  
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());
  while (arch_stmt != NULL) {
    ostringstream objectname;
    ostringstream labelname;
    _set_current_publish_name( "" );
    _set_current_publish_node( this );
    
    if (arch_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      _publish_cc_generate_for_loop( _cc_out, declarations );
      _cc_out << "{\n";
      _publish_cc_assign_to_loop_constant( _cc_out, declarations );
      
      if( arch_stmt->_get_mangled_label() != NULL ){
	objectname << *(arch_stmt->_get_mangled_label());
      }
      else {
	objectname << "ANON_PROCESS" << arch_stmt;
      }
      objectname << "_elab_obj";
      _set_current_elab_name( objectname.str() );
      arch_stmt->_publish_createNetInfo( _cc_out, declarations );
      _cc_out << "}\n";
    }
    else if ((arch_stmt->get_kind() == IIR_BLOCK_STATEMENT) ||
	     (arch_stmt->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT)||
	     (arch_stmt->get_kind() == IIR_CONCURRENT_GENERATE_IF_STATEMENT)) {
      // Nothing to be done for these guys
    }
    else if (arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
      _publish_cc_generate_for_loop( _cc_out, declarations );
      _cc_out << "{\n";
      _publish_cc_assign_to_loop_constant( _cc_out, declarations );
      _publish_cc_body_for_component_createNetInfo( _cc_out, arch_stmt, declarations );
      _cc_out << "  }\n";
    }
    else {
      cerr << "ERROR! TyvisConcurrentGenerateForStatement::"
	   << "_publish_cc_createNetInfo( _cc_out ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "| in arch" << endl;
    }
    _set_current_elab_name( "" );
    found = FALSE;
    wanted_instantiation = 1;
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor(arch_stmt));
  }
  _cc_out << "}\n\n";
  _set_current_elab_name( tmp );
  _set_current_publish_name( tmp2 );
  _set_current_publish_node( tmpNode );

  _set_currently_publishing_unit(old_publishing_unit);
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_connect( published_file &_cc_out,
							  PublishData *declarations ){
  PublishedUnit oldUnit = _get_currently_publishing_unit();

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_connect" );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/SignalNetinfo.hh" );

  _set_currently_publishing_unit(GENERATE_FOR);
  _publish_cc_include( _cc_out, "cstdarg", true );
  _cc_out << "void" << NL()
	  << _get_cc_elaboration_class_name()
	  << OS("::connect( int inputsignals, int outputsignals, ... ){")
	  << "int noOfSignals = inputsignals + outputsignals;"
	  << "va_list ap;" << NL();

  _cc_out << "int generateMin = ";

  TyvisScalarTypeDefinition *range =
    dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype());

  if ( range->is_ascending_range() == TRUE )
    range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
  else
    range->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );

  _cc_out << ";" << NL();
  
  _cc_out << "int i;" << NL();
  _cc_out << "va_start(ap, outputsignals);" << NL();
  _cc_out << OS("for(i = 0; (i < noOfSignals); i++) {");
  _cc_out << "addToFanOut( va_arg(ap, RValue*) );" << NL();
  _cc_out << CS("}") << NL();
  _cc_out << "va_end(ap);" << NL();
  
  TyvisArchitectureStatement *arch_stmt;
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->first());
  while (arch_stmt != NULL) {
    ostringstream objectname;
    if (arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
      _publish_cc_generate_for_loop( _cc_out, declarations );
      _cc_out << OS("{");
      _publish_cc_assign_to_loop_constant( _cc_out, declarations );
      objectname << *(arch_stmt->_get_mangled_label()) << "_elab_obj[i]";
      _set_current_elab_name( objectname.str() );
      (dynamic_cast<TyvisComponentInstantiationStatement*>(arch_stmt))->_publish_connect( _cc_out, declarations );
      _cc_out << CS("}") << NL();
    }
    else if (arch_stmt->_is_concurrent_generate_statement() == TRUE) {
      _publish_cc_generate_for_loop( _cc_out, declarations );
      _cc_out << OS("{");
      _publish_cc_assign_to_loop_constant( _cc_out, declarations );
      
      arch_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out, declarations );
      if (_get_currently_publishing_unit() == GENERATE_FOR) {
	_cc_out << "_elab_obj[i - generateMin].connect(0, 0);" << NL();
      }
      else {
	_cc_out << "->connect(0, 0);" << NL();
      }
      _cc_out << CS("}") << NL();
    }
    else if (arch_stmt->_is_block_statement() == TRUE) {
      _publish_cc_generate_for_loop( _cc_out, declarations );
      _cc_out << OS("{");
      _publish_cc_assign_to_loop_constant( _cc_out, declarations );
      dynamic_cast<TyvisBlockStatement *>(arch_stmt)->_publish_cc_connect_call( _cc_out, declarations );
      _cc_out << CS("}") << NL();
    }

    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_concurrent_statement_part()->successor( arch_stmt ));
  }
  _cc_out << CS("}");

  _set_currently_publishing_unit(oldUnit);
}


void
TyvisConcurrentGenerateForStatement::_publish_cc_generate_for_loop_with_zero( published_file &_cc_out,
                                                                                     PublishData *declarations,
                                                                                     char *variable_name){
  TyvisScalarTypeDefinition *range = NULL;

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc_generate_for_loop_with_zero" );

  range = dynamic_cast<TyvisScalarTypeDefinition *>(_get_generate_parameter_specification()->_get_subtype());
  ASSERT( range != NULL );

  _cc_out << "  for(int " << variable_name << " = ";
  if ( range->is_ascending_range() == TRUE ){
    _cc_out << "0";
  }
  else {
    _cc_out << "(";
    range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
    range->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    _cc_out << ")";
  }
  _cc_out << "; (" << variable_name << " ";

  if (range->is_ascending_range() == TRUE) {
    _cc_out << " <= ";
  }
  else {
    _cc_out << " >= ";
  }

  if ( range->is_ascending_range() == TRUE ){
    _cc_out << "(";
    range->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
    range->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << ")";
  }
  else {
    _cc_out << "0";
  }
  _cc_out << "); ";

  if (range->is_ascending_range() == TRUE) {
    _cc_out << variable_name << "++";
  }
  else {
    _cc_out << variable_name << "--";
  }

  _cc_out << ")  ";
}

TyvisList *
TyvisConcurrentGenerateForStatement::_get_statement_list() { 
  return dynamic_cast<TyvisList *>(get_concurrent_statement_part()); 
}

TyvisConstantDeclaration *
TyvisConcurrentGenerateForStatement::_get_generate_parameter_specification() {
  return dynamic_cast<TyvisConstantDeclaration *>(get_generate_parameter_specification());
}

TyvisDeclarationList *
TyvisConcurrentGenerateForStatement::_get_block_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>(get_block_declarative_part());
}

TyvisArchitectureStatementList  *
TyvisConcurrentGenerateForStatement::_get_concurrent_statement_part() {
  return dynamic_cast<TyvisArchitectureStatementList *>(get_concurrent_statement_part());
}

void
TyvisConcurrentGenerateForStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  ostringstream ent_arch_generate_for;
  ostringstream arch_name;

  Tyvis *temp                   = _get_current_publish_node();
  const string old_architecture_name = _get_current_architecture_name();
  const string old_current_name      = _get_current_publish_name();
  PublishedUnit saved_publishing_unit = _get_currently_publishing_unit();

  _set_currently_publishing_unit(GENERATE_FOR);
  _set_current_publish_node( this );
  
  ent_arch_generate_for << _get_current_entity_name()
			<< "_" << _get_current_architecture_name();
  _publish_cc_enclosing_stmt_to_architecture_path(ent_arch_generate_for);

  ent_arch_generate_for << *(_get_mangled_label()->_get_mangled_declarator()) << "_waits";
  
  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     ent_arch_generate_for.str(),
				     this );

  CC_REF( header_file, "TyvisConcurrentGenerateForStatement::_publish_cc" );

  header_file << "class State;\n\n";

  published_cc_file cc_file( _get_work_library()->get_path_to_directory(), 
			     ent_arch_generate_for.str(),
			     this );

  CC_REF( _cc_out, "TyvisConcurrentGenerateForStatement::_publish_cc" );
  _set_current_publish_name( "SG" );
  _get_concurrent_statement_part()->_publish_cc( cc_file, declarations );

  _set_current_publish_name( old_current_name );
  _set_current_architecture_name( old_architecture_name );
  
  CC_REF( header_file, "TyvisConcurrentGenerateForStatement::_publish_cc" );
  
  _publish_cc_declarations( header_file, declarations );
  
  _set_current_publish_node( temp );

  _set_currently_publishing_unit(saved_publishing_unit);
}

TyvisDeclarationList*
TyvisConcurrentGenerateForStatement::_get_declaration_list() {
  return dynamic_cast<TyvisDeclarationList *>(get_block_declarative_part());
}

void
TyvisConcurrentGenerateForStatement::_publish_cc_static_type_info_fn( published_file &_cc_out,
							    PublishData *declarations ){
  _get_block_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_declaration_list()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_statement_list()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_generate_parameter_specification()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  _get_concurrent_statement_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

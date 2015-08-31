
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

#include "TyvisEntityDeclaration.hh"
#include "TyvisAliasDeclaration.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisAssociationList.hh"
#include "TyvisAttribute.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisConstantInterfaceDeclaration.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisGenericList.hh"
#include "TyvisLabel.hh"
#include "TyvisName.hh"
#include "TyvisPortList.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisUseClause.hh"
#include "TyvisDesignUnitList.hh"

#include "savant/library_manager.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include <sstream>
#include "savant/language_processing_control.hh"
#include "PublishData.hh"

extern language_processing_control *lang_proc;

TyvisEntityDeclaration::TyvisEntityDeclaration() {  
  set_generic_clause(new TyvisGenericList());
  set_port_clause(new TyvisPortList());
  set_entity_declarative_part(new TyvisDeclarationList());
  set_entity_statement_part(new TyvisArchitectureStatementList());
  set_architectures(new TyvisDesignUnitList());
}

TyvisEntityDeclaration::~TyvisEntityDeclaration(){
  //Release the list memory
  delete get_generic_clause();
  delete get_port_clause();
  delete get_entity_declarative_part();
  delete get_entity_statement_part();
  delete get_architectures();
}

TyvisArchitectureDeclaration *
TyvisEntityDeclaration::_get_cc_instantiated_architecture(){
  return dynamic_cast<TyvisArchitectureDeclaration *>(library_manager::instance()->lookup_default_architecture( this ));
}

TyvisEntityDeclaration *
TyvisEntityDeclaration::_get_cc_instantiated_entity(){
  return this;
}

void
TyvisEntityDeclaration::_publish_cc_elaborate(){
  _set_current_publish_node( this );
  _publish_cc_headerfile(_declarations);
  _publish_cc_ccfile(_declarations);
  _set_current_publish_node( 0 );  
}

void
TyvisEntityDeclaration::_publish_cc_headerfile(PublishData *declarations){
  ostringstream filename;
  _publish_cc_entity_file_name( filename );

  published_header_file header_file( _get_library_directory(),
				     filename.str(),
				     this );
  CC_REF( header_file, "TyvisEntityDeclaration::_publish_cc_headerfile" );
  
  Tyvis::_publish_cc_include( header_file, "tyvis/_savant_entity_elab.hh" );
  
  // For AMS constructs
  if (lang_proc->processing_vhdl_ams()) {
    Tyvis::_publish_cc_include( header_file, "tyvis/_savant_entity_ams_elab.hh");
  }
  
  Tyvis::_publish_cc_include( header_file, "tyvis/SignalNetinfo.hh" );
  Tyvis::_publish_cc_include( header_file, "tyvis/STDTypes.hh" );

  // All the declarations are included through this file 
  _publish_cc_include_decls( header_file );
  _publish_cc_class_includes( header_file, _get_entity_statement_part() );
  _publish_cc_class( header_file, declarations );
  
}

void
TyvisEntityDeclaration::_publish_cc_class( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_class" );
  
  _cc_out <<"class " << _get_cc_elaboration_class_name();
  if(lang_proc->processing_vhdl_ams()) {
    _cc_out << " : public _savant_entity_ams_elab";
  }
  else {
    _cc_out << " : public _savant_entity_elab";
  }
  _cc_out << OS("{")
	  << "\npublic:\n";
  _cc_out << _get_cc_elaboration_class_name() << OS("(");
  _get_port_clause()->_publish_cc_unconstrained_ports( _cc_out, declarations, TRUE, FALSE, FALSE );
  _cc_out << CS(");");
  
  if( _get_generic_clause()->size() > 0 ){
    _cc_out << _get_cc_elaboration_class_name() << OS("(");
    _get_generic_clause()->_publish_generic_parameter_list( _cc_out, declarations );
    _get_port_clause()->_publish_cc_unconstrained_ports( _cc_out,
							 declarations, TRUE, FALSE, TRUE );
    _cc_out << CS(");");
  }

  _cc_out << "~" << _get_cc_elaboration_class_name() << "();" << NL();

  _get_generic_clause()->_publish_cc_elaborate( _cc_out, declarations );
  _get_port_clause()->_publish_cc_elaborate( _cc_out, declarations );
  //Aliases for signal objects are also published
  _publish_cc_signals( _cc_out, _get_entity_declarative_part(), declarations );
  _publish_cc_object_pointers( _cc_out, _get_entity_statement_part(), declarations );
  
  if (get_generic_clause()->first() != NULL) {
    _cc_out << "void copyGenericsToGlobals();" << NL();
  }
  _get_entity_declarative_part()->_publish_cc_constants( _cc_out, declarations );
  _cc_out << CS("};");
}

void
TyvisEntityDeclaration::_publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_object_pointers_init" );
  
  Tyvis::_publish_cc_object_pointers_init( _cc_out, 
					   _get_entity_statement_part(), 
					   _get_entity_declarative_part(),
					   declarations );
}

void
TyvisEntityDeclaration::_publish_cc_createNetInfo( published_file &_cc_out,
						   PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_createNetInfo" );

  TyvisArchitectureStatement* arch_stmt;
  ostringstream objectname;
  int found = 0;
  
  //Publishing createNetInfo from entity statement part
  arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_entity_statement_part()->first());
  while (arch_stmt != NULL) {
    ostringstream objectname;
    if (arch_stmt->get_kind() == IIR_PROCESS_STATEMENT) {
      arch_stmt->_publish_cc_binding_name( objectname );
      objectname << "_elab_obj";
      _set_current_elab_name( objectname.str() );
      arch_stmt->_publish_createNetInfo( _cc_out, declarations );
    }
    else if( arch_stmt->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT ){
      TyvisName *componentname = 
	dynamic_cast<TyvisName *>((dynamic_cast<TyvisComponentInstantiationStatement*>(arch_stmt))->_get_instantiated_unit());
      // Searching for configuration specification
      if( componentname->get_prefix()->get_kind() == IIR_COMPONENT_DECLARATION ){
	TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(get_entity_declarative_part()->first());
	while(decl != NULL) {
	  if(decl->get_kind() == IIR_CONFIGURATION_SPECIFICATION) {
	    if(componentname->get_prefix() == (dynamic_cast<TyvisConfigurationSpecification*>(decl))->get_component_name()) {
	      TyvisDesignator* designator = dynamic_cast<TyvisDesignator *>(decl->_get_instantiation_list()->first());
	      //By_others, By_all follows By_Explicit
	      while(designator != NULL) {
		switch(designator->get_kind()) {
		case IIR_DESIGNATOR_EXPLICIT:
		  if( designator->_get_name() == arch_stmt->_get_label() ){
		    found = 1;
		    if((dynamic_cast<TyvisConfigurationSpecification*>(decl))->get_port_map_aspect()->size() == 0) {
		      _set_current_publish_node( componentname->_get_prefix() );
		      objectname << " ((" << *(decl->_get_entity_aspect()->_get_mangled_declarator()) <<
			"_elab*)\n    " << arch_stmt->_get_label() <<
			"_elab_obj->boundedEntity)->";
		      _set_current_publish_name( objectname.str() );
		      (dynamic_cast<TyvisEntityDeclaration*>(decl->_get_entity_aspect()))->_get_port_clause()->_publish_cc_port_map_aspect( _cc_out, declarations );
		    }
		    else {
		      _set_current_publish_node( componentname->_get_prefix() );
		      objectname << "  ((";
		      TyvisLibraryUnit *lib_unit = decl->_get_entity_aspect(); 
		      if(lib_unit->get_kind() == IIR_ENTITY_DECLARATION) {
			objectname << *(decl->_get_entity_aspect()->_get_mangled_declarator());
		      }
		      else if( lib_unit->get_kind() == IIR_ARCHITECTURE_DECLARATION ){
			TyvisArchitectureDeclaration* arch_decl = 
			  dynamic_cast<TyvisArchitectureDeclaration*>(decl->_get_entity_aspect());
			objectname << *(arch_decl->_get_mangled_declarator());
		      }
		      objectname << "_elab *)\n    " << arch_stmt->_get_label();
		      objectname << "_elab_obj->boundedEntity)->";
		      _set_current_publish_name( objectname.str() );
		      (dynamic_cast<TyvisConfigurationSpecification *>(decl))->_get_port_map_aspect()->_publish_cc_elaborate( _cc_out, declarations );
		    }
		  }
		  break;
		case IIR_DESIGNATOR_BY_ALL:
		case IIR_DESIGNATOR_BY_OTHERS:
		  found = 1;
		  if((dynamic_cast<TyvisConfigurationSpecification*>(decl))->get_port_map_aspect()->size() == 0){
		    _set_current_publish_node( componentname->_get_prefix() );
		    objectname << "(( ";
		    TyvisLibraryUnit* lib_unit = decl->_get_entity_aspect(); 
		    if( lib_unit->get_kind() == IIR_ENTITY_DECLARATION ){
		      objectname << *(decl->_get_entity_aspect()->_get_mangled_declarator());
		    }
		    else if(lib_unit->get_kind() == IIR_ARCHITECTURE_DECLARATION) {
		      TyvisArchitectureDeclaration* arch_decl = 
			dynamic_cast<TyvisArchitectureDeclaration*>(decl->_get_entity_aspect());
		      objectname << *(arch_decl->_get_mangled_declarator());
		    }
		    else {
		      //Nothing to be done for now
		    }
		    objectname << "_elab*) " << arch_stmt->_get_label();
		    objectname << "_elab_obj->boundedEntity)->";
		    _set_current_publish_name( objectname.str() );
		    (dynamic_cast<TyvisEntityDeclaration*>(decl->_get_entity_aspect()))->_get_port_clause()->_publish_cc_port_map_aspect( _cc_out, declarations );
		  }
		  else {
		    _set_current_publish_node( componentname->_get_prefix() );
		    objectname << "(( ";
		    TyvisLibraryUnit *lib_unit = decl->_get_entity_aspect(); 
		    if(lib_unit->get_kind() == IIR_ENTITY_DECLARATION) {
		      objectname << *(decl->_get_entity_aspect()->_get_mangled_declarator());
		    }
		    else if(lib_unit->get_kind() == IIR_ARCHITECTURE_DECLARATION) {
		      TyvisArchitectureDeclaration* arch_decl = 
			dynamic_cast<TyvisArchitectureDeclaration*>(decl->_get_entity_aspect());
		      objectname << *(arch_decl->_get_mangled_declarator());
		    }
		    else {
		      //Nothing to be done for now
		    }
		    objectname << "_elab*) " << arch_stmt->_get_label();
		    objectname << "_elab_obj->boundedEntity)->";
		    _set_current_publish_name( objectname.str() );
		    (dynamic_cast<TyvisConfigurationSpecification*>(decl))->_get_port_map_aspect()->_publish_cc_elaborate( _cc_out, declarations );
		  }
		  break;

		default:
		  break;
		}
		if(found ==1) {
		  ///Has to exit the two enclosing loops
		  designator = NULL;
		  decl = NULL;
		}
		else {
		  designator = dynamic_cast<TyvisDesignator *>(decl->_get_instantiation_list()->successor(designator));
		}
	      }
	    }
	  }
	  decl = dynamic_cast<TyvisDeclaration *>(get_entity_declarative_part()->successor(decl));
	}
      }
      //###yet to check for label
      objectname << arch_stmt->_get_label() << "_elab_obj";
      _set_current_elab_name( objectname.str() );
      arch_stmt->_publish_createNetInfo( _cc_out, declarations );
    }
    else {
      cerr << "ERROR! TyvisEntityDeclaration::"
	   << "_publish_cc_createNetInfo( _cc_out ): unknown concurrent statement "
	   << "type |" << arch_stmt->get_kind_text() << "| in entity" << endl;
      abort();
    }
    _set_current_elab_name( "" );
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_entity_statement_part()->successor(arch_stmt));
  }
}

void
TyvisEntityDeclaration::_publish_cc_binding_name(ostream& outstream) {
//   CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_binding_name" );
  outstream << *(_get_mangled_declarator());
}

void
TyvisEntityDeclaration::_publish_cc_ccfile(PublishData *declarations){
  ostringstream file_name;
  _publish_cc_entity_file_name( file_name );

  published_cc_file cc_file( _get_library_directory(),
			     file_name.str(),
			     this );
  CC_REF( cc_file, "TyvisEntityDeclaration::_publish_cc_ccfile" );

  cc_file << "#include \"" << file_name.str() << ".hh\"\n";

  cc_file << "class VHDLKernel;\n";
  cc_file << "extern VHDLKernel *proc_array[];\n";

  CC_REF( cc_file, "TyvisEntityDeclaration::_publish_cc_ccfile" );

  _publish_cc_headerfiles_for_cc( cc_file );
  _publish_cc_constructor( cc_file, declarations );
  _publish_cc_destructor( cc_file, _get_entity_statement_part() );

  if (get_generic_clause()->first() != NULL) {
    _publish_cc_copy_generics_to_globals( cc_file, declarations );
  }

  _publish_cc_component_decl( cc_file, declarations );
}

void 
TyvisEntityDeclaration::_publish_cc_entity_file_name( ostream &os ){
  os << _get_cc_elaboration_class_name();
}

void
TyvisEntityDeclaration::_publish_cc_headerfiles_for_cc( published_file &_cc_out ) {
  TyvisArchitectureStatement* arch_stmt = NULL;

  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_headerfiles_for_cc" );
  
  Tyvis::_publish_cc_include( _cc_out, "tyvis/ObjectBase.hh" );

  while (arch_stmt != NULL) {
    //###currently the process name doesn't include its entity and
    //architecture name
    if( arch_stmt->get_kind() == IIR_PROCESS_STATEMENT ) {
      arch_stmt->_publish_cc_include( _cc_out );
    }
    //Nothing to be done for other statements
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(get_entity_statement_part()->successor( arch_stmt ));
  }
}

void
TyvisEntityDeclaration::_publish_cc_constructor( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_constructor" );
  
  _publish_cc_constructor_with_no_arguments( _cc_out, declarations );
  if (get_generic_clause()->size() > 0) {
    _publish_cc_constructor_with_arguments( _cc_out, declarations );
  }
}

void
TyvisEntityDeclaration::_publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations ) {
  int numGenericClause = get_generic_clause()->size();
  int numPortClause    = get_port_clause()->size();
  IIR_Boolean first    = TRUE;
  
  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_constructor_with_no_arguments" );

  _cc_out << _get_cc_elaboration_class_name() << "::" << _get_cc_elaboration_class_name() << "(";
  _get_port_clause()->_publish_cc_unconstrained_ports( _cc_out, declarations, TRUE, TRUE, FALSE );
  _cc_out << ")";

  bool firstDeclFlag = false;  
  if( numGenericClause > 0 ) {
    _cc_out << ":\n    ";
    _get_generic_clause()->_publish_generic_init( _cc_out, declarations );
    if( numPortClause > 0 ){
      _cc_out << ",\n   ";
      _get_port_clause()->_publish_cc_port_init( _cc_out, declarations );
    }    
  }
  else {
    if(numPortClause > 0) {
      _cc_out << ":\n    ";
      _get_port_clause()->_publish_cc_port_init( _cc_out, declarations );
    }
    else {
      firstDeclFlag = true;
    }
  }
  
  first = _get_entity_declarative_part()->_publish_cc_constants_init( _cc_out, declarations, first);
  _publish_cc_signal_objects_init( _cc_out, declarations, first );  
  _cc_out << OS("{");
  // before newing the elab guys down the heirarchy copy generics of this
  // entity to the global generic pointers in the entity_decls file
  if (numGenericClause > 0) {
    _cc_out << "copyGenericsToGlobals();\n";
  }

  _get_entity_declarative_part()->_publish_cc_global_constants_assignments( _cc_out, declarations );
  
  _publish_cc_object_pointers_init( _cc_out, declarations );
  _cc_out << CS("}");

  _get_entity_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisEntityDeclaration::_publish_cc_constructor_with_arguments( published_file &_cc_out,
								PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_constructor_with_arguments" );  
  _cc_out << _get_cc_elaboration_class_name() << "::" << _get_cc_elaboration_class_name() << "(";
  _get_generic_clause()->_publish_generic_parameter_list( _cc_out, declarations );
  _get_port_clause()->_publish_cc_unconstrained_ports( _cc_out, declarations, TRUE, TRUE, TRUE );
  _cc_out << ")";
  
  if( get_generic_clause()->size() > 0 ){
    _cc_out << ":" << NL();
    _get_generic_clause()->_publish_generic_init_by_arguments( _cc_out, declarations );
    if(get_port_clause()->size() > 0) {
      _cc_out << "," << NL();
      _get_port_clause()->_publish_cc_port_init( _cc_out, declarations );
    }
  }
  else {
    if(get_port_clause()->size() > 0) {
      _cc_out << ":" << NL();
      _get_port_clause()->_publish_cc_port_init( _cc_out, declarations );
    }
  }
  _publish_cc_signal_objects_init( _cc_out, declarations );
  _cc_out << OS("{");
  // before newing the elab guys down the heirarchy copy generics of this entity 
  // to the global generic pointers in the entity_decls file
  if( get_generic_clause()->size() > 0 ){
    _cc_out << "copyGenericsToGlobals();" << NL();
  }
  
  _get_entity_declarative_part()->_publish_cc_global_constants_assignments( _cc_out, declarations );
  _publish_cc_object_pointers_init( _cc_out, declarations );
  _cc_out << CS("}");
}

void
TyvisEntityDeclaration::_publish_cc_component_decl( published_file &_cc_out, PublishData *declarations ) {
  TyvisDeclaration *decl;

  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_component_decl" );
  
  decl = dynamic_cast<TyvisDeclaration *>(get_entity_declarative_part()->first());
  while (decl != NULL) {
    if (decl->get_kind() == IIR_COMPONENT_DECLARATION) {
      decl->_publish_cc_elaborate( _cc_out, declarations );
    }  
    decl = dynamic_cast<TyvisDeclaration *>(get_entity_declarative_part()->successor(decl));
  }
}

void
TyvisEntityDeclaration::_publish_cc_signal_objects_init( published_file &_cc_out,
                                                                PublishData *declarations,
                                                                const IIR_Boolean firstFlag ){
  TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration *>(get_entity_declarative_part()->first());

  int numGenericClause = get_generic_clause()->size();
  int numPortClause    = get_port_clause()->size();
  IIR_Boolean first    = firstFlag;
  
  CC_REF( _cc_out, "TyvisEntityDeclaration::_publish_cc_signal_objects_init" );
  
  while (decl != NULL) {
    if (decl->get_kind() == IIR_SIGNAL_DECLARATION) {
      if (first == TRUE) {
	_cc_out << ":\n";
	first = FALSE;
      }
      else {
	_cc_out << ",\n";
      }
      decl->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "(";
      decl->_get_subtype()->_publish_cc_type_info_xxx(_cc_out, declarations);
      _cc_out << "," << NL() << "  \"" << *(decl->_get_declarator()) << "\"" << NL();
/* DNS: old output or range, which has no apparent use
      if( decl->_get_subtype()->is_array_type() ){
	_cc_out << ",\n";
	decl->_get_subtype()->_publish_cc_range( _cc_out, declarations );
      }
*/
      _cc_out << ")";
      // If the Signals has implicit signals, they are also needed for
      // elaboration info
      if( decl->_get_implicit_declarations() != NULL &&
          decl->_get_implicit_declarations()->size() != 0) {
	TyvisDeclaration* imp_decl = decl->_get_implicit_declarations()->getElement();
	while(imp_decl != NULL) {
	  if(imp_decl->is_signal() == TRUE) {
	    _cc_out << ",\n    ";
	    (imp_decl->_get_mangled_declarator())->_publish_cc_elaborate( _cc_out, declarations );
	    _cc_out << "(ObjectBase::SIGNAL_NETINFO";
	    TyvisTypeDefinition* type_def = imp_decl->_get_attribute_name()->_get_explicit_signal_type();
	    if((type_def->is_array_type() == TRUE ) &&
	       (type_def->is_anonymous() == TRUE)) {
	      _cc_out << ",\n    ";
	      type_def->_publish_cc_range( _cc_out, declarations );
	    }	
	    _cc_out << ")";
	  }
	  imp_decl = decl->_get_implicit_declarations()->getNextElement();
	}
      }
    }
    else if(decl->get_kind() == IIR_ALIAS_DECLARATION) {
      if((dynamic_cast<TyvisAliasDeclaration*>(decl))->_get_name()->is_signal()) {
	if((first == 0)&& (numGenericClause == 0)&& (numPortClause ==0)) {
	  _cc_out << ":\n    ";
	  first = FALSE;
	}
	else {
	  _cc_out << ",\n    ";
	}
	(dynamic_cast<TyvisAliasDeclaration*>(decl))->_publish_cc_elaborate_alias_init( _cc_out, declarations );
      }
    }
    decl = dynamic_cast<TyvisDeclaration *>(get_entity_declarative_part()->successor(decl));
  }
}

void 
TyvisEntityDeclaration::_publish_cc_include_decls_prefix( ostream &os ){
  os << *_get_mangled_declarator();
}

void
TyvisEntityDeclaration::_publish_cc_scoping_prefix( ostream &, Tyvis *, Tyvis * ){
  // Nothing to be done
}


TyvisGenericList *
TyvisEntityDeclaration::_get_generic_clause(){
  return dynamic_cast<TyvisGenericList *>(get_generic_clause());
}

TyvisEntityDeclaration*
TyvisEntityDeclaration::_get_entity() {
  return this;
}

void 
TyvisEntityDeclaration::_publish_cc(){
  Tyvis *old_node = _get_current_publish_node();

  _set_current_publish_name( "" );
  _set_current_entity_name( _get_mangled_declarator()->convert_to_string() );

  published_header_file header_file( _get_library_directory(),
				     _get_current_entity_name(),
				     this );
  CC_REF( header_file, "TyvisEntityDeclaration::_publish_cc" );;

  _set_current_publish_node( this );
  _set_currently_publishing_unit(ENTITY_DECL);

  Tyvis::_publish_cc_include( header_file, "tyvis/STDTypes.hh" );

  _get_context_items()->_publish_cc( header_file, _declarations );
  _get_generic_clause()->_publish_cc_elaborate_as_pointers( header_file, _declarations, FALSE );
  _get_entity_declarative_part()->_publish_cc( header_file, _declarations );

  _get_generic_clause()->_publish_cc_static_type_info_fn( header_file, _declarations );
  _get_port_clause()->_publish_cc_static_type_info_fn( header_file, _declarations );
  _get_entity_declarative_part()->_publish_cc_static_type_info_fn( header_file, _declarations );
  _get_port_list()->_publish_cc_static_type_info_fn( header_file, _declarations );


  published_cc_file cc_file( _get_library_directory(),
			     _get_current_entity_name(),
			     this );

  CC_REF( cc_file, "TyvisEntityDeclaration::_publish_cc" );;
  _publish_cc_include_decls( cc_file );

  _get_generic_clause()->_publish_cc_elaborate_as_pointers( cc_file, _declarations, TRUE );
  _get_entity_declarative_part()->_publish_cc_decl( cc_file, _declarations );

  _get_entity_statement_part()->_publish_cc( cc_file, _declarations );
  _set_current_publish_node( old_node );
  _set_currently_publishing_unit(NONE);
}

TyvisDeclarationList *
TyvisEntityDeclaration::_get_entity_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>(get_entity_declarative_part());
}

TyvisArchitectureStatementList *
TyvisEntityDeclaration::_get_entity_statement_part() {
  return dynamic_cast<TyvisArchitectureStatementList *>(get_entity_statement_part());
}


TyvisPortList *
TyvisEntityDeclaration::_get_port_clause(){
  return dynamic_cast<TyvisPortList *>(get_port_clause());
}

TyvisGenericList *
TyvisEntityDeclaration::_get_generic_list(){
  return _get_generic_clause();
}

TyvisPortList *
TyvisEntityDeclaration::_get_port_list(){
  return _get_port_clause();
}

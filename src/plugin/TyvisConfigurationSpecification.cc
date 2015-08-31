
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
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisConfigurationDeclaration.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisLabel.hh"
#include "TyvisTextLiteral.hh"
#include "published_file.hh"

TyvisConfigurationSpecification::TyvisConfigurationSpecification() {
  set_instantiation_list(new TyvisDesignatorList());
  set_generic_map_aspect(new TyvisAssociationList());
  set_port_map_aspect(new TyvisAssociationList());
}

TyvisConfigurationSpecification::~TyvisConfigurationSpecification() {
  //Release the list memory
  delete get_instantiation_list();
  delete get_generic_map_aspect();
  delete get_port_map_aspect();
}

void
TyvisConfigurationSpecification::_publish_cc_port_map_aspect( published_file &_cc_out,
							      PublishData *declarations ) {
  ASSERT( _get_current_publish_node() != NULL );
  ASSERT( dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node()) );
  TyvisConcurrentStatement* concurrent_stmt = 
    dynamic_cast<TyvisConcurrentStatement*>( _get_current_publish_node());
  TyvisAssociationElement* actual_clause = NULL;

  CC_REF( _cc_out, "TyvisConfigurationSpecification::_publish_cc_port_map_aspect" );
  if(get_port_map_aspect()->size() > 0) {
    _cc_out << OS("{");
    _cc_out.insert_comment( "Alias instantiated component as \"component\"" );
    _cc_out << concurrent_stmt->_get_instantiated_unit()->_get_cc_elaboration_class_name()
	    << " *component = ";
    if (_get_currently_publishing_unit() == GENERATE_FOR) {
      _cc_out << "(";
      concurrent_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out.get_stream() );
      _cc_out << "_elab_obj + i);" << NL();
    }
    else {
      concurrent_stmt->_get_mangled_label()->_publish_cc_elaborate( _cc_out.get_stream() );
      _cc_out << "_elab_obj;" << NL();
    }

    _cc_out.insert_comment( "Alias architecture as \"architecture\"" );    
    if( get_entity_aspect()->get_kind() == IIR_CONFIGURATION_DECLARATION ){
      TyvisConfigurationDeclaration* config_decl = dynamic_cast<TyvisConfigurationDeclaration*>(get_entity_aspect());
      _cc_out << config_decl->_get_configured_architecture()->_get_cc_elaboration_class_name()
	      << " *architecture = (("
	      << config_decl->_get_configured_architecture()->_get_cc_elaboration_class_name()
	      << "*)(("
	      << _get_entity_aspect()->_get_cc_elaboration_class_name() << "*)"
	      << "component->boundedEntity)->configuredEntity);" << NL();
    }
    else {
      _cc_out << _get_entity_aspect()->_get_cc_elaboration_class_name()
	      << " *architecture = ("
	      << _get_entity_aspect()->_get_cc_elaboration_class_name()
	      << " *)"
	      << "component->boundedEntity;" << NL();
    }
    
    _cc_out.insert_comment( "Add a port association - order is actual, formal" );
    _cc_out << "PortMap *tmpPortMap = architecture->getPortMapAspect();" << NL();
    _get_port_map_aspect()->_publish_cc_port_map_associations( _cc_out, declarations );
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
    while( actual_clause && actual_clause->_get_actual() ){
      _cc_out << "component->";
      actual_clause->_get_actual()->_publish_cc_object_name( _cc_out, declarations );
      _cc_out << ".copyId( architecture->";
      actual_clause->_get_formal()->_publish_cc_object_name( _cc_out, declarations );
      _cc_out << ");" << NL();
      actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
    }
      
    _cc_out << CS("}");
  }
}

TyvisDesignatorList * 
TyvisConfigurationSpecification::_get_instantiation_list() {
  return dynamic_cast<TyvisDesignatorList *>(get_instantiation_list());
}

TyvisAssociationList *
TyvisConfigurationSpecification::_get_generic_map_aspect() {
  return dynamic_cast<TyvisAssociationList *>(get_generic_map_aspect());
}

TyvisAssociationList *
TyvisConfigurationSpecification::_get_port_map_aspect() {
  return dynamic_cast<TyvisAssociationList *>(get_port_map_aspect());
}

TyvisLibraryUnit *
TyvisConfigurationSpecification::_get_entity_aspect(){
  return dynamic_cast<TyvisLibraryUnit *>(get_entity_aspect());
}

Tyvis *
TyvisConfigurationSpecification::_get_component_name( ){
  return dynamic_cast<Tyvis *>(get_component_name());
}

IIR_Boolean
TyvisConfigurationSpecification::_configures_this_component(TyvisComponentInstantiationStatement* conc_stmt) {
  return this->_get_instantiation_list()->_is_component_in_the_list(conc_stmt->_get_label()->_get_declarator());
}

IIR_Boolean
TyvisConfigurationSpecification::_has_same_component(TyvisComponentInstantiationStatement* conc_stmt) {
  TyvisComponentDeclaration* componentname = dynamic_cast<TyvisComponentDeclaration *>(conc_stmt->get_instantiated_unit());
  ASSERT(dynamic_cast<TyvisComponentDeclaration *>(componentname) != NULL);
  TyvisTextLiteral* local_componentname = componentname->_get_declarator();
  ASSERT( dynamic_cast<TyvisDeclaration *>(get_component_name()) != NULL );
  TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration *>(get_component_name());
  TyvisTextLiteral* binding_componentname = decl->_get_declarator();
  if(0 == IIRBase_TextLiteral::cmp(local_componentname, binding_componentname)) {
    return TRUE;
  }
  return FALSE;
}

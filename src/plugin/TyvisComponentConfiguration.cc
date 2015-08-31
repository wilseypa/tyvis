
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

#include "Tyvis.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisAssociationElement.hh"
#include "TyvisAssociationList.hh"
#include "TyvisBlockConfiguration.hh"
#include "TyvisComponentConfiguration.hh"
#include "TyvisConfigurationDeclaration.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisDesignator.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryUnit.hh"
#include "TyvisLibraryUnitList.hh"
#include "TyvisName.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisStatement.hh"
#include "savant/error_func.hh"
#include "published_file.hh"
#include <sstream>

TyvisComponentConfiguration::TyvisComponentConfiguration(){
  set_instantiation_list(new TyvisDesignatorList());
  set_generic_map_aspect(new TyvisAssociationList());
  set_port_map_aspect(new TyvisAssociationList());
}

TyvisComponentConfiguration::~TyvisComponentConfiguration(){
  //Release the list memory
  delete get_instantiation_list();
  delete get_generic_map_aspect();
  delete get_port_map_aspect();
}

void
TyvisComponentConfiguration::_publish_cc_headers( published_file &_cc_out ){

  CC_REF( _cc_out, "TyvisComponentConfiguration::_publish_cc_headers" );

  ASSERT( _get_component_name() != NULL );
  _get_component_name()->_publish_cc_include( _cc_out );
//   if(_get_entity_aspect() != NULL) {
//     _get_entity_aspect()->_publish_cc_include_elab( _cc_out );
//   }
  if (_get_block_configuration() != NULL)
  _get_block_configuration()->_publish_cc_headers( _cc_out );
}

void
TyvisComponentConfiguration::_publish_cc_configure_block( published_file &_cc_out,
							  PublishData *declarations,
							  IIR_Boolean _is_within_generate_for){

  string scratch1_ptr;
  string scratch2_ptr;
  string referenceStr;
  
  TyvisDesignator* designator = dynamic_cast<TyvisDesignator *>(get_instantiation_list()->first());

  CC_REF( _cc_out, "TyvisComponentConfiguration::_publish_cc_configure_block" );

  while(designator != NULL) { 
    ostringstream scratch1;
    ostringstream scratch2;
    if( designator->get_kind() != IIR_DESIGNATOR_EXPLICIT ){
      ostringstream err;
      err << designator->get_kind_text() << " not yet handled.";
      report_error( designator, err.str() );
      abort();
    }

    ASSERT(designator->get_kind() == IIR_DESIGNATOR_EXPLICIT);
    ASSERT(designator->_get_name()->get_kind() == IIR_LABEL);
    TyvisLabel* label = dynamic_cast<TyvisLabel*>(designator->_get_name());
    ASSERT(label->is_resolved() == TRUE);
    scratch1 << "enclosingArch->";
    if(label->_get_statement() == NULL) {
      scratch1 << *(label->_get_mangled_declarator());
    }
    else {
      scratch1 << *(label->_get_statement()->_get_mangled_label()->_get_mangled_declarator());
    }
    scratch1 << "_elab_obj";

    if (_is_within_generate_for == TRUE) {
      scratch1 << "[generateConstant]";
      referenceStr = ".";
    }
    else {
      referenceStr = "->";
    }
      
    scratch1_ptr = scratch1.str();

    scratch2 << "enclosingArch1->";
    if(label->_get_statement() == NULL) {
      scratch2 << *(label->_get_mangled_declarator());
    }
    else {
      scratch2 << *(label->_get_statement()->_get_mangled_label()->_get_mangled_declarator());
    }
    scratch2 << "_elab_obj";
    if (_is_within_generate_for == TRUE) {
      scratch2 << "[generateConstant]";
    }
    scratch2_ptr = scratch2.str();

    ASSERT(_get_component_name()->is_resolved() == TRUE);
    if(_get_entity_aspect() != NULL) {
      _get_entity_aspect()->_publish_cc_include( _cc_out );
      _cc_out << "delete " << scratch1_ptr << referenceStr << "boundedEntity;" << NL()
	      << scratch1_ptr << referenceStr << "boundedEntity = new "
	      << _get_entity_aspect()->_get_cc_elaboration_class_name() << "(";
      if( _get_generic_map_aspect()->size() > 0 ){
	Tyvis* tempNode = _get_current_publish_node();
	_set_current_publish_node( designator->_get_name() );
	_get_generic_map_aspect()->_publish_cc_generic_map_aspect( _cc_out, declarations );
	_set_current_publish_node( tempNode );
      }
      _cc_out << ");" << NL();
      if(_get_port_map_aspect()->size() > 0) {
	_cc_out << OS("{")
		<< _get_component_name()->_get_cc_elaboration_class_name()
		<< " *component = ";
	if (_is_within_generate_for == TRUE)  {
	  _cc_out << "&";
	}
	
	_cc_out << scratch1_ptr << ";" << NL();
	if(_get_entity_aspect()->get_kind() == IIR_CONFIGURATION_DECLARATION) {
	  TyvisConfigurationDeclaration* config_decl = dynamic_cast<TyvisConfigurationDeclaration*>(get_entity_aspect());
	  _cc_out << config_decl->_get_configured_architecture()->_get_cc_elaboration_class_name()
		  << "* architecture = (("
		  << config_decl->_get_configured_architecture()->_get_cc_elaboration_class_name()
		  << " *)(( " << _get_entity_aspect()->_get_cc_elaboration_class_name()
		  << " *)"
		  << "component->boundedEntity)->configuredEntity);" << NL();
	}
	else {
	  _cc_out << _get_entity_aspect()->_get_cc_elaboration_class_name()
		  << "* architecture = ("
		  << _get_entity_aspect()->_get_cc_elaboration_class_name()
		  << " *) component->boundedEntity;" << NL();
	}
	_cc_out << "PortMap* tmpPortMap = architecture->getPortMapAspect();";
	_get_port_map_aspect()->_publish_cc_port_map_associations( _cc_out, declarations );
	_cc_out << CS("}");
      }
    }
    if ( _get_block_configuration() != NULL ){
      _cc_out << _get_entity_aspect()->_get_cc_elaboration_class_name()
	      << "* enclosingArch = ("
	      << _get_entity_aspect()->_get_cc_elaboration_class_name()
	      << " *)" << scratch2_ptr << "->boundedEntity;" << NL()
	      << _get_entity_aspect()->_get_cc_elaboration_class_name()
	      << "* enclosingArch1 = enclosingArch;" << NL()
	      << OS("{");
      _get_block_configuration()->_publish_cc_configure_block( _cc_out, declarations );
      _cc_out << CS("}");
    }
    designator = dynamic_cast<TyvisDesignator *>(get_instantiation_list()->successor(designator));
  }
}

TyvisDesignatorList *
TyvisComponentConfiguration::_get_instantiation_list() {
  return dynamic_cast<TyvisDesignatorList *>(get_instantiation_list());
}

TyvisAssociationList *
TyvisComponentConfiguration::_get_generic_map_aspect() {
  return dynamic_cast<TyvisAssociationList *>(get_generic_map_aspect());
}

TyvisAssociationList *
TyvisComponentConfiguration::_get_port_map_aspect() {
  return dynamic_cast<TyvisAssociationList *>(get_port_map_aspect());
}

TyvisBlockConfiguration *
TyvisComponentConfiguration::_get_block_configuration() {
  return dynamic_cast<TyvisBlockConfiguration *>(get_block_configuration());
}

TyvisLibraryUnit *
TyvisComponentConfiguration::_get_entity_aspect(){
  return dynamic_cast<TyvisLibraryUnit *>(get_entity_aspect());
}

Tyvis *
TyvisComponentConfiguration::_get_component_name( ){
  return dynamic_cast<Tyvis *>(get_component_name());
}

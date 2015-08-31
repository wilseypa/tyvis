
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
#include "TyvisArchitectureStatement.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisSimultaneousStatement.hh"
#include "TyvisProcessStatement.hh"

#include "savant/savant.hh"
#include "savant/error_func.hh"
#include "published_cc_file.hh"
#include "savant/language_processing_control.hh"

#include <iostream>

using std::cerr;

extern language_processing_control *lang_proc;

void 
TyvisArchitectureStatementList::_publish_cc( published_file &_cc_out,
					     PublishData *declarations  ) {
  TyvisArchitectureStatement        *node                   = NULL;
  TyvisArchitectureStatement        *transmutedNode         = NULL;
  Tyvis                              *_old_publish_node      = _get_current_publish_node();

  CC_REF( _cc_out, "TyvisArchitectureStatementList::_publish_cc" );

  for (node = dynamic_cast<TyvisArchitectureStatement *>(first()); 
       node != NULL; 
       node = dynamic_cast<TyvisArchitectureStatement *>(successor(node))) {
    node->set_declarative_region( _old_publish_node );
    transmutedNode = dynamic_cast<TyvisArchitectureStatement *>(node->_transmute());
    if(node != transmutedNode) {
      _replace(node, transmutedNode);
      node = transmutedNode;
    }
    node->set_declarative_region( _old_publish_node );
    node->_publish_cc( _cc_out, declarations );
  }
}

void 
TyvisArchitectureStatementList::_publish_cc_static_type_info_fn( published_file &_cc_out,
						       PublishData *declarations ) {
  TyvisArchitectureStatement *node = NULL;
  TyvisArchitectureStatement *transmutedNode = NULL;

  CC_REF( _cc_out, "TyvisArchitectureStatementList::_publish_cc_type_info" );
  
  for (node = dynamic_cast<TyvisArchitectureStatement *>(first()); 
       node != NULL; node = dynamic_cast<TyvisArchitectureStatement *>(successor(node))) {
    //4 lines added to do transmogrification once
    transmutedNode = dynamic_cast<TyvisArchitectureStatement*>(node->_transmute());
    if(node != transmutedNode) {
      _replace(node, transmutedNode);
      //      insert_after(node, transmutedNode);
      //      remove(node);
      node = transmutedNode;
    }
    node->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
}

void 
TyvisArchitectureStatementList::_publish_cc_instantiate_calls( published_file &_cc_out,
							       PublishData *declarations ){
  TyvisArchitectureStatement *node;

  for (node = dynamic_cast<TyvisArchitectureStatement *>(first()); 
       node != NULL; 
       node = dynamic_cast<TyvisArchitectureStatement *>(successor(node))) {
    node->_publish_cc_instantiate_call( _cc_out, declarations );
  }
}

void
TyvisArchitectureStatementList::_publish_cc_characteristic_expressions(published_file &_cc_out) {
  TyvisArchitectureStatement *node;
  for (node = dynamic_cast<TyvisArchitectureStatement *>(first()); 
       node != NULL; 
       node = dynamic_cast<TyvisArchitectureStatement *>(successor(node))) {
    if(node->get_kind() == IIR_SIMPLE_SIMULTANEOUS_STATEMENT || 
       node->get_kind() == IIR_SIMULTANEOUS_IF_STATEMENT ) {
      node->_publish_cc_characteristic_expressions(TyvisArchitectureStatement::None,_cc_out);
    }
  }
}

void
TyvisArchitectureStatementList::_publish_cc_ams_function_body(published_file & _cc_out) {
  TyvisArchitectureStatement *node;
  for (node = dynamic_cast<TyvisArchitectureStatement *>(first()); 
       node != NULL; 
       node = dynamic_cast<TyvisArchitectureStatement *>(successor(node))) {
    switch( node->get_kind() ){
    case IIR_SIMPLE_SIMULTANEOUS_STATEMENT:
    case IIR_SIMULTANEOUS_IF_STATEMENT:
      (dynamic_cast<TyvisSimultaneousStatement *>(node))->_publish_cc_ams_function_body(_cc_out);
      break;
    default:
      // No action required
      break;
    }
  }
}

void
TyvisArchitectureStatementList::_publish_cc_ams_function_prototype(published_file &_cc_out) {
  TyvisArchitectureStatement *node;
  if ( lang_proc->processing_vhdl_ams() ){
    _cc_out<<"#include \"Component.hh\"\n";
    _cc_out<<"#include <adouble.h>\n";
    _cc_out<<"#include <adutils.h>\n";
  } 
  for (node = dynamic_cast<TyvisArchitectureStatement *>(first()); 
       node != NULL; 
       node = dynamic_cast<TyvisArchitectureStatement *>(successor(node))) {
    switch(node->get_kind()){
    case IIR_SIMPLE_SIMULTANEOUS_STATEMENT:
    case IIR_SIMULTANEOUS_IF_STATEMENT:
      dynamic_cast<TyvisSimultaneousStatement *>(node)->_publish_cc_ams_function_prototype(_cc_out);
      break;
    default:
      // No action required
      break;
    }
  }
}

void
TyvisArchitectureStatementList::_publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations ) {
  // Publish createNetinfo from architecture statement part
  TyvisArchitectureStatement *arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(first());
  while (arch_stmt!=NULL) {
    _set_current_publish_name( "" );
    _set_current_publish_node( NULL );

    switch( arch_stmt->get_kind() ){
    case IIR_PROCESS_STATEMENT:
      _set_current_elab_name( dynamic_cast<TyvisProcessStatement *>(arch_stmt)->_get_cc_binding_name() + "_elab_obj" );
      // No break here as we want the arch_stmt->_publish_createNetInfo() here.
    case IIR_BLOCK_STATEMENT:
    case IIR_COMPONENT_INSTANTIATION_STATEMENT:{
      arch_stmt->_publish_createNetInfo( _cc_out, declarations );
      break;    
    }
    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
      // No action is needed in this case.
      break;
    case IIR_SIMPLE_SIMULTANEOUS_STATEMENT:
    case IIR_SIMULTANEOUS_IF_STATEMENT:
      // No action is needed in this case.
      break;
    default:
      cerr << "ERROR! TyvisArchitectureDeclaration::"
	   << "_publish_cc_createNetInfo( _cc_out ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "| in arch\n";
      abort();
    }
    _set_current_elab_name( "" );
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(successor(arch_stmt));
  }
}

int
TyvisArchitectureStatementList::_get_number_of_component_instantiations() {
  int compInstCounter = 0;
  TyvisArchitectureStatement *node;

  for (node = dynamic_cast<TyvisArchitectureStatement *>(first());
       node != NULL;
       node = dynamic_cast<TyvisArchitectureStatement *>(successor(node))) {
    if (node->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
      compInstCounter++;
    }
  }
  return compInstCounter;
}


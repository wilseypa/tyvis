
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

#include "TyvisComponentDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisPortList.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"

void
TyvisPortList::_publish_cc_port_init( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisPortList::_publish_cc_port_init" );
  TyvisInterfaceDeclaration* i = dynamic_cast<TyvisInterfaceDeclaration *>(first());
  while( i != NULL ) {
    switch(i->get_kind()){
    case IIR_SIGNAL_INTERFACE_DECLARATION:{
      i->_publish_cc_signal_net_info( _cc_out, DEFINITION, declarations );
      break;
    }
    default:{
      //do nothing
    }
      break;
    }
    i = dynamic_cast<TyvisInterfaceDeclaration *>(successor(i));
  } 
}

int
TyvisPortList::_publish_cc_unconstrained_ports( published_file &_cc_out,
                                                       PublishData *declarations,
                                                       IIR_Boolean publishPortTypes, 
                                                       IIR_Boolean publishPortNames, 
                                                       IIR_Boolean commaFlag ){
  int noOfUnconstrainedPorts           = 0;
  TyvisInterfaceDeclaration *sig = 
    dynamic_cast<TyvisInterfaceDeclaration *>(first());  

  while (sig != NULL) {
    CC_REF( _cc_out, "TyvisPortList::_publish_cc_unconstrained_ports" );
    ASSERT ( sig->_get_subtype() != NULL );
    if (sig->_get_subtype()->is_unconstrained_array_type() == TRUE) {
      if (commaFlag == TRUE) {
	_cc_out << "," << NL();
      }
      if (publishPortTypes == TRUE) {
	_cc_out << "const ";
	sig->_get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
	_cc_out << "& ";
      }
      if (publishPortNames == TRUE) {
	sig->_publish_cc_elaborate( _cc_out, declarations );
	_cc_out << "_port";
      }
      commaFlag = TRUE;
      noOfUnconstrainedPorts++;
    }
    
    sig = dynamic_cast<TyvisInterfaceDeclaration *>(successor(sig));
  }
  return noOfUnconstrainedPorts;
}

void
TyvisPortList::_publish_cc_port_map_aspect( published_file &_cc_out, PublishData *declarations ) {
  ASSERT( _get_current_publish_node()->is_component_declaration() == TRUE );
  TyvisComponentDeclaration *as_component_decl = 
    dynamic_cast<TyvisComponentDeclaration*>(_get_current_publish_node());

  TyvisSignalInterfaceDeclaration *local_clause = 
    dynamic_cast<TyvisSignalInterfaceDeclaration *>(as_component_decl->get_local_port_clause()->first());

  TyvisSignalInterfaceDeclaration *actual_clause = 
    dynamic_cast<TyvisSignalInterfaceDeclaration *>(first());

  for(; local_clause != NULL; ){
// This portion probably needs refactoring and removal; not sure what is being done with local_clause
// This is the only section in the code base that refers to local_clause
    CC_REF( _cc_out, "TyvisPortList::_publish_cc_port_map_aspect" );
  //  _cc_out << "Add" << OS("("); 
  //  _cc_out << _get_current_elab_name();
  //  local_clause->_publish_cc_elaborate( _cc_out, declarations );
  //  _cc_out << ",";
    
    // ### Temporary Fix
   // if( actual_clause != NULL ){
   //   _cc_out << "(("
//	      << actual_clause->_get_declarative_region()->_get_cc_elaboration_class_name()
//	      << " *)" << _get_current_elab_name() << "boundedEntity)->";
 //     actual_clause->_publish_cc_elaborate( _cc_out, declarations );
  //  }
   // _cc_out << CS(");");
    
   // if( local_clause->get_mode() == IIR_OUT_MODE || 
    //    local_clause->get_mode() == IIR_INOUT_MODE ){
    //  CC_REF( _cc_out, "TyvisPortList::_publish_cc_port_map_aspect" );
    //  _cc_out << _get_current_elab_name();
    //  local_clause->_publish_cc_elaborate( _cc_out, declarations );
    //  _cc_out << "->addChild" << OS("(");
      // ### Temporary Fix
    //  if( actual_clause != NULL ){
//	_cc_out << "(("
//		<< actual_clause->_get_declarative_region()->_get_cc_elaboration_class_name()
//		<< " *)" << _get_current_elab_name() << "boundedEntity)->";
//	actual_clause->_publish_cc_elaborate( _cc_out, declarations );
 //     }
  //    _cc_out << CS(");");
  //  }
    local_clause = 
      dynamic_cast<TyvisSignalInterfaceDeclaration *>(as_component_decl->get_local_port_clause()->successor( local_clause ));

    actual_clause = 
      dynamic_cast<TyvisSignalInterfaceDeclaration *>(successor(actual_clause));
  }  
}

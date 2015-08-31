
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

#include "TyvisAssociationList.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationElementOpen.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIndexedName.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisPortList.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDefinition.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>

void
TyvisAssociationList::_publish_cc_for_index( published_file &_cc_out, PublishData *declarations ) {
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_for_index" );
  
  if (node != NULL) {
    node->_publish_cc_rvalue( _cc_out, declarations );
    for (node = dynamic_cast<TyvisAssociationElement *>(successor(node)); 
         node != NULL; node = dynamic_cast<TyvisAssociationElement *>(successor(node))) {
      _cc_out << "][";
      node->_publish_cc_rvalue( _cc_out, declarations );
    }
  }
}

void 
TyvisAssociationList::_publish_cc_generic_map_aspect( published_file &_cc_out, PublishData *declarations ) {
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());
  PublishedUnit tmpUnit = _get_currently_publishing_unit();  //DRH
  _set_currently_publishing_unit(GENERIC_MAP);  //DRH
  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_generic_map_aspect" );

  if (node != NULL) {
    node->_publish_cc_generic_map_aspect( _cc_out, declarations );
    for (node = dynamic_cast<TyvisAssociationElement *>(successor(node)); 
         node != NULL; node = dynamic_cast<TyvisAssociationElement *>(successor(node))) {
      _cc_out << "," << NL();
      node->_publish_cc_generic_map_aspect( _cc_out, declarations );
    }
  }
  _set_currently_publishing_unit(tmpUnit); //DRH
}

void
TyvisAssociationList::_publish_cc_generic_map_aspect_for_conc_stmts( published_file &_cc_out, PublishData *declarations )
{
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());
  PublishedUnit tmpUnit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(GENERIC_MAP);

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_generic_map_aspect_for_conc_stmts" );

  while (node != NULL) {
    node->_publish_cc_generic_map_aspect_for_conc_stmts( _cc_out, declarations );
    node = dynamic_cast<TyvisAssociationElement *>(successor(node));

    if (node != NULL) {
      _cc_out << "," << NL();
    }
  }
  _set_currently_publishing_unit(tmpUnit);
}

void 
TyvisAssociationList::_publish_cc_bounds( published_file &_cc_out, PublishData * ){
  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_bounds" );
  _cc_out << "defaultInfo()" ;
}

void 
TyvisAssociationList::_publish_cc_state_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_state_object_init" );

  if (node != NULL) {
    node->_publish_cc_state_object_init( _cc_out, declarations );
    _set_aggregate_iterator_counter( _get_aggregate_iterator_counter() + 1);
    for (node = dynamic_cast<TyvisAssociationElement *>(successor(node)); 
         node != NULL; node = dynamic_cast<TyvisAssociationElement *>(successor(node))) {
      _cc_out << "<<" << NL();
      node->_publish_cc_state_object_init( _cc_out, declarations );
      _set_aggregate_iterator_counter( _get_aggregate_iterator_counter() + 1);
    }
  }
}

void
TyvisAssociationList::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisAssociationList::_publish_cc_subprogram_arguments( published_file &_cc_out ) {
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_subprogram_arguments" );

  if (node != NULL) {
    node->_publish_cc_subprogram_arguments( _cc_out );
    for (node = dynamic_cast<TyvisAssociationElement *>(successor(node)); 
         node != NULL; node = dynamic_cast<TyvisAssociationElement *>(successor(node))) {
      _cc_out << "," << NL();
      node->_publish_cc_subprogram_arguments( _cc_out );
    }
  }
}

void
TyvisAssociationList::_publish_cc_port_map_associations( published_file &_cc_out,
							 PublishData *declarations ) {
  TyvisAssociationElement *association = NULL;
  Tyvis *formal = NULL;
  Tyvis *interface_decl = NULL;

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_port_map_associations" );

  association = dynamic_cast<TyvisAssociationElement *>(first());
  formal = association->_get_formal();
  ASSERT(formal != NULL);
  if(formal != NULL) {
    // ### assumes the parameter in order, also on by expression not open yet
    while( association && association->_get_actual() ) {
      interface_decl = association->_get_formal();
      _cc_out << OS("tmpPortMap->addPortAssociation(") 
	      << "&component->";
      association->_get_actual()->_publish_cc_object_name( _cc_out, declarations );
      _cc_out << "," << NL();
      _cc_out << "&architecture->";
      association->_get_formal()->_publish_cc_object_name( _cc_out, declarations );
      _cc_out << "," << NL();
      if( interface_decl->get_kind() == IIR_TERMINAL_INTERFACE_DECLARATION ) {
	_cc_out<< "IN" ;
      }
      else {
	switch( interface_decl->_get_mode() ){
	case IIR_IN_MODE:
	  _cc_out << "IN";
	  break;
	case IIR_OUT_MODE:
	  _cc_out << "OUT";
	  break;
	case IIR_INOUT_MODE:
	  _cc_out << "INOUT";
	  break;
	case IIR_BUFFER_MODE:
	  _cc_out << "BUFFER";
	  break;
	case IIR_LINKAGE_MODE:
	  _cc_out << "LINKAGE";
	  break;
	default:
	  cerr << "Warning!!! UnknownMode" << endl;
	  _cc_out << "IN";
	  break;
	};
      }
      _cc_out << CS(");");
      association = dynamic_cast<TyvisAssociationElement *>(successor(association));
    }
  }
}

int
TyvisAssociationList::_publish_cc_unconstrained_ports( published_file &_cc_out,
						       PublishData *declarations,
						       IIR_Boolean commaFlag ) {
  int numUnconstrainedPorts       = 0;
  TyvisAssociationElement *element = dynamic_cast<TyvisAssociationElement *>(first());

  while (element != NULL) {
    if (element->_get_formal() != NULL) {
      ASSERT ( element->_get_formal()->_get_subtype() != NULL );
      if (element->_get_formal()->_get_subtype()->is_unconstrained_array_type() == TRUE) {
	// An unconstrained formal must be constrained by an actual..
	ASSERT ( element->_get_actual() != NULL );
	if (commaFlag == TRUE) {
	  _cc_out << "," << NL();
	}
	element->_get_actual()->_publish_cc_object_name( _cc_out, declarations );

	numUnconstrainedPorts++;
	commaFlag = TRUE;
      }
    }

    element = dynamic_cast<TyvisAssociationElement *>(successor(element));
  }

  return numUnconstrainedPorts;
}

void
TyvisAssociationList::_publish_cc_elaborate( published_file &_cc_out,
					     PublishData *declarations ){
  TyvisInterfaceDeclaration* port_clause;
  TyvisAssociationElement* actual_clause;
  Tyvis* formal;
  TyvisDeclaration* interfacedecl;
  TyvisDeclaration* actualdecl;

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_elaborate" );
  
  port_clause = dynamic_cast<TyvisInterfaceDeclaration *>((dynamic_cast<TyvisComponentDeclaration*>(_get_current_publish_node()))->get_local_port_clause()->first());
  actual_clause = dynamic_cast<TyvisAssociationElement *>(first());
  formal = dynamic_cast<TyvisAssociationElement *>(actual_clause->_get_formal());
  if(formal != NULL) {
    //### assumes the parameter in order, also on by expression not open yet
    for(; actual_clause != NULL; ) {
      interfacedecl = dynamic_cast<TyvisDeclaration*>(actual_clause->_get_formal());
      //->get_identifier()->_get_declaration();
      //      switch(((TyvisInterfaceDeclaration*)interfacedecl)->get_mode()) {
      //      case IIR_IN_MODE:
      //      case IIR_INOUT_MODE:
      _cc_out << "Add(";
      
      actualdecl = dynamic_cast<TyvisDeclaration *>((dynamic_cast<TyvisAssociationElementByExpression *>(actual_clause))->_get_actual());
      
      ASSERT ( actualdecl != NULL );
      
      _publish_cc_scoping_prefix( _cc_out.get_stream(),
				  actualdecl->_get_declarative_region(), this );
      
      _cc_out << _get_current_elab_name();
      (dynamic_cast<TyvisAssociationElementByExpression*>(actual_clause))->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "," << NL();
      _cc_out << _get_current_publish_name();
      actual_clause->_get_formal()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << ");" << NL();

      CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_elaborate" );
      if(((dynamic_cast<TyvisInterfaceDeclaration*>(interfacedecl))->get_mode() 
	  == IIR_OUT_MODE) || 
	 ((dynamic_cast<TyvisInterfaceDeclaration*>(interfacedecl))->get_mode() 
	  == IIR_INOUT_MODE)) {
	_cc_out << "addChild(";
	actualdecl = dynamic_cast<TyvisDeclaration *>((dynamic_cast<TyvisAssociationElementByExpression *>(actual_clause))->_get_actual());
	ASSERT ( actualdecl != NULL );
	  
	_publish_cc_scoping_prefix( _cc_out.get_stream(),
				    actualdecl->_get_declarative_region(), this );
	_cc_out << _get_current_elab_name();
	(dynamic_cast<TyvisAssociationElementByExpression*>(actual_clause))->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
	_cc_out << "," << NL();
	_cc_out << _get_current_publish_name();
	actual_clause->_get_formal()->_publish_cc_elaborate( _cc_out, declarations );
	_cc_out << ");" << NL();
      }
      //	break;
      //      }
      actual_clause = dynamic_cast<TyvisAssociationElement *>(successor(actual_clause));
    }
  }
  else {//### currently assumes parameters are in order. needs work
    while (port_clause != NULL) {
      //      switch(port_clause->get_mode()) {
      //      case IIR_IN_MODE:
      ///      case IIR_INOUT_MODE:
      _cc_out << "Add(";
      _cc_out << _get_current_elab_name();

      actualdecl = dynamic_cast<TyvisDeclaration *>((dynamic_cast<TyvisAssociationElementByExpression *>(actual_clause))->_get_actual());
      ASSERT ( actualdecl != NULL );

      _publish_cc_scoping_prefix( _cc_out.get_stream(),
				  actualdecl->_get_declarative_region(), this );
      
      (dynamic_cast<TyvisAssociationElementByExpression*>(actual_clause))->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "," << NL();
      _cc_out << _get_current_publish_name();
      port_clause->_get_mangled_declarator()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "_info);" << NL();
      
      CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_elaborate" );

      if((port_clause->get_mode() == IIR_OUT_MODE) || 
	 (port_clause->get_mode() == IIR_INOUT_MODE)) {	
	_cc_out << "addChild(";
	_cc_out << _get_current_elab_name();

	actualdecl = dynamic_cast<TyvisDeclaration *>((dynamic_cast<TyvisAssociationElementByExpression *>(actual_clause))->_get_actual());
	ASSERT ( actualdecl != NULL );
	
	_publish_cc_scoping_prefix( _cc_out.get_stream(),
				    actualdecl->_get_declarative_region(),this );
	
	(dynamic_cast<TyvisAssociationElementByExpression*>(actual_clause))->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
	_cc_out << "," << NL();
	_cc_out << _get_current_publish_name();
	port_clause->_get_mangled_declarator()->_publish_cc_elaborate( _cc_out, declarations );
	_cc_out << "_info);" << NL();
      }
      //	break;
      //      }
      port_clause = dynamic_cast<TyvisInterfaceDeclaration *>((dynamic_cast<TyvisComponentDeclaration*>(_get_current_publish_node()))->get_local_port_clause()->successor( dynamic_cast<IIR_SignalInterfaceDeclaration*>(port_clause)));
      actual_clause = dynamic_cast<TyvisAssociationElement *>(successor(actual_clause));
    }
  }
}

void
TyvisAssociationList::_publish_cc_necessary_temporaries_for_TC( published_file &_cc_out,
                                                                TyvisInterfaceList *formals_list,
                                                                PublishData *declarations) {

  TyvisAssociationElement   *call_parameter = dynamic_cast<TyvisAssociationElement *>(first());
  TyvisInterfaceDeclaration *procedure_parameter = NULL;

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_necessary_temporaries_for_TC" );

  ASSERT (formals_list->get_kind() == IIR_INTERFACE_LIST);
  procedure_parameter = dynamic_cast<TyvisInterfaceDeclaration *>(formals_list->first());
  
  while (call_parameter != NULL) {
    if (call_parameter->_get_formal()->get_kind() == IIR_FUNCTION_CALL) {
      procedure_parameter->_publish_cc_state_object_init( _cc_out, declarations );
      _cc_out << ";" << NL();
    }
    
    call_parameter = dynamic_cast<TyvisAssociationElement *>(successor(call_parameter));
    procedure_parameter = dynamic_cast<TyvisInterfaceDeclaration *>(formals_list->successor(procedure_parameter));
  }
}

void
TyvisAssociationList::_publish_cc_with_temporaries_for_TC( published_file &_cc_out,
                                                           TyvisInterfaceList *formals_list,
                                                           PublishData *declarations) {
  
  TyvisAssociationElement   *node = dynamic_cast<TyvisAssociationElement *>(first());
  TyvisInterfaceDeclaration *proc_parameter = NULL;

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_with_temporaries_for_TC" );

  ASSERT (formals_list->get_kind() == IIR_INTERFACE_LIST);
  proc_parameter = dynamic_cast<TyvisInterfaceDeclaration *>(formals_list->first());
  
  if (node != NULL) {
    if (node->_get_formal()->get_kind() == IIR_FUNCTION_CALL) {
      _cc_out << *proc_parameter->_get_mangled_declarator();
    }
    else {
      /* OUT and INOUT fields require LValues */
      if ( proc_parameter != NULL && (proc_parameter->get_mode() == IIR_OUT_MODE
           || proc_parameter->get_mode() == IIR_INOUT_MODE) ) {
        node->_publish_cc_lvalue( _cc_out, declarations );
      }
      else {
        node->_publish_cc_rvalue( _cc_out, declarations );
      }
    }

    proc_parameter = dynamic_cast<TyvisInterfaceDeclaration *>(formals_list->successor(proc_parameter));
    
    for (node = dynamic_cast<TyvisAssociationElement *>(successor(node)); 
         node != NULL; node = dynamic_cast<TyvisAssociationElement *>(successor(node))) {
      _cc_out << "," << NL();
      if (node->_get_formal()->get_kind() == IIR_FUNCTION_CALL) {
	_cc_out << *proc_parameter->_get_mangled_declarator();
      }
      else {
	/* OUT and INOUT fields require LValues */
	if ( proc_parameter != NULL && (proc_parameter->get_mode() == IIR_OUT_MODE
	     || proc_parameter->get_mode() == IIR_INOUT_MODE) ) {
	  node->_publish_cc_lvalue( _cc_out, declarations );
	} 
	else {
	  node->_publish_cc_rvalue( _cc_out, declarations );
	}
      }
      proc_parameter = dynamic_cast<TyvisInterfaceDeclaration *>(formals_list->successor(proc_parameter));
    }
  }
}

void
TyvisAssociationList::_publish_cc_restore_from_temporaries( published_file &_cc_out,
							    TyvisInterfaceList *,
							    PublishData *declarations){

  TyvisAssociationElement *call_parameter = dynamic_cast<TyvisAssociationElement *>(first());
  const string _old_publish_prefix_string = _get_publish_prefix_string();

  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_restore_from_temporaries" );
  
  while (call_parameter != NULL) {
    if (call_parameter->_get_formal()->get_kind() == IIR_FUNCTION_CALL) {
      _cc_out << "Variable::assign(";
      call_parameter->_get_actual()->_publish_cc_first_objectParameter( _cc_out, declarations );
      _cc_out << "," << NL();

      // This is done to supress publishing of prefix string as the
      // parameters are actually local and not is state
      _set_publish_prefix_string( "" );
      call_parameter->_get_formal()->_publish_cc_rvalue( _cc_out, declarations );
      _set_publish_prefix_string( _old_publish_prefix_string );      
      _cc_out << ")";
      _cc_out.end_statement();
    }
    
    call_parameter = dynamic_cast<TyvisAssociationElement *>(successor(call_parameter));
  }
}

void
TyvisAssociationList::_publish_cc_array_index( published_file &_cc_out,
					       PublishData *declarations ){
  CC_REF( _cc_out, "TyvisAssociationList::_publish_cc_array_index" );
  _publish_cc_elaborate( _cc_out, declarations );
}

void 
TyvisAssociationList::_add_decl_into_declaration_collection(PublishData *declarations) {
  TyvisAssociationElement *current = dynamic_cast<TyvisAssociationElement *>(first());
  while( current != NULL ){
    if(current->get_kind() == IIR_ASSOCIATION_ELEMENT_BY_EXPRESSION) {
      current->_add_decl_into_declaration_collection(declarations);
    }
    current = dynamic_cast<TyvisAssociationElement *>(successor( current ));
  }
}

IIR_Int32
TyvisAssociationList::_get_number_of_element_associations() {
  IIR_Int32 number_of_elements = 0;
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());
  for (; node != NULL; node = dynamic_cast<TyvisAssociationElement *>(successor(node))) {
    number_of_elements += node->_get_number_of_element_associations();
  }
  return number_of_elements;
}

IIR_Boolean  
TyvisAssociationList::_is_globally_static_for_array(){
  IIR_Boolean retval = TRUE;
  TyvisAssociationElement *current = dynamic_cast<TyvisAssociationElement *>(first());
  while( current != NULL ){
    if( current->_is_globally_static_for_array() == FALSE ){
      retval = FALSE;
      break;
    }
    current = dynamic_cast<TyvisAssociationElement *>(successor( current ));
  }
  
  return retval;
}

IIR_Boolean  
TyvisAssociationList::_is_globally_static_for_record( ){
  IIR_Boolean retval = TRUE;
  TyvisAssociationElement *current = dynamic_cast<TyvisAssociationElement *>(first());
  while( current != NULL ){
    if( current->_is_globally_static_for_record() == FALSE ){
      retval = FALSE;
      break;
    }
    current = dynamic_cast<TyvisAssociationElement *>(successor( current ));
  }
  
  return retval;
}
 
IIR_Boolean 
TyvisAssociationList::_is_globally_static_for_function( ){
  IIR_Boolean retval = TRUE;
  TyvisAssociationElement *current = dynamic_cast<TyvisAssociationElement *>(first());
  while( current != NULL ){
    if( current->_is_globally_static_for_function() == FALSE ){
      retval = FALSE;
      break;
    }
    current = dynamic_cast<TyvisAssociationElement *>(successor( current ));
  }
  
  return retval;
}

IIR_Boolean
TyvisAssociationList::_is_static_expression() {
  return _is_globally_static_for_function();
}

IIR_Boolean
TyvisAssociationList::_are_all_scalars() {
  TyvisAssociationElement *current = dynamic_cast<TyvisAssociationElement *>(first());
  while( current != NULL ) {
    ASSERT ( current->_get_formal() != NULL );
    if( current->_get_formal()->is_scalar_type() == FALSE ){
      return FALSE;
    }
    current = dynamic_cast<TyvisAssociationElement *>(successor( current ));
  }
  
  return TRUE;
}

IIR_Boolean
TyvisAssociationList::_are_any_signals() {
  TyvisAssociationElement *current = dynamic_cast<TyvisAssociationElement *>(first());
  while( current != NULL ) {
    ASSERT ( current->_get_formal() != NULL );
    if (current->_get_actual()){
      if( current->_get_actual()->get_kind() == IIR_SIGNAL_DECLARATION ){
        return TRUE;
      }
    }
    current = dynamic_cast<TyvisAssociationElement *>(successor( current ));
  }
  
  return FALSE;
}

string
TyvisAssociationList::_get_signal_names() {
  TyvisAssociationElement *current = dynamic_cast<TyvisAssociationElement *>(first());
  string sigs;
  while( current != NULL ) {
    ASSERT ( current->_get_formal() != NULL );
    if (current->_get_actual()){
      if( current->_get_actual()->get_kind() == IIR_SIGNAL_DECLARATION ){
        sigs += " " + current->_get_actual()->_to_string();
      }
    }
    current = dynamic_cast<TyvisAssociationElement *>(successor( current ));
  }
  
  return sigs;
}

void
TyvisAssociationList::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  TyvisAssociationElement *current = dynamic_cast<TyvisAssociationElement *>(first());
  while( current != NULL ) {
    current->_build_sensitivity_list(sensitivity_list);
    current = dynamic_cast<TyvisAssociationElement *>(successor( current ));
  }
}

void
TyvisAssociationList::_get_list_of_input_signals(  savant::set<Tyvis>* list ){
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());
  while( node != NULL ){
    node->_get_list_of_input_signals(list);
    node = dynamic_cast<TyvisAssociationElement *>(successor(node));
  }
}

void
TyvisAssociationList::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());
  while(node != NULL) {
    node->_get_signal_source_info(siginfo);
    node = dynamic_cast<TyvisAssociationElement *>(successor(node));
  }
}

void
TyvisAssociationList::_set_passed_through_out_port(IIR_Boolean) {
  // The parameter is just to keep the interface consistent

  TyvisAssociationElement*   tempAssociation = dynamic_cast<TyvisAssociationElement *>(first());
  TyvisAssociationElementByExpression* portAssociation;
  
  while (tempAssociation != NULL) {
    if( tempAssociation->get_kind() == IIR_ASSOCIATION_ELEMENT_BY_EXPRESSION ){
      portAssociation = dynamic_cast<TyvisAssociationElementByExpression *>(tempAssociation);
    
      switch (portAssociation->_get_formal()->_get_mode()) {
      case IIR_IN_MODE:
        break;
      
      case IIR_OUT_MODE:
	if ((portAssociation->_get_actual() != NULL) &&
	    (portAssociation->_get_actual()->is_signal() == TRUE)) {
//          cerr << dynamic_cast<Tyvis *>(portAssociation->_get_actual())->_get_cc_rvalue() << endl;
	  portAssociation->_get_actual()->_set_passed_through_out_port(TRUE);
	}
	else {
          if (portAssociation->_get_actual() != NULL) {
	    cerr << "Warning : "
	       << "TyvisAssociationList::_set_passed_through_out_port()"
	       << " - Unhandled case in IIR_OUT_MODE" << endl;
          }
	}
	break;
      
      case IIR_INOUT_MODE:
	if (portAssociation->_get_actual() == NULL) {
	  break;
	}
      
        if (portAssociation->_get_actual()->get_kind() == IIR_SIGNAL_DECLARATION) {
          (dynamic_cast<TyvisSignalDeclaration *>(portAssociation->_get_actual()))->
            _set_passed_through_out_port(TRUE);
        }
        else if(portAssociation->_get_actual()->get_kind() == IIR_FUNCTION_CALL){
          TyvisFunctionCall* tempFC = dynamic_cast<TyvisFunctionCall *>(portAssociation->_get_actual());
          if (tempFC->get_parameter_association_list()->first()->get_kind() == IIR_SIGNAL_DECLARATION) {
            (dynamic_cast<TyvisSignalDeclaration *>(tempFC->get_parameter_association_list()->first()))->_set_passed_through_out_port(TRUE);
          }
        }
        else if (portAssociation->_get_actual()->get_kind() ==
                 IIR_SIGNAL_INTERFACE_DECLARATION) {
          // Nothing to do here. Check just to suppress the warning from
          // popping up.
        }
        else if ((portAssociation->_get_actual()->get_kind() == IIR_INDEXED_NAME)
                 || (portAssociation->_get_actual()->get_kind() == IIR_SELECTED_NAME)) {
          portAssociation->_get_actual()->_set_passed_through_out_port(TRUE);
        }
        else {
          cerr << "Warning : "
               << "TyvisAssociationList::_set_passed_through_out_port()"
               << " - Unhandled case in IIR_INOUT_MODE" << endl;
        }
        break;
      default:
        // Do nothing.
        break;
      }
    }
    tempAssociation = dynamic_cast<TyvisAssociationElement *>(successor(tempAssociation));
  }
}

void
TyvisAssociationList::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  TyvisAssociationElement *node = dynamic_cast<TyvisAssociationElement *>(first());
  for (;node != NULL; node = dynamic_cast<TyvisAssociationElement *>(successor(node))) {
    node->_build_above_attribute_set(to_build);
  }
}

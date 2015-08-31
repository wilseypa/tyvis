
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

#include "TyvisConcurrentStatement.hh"
#include "TyvisDeclaration.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisName.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisTextLiteral.hh"


#include "savant/set.hh"
#include "published_file.hh"

using std::cerr;

//This function is used to publish the type of the
//name, i.e.
// Signal a: bitvector(1, to 3),
// then a(3) has the type bit.
//Currently this need is in the elaboration code
//presume needed in future
//#####This function needs a lot of work and thought

void
TyvisName::_publish_cc_type_info_xxx( published_file &_cc_out,
				      PublishData *declarations ){
  CC_REF( _cc_out, "TyvisName::_publish_cc_type_info_xxx" );
  TyvisTypeDefinition* type_def = _get_subtype();
  if(type_def != NULL) {
    type_def->_publish_cc_type_info_xxx( _cc_out, declarations );
  }
}

void
TyvisName::_publish_cc_addChild( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisName::_publish_cc_addChild" );

  ASSERT( is_signal() == TRUE );
  ASSERT( _get_current_publish_name() != "" );
  const string ptr = _get_current_publish_name(); // Set in TyvisProcessStatement
  if( _is_resolved_signal() == TRUE || 
      _get_prefix_declaration()->_have_added_driver() == FALSE) {
    _cc_out << OS("{");
    CC_REF( _cc_out, "TyvisName::_publish_cc_addChild" );
    if ( _get_current_publish_node() != NULL &&
	 dynamic_cast<TyvisConcurrentStatement *>(_get_current_publish_node())!= NULL ){
      _get_current_publish_node()->_publish_cc_scoping_prefix( _cc_out.get_stream(),
      _get_prefix_declaration()->_get_declarative_region(), _get_current_publish_node());
    }
    _publish_cc_lvalue( _cc_out, declarations ); 
    _cc_out << ".addChild(";
    _cc_out << "dynamic_cast<SignalNetinfo &>( "; 
    _publish_cc_elaborate( _cc_out, declarations );
    _cc_out << " )";
    _cc_out <<  _get_current_publish_name() << NL();

    if (_get_subtype()->_get_resolution_function() != 0 ) {
      _cc_out << "setResolutionFunctionId(";
      _cc_out << "dynamic_cast<SignalNetinfo &>( "; 
      _publish_cc_elaborate( _cc_out, declarations );
      _cc_out << " ), ";
      _cc_out << *_get_subtype()->_get_resolution_function()->_get_mangled_declarator()
              << ");";
    }
    _cc_out << CS("};");
  }
}

void
TyvisName::_publish_cc_binding_name(ostream& outstream) {
  _get_prefix()->_publish_cc_binding_name(outstream);
}

TyvisTextLiteral *
TyvisName::_get_mangled_declarator() {
  return _get_prefix()->_get_mangled_declarator();
}

Tyvis *
TyvisName::_get_prefix() {
  return dynamic_cast<Tyvis *>(get_prefix());
}

IIR_Boolean
TyvisName::_is_static_expression() {
  if(_get_suffix() != NULL) {
    return (_get_prefix()->_is_static_expression() == TRUE &&
	    _get_suffix()->_is_static_expression() == TRUE);
  } else {
    return (_get_prefix()->_is_static_expression() == TRUE);
  }    
}

void 
TyvisName::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  if(is_signal() == FALSE) {
    return;
  }
  if(_is_longest_static_prefix() == TRUE) {
    TyvisDesignator *node = dynamic_cast<TyvisDesignator *>(sensitivity_list->first());
    while (node != NULL) {
      ASSERT(node->get_kind() == IIR_DESIGNATOR_EXPLICIT);
      if ((dynamic_cast<TyvisDesignatorExplicit*>(node))->get_name() == this) { break; }
      node = dynamic_cast<TyvisDesignator *>(sensitivity_list->successor(node));
    }
    if (node == NULL) {
      TyvisDesignatorExplicit *new_designator = new TyvisDesignatorExplicit;
      copy_location( this, new_designator );
      new_designator->set_name(this);
      sensitivity_list->append(new_designator);
    }
  }
  else {
    _get_prefix()->_build_sensitivity_list(sensitivity_list);
  }
}

IIR_Boolean
TyvisName::_is_longest_static_prefix() {
  if(_get_suffix() == NULL) {
    return ( dynamic_cast<TyvisDeclaration *>(_get_prefix()) != NULL ||
	    _get_prefix()->_is_longest_static_prefix());
  } else {
    if(_get_suffix()->_is_static_expression() == TRUE) {
      if(dynamic_cast<TyvisDeclaration *>(_get_prefix()) != NULL) {
	return TRUE;
      } else {
	return _get_prefix()->_is_longest_static_prefix();
      }
    } else {
      return FALSE;
    }
  }
}

void
TyvisName::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  if(_is_longest_static_prefix() == TRUE) {
    siginfo->add( this );
  }
  else {
    _get_prefix()->_get_signal_source_info(siginfo);
  }
}

void
TyvisName::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  if ( is_signal() == TRUE ){
    if( _is_longest_static_prefix() == TRUE ){
      list->add( this );
    }
    else{
      _get_prefix()->_get_list_of_input_signals(list);       
    }
  }
  
  if( _get_suffix() != NULL ){
    if( _get_suffix()->is_signal() == TRUE ){
      _get_suffix()->_get_list_of_input_signals(list);
    }
  }
}

IIR_Boolean
TyvisName::_is_composite_resolved_signal() {
  TyvisTypeDefinition* type_def = _get_subtype();

  if(is_signal() == FALSE) {
    return FALSE;
  }
  else {
    if(type_def != NULL) {
      if((type_def->_get_resolution_function() != NULL) &&
	 ((type_def->is_array_type() == TRUE)|| 
	  (type_def->is_record_type() == TRUE))) {
	return TRUE;
      }
      else {
	return FALSE;
      }  
    }
    else {
      return FALSE;
    }
  }
}


// Copyright (c) 1996-2010 The University of Cincinnati.
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

#include "Tyvis.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisComponentConfiguration.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisGenericList.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIndexedName.hh"
#include "TyvisSliceName.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisPortList.hh"
#include "TyvisSelectedName.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisSimpleName.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "savant/error_func.hh"
#include "savant/set.hh"
#include "savant/library_manager.hh"
#include "published_file.hh"
#include <sstream>
#include <string>
#include <map>
#include <typeinfo>

TyvisComponentInstantiationStatement::TyvisComponentInstantiationStatement() {
  set_generic_map_aspect(new TyvisAssociationList());
  set_port_map_aspect(new TyvisAssociationList());
}

TyvisComponentInstantiationStatement::~TyvisComponentInstantiationStatement(){
  //Release the list memory
  delete get_generic_map_aspect();
  delete get_port_map_aspect();
}

void
TyvisComponentInstantiationStatement::_publish_createNetInfo( published_file &_cc_out, PublishData *declarations ){  
  TyvisDeclaration* decl = dynamic_cast<TyvisDeclaration *>(_get_instantiated_unit());
  TyvisAssociationElement* actual_clause = NULL;
  Tyvis* formal = NULL;
  TyvisLabel *label = _get_mangled_label();

  ASSERT(decl != NULL);

  if(decl->get_kind() == IIR_COMPONENT_DECLARATION) {
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
    if(actual_clause == NULL){
      return;
    }
    label->_publish_cc_elaborate( _cc_out.get_stream() );

    if (_get_currently_publishing_unit() == GENERATE_FOR) {
      _cc_out << "_elab_obj[i - generateMin].buildSignalTree();" << NL();
    }
    else {
      _cc_out << "_elab_obj->buildSignalTree();" << NL();
    }
    
    while( actual_clause && actual_clause->_get_actual() ) {
      formal = actual_clause->_get_formal();
      if (formal != NULL) {
	ASSERT(formal->is_resolved() == TRUE);
	ASSERT(actual_clause->_get_actual() != NULL);
	switch(formal->_get_mode()) {
	case IIR_IN_MODE:
	  _publish_cc_elaborate_add( _cc_out, actual_clause, label, declarations );
	  if (actual_clause->_get_actual()->get_kind() == IIR_FUNCTION_CALL)
	    _publish_cc_elaborate_upType( _cc_out, actual_clause, label, declarations );
	  break;
	case IIR_OUT_MODE:
	  // Code for adding the resolution information tree
	  _publish_cc_elaborate_addChild( _cc_out, actual_clause, label, declarations );
	  if (formal->get_kind() == IIR_FUNCTION_CALL)
	    _publish_cc_elaborate_upType( _cc_out, actual_clause, label, declarations );
	  break;
	case IIR_INOUT_MODE:
	  _publish_cc_elaborate_add( _cc_out, actual_clause, label, declarations );
	  // Code for adding the resolution information tree
	  _publish_cc_elaborate_addChild( _cc_out, actual_clause, label, declarations );
	  if (formal->get_kind() == IIR_FUNCTION_CALL)
	    _publish_cc_elaborate_upType( _cc_out, actual_clause, label, declarations );
	  break;
	default:
	  if(formal->get_kind() == IIR_TERMINAL_INTERFACE_DECLARATION) {
	    //do nothing
	  }
	  else {
	    ostringstream err;
	    err << "Buffer and linkage mode not yet supported in "
		<< "Component instantiation statements\n";
	    report_error( this, err.str() );
	    break;
	  }
	}
      }
      actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
    }
  }
}

void
TyvisComponentInstantiationStatement::_publish_cc_binding_name(ostream& outstream) {
  ASSERT(_get_instantiated_unit() != NULL);
  _get_instantiated_unit()->_publish_cc_binding_name(outstream);
}

void 
TyvisComponentInstantiationStatement::_publish_cc_elaborate_add( published_file &_cc_out,
                                                                        TyvisAssociationElement *actual_clause,
                                                                        TyvisLabel *label,
                                                                        PublishData *declarations){
  Tyvis* decl = actual_clause->_get_formal();
  TyvisFunctionCall *upTypeConversionFn = NULL;
  TyvisFunctionCall *downTypeConversionFn = NULL;
  TyvisDeclaration* tempDeclaration = NULL;

  CC_REF( _cc_out, "TyvisComponentInstantiationStatement::_publish_cc_elaborate_Add" );
  
  if( actual_clause->_get_actual() != NULL &&
      actual_clause->_get_actual()->get_kind() != IIR_FUNCTION_CALL &&
      actual_clause->_get_formal()->get_kind() != IIR_FUNCTION_CALL &&
      actual_clause->_get_actual()->get_kind() != IIR_CONSTANT_DECLARATION){
    ASSERT(actual_clause->_get_actual() != NULL);
    if(actual_clause->_get_actual()->get_kind() == IIR_FUNCTION_CALL) {
      downTypeConversionFn = dynamic_cast<TyvisFunctionCall *>(actual_clause->_get_actual());
      downTypeConversionFn->_publish_cc_elaborate_arg( _cc_out, declarations );
    } 
    else {
      _cc_out << "dynamic_cast<SignalNetinfo&>(";
      tempDeclaration = dynamic_cast<TyvisDeclaration *> (actual_clause->_get_actual());
      if( tempDeclaration != NULL ){
	_publish_cc_scoping_prefix( _cc_out.get_stream(),
				    tempDeclaration->_get_declarative_region(),
				    this);
      }
      else if( actual_clause->_get_actual()->is_name() == TRUE ){
	tempDeclaration = actual_clause->_get_actual()->_get_prefix_declaration();
	_publish_cc_scoping_prefix( _cc_out.get_stream(), 
				    tempDeclaration->_get_declarative_region(),
				    this);
      }
      else {
	cerr << "Error : TyvisComponentInstantiationStatement::_publish_cc_elaborate_addChild( published_file &_cc_out, PublishData *declarations ) - declaration kind not handled yet.\n";
	abort();
      }
      actual_clause->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << OS(").addToFanOut(");
    }

    if (decl->get_kind() == IIR_INDEXED_NAME) {
	_cc_out << "dynamic_cast<SignalNetinfo &>(";
    }
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
    } else if (decl->get_kind() == IIR_INDEXED_NAME) {
	decl->_publish_cc_rvalue( _cc_out, declarations );
	_cc_out << ")";
    } else {
      //decl->_publish_cc_elaborate( _cc_out, declarations );
      decl->_publish_cc_object_name( _cc_out, declarations );
    }    
    _cc_out << CS(");");
    
    if(upTypeConversionFn != NULL) {
      _cc_out << OS("setUpConversionFunctionId(");
      label->_publish_cc_elaborate( _cc_out.get_stream() );
      if (_get_currently_publishing_unit() == GENERATE_FOR) {
	_cc_out << "_elab_obj[i - generateMin].";
      }
      else {
	_cc_out << "_elab_obj->";
      }
      
      upTypeConversionFn->_publish_cc_elaborate_arg( _cc_out, declarations );
      _cc_out << "," << NL();
      upTypeConversionFn->_get_implementation()->_publish_cc_type_conversion_function_name( _cc_out, declarations );
      _cc_out << CS(");");
    }
  }
}

void 
TyvisComponentInstantiationStatement::_publish_cc_elaborate_addChild( published_file &_cc_out,
								      TyvisAssociationElement *actual_clause,
								      TyvisLabel *label,
								      PublishData *declarations) {
  Tyvis* decl = actual_clause->_get_formal();
  TyvisDeclaration* tempDeclaration = NULL;

  if (actual_clause->_get_actual() == NULL) {
    cerr << "Warning: the actual in the association was NULL. Assuming to be"
	 << "open clause and ignoring.\n";
    return;
  }
  bool slice = false;
  if (dynamic_cast<TyvisSliceName *>(actual_clause->_get_formal()) ){
    slice = true;
  }
  if( actual_clause->_get_actual()->get_kind() != IIR_FUNCTION_CALL &&
      actual_clause->_get_formal()->get_kind() != IIR_FUNCTION_CALL ){
    CC_REF( _cc_out, "TyvisComponentInstantiationStatement::_publish_cc_elaborate_addChild" );

    if (slice) {
      _cc_out << OS("{");
      _cc_out << "SignalNetinfo SNITarget(";
    }
    else {
      _cc_out << OS("addChild(");
      _cc_out << "dynamic_cast<SignalNetinfo&>(";
    }
    if(dynamic_cast<TyvisDeclaration *>(actual_clause->_get_actual()) != NULL){
	_publish_cc_scoping_prefix( _cc_out.get_stream(),
				    dynamic_cast<TyvisDeclaration *>(actual_clause->_get_actual())->_get_declarative_region(),
				    this );
    }
    else if( actual_clause->_get_actual()->is_name() ){
      _publish_cc_scoping_prefix( _cc_out.get_stream(), 
				  actual_clause->_get_actual()->_get_prefix_declaration(),
				  this );
    }
    else {
      cerr << "Error : TyvisComponentInstantiationStatement::_publish_cc_elaborate_addChild( published_file &_cc_out, PublishData *declarations ) - declaration kind not handled yet.\n";
      abort();
    }
    if (slice) {
      dynamic_cast<TyvisSliceName *>(actual_clause->_get_actual())->_publish_cc_rvalue_noprefix( _cc_out, declarations );
      _cc_out << ";" << NL();
      _cc_out << "SignalNetinfo SNISource(";
    }
    else {
      actual_clause->_get_actual()->_publish_cc_rvalue( _cc_out, declarations );
      _cc_out << ")," << NL();
      _cc_out << "dynamic_cast<SignalNetinfo&>(";
    }
    label->_publish_cc_object_name( _cc_out, declarations );
    if (_get_currently_publishing_unit() == GENERATE_FOR) {
      _cc_out << "_elab_obj[i - generateMin].";
    }
    else {
      _cc_out << "_elab_obj->";
    }
    if (dynamic_cast<TyvisDeclaration *>(decl) != NULL) {
      tempDeclaration = dynamic_cast<TyvisDeclaration *>(decl);
    } 
    else if ((tempDeclaration == NULL) && ( decl->is_name() == TRUE )) {
      tempDeclaration = dynamic_cast<TyvisDeclaration *>(decl->_get_prefix_declaration());
      ASSERT ( tempDeclaration != NULL );
      _publish_cc_scoping_prefix( _cc_out.get_stream(), tempDeclaration->_get_declarative_region(),this );
    }
    else {
      cerr << "Error : TyvisComponentInstantiationStatement::_publish_cc_elaborate_addChild( published_file &_cc_out, PublishData *declarations ) - declaration kind not handled yet.\n";
      abort();
    }
    
    if (slice) {
      dynamic_cast<TyvisSliceName *>(decl)->_publish_cc_rvalue_noprefix( _cc_out, declarations );
      _cc_out << ";" << NL();
      _cc_out << "addChild (SNITarget, SNISource);" << NL();
      _cc_out << CS("}");
    }
    else {
      decl->_publish_cc_rvalue( _cc_out, declarations );
      _cc_out << CS(") );");
    }
  }
}

void
TyvisComponentInstantiationStatement::_publish_cc_connect_call( published_file &_cc_out, PublishData *declarations ){
  ostringstream objectName;

  objectName << *(_get_mangled_label()) << "_elab_obj";
  _set_current_elab_name( objectName.str() );
  
  _publish_connect( _cc_out, declarations );
}

void
TyvisComponentInstantiationStatement::_publish_cc_concurrent_stmt_init(published_file &_cc_out,
                                                                       TyvisDeclarationList* ,
                                                                       PublishData *declarations ){
  CC_REF( _cc_out, "TyvisComponentInstantiationStatement::_publish_cc_concurrent_stmt_init" );
  _cc_out << OS("{");
  ASSERT( IIRBase_ComponentInstantiationStatement::get_instantiated_unit()->get_kind() ==
	  IIR_COMPONENT_DECLARATION );
  TyvisComponentDeclaration *component_name =
    dynamic_cast<TyvisComponentDeclaration *>(_get_instantiated_unit());
  
  if (_get_currently_publishing_unit() != GENERATE_FOR) {
    _get_mangled_label()->_publish_cc_elaborate( _cc_out.get_stream() );
    _cc_out << "_elab_obj = new "
	    << component_name->_get_cc_elaboration_class_name()
	    << OS("(");
    if ( _get_generic_map_aspect()->_are_any_signals()) {
      // DNS: signals found in generic port map.  Need to generate error.
      cerr << "FATAL: Signal(s) \"" << _get_generic_map_aspect()->_get_signal_names() <<
           "\" found in generic map in file " <<
           _get_file_name()->_to_string() << " at line " <<
           get_line_number() << endl;
    }
    _get_generic_map_aspect()->_publish_cc_rvalues( _cc_out, declarations, "," );
    _cc_out << CS(");");
  }
  
  _cc_out << _get_current_publish_node()->_get_cc_elaboration_class_name()
	  << " *enclosingArch = this;" << NL()
	  << * _get_mangled_label()->_get_mangled_declarator()
	  << "_elab_obj";
  if (_get_currently_publishing_unit() == GENERATE_FOR) {
    _cc_out << "[i].boundedEntity = ";
  }
  else {
    _cc_out << "->boundedEntity = ";
  }
  
  if (_get_configuration() != NULL) {
    if ( _get_configuration()->get_kind() == IIR_CONFIGURATION_SPECIFICATION ){
      TyvisConfigurationSpecification *config_spec_decl = 
	dynamic_cast<TyvisConfigurationSpecification*>(_get_configuration());
      
      if(config_spec_decl == NULL) {
	_cc_out << "NULL;";	
      } 
      else if(config_spec_decl->_get_entity_aspect() != NULL) {
	CC_REF( _cc_out,
		"TyvisComponentInstantiationStatement::_publish_cc_concurrent_stmt_init" );

	// Add an include for the entity aspect's elab file...
	config_spec_decl->_get_entity_aspect()->_publish_cc_include_elab( _cc_out );
	_cc_out << "new "
		<< config_spec_decl->_get_entity_aspect()->_get_cc_elaboration_class_name()
		<< "(";
	Tyvis* tempNode = _get_current_publish_node();
	_set_current_publish_node( this );
	// Set and resent current_published_unit to print the generic
	// constants based on the fact that a component_instantiation
	// is being generated...
	PublishedUnit oldUnit = _get_currently_publishing_unit();
	_set_currently_publishing_unit(NONE);
	config_spec_decl->_get_generic_map_aspect()->_publish_cc_generic_map_aspect( _cc_out, declarations );
	
	IIR_Boolean commaFlag = FALSE;
	if (config_spec_decl->get_generic_map_aspect()->first() != NULL) {
	  commaFlag = TRUE;
	}
	_get_port_map_aspect()->_publish_cc_unconstrained_ports( _cc_out, declarations, commaFlag );
	_cc_out << ");" << NL();
	
	_set_currently_publishing_unit(oldUnit);
	config_spec_decl->_publish_cc_port_map_aspect( _cc_out, declarations );
	_set_current_publish_node( tempNode );
      } 
      else {
	_cc_out << " NULL;";
      }
    }
    else if (_get_configuration()->get_kind() == IIR_COMPONENT_CONFIGURATION ){
      CC_REF( _cc_out,
	      "TyvisComponentInstantiationStatement::_publish_cc_concurrent_stmt_init" );
      TyvisComponentConfiguration* config_spec_decl = dynamic_cast<TyvisComponentConfiguration*>(_get_configuration());
      if(config_spec_decl == NULL) {
	_cc_out << " NULL;";	
      } 
      else if(config_spec_decl->_get_entity_aspect() != NULL) {
	config_spec_decl->_get_entity_aspect()->_publish_cc_include_elab( _cc_out );
	_cc_out << " new "
		<< config_spec_decl->_get_entity_aspect()->_get_cc_elaboration_class_name()
		<< "(";
	Tyvis* tempNode = _get_current_publish_node();
	_set_current_publish_node( this );
	PublishedUnit oldUnit = _get_currently_publishing_unit();
	_set_currently_publishing_unit(NONE);
	config_spec_decl->_get_generic_map_aspect()->_publish_cc_generic_map_aspect( _cc_out, declarations );
	_set_currently_publishing_unit(oldUnit);
	_cc_out << ");" << NL();
	if (config_spec_decl->_get_entity_aspect()->_get_port_list() != NULL) {
	  const string old_elab_name = _get_current_elab_name();
	  ostringstream elabStream;
	  elabStream << *(_get_mangled_declarator()) << "_elab_obj->";
	  _set_current_elab_name( elabStream.str() );
	  _set_current_publish_node( _get_instantiated_unit() );
	  config_spec_decl->_get_entity_aspect()->_get_port_list()->_publish_cc_port_map_aspect( _cc_out, declarations );
	  _set_current_elab_name( old_elab_name );
	}
	_set_current_publish_node( tempNode );
      } 
      else {
	_cc_out << " NULL;";
      }
      _cc_out << NL();
    }
  }
  else {
    _cc_out << " NULL;" << NL();
    _cc_out << "cerr << \"Warning: Unbounded component.\" << endl;";
  }
 
  _cc_out << CS("}");
  
  _get_port_map_aspect()->_set_passed_through_out_port(TRUE);
}

void
TyvisComponentInstantiationStatement::_publish_cc_elaborate_upType( published_file &_cc_out,
                                                                           TyvisAssociationElement *actual_clause, 
                                                                           TyvisLabel *label,
                                                                           PublishData *declarations){
  Tyvis *formal = actual_clause->_get_formal();
  Tyvis *actual = actual_clause->_get_actual();
  IIR_Mode mode = formal->_get_mode();
  Tyvis *formal_publish_target = formal;
  TyvisFunctionCall *upTypeConversionFn = NULL;
  if( formal->get_kind() == IIR_FUNCTION_CALL ){
    formal_publish_target = dynamic_cast<Tyvis *>((dynamic_cast<TyvisFunctionCall *>(formal))->get_parameter_association_list()->first());
    upTypeConversionFn = dynamic_cast<TyvisFunctionCall *>(formal);
  }

  TyvisFunctionCall *actual_as_function_call = NULL;
  if( actual->get_kind() == IIR_FUNCTION_CALL ){
    actual_as_function_call = dynamic_cast<TyvisFunctionCall *>(actual);
  } 
  TyvisFunctionCall *formal_as_function_call = NULL;
  if( formal->get_kind() == IIR_FUNCTION_CALL ){
    formal_as_function_call = dynamic_cast<TyvisFunctionCall *>(formal);
  } 

  CC_REF(_cc_out, "TyvisComponentInstantiationStatement::_publish_cc_elaborate_UpType");

  if( mode == IIR_OUT_MODE || mode == IIR_INOUT_MODE  ){
    TyvisTypeDefinition *target_actual_subtype = NULL;
    if( mode == IIR_OUT_MODE ){
      target_actual_subtype = actual->_get_subtype();
    }
    else if( mode == IIR_INOUT_MODE ){
      target_actual_subtype = dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>(actual_as_function_call->get_parameter_association_list()->first())->get_formal())->_get_subtype();
    }
    ASSERT( target_actual_subtype != NULL );
    // New TypeConvert ptr
    _cc_out << "TypeConvert *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr = new TypeConvert(this);" << NL();
      
    _cc_out << "SignalNetinfo *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo = new SignalNetinfo";
    _cc_out << OS("(");

    target_actual_subtype->_publish_cc_object_type_info( _cc_out, declarations );
    _cc_out << "," << NL();
    _cc_out << "\"" << actual->_to_string() << "\"";

    if( target_actual_subtype->is_unconstrained_array_type() == TRUE ){
      target_actual_subtype->_publish_cc_range( _cc_out, declarations );
    }
    _cc_out << CS(");");
    
    // Code added for handling arrays
    //----------------------------------------------
    
    //New variable of actual
    _cc_out << "Signal *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigVariable = new Signal";
    _cc_out << OS("(");

    target_actual_subtype = NULL;
    
    if( mode == IIR_OUT_MODE  ){
      target_actual_subtype = actual->_get_subtype();
    }
    else if( mode == IIR_INOUT_MODE  ){
      target_actual_subtype = 
	dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>(actual_as_function_call->get_parameter_association_list()->first())->get_formal())->_get_subtype();
    }
    ASSERT (target_actual_subtype != NULL);
    target_actual_subtype->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "," << NL();
    _cc_out << "\"" << actual->_to_string() << "\"";

    if( target_actual_subtype->is_unconstrained_array_type() == TRUE ){
      target_actual_subtype->_publish_cc_range( _cc_out, declarations );
    }
    _cc_out << CS(");");
    
    
    //New signal of formal
    _cc_out << "Signal *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSignal = new Signal";
    _cc_out << OS("(");
    if( mode == IIR_INOUT_MODE ){
      actual->_get_subtype()->_publish_cc_type_info_xxx(_cc_out, declarations );
    }
    else if( mode == IIR_OUT_MODE ){
      dynamic_cast<TyvisAssociationElement *>(upTypeConversionFn->get_parameter_association_list()->first())->_get_formal()->_get_subtype()->_publish_cc_type_info_xxx(_cc_out, declarations );
    }
    _cc_out << "," << NL();
    _cc_out << "\"" << actual->_to_string() << "\"";
    _cc_out << CS(");");
    
    //addTypeConversionFn(UptypeFn)
    _cc_out << "int ";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_id = addTypeConversionFn("; 
    upTypeConversionFn->_get_implementation()->_publish_cc_type_conversion_function_name( _cc_out, declarations );
    _cc_out << ");" << NL();

    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr->setSignal(";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSignal);" << NL();
    
    //TypeConvertptr->setVariable(actual_variable)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr->setVariable(";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigVariable);" << NL();
 
    //TypeConvertptr->setSignalNetinfo(actual_signalnetinfo)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr->setSignalNetinfo(";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo);" << NL();
    
    //TypeConvertptr->setConversionFunctionId(formal_fnID)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr->setConversionFunctionId(";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_id);" << NL();

    //formal_signal->setElaborationinfo(formal)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSignal->setElaborationInfo(";

    label->_publish_cc_elaborate( _cc_out.get_stream() );
    _cc_out << "_elab_obj->";
    dynamic_cast<Tyvis *>((dynamic_cast<TyvisFunctionCall*>(formal))->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << ");" << NL();
    
    //addUpConvertDriver(*actual_signetinfo, *actual_variable, TCptr)
    _cc_out << "addUpConvertDriver(*";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo, *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigVariable, ";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr);" << NL();
    
    CC_REF( _cc_out, "TyvisComponentInstantiationStatement::_publish_cc_elaborate_UpType" );

    //addChild(actual, *actual_signetinfo)
    _cc_out << OS("addChild(");
    if( actual->get_kind() == IIR_FUNCTION_CALL ){
      dynamic_cast<Tyvis *>(actual_as_function_call->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
    }
    else {
      actual->_publish_cc_elaborate( _cc_out, declarations );
    }
    _cc_out << ", " << NL() <<  "*";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo" << CS(");");

    //addDriver(actual, *formal_signal)
    _cc_out << OS("addDriver(");
    if( actual->get_kind() == IIR_FUNCTION_CALL ){
      dynamic_cast<Tyvis *>(actual_as_function_call->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
    }
    else {
      actual_clause->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
    }
    _cc_out << "," << NL() << "*";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSignal" << CS(");");
    _cc_out << OS("Addall(");
    if( actual->get_kind() == IIR_FUNCTION_CALL ){
      dynamic_cast<Tyvis *>(actual_as_function_call->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
    }
    else {
      actual_clause->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
    }
/*
    dynamic_cast<Tyvis *>(actual_as_function_call->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
*/
    _cc_out << "," << NL() << "*";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo";
    _cc_out << CS(");");
  }
}
/*qqq
    label->_publish_cc_elaborate( _cc_out.get_stream() );
    _cc_out << "_elab_obj->";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );    
*/

void 
TyvisComponentInstantiationStatement::_publish_cc_elaborate_downType( published_file &_cc_out,
                                                                      TyvisAssociationElement *actual_clause,
                                                                      TyvisLabel *,
                                                                      PublishData *declarations){

  Tyvis *formal = actual_clause->_get_formal();
  Tyvis *actual = actual_clause->_get_actual();
  IIR_Mode mode = formal->_get_mode();
  Tyvis *formal_publish_target = formal;
  if( formal->get_kind() == IIR_FUNCTION_CALL ){
    formal_publish_target = dynamic_cast<Tyvis *>((dynamic_cast<TyvisFunctionCall *>(formal))->get_parameter_association_list()->first());
  }
  //  ASSERT( actual->get_kind() == IIR_FUNCTION_CALL );
  TyvisFunctionCall *downTypeConversionFn = 0;
  if( actual->get_kind() == IIR_FUNCTION_CALL ){
    downTypeConversionFn = dynamic_cast<TyvisFunctionCall *>(actual);
  }
  CC_REF( _cc_out, 
		"TyvisComponentInstantiationStatement::_publish_cc_elaborate_DownType" );
  if( mode == IIR_OUT_MODE  ){
    _cc_out << "SignalNetinfo *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo = new SignalNetinfo";
    _cc_out << OS("(");
    dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>((dynamic_cast<TyvisFunctionCall *>(formal))->get_parameter_association_list()->first())->get_formal())->_get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "," << NL();
    if( formal->get_kind() == IIR_FUNCTION_CALL)  {
       _cc_out << "\"" << dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>((dynamic_cast<TyvisFunctionCall *>(formal))->get_parameter_association_list()->first())->get_actual())->_to_string() << "\"";
    }
    else {
      _cc_out << "\"" << formal->_to_string() << "\"";
    }
    _cc_out << CS(");");
    
    _cc_out << OS("Addall(*");
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo," << NL();
    actual->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << CS(");");
  }

  if( mode == IIR_IN_MODE || mode == IIR_INOUT_MODE ){
    TyvisTypeDefinition *formal_subtype = NULL;
    if (formal_publish_target->get_kind() == IIR_ASSOCIATION_ELEMENT_BY_EXPRESSION)  {
      formal_subtype = (dynamic_cast<TyvisAssociationElementByExpression *>(formal_publish_target))->_get_formal()->_get_subtype();
    }
    else {
      formal_subtype = formal_publish_target->_get_subtype();
    }
    
    ASSERT (formal_subtype != NULL);
    
    //New TypeConvert 
    _cc_out << "TypeConvert *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr = new TypeConvert(this);" << NL();
    
    //New SignalNetinfo of formal
    _cc_out << "SignalNetinfo *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo = new SignalNetinfo";
    _cc_out << OS("(");
    formal_subtype->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << "," << NL();
    if( formal->get_kind() == IIR_FUNCTION_CALL)  {
      _cc_out << "\"" << dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>((dynamic_cast<TyvisFunctionCall *>(formal))->get_parameter_association_list()->first())->get_actual())->_to_string() << "\"";
    }
    else {
      _cc_out << "\"" << formal->_to_string() << "\"";
    }
    
    if ((formal_subtype->is_scalar_type() == FALSE) &&
	(formal_subtype->is_access_type() == FALSE)) {
      _cc_out << "," << NL();
      formal_subtype->_publish_cc_resolution_function_id( _cc_out, declarations);
    }
    
    _cc_out << CS(");");
    
    //New Variable of formal
    _cc_out << "Signal *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigVariable = new Signal";
    _cc_out << OS("(");
    formal_subtype->_publish_cc_object_type_info(_cc_out, declarations, TRUE, "", TRUE);
    _cc_out << "," << NL();
    if( formal->get_kind() == IIR_FUNCTION_CALL)  {
      _cc_out << "\"" << dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>((dynamic_cast<TyvisFunctionCall *>(formal))->get_parameter_association_list()->first())->get_actual())->_to_string() << "\"";
    }
    else {
      _cc_out << "\"" << formal->_to_string() << "\"";
    }

    if ((formal_subtype->is_scalar_type() == FALSE) &&
	(formal_subtype->is_access_type() == FALSE)) {
      _cc_out << "," << NL();
      formal_subtype->_publish_cc_resolution_function_id( _cc_out, declarations);
    }
    _cc_out << CS(");");
      
    
    //New Signal of actual
    _cc_out << "Signal *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSignal = new Signal";
    _cc_out << OS("(");
    TyvisTypeDefinition *target_actual_subtype = NULL;
    if( actual->get_kind()==IIR_FUNCTION_CALL)  {
      target_actual_subtype = dynamic_cast<TyvisAssociationElement *>(dynamic_cast<TyvisFunctionCall *>(actual)->get_parameter_association_list()->first())->_get_formal()->_get_subtype();
    }
    else {
      target_actual_subtype = actual->_get_subtype();
    }
    target_actual_subtype->_publish_cc_object_type_info( _cc_out, declarations );
    _cc_out << "," << NL();
    if( formal->get_kind()==IIR_FUNCTION_CALL)  {
      _cc_out << "\"" << dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>((dynamic_cast<TyvisFunctionCall *>(formal))->get_parameter_association_list()->first())->get_actual())->_to_string() << "\"";
    }
    else {
      _cc_out << "\"" << formal->_to_string() << "\"";
    }

    _cc_out << CS(");");

    //addTypeConversionFn(DownTypeFn) 
    _cc_out << "int ";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_id";
    _cc_out << " = addTypeConversionFn("; 
    downTypeConversionFn->_get_implementation()->_publish_cc_type_conversion_function_name( _cc_out, declarations );
    _cc_out << ");" << NL();

    //TCptr->setSignal(actual_signal)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr->";
    _cc_out << "setSignal(";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSignal);" << NL();
    
    //TCptr->setVariable(formal_variable)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr->";
    _cc_out << "setVariable(";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigVariable);" << NL();
    
    //TCptr->setSignalNetinfo(formal_signetinfo)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr->";
    _cc_out << "setSignalNetinfo(";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo);" << NL();
    
    //TCptr->setConversionFunctionId(actual_fn_id)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr->";
    _cc_out << "setConversionFunctionId(";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_id);" << NL();

    //actual_signal->setElaborationInfo(actual)
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSignal->setElaborationInfo(";
    
    dynamic_cast<Tyvis *>((dynamic_cast<TyvisFunctionCall*>(actual))->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << ");" << NL();
    
    //addUpConvertDriver(*formal_signetinfo, *formal_variable, TCptr)
    _cc_out << "addUpConvertDriver(*";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo, *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigVariable, ";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_typeConvertptr);" << NL();
    
    //addDriver(formal, *actual_signal)
    _cc_out << "addDriver(*";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSigNetInfo, *";
    formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_tmpSignal);" << NL();

    if( mode == IIR_INOUT_MODE ){
      _cc_out << OS("Addall( *");
      formal_publish_target->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << "_tmpSigNetInfo," << NL();
      
      if( actual->get_kind()==IIR_FUNCTION_CALL ){
	dynamic_cast<Tyvis *>((dynamic_cast<TyvisFunctionCall*>(actual))->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
      }
      else {
	actual->_publish_cc_elaborate( _cc_out, declarations );
      }
      _cc_out << CS(");");
    }
  }
}

void
TyvisComponentInstantiationStatement::_publish_connect( published_file &_cc_out, PublishData *declarations ){
  TyvisLabel *label = _get_mangled_label();
  int noofinputsignals =0;
  int noofoutputsignals =0;
  IIR_Boolean first = false;
  TyvisDeclaration* tempDeclaration = NULL;
  ASSERT( dynamic_cast<TyvisDeclaration *>(_get_instantiated_unit()) != NULL );
  TyvisAssociationElement* association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
  TyvisAssociationElement* conf = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());

  while(association != NULL) {
    if (association->_get_formal() != NULL) {
      switch (association->_get_formal()->_get_mode()) {
      case IIR_OUT_MODE:
	if( association->_get_formal()->get_kind() == IIR_FUNCTION_CALL ){
	  _publish_cc_elaborate_downType( _cc_out, association, label, declarations);
	}
	break;
      case IIR_IN_MODE:
      case IIR_INOUT_MODE:
	if (association->_get_actual() != NULL) {
	  if( association->_get_actual()->get_kind()==IIR_FUNCTION_CALL ){
	    _publish_cc_elaborate_downType( _cc_out, association, label, declarations );
	  }
	}
	break;
      default:
	break;
      }
    }
    association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(association));
  }

  vector<string> inPorts, outPorts;
  TyvisAssociationList * pm = 0;
  if (!_get_configuration()) {
    _cc_out << "CONNECT FAILED" << NL();
    cerr << "FATAL: Unknown entity associated with component ";
    cerr  << *(_get_instantiated_unit()->get_declarator()) << " in file: ";
    cerr << get_file_name()->convert_to_string() << " at line " << get_line_number() << endl;
    abort();
  }
  TyvisConfigurationSpecification *config_spec =
       dynamic_cast<TyvisConfigurationSpecification *>(_get_configuration());
  if ( config_spec ){
    pm = config_spec->_get_port_map_aspect( );
  }
  else {
    TyvisComponentConfiguration *config_spec =
       dynamic_cast<TyvisComponentConfiguration *>(_get_configuration());
    if ( config_spec ){
      pm = config_spec->_get_port_map_aspect( );
    }
  }
  ASSERT( pm );

  map<TyvisAssociationElement*, string> compNameMap;
  association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
  bool enclose_composites = false;
  while ( association != NULL ) {
    TyvisAssociationElement* nextAssociation =
       dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(association));
    string sigName;
    if (association->_get_formal()->get_kind() != IIR_FUNCTION_CALL ){
      sigName = association->_get_formal()->_get_mangled_declarator()-> get_string();
    }
    if ( nextAssociation && nextAssociation->_get_formal()->get_kind()!=IIR_FUNCTION_CALL &&
         sigName == nextAssociation->_get_formal()->_get_mangled_declarator()-> get_string() ) {
      if ( !enclose_composites ){
        _cc_out << OS("{");
        enclose_composites = true;
      }
      string compName = sigName + "_composite";
      compNameMap[association] = compName;
      _publish_cc_include( _cc_out, "tyvis/TyvisVarArgs.hh" );
      _cc_out << "SignalNetinfo " << compName << " (";
      conf = dynamic_cast<TyvisAssociationElement *>(pm->first());
      while(conf != NULL) {
        if (sigName == conf->_get_actual()->_get_mangled_declarator()->get_string() ){
          conf->_get_formal()->_get_subtype()->_publish_cc_type_info_xxx( _cc_out,
                                                                       declarations );
          break;
        }
        conf = dynamic_cast<TyvisAssociationElement *>(pm->successor(conf));
      }
      _cc_out << "," << NL();
      _cc_out << "TyvisVarArgs(LVALUE) ";

      nextAssociation = association;
      while (nextAssociation && sigName == 
          nextAssociation->_get_formal()->_get_mangled_declarator()-> get_string() ) {
        _cc_out << "<<" << NL();
        _cc_out << "ElementAssociation( ";
        bool slice = false;
        if (dynamic_cast<TyvisSliceName *>(nextAssociation->_get_formal()) )
          slice = true;
        if (slice){
          _cc_out << "ArrayInfo(";
          nextAssociation->_get_formal()->_publish_cc_range( _cc_out, declarations );
          _cc_out << ")";
        }
        else
          nextAssociation->_get_formal()->_publish_cc_value( _cc_out, declarations );
        _cc_out << "," << NL();
        if (slice) {
          _cc_out << "&SignalNetinfo(";
          nextAssociation->_get_actual()->_publish_cc_object_name( _cc_out, declarations );
          _cc_out << ", IntegerTypeInfo(";
          nextAssociation->_get_actual()->_publish_cc_range( _cc_out, declarations );
          _cc_out << "))";
        }
        else {
          _cc_out << "&";
          nextAssociation->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );

        }
        _cc_out << ")" << NL();
        nextAssociation = dynamic_cast<TyvisAssociationElement *>
                                      (get_port_map_aspect()->successor(nextAssociation));
      }
      _cc_out << ");" << NL();
    }
    association = nextAssociation;
  }

  association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first()); 
  if(association == NULL){
    label->_publish_cc_elaborate( _cc_out.get_stream() );
    if (_get_currently_publishing_unit() == GENERATE_FOR) {
      _cc_out << "_elab_obj[i - generateMin].connect(0, 0);" << NL();
    }
    else {
      _cc_out << "_elab_obj->connect(0,0);" << NL();
    }
    return;
  }
  label->_publish_cc_elaborate( _cc_out.get_stream() );
  if (_get_currently_publishing_unit() == GENERATE_FOR) {
    _cc_out << OS("_elab_obj[i - generateMin].connect(");
  }
  else {
    _cc_out << OS("_elab_obj->connect(");
  }

  conf = dynamic_cast<TyvisAssociationElement *>(pm->first());
  while(conf != NULL) {
    if (conf->_get_actual()) { //DNS: I don't think that this if test should be here.  PA2 fails without it
      switch(conf->_get_formal()->_get_mode()) {
        case IIR_IN_MODE:
          inPorts.push_back(conf->_get_actual()->_get_declarator()->get_string());
          break;
        case IIR_OUT_MODE:
        case IIR_INOUT_MODE:
          outPorts.push_back(conf->_get_actual()->_get_declarator()->get_string());
          break;
        default:
          break;
      }
    }
    else {
      cerr << "Apparently missing formal argument ";
      cerr << conf->_get_formal()->_get_mangled_declarator()->get_string();
      cerr << " from component referenced in " << get_file_name()->convert_to_string();
      cerr << " at line " << get_line_number() << endl;
    }
    conf = dynamic_cast<TyvisAssociationElement *>(pm->successor(conf));
  }
  _cc_out << (int) inPorts.size() << "," << NL() << (int) outPorts.size();
  if ( inPorts.size() + outPorts.size() > 0 )
    first = true;

  if(first == true) {
    vector<string>::iterator p_i, p_end;
    int pass;

    string::size_type st;
    if (inPorts.size() > 0)
      st = inPorts[0].rfind("__") + 1;
    else if (outPorts.size() > 0) 
      st = outPorts[0].rfind("__") + 1;

    p_i = inPorts.begin();
    p_end = inPorts.end();
    association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
    for ( ; p_i != p_end ; ++p_i ){
      pass = 0;
      while ( pass < 2 ){
        string myName;
        if( association->_get_formal()->get_kind() == IIR_FUNCTION_CALL)  {
          myName = dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>
             ((dynamic_cast<TyvisFunctionCall *>(association->_get_formal()))->
             get_parameter_association_list()->first())->get_actual())->_to_string();
        }
        else {
          if (association->_get_formal()->get_kind() == IIR_INDEXED_NAME){
            myName = dynamic_cast<TyvisIndexedName *>(association->_get_formal())->_get_prefix()->_to_string();
          }
          else {
            myName = association->_get_formal()->_to_string();
          }
        }
        if ( myName == (*p_i).substr(st) ){
          break;
        }
        association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(association));
        if ( association == NULL ) {
          association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
          if ( ++pass == 2 ) {
            cerr << "FATAL: no actual found for: " << *p_i << endl;
            ASSERT(0);
          }
        }
      }
      if( association->_get_formal() && association->_get_actual() ){
	switch(association->_get_formal()->_get_mode()) {
	case IIR_IN_MODE:
	  _cc_out << "," << NL();
          _cc_out << "dynamic_cast<RValue *>(";
	  if (association->_get_actual()->get_kind()!=IIR_FUNCTION_CALL){
	    _cc_out << "&";
	  }
	  
	  ASSERT(association->_get_actual() != NULL);
	  if (association->_get_actual()->get_kind() == IIR_FUNCTION_CALL) {
	    if (association->get_formal()->get_kind()== IIR_FUNCTION_CALL){
	      dynamic_cast<Tyvis *>(dynamic_cast<TyvisFunctionCall *>(association->_get_formal())->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
	    }
	    else{
	      association->_get_formal()->_publish_cc_elaborate( _cc_out, declarations );
	    }
	    _cc_out << "_tmpSigNetInfo";
	  }
	  else if (association->_get_actual()->get_kind() == IIR_INDEXED_NAME){
	    TyvisIndexedName *tempIdxName = dynamic_cast<TyvisIndexedName *>(association->_get_actual());
	    ASSERT ( tempIdxName->get_kind() == IIR_INDEXED_NAME );
	    if ( dynamic_cast<TyvisDeclaration *>(tempIdxName->_get_prefix()) ) {
	      tempDeclaration = dynamic_cast<TyvisDeclaration *>(tempIdxName->_get_prefix());
	      ASSERT ( tempDeclaration!= NULL );
	      _publish_cc_scoping_prefix( _cc_out.get_stream(), 
					  tempDeclaration->_get_declarative_region(),
					  this);
	    }
            if ( compNameMap.find(association) != compNameMap.end() )
              _cc_out << compNameMap[association];
            else
              association->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
	  }
	  else {
	    TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(association->_get_actual());
            if (decl != NULL) {
	      _publish_cc_scoping_prefix( _cc_out.get_stream(), decl->_get_declarative_region(), 
					this);
            if ( compNameMap.find(association) != compNameMap.end() )
              _cc_out << compNameMap[association];
            else
	      decl->_publish_cc_elaborate( _cc_out, declarations );
            }
            else {
// This happens when we try to pass a slice through a component.
	      ASSERT ( decl != NULL );
              _cc_out << "DNS>";
              _cc_out << typeid(*(association->_get_actual())).name();
              _cc_out << "<DNS";
            }
	  }
          _cc_out << ")";
	  break;

	default:
	  break;
	}
      }
      association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(association));
      if ( association == NULL )
        association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
    }
    
    p_i = outPorts.begin();
    p_end = outPorts.end();
    association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
    for ( ; p_i != p_end ; ++p_i ){
      pass = 0;
      while ( pass < 2 ){
        string myName;
        if( association->_get_formal()->get_kind() == IIR_FUNCTION_CALL)  {
          myName = dynamic_cast<Tyvis *>(dynamic_cast<TyvisAssociationElement *>
             ((dynamic_cast<TyvisFunctionCall *>(association->_get_formal()))->
             get_parameter_association_list()->first())->get_actual())->_to_string();
        }
        else {
          if (association->_get_formal()->get_kind() == IIR_INDEXED_NAME){
            myName = dynamic_cast<TyvisIndexedName *>(association->_get_formal())->_get_prefix()->_to_string();
          }
          else {
            myName = association->_get_formal()->_to_string();
          }
        }
        if ( myName == (*p_i).substr(st) ){
          break;
        }
        association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(association));
        if ( association == NULL ) {
          association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
          if ( ++pass == 2 ) {
            cerr << "FATAL: no actual found for: " << *p_i << endl;
            ASSERT(0);
          }
        }
      }

      if (association->_get_actual() == NULL) {
	cerr << "Warning: actual in port map was NULL. Assuming open clause"
	     << " and generating defaults.\n";
	ASSERT ( association->_get_formal() != NULL );
	ASSERT ( association->_get_formal()->_get_subtype() != NULL );
	
	_cc_out << "," << NL() << "dynamic_cast<RValue *>(new Signal( ";
	association->_get_formal()->_get_subtype()->_publish_cc_type_info_xxx( _cc_out,
									       declarations );
	_cc_out << ") )";
	association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(association));
        if ( association == NULL )
          association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
	continue;
      }

      if (association->_get_formal() != NULL) {
	switch(association->_get_formal()->_get_mode()) {
	case IIR_OUT_MODE:
	case IIR_INOUT_MODE:
	  _cc_out << "," << NL();
          _cc_out << "dynamic_cast<RValue *>(";
	  if (association->_get_formal()->get_kind() != IIR_FUNCTION_CALL) {
	    _cc_out << "&";
	  }
	  
	  if (( association->_get_actual() &&
		association->_get_actual()->get_kind() == IIR_FUNCTION_CALL ) ||
	      association->_get_formal()->get_kind() == IIR_FUNCTION_CALL ) {
	    if( association->_get_formal()->_get_mode() == IIR_INOUT_MODE ){
	      if (association->_get_formal()->get_kind()==IIR_FUNCTION_CALL){
		dynamic_cast<Tyvis *>(dynamic_cast<TyvisFunctionCall *>(association->_get_formal())->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
	      }
	      else{
		association->_get_formal()->_publish_cc_elaborate( _cc_out, declarations );
	      }
	      _cc_out << "_tmpSigNetInfo";
	    }
	    if( association->_get_formal()->_get_mode()==IIR_OUT_MODE ){
	      if( association->_get_formal()->get_kind()==IIR_FUNCTION_CALL ){
		dynamic_cast<Tyvis *>(dynamic_cast<TyvisFunctionCall *>(association->_get_formal())->get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
	      }
	      else {
		association->_get_formal()->_publish_cc_elaborate( _cc_out, declarations );
	      }
	      _cc_out << "_tmpSigNetInfo";
	    }  
	  }
	  else {
//qqq
	    if ( dynamic_cast<TyvisDeclaration *>(association->_get_actual()) ) {
	      tempDeclaration = dynamic_cast<TyvisDeclaration *>(association->_get_actual());
	      ASSERT ( tempDeclaration != NULL );
	      _publish_cc_scoping_prefix( _cc_out.get_stream(), 
					  tempDeclaration->_get_declarative_region(),
					  this);
	    }
	    else if (association->_get_actual()->get_kind() == IIR_INDEXED_NAME) {
	      TyvisIndexedName *tempIdxName = dynamic_cast<TyvisIndexedName *>(association->_get_actual());
	      ASSERT ( tempIdxName->get_kind() == IIR_INDEXED_NAME );
	      if ( dynamic_cast<TyvisDeclaration *>(tempIdxName->_get_prefix()) ) {
		tempDeclaration = dynamic_cast<TyvisDeclaration *>(tempIdxName->_get_prefix());
		ASSERT ( tempDeclaration != NULL );
		_publish_cc_scoping_prefix( _cc_out.get_stream(),
					    tempDeclaration->_get_declarative_region(),
					    this);
	      }
	    }
	    
            if ( compNameMap.find(association) != compNameMap.end() )
              _cc_out << compNameMap[association];
            else
	      association->_get_actual()->_publish_cc_elaborate( _cc_out, declarations );
	  }
          _cc_out << ")";
	  
	  break;

	default:
	  break;
	}
      }
      association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(association));
      if ( association == NULL )
        association = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
    }
  }
  _cc_out << CS(");");
  if (enclose_composites)
    _cc_out << CS("}");
}

void
TyvisComponentInstantiationStatement::_publish_connect_terminals( published_file &_cc_out, PublishData *declarations ) {
  // local variables needed for publishing this node ...
  //  ASSERT(get_instantiated_unit()->_is_terminal() == TRUE);
  ASSERT( dynamic_cast<TyvisDeclaration *>(_get_instantiated_unit()) != NULL );
  
  IIR_Boolean first = false;
  TyvisAssociationElement* actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
  Tyvis* current_formal = actual_clause->_get_formal();
  TyvisLabel *label = _get_mangled_label();
  IIR_Int32 noofterminals = 0;
  Tyvis *port_map_element = NULL;
  
  label->_publish_cc_elaborate( _cc_out, declarations);
  _cc_out << "_elab_obj->connectTerminals(";
  //Find out if the formals and actuals of port_map_aspect are present
  //If they are present use them otherwise get the order from component decl
  ASSERT(current_formal != NULL);

  while (actual_clause != NULL) {
    if (actual_clause->get_formal()->get_kind() ==
	IIR_TERMINAL_INTERFACE_DECLARATION) {
      noofterminals++;
      first = true;
    }
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
  }
  _cc_out << noofterminals; 
  
  if (first == true) {
    actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->first());
    while (actual_clause != NULL) {
      port_map_element = actual_clause->_get_actual();
      ASSERT(port_map_element != NULL);
      switch (port_map_element->get_kind()) {
      case IIR_TERMINAL_INTERFACE_DECLARATION: {
        _cc_out << ", ";
        port_map_element->_publish_cc_lvalue( _cc_out, declarations );
      }
	break;
	// have to look if this condition will ever occur!
      case IIR_TERMINAL_DECLARATION: {
        _cc_out << ", &";
        port_map_element->_publish_cc_lvalue( _cc_out, declarations );
      }
	break;
      // this case is for the electrical'reference case !!
      /*case IIR_REFERENCE_ATTRIBUTE: {
    
        IIR_ReferenceAttribute *ref_attribute = ((IIR_ReferenceAttribute*)port_map_element);
  
        ASSERT(ref_attribute->get_prefix()->get_kind() == IIR_NATURE_DECLARATION);
  
        IIR_NatureDeclaration *nature_decl =
    
          ((IIR_NatureDeclaration*)ref_attribute->get_prefix());
      
        _cc_out << ", &";
        nature_decl->_get_reference_terminal()->_publish_cc( _cc_out, declarations );
      }
      break;*/
      default:
        // nothing to be done for the default case !!
        break;
      }
      actual_clause = dynamic_cast<TyvisAssociationElement *>(get_port_map_aspect()->successor(actual_clause));
    }  
  }
  _cc_out << ", \"" ;
  _get_label()->print(_cc_out.get_stream());
  _cc_out << "\");" << NL();
          
}   

void
TyvisComponentInstantiationStatement::_publish_form_characteristic_expressions(published_file &_cc_out) {
  TyvisLabel *label = _get_mangled_label();
  label->_publish_cc_elaborate(_cc_out.get_stream());
  _cc_out << "_elab_obj->formCharacteristicExpressions();" << NL();
}

Tyvis*
TyvisComponentInstantiationStatement::_transmute() {
  // Code to transmute direct component instantiations to a component
  // declaration, configuration specification and a new instantiation.

  ASSERT ( _get_instantiated_unit() != NULL );
  if( _get_instantiated_unit()->is_component_declaration() ){
    return this;
  }

  TyvisArchitectureDeclaration *arch
    = _get_instantiated_unit()->_get_cc_instantiated_architecture();
  TyvisEntityDeclaration *entity
    = _get_instantiated_unit()->_get_cc_instantiated_entity();
  
  ASSERT( entity != 0 );

  TyvisConfigurationSpecification *config_spec = 0;  
  if( arch == 0 ){
    // No architecture was specified, let's see if there was an explicit
    // configuration specification.
    if( _get_configuration() != 0 ){
      ASSERT( dynamic_cast<TyvisConfigurationSpecification *>(_get_configuration()) );
      config_spec = dynamic_cast<TyvisConfigurationSpecification *>(_get_configuration());
    }
    else{
      // We'll go find the default arch for the entity then.
      arch = dynamic_cast<TyvisArchitectureDeclaration *>(library_manager::instance()->lookup_default_architecture( entity ));
      if( arch == 0 ){
	string err( "Could not find a default architecture for entity |" +
		    entity->_get_declarator()->_to_string() + "| and no architecure was " +
		    "explicitly specified." );
	report_error( this, err );
	return this;
      }
    }
  }

  ASSERT (entity != 0);

  ostringstream component_stream;
  string componentName;
  
  component_stream << "work_" << *entity << "_" << *arch;
  componentName = component_stream.str();
  
  TyvisComponentDeclaration *component_decl = 0;
  TyvisDeclarationList *enclosingDecls = 0;
  savant::set<IIR_Declaration> *compSet = 0;
  
  enclosingDecls = _get_declarative_region()->_get_declaration_list();
  ASSERT (enclosingDecls != 0);
  
  compSet = enclosingDecls->find_declarations( IIRBase_Identifier::get(componentName, get_design_file()->get_class_factory()) );
  if( compSet == 0 ){
    // A component declaration and cofiguration specification does not exist
    // so create a new one and add them to the list.
    component_decl = _build_implicit_component_declaration( entity, componentName );
    enclosingDecls->append(component_decl);

    if( config_spec == 0 ){
      ASSERT( arch != 0 );
      config_spec = _build_implicit_configuration_specification( arch, component_decl );
      config_spec->set_generic_map_aspect(get_generic_map_aspect());
      config_spec->set_port_map_aspect(get_port_map_aspect());
      enclosingDecls->append( config_spec );
    }
  }
  else {
    ASSERT ( compSet->getElement()->get_kind() == IIR_COMPONENT_DECLARATION );
    component_decl = dynamic_cast<TyvisComponentDeclaration *>(compSet->getElement());
  }
  
  TyvisComponentInstantiationStatement *newStmt = new TyvisComponentInstantiationStatement;
  
  copy_location(this, newStmt);
  newStmt->set_label(get_label());
  newStmt->set_generic_map_aspect(get_generic_map_aspect());
  newStmt->set_port_map_aspect(get_port_map_aspect());
  newStmt->set_instantiated_unit( component_decl );
  newStmt->set_configuration( config_spec );
  
  _add_to_configuration_specification( enclosingDecls, component_decl, newStmt );

  return newStmt;
}

TyvisAssociationList *
TyvisComponentInstantiationStatement::_get_generic_map_aspect() {
  return dynamic_cast<TyvisAssociationList *>(get_generic_map_aspect());
}

TyvisAssociationList *
TyvisComponentInstantiationStatement::_get_port_map_aspect() {
  return dynamic_cast<TyvisAssociationList *>(get_port_map_aspect());
}

Tyvis *
TyvisComponentInstantiationStatement::_get_configuration() {
  return dynamic_cast<Tyvis *>(get_configuration());
}
Tyvis *
TyvisComponentInstantiationStatement::_get_instantiated_unit() {
  return dynamic_cast<Tyvis *>(get_instantiated_unit());
}

TyvisConfigurationSpecification*
TyvisComponentInstantiationStatement::_get_configuration_specification(TyvisDeclarationList* decl_list) {
  
  TyvisComponentDeclaration* componentname = NULL;
  TyvisConfigurationSpecification* config_spec_decl = NULL;

  ASSERT( _get_instantiated_unit()->get_kind() == IIR_COMPONENT_DECLARATION );  
  componentname = dynamic_cast<TyvisComponentDeclaration *>(_get_instantiated_unit());

  // Searching for configuration specification
  TyvisDeclaration* decl = dynamic_cast<TyvisComponentDeclaration *>(decl_list->first());
  while(decl != NULL) {
    config_spec_decl = dynamic_cast<TyvisConfigurationSpecification*>(decl);
    if( config_spec_decl != NULL ) {
      if(config_spec_decl->_has_same_component((TyvisComponentInstantiationStatement*)this) == TRUE) {
	if (_get_declarative_region()->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT) {
	  return config_spec_decl;
	}
	if(config_spec_decl->_configures_this_component(dynamic_cast<TyvisComponentInstantiationStatement*>(this)) == TRUE) {
	  return config_spec_decl;
	}
      }
    }
    decl = dynamic_cast<TyvisComponentDeclaration *>(decl_list->successor(decl));
  }
  return NULL;
}

TyvisConfigurationSpecification*
TyvisComponentInstantiationStatement::_get_configuration_specification_from_any_scope(TyvisDeclarationList *decl_list) {
  // This method iteratively searches the outer scope(s) to locate a
  // configuration specification if one was not found in the inner scope
  TyvisConfigurationSpecification *config_spec = NULL;
  Tyvis* current_scope                          = _get_declarative_region();
  
  do {
    config_spec = _get_configuration_specification(decl_list);
    if (config_spec != NULL) {
      return config_spec;
    }
    
    if (current_scope != NULL) {
      decl_list     = current_scope->_get_declaration_list();
      ASSERT ( decl_list != NULL );
      current_scope = current_scope->_get_declarative_region();
    }
    else {
      decl_list = NULL;
    }
  } while (decl_list != NULL);

  return NULL;
}

void
TyvisComponentInstantiationStatement::_add_to_configuration_specification(TyvisDeclarationList *searchList, TyvisComponentDeclaration *compDecl, TyvisComponentInstantiationStatement *newStmt) {
  TyvisConfigurationSpecification *configSpec = NULL;
  TyvisDeclaration                *decl = NULL;
  
  decl = dynamic_cast<TyvisDeclaration *>(searchList->first());
  while (decl != NULL) {
    if (decl->get_kind() == IIR_CONFIGURATION_SPECIFICATION) {
      configSpec = dynamic_cast<TyvisConfigurationSpecification *>(decl);
      if (configSpec->get_component_name() == compDecl) {
	break;
      }
      configSpec = NULL;
    }
    decl = dynamic_cast<TyvisDeclaration *>(searchList->successor(decl));
  }
  
  if (configSpec == NULL) {
    cerr << "Error :- Unable to locate configuration specification for"
	 << " anonymous component declaration.\n";
    abort();
  }
  
  ASSERT (configSpec != NULL);
  
  TyvisDesignatorExplicit *newDesignator = new TyvisDesignatorExplicit;
  copy_location(this, newDesignator);
  newDesignator->set_name(newStmt->get_label());
  configSpec->get_instantiation_list()->append(newDesignator);
}
  

TyvisComponentDeclaration*
TyvisComponentInstantiationStatement::_build_implicit_component_declaration(TyvisEntityDeclaration *entity,
										const string &compName) {
  TyvisComponentDeclaration *componentDecl = new TyvisComponentDeclaration;
  
  copy_location(this, componentDecl);
  componentDecl->set_local_generic_clause(entity->get_generic_clause());
  componentDecl->set_local_port_clause(entity->get_port_clause());
  componentDecl->set_declarator(TyvisIdentifier::get( compName, get_design_file()->get_class_factory() ));

  return componentDecl;
}

TyvisConfigurationSpecification*
TyvisComponentInstantiationStatement::_build_implicit_configuration_specification( TyvisLibraryUnit *instantiatedUnit, 
                                                                                          TyvisComponentDeclaration *compDecl) {
  TyvisConfigurationSpecification *config_spec = new TyvisConfigurationSpecification;
  copy_location(this, config_spec);
  config_spec->set_component_name(compDecl);
  config_spec->set_entity_aspect(instantiatedUnit);

  TyvisDesignatorExplicit *designator = new TyvisDesignatorExplicit();
  designator->set_name( get_label() );
  config_spec->get_instantiation_list()->append( designator );


  return config_spec;
}

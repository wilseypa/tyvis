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
#include "TyvisAttributeSpecification.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisComponentDeclaration.hh"
#include "TyvisConstantInterfaceDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisGenericList.hh"
#include "TyvisIdentifier.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisPortList.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisTerminalInterfaceDeclaration.hh"
#include "savant/IIRBase_ComponentDeclaration.hh"
#include "savant/IIR_EntityDeclaration.hh"

#include "savant/resolution_func.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include <sstream>
#include <map>
#include "savant/language_processing_control.hh"

using std::map;

extern language_processing_control *lang_proc;

TyvisComponentDeclaration::TyvisComponentDeclaration() {
  set_local_generic_clause(new TyvisGenericList());
  set_local_port_clause(new TyvisPortList());
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisComponentDeclaration::~TyvisComponentDeclaration() {
  //Release the list memory
  delete get_local_generic_clause();
  delete get_local_port_clause();
  delete get_attributes();
}

void
TyvisComponentDeclaration::_publish_cc_binding_name(ostream& outstream) {
  outstream << *_get_mangled_declarator();
}

void
TyvisComponentDeclaration::_publish_cc_package_decl( published_file &_cc_out,
						     PublishData *declarations ){
  _publish_cc_elaborate( _cc_out, declarations );
}


void
TyvisComponentDeclaration::_publish_cc_elaborate( published_file &, PublishData *declarations ){
  _publish_cc_headerfile(declarations);
  _publish_cc_ccfile(declarations);
}

void
TyvisComponentDeclaration::_publish_cc_headerfile(PublishData *declarations){
  const string filename = _get_cc_elaboration_class_name();

  PublishedUnit oldUnit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(DUMMY_ENTITY_DECL);
  
  published_header_file header_file( _get_work_library()->get_path_to_directory(), 
				     filename,
				     this );
  CC_REF( header_file, "TyvisComponentDeclaration::_publish_cc_headerfile" );

  Tyvis::_publish_cc_include( header_file, "tyvis/_savant_entity_elab.hh" );
  if ( lang_proc->processing_vhdl_ams() ) {
    Tyvis::_publish_cc_include( header_file, "tyvis/_savant_entity_ams_elab.hh" );
  }
  Tyvis::_publish_cc_include( header_file, "tyvis/SignalNetinfo.hh" );

  // Request the Hierarchy.hh for our hierarchy stuff
  Tyvis::_publish_cc_include( header_file, "tyvis/Hierarchy.hh" );

  if( lang_proc->processing_vhdl_ams() ){
    Tyvis::_publish_cc_include( header_file, "tyvis/_savant_entity_ams_elab.hh" );
  }


  _set_currently_publishing_unit(oldUnit);
  
  _publish_cc_class( header_file, declarations );
}

void
TyvisComponentDeclaration::_publish_cc_class( published_file &_cc_out,
					      PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_class" );

  ostringstream elabStream;
  if( lang_proc->processing_vhdl_ams() ){  
    elabStream << " _savant_entity_ams_elab";
  }
  else {
    elabStream << " _savant_entity_elab";
  }
  string elabString = elabStream.str();

  _cc_out << "class " << _get_cc_elaboration_class_name()
          << OS(": public" + elabString + "{");
  _cc_out << "public:" << NL();
  _cc_out << _get_cc_elaboration_class_name() << "();" << NL();

  if( _get_local_generic_clause()->size() > 0 ){
    _cc_out << OS( _get_cc_elaboration_class_name() + "(" ) << NL();
    _get_local_generic_clause()->_publish_generic_parameter_list( _cc_out, declarations );
    _cc_out << CS(");");
  }

  _cc_out << "~" << _get_cc_elaboration_class_name() << "();" << NL();

  _get_local_generic_clause()->_publish_cc_elaborate( _cc_out, declarations );
  _get_local_port_clause()->_publish_cc_elaborate( _cc_out, declarations );
  _cc_out << elabString << " *boundedEntity;" << NL()
	  << "void instantiate( Hierarchy *hier, const string parent_base, const char *local_name );" << NL()
          << "std::string _base;" << NL()
          << "std::string get_base() {return(_base);}" << NL()
	  << "void createNetInfo();" << NL()
	  << "void connect(int, int, ...);" << NL()
	  << "void partition(){}" << NL()
	  << "void buildSignalTree();" << NL();
  if( lang_proc->processing_vhdl_ams() ){
    _cc_out << "void connectTerminals(int, ...);"<<NL()
            << "void formCharacteristicExpressions();"<<NL();
  }  

  _cc_out << CS("};");

  // Now publish type_infos as needed
  if(_get_local_generic_clause()->size() > 0) {
    _get_local_generic_clause()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
  if(_get_local_port_clause()->size() > 0) {
    _get_local_port_clause()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  }
}

void
TyvisComponentDeclaration::_publish_cc_ccfile(PublishData *declarations){
  const string filename = _get_cc_elaboration_class_name();
  PublishedUnit oldUnit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(DUMMY_ENTITY_DECL);

  published_cc_file cc_file( _get_work_library()->get_path_to_directory(),
			     filename,
			     this );

  CC_REF( cc_file, "TyvisComponentDeclaration::_publish_cc_ccfile" );

  _publish_cc_include( cc_file );
  _publish_cc_constructor( cc_file, declarations );
  _publish_cc_destructor( cc_file );
  _publish_cc_instantiate( cc_file );
  _publish_cc_createNetInfo( cc_file, declarations );
  _publish_cc_connect( cc_file, declarations );
  //  _publish_cc_partition( cc_file );
  _publish_cc_getboundentityinfo( cc_file );
  if( lang_proc->processing_vhdl_ams() ) {
    _publish_cc_ams_connect_terminals( cc_file, declarations );
    _publish_cc_ams_form_characteristic_expressions( cc_file );
  }
  _set_currently_publishing_unit(oldUnit);
}

void
TyvisComponentDeclaration::_publish_cc_constructor( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_constructor" );

  _publish_cc_constructor_with_no_arguments( _cc_out, declarations );
  if (_get_local_generic_clause()->size() > 0) {
    _publish_cc_constructor_with_arguments( _cc_out, declarations );
  }
}

void 
TyvisComponentDeclaration::_publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations ) {
  int numPortClause = _get_local_port_clause()->size();
  
  bool published_initializers = false;
  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_constructor_with_no_arguments" );

  _cc_out << _get_cc_elaboration_class_name()
	  << "::"
	  << _get_cc_elaboration_class_name() << "()";

  if( numPortClause > 0 || _get_local_generic_clause()->size() > 0 ) {
    _cc_out << OS(":") << NL();
    published_initializers = true;
  }
  _get_local_generic_clause()->_publish_generic_init( _cc_out, declarations );
  if(_get_local_generic_clause()->size() > 0 ) {
    if(_get_local_port_clause()->size() > 0){
      _cc_out << "," << NL();
    }
  }

  if ( published_initializers ){
    _cc_out << CS("");
  }

  _get_local_port_clause()->_publish_cc_port_init( _cc_out, declarations );
  _cc_out << OS("{")
	  << "boundedEntity = NULL;"
	  << CS("}");
}

void 
TyvisComponentDeclaration::_publish_cc_constructor_with_arguments( published_file &_cc_out, PublishData *declarations ){
  int numPortClause = _get_local_port_clause()->size();

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_constructor_with_arguments" );

  _cc_out << _get_cc_elaboration_class_name() << "::"
	  << _get_cc_elaboration_class_name() << "(" << NL();
  _get_local_generic_clause()->_publish_generic_parameter_list( _cc_out, declarations );
  _cc_out << " )";

  _cc_out << ":" << NL();
  _get_local_generic_clause()->_publish_generic_init_by_arguments( _cc_out, declarations );
  if( numPortClause > 0 ){
    _cc_out << "," << NL();
    _get_local_port_clause()->_publish_cc_port_init( _cc_out, declarations );
  }

  _cc_out << OS("{")
	  << "boundedEntity = NULL;\n"
	  << CS("}");
}

void
TyvisComponentDeclaration::_publish_cc_destructor( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_destructor" );

  _cc_out << OS(_get_cc_elaboration_class_name() + "::~"
		+ _get_cc_elaboration_class_name() + "(){")
	  << "delete boundedEntity;" << NL()
	  << CS("}");
}

void
TyvisComponentDeclaration::_publish_cc_instantiate( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_instantiate" );

  _cc_out << "void" << NL() 
	  << OS(_get_cc_elaboration_class_name() +
		"::instantiate( Hierarchy *hier, const string parent_base, const char *local_name ){")
          << OS("_base = hier->addFrame(parent_base, local_name);")
	  << OS("if (boundedEntity ){")
	  << "boundedEntity->instantiate(hier, _base, \"\");" << NL()
	  << CS("}")
	  << CS("}");
}

void
TyvisComponentDeclaration::_publish_cc_createNetInfo( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_createNetInfo" );

  _cc_out << "void" << NL()
	  << OS( _get_cc_elaboration_class_name() +
		 "::createNetInfo(){");
  _cc_out << OS("if (boundedEntity != NULL) {")
	  << "boundedEntity->createNetInfo();" << NL()
	  << CS("}")
	  << CS("}");
}

void
TyvisComponentDeclaration::_publish_cc_connect( published_file &_cc_out, PublishData *declarations ) {
  ostringstream connectparams;

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_connect" );
  
  TyvisSignalInterfaceDeclaration* portelement = NULL;
  TyvisPortList *portClause = NULL;
  map<string, TyvisSignalInterfaceDeclaration*> localMap;

  portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(_get_local_port_clause()->first());
  for (; portelement != NULL; ) {
    localMap[portelement->_get_declarator()->get_string()] = portelement;
    portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>
                              (_get_local_port_clause()->successor(portelement));
  }

  portClause = _get_entity_port_clause();
  if (!portClause) { 
    cerr << "Warning: No default entity associated with component "; 
    cerr  << *(get_declarator()) << " in file: ";
    cerr << get_file_name()->convert_to_string() << " at line " << get_line_number() << endl;
    portClause = _get_local_port_clause();
  }    

  _publish_cc_include( _cc_out, "cstdarg", true );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/SignalNetinfo.hh" );

  _cc_out << "void" << NL()
	  << OS( _get_cc_elaboration_class_name() +
		 "::connect(int inputsignals, int outputsignals, ...){")
	  << "int NoofSignals = inputsignals + outputsignals;" << NL()
	  << "va_list ap;" << NL()
	  << "int currentSignal = 0;" << NL()
	  << "va_start(ap, outputsignals);" << NL()
	  << "for(int i=0; i <NoofSignals; i++) {" << NL()
	  << "addToFanOut(va_arg(ap, RValue*) );" << NL()
	  << "}" << NL()
	  << "va_end(ap);" << NL();
  
  IIR_Mode mode;
  int index = 0;
  int first = 0;

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_connect" );
  _cc_out << OS("if ( inputsignals > 0 ){");
  portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(portClause->first());
  for (; portelement != NULL; ) {
    mode = portelement->get_mode();
    if ((mode == IIR_IN_MODE)) {
      CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_connect" );
      first = 1;
      _cc_out << OS("currentSignal = checkSetSourceInfo(");
      localMap[portelement->_get_declarator()->get_string()]->
                      _publish_cc_object_name( _cc_out, declarations );
      _cc_out << "," << NL()
	      << "currentSignal,"
	      << CS("fanOutInfo);");
      index++;
    }
    portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(portClause->successor(portelement));
  }
  _cc_out << CS("}");
  _cc_out << OS("if ( outputsignals > 0 ){");
  portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(portClause->first());
  for (; portelement != NULL; ) {
    if (portelement->get_kind() != IIR_TERMINAL_INTERFACE_DECLARATION) {
      mode = portelement->get_mode();
      if (mode == IIR_INOUT_MODE) {
	CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_connect" );
	first = 1;
	_cc_out << OS("checkAdd( ");
	localMap[portelement->_get_declarator()->get_string()]->
                        _publish_cc_object_name( _cc_out, declarations ); 
	_cc_out << "," << NL()
		<< "currentSignal," << NL()
		<< CS("fanOutInfo );");
	
	_cc_out << OS("currentSignal = checkSetSourceInfo(");
	localMap[portelement->_get_declarator()->get_string()]->
                        _publish_cc_object_name( _cc_out, declarations ); 
	_cc_out << "," 
		<< NL() << "currentSignal,"
		<< CS("fanOutInfo);");
	index++;
      }
      else if ( mode == IIR_OUT_MODE ){
	first = 1;
	_cc_out << OS("currentSignal = checkAdd(");
	localMap[portelement->_get_declarator()->get_string()]->
                        _publish_cc_object_name( _cc_out, declarations ); 
	_cc_out << "," << NL()
		<< "currentSignal,"
		<< CS("fanOutInfo);");
	index++;
      } 
      else if ( mode != IIR_IN_MODE ){
	cerr << "TyvisComponentDeclaration::_publish_cc_connect Invalid mode in the port " 
	     << endl;
	abort();
      }
    }
    portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(portClause->successor(portelement));
  }
  _cc_out << CS("}");
  
  //###Assumes binding entity and component has same # of o/io ports
  
  _cc_out << OS("if ( boundedEntity != NULL ){");
  _cc_out << OS("boundedEntity->connect(inputsignals, outputsignals");
  if(first == 1) {
    portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(portClause->first());
    for (; portelement != NULL; ) {
      CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_connect" );
      mode = portelement->get_mode();
      if ((mode == IIR_IN_MODE )) {
	_cc_out << "," << NL() << "dynamic_cast<RValue *>(&";
	localMap[portelement->_get_declarator()->get_string()]->
                        _publish_cc_object_name( _cc_out, declarations );
        _cc_out << ")";
      }
      portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(portClause->successor(portelement));
    }
    portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(portClause->first());
    for (; portelement != NULL; ) {
      mode = portelement->get_mode();
      if ((mode == IIR_OUT_MODE ) || (mode == IIR_INOUT_MODE)) {
	_cc_out << "," << NL() << "dynamic_cast<RValue *>(&";
	localMap[portelement->_get_declarator()->get_string()]->
                        _publish_cc_object_name( _cc_out, declarations );
        _cc_out << ")";
      }
      portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(portClause->successor(portelement));
    }
  }
  _cc_out << CS(");");
  _cc_out << CS("}");
  _cc_out << CS("}");
}

void
TyvisComponentDeclaration::_publish_cc_getboundentityinfo( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_getboundentityinfo" );

  _cc_out << "void" << NL()
	  << OS( _get_cc_elaboration_class_name() +
		 "::buildSignalTree(){")
	  << OS("if (boundedEntity != NULL) {")
	  << "boundedEntity->buildSignalTree();" << NL()
	  << CS("}")
	  << CS("}");
}

void
TyvisComponentDeclaration::_publish_cc_include( published_file &_cc_out ){
  CC_REF( _cc_out, 
		"TyvisComponentDeclaration::_publish_cc_include( published_file &_cc_out )");
  const string filename = _get_cc_elaboration_class_name() + ".hh";
  Tyvis::_publish_cc_include(_cc_out, filename );
}

void
TyvisComponentDeclaration::_publish_cc_ams_connect_terminals(published_file &_cc_out, PublishData *declarations) {
  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_connect_terminals" );
  ostringstream objectname;
  TyvisInterfaceDeclaration *portelement = NULL;
  int terminal_index = 0;
  _cc_out << "void\n";
  this->_publish_cc_binding_name(_cc_out.get_stream());
  _cc_out << "_elab::connectTerminals(int numberOfTerminals, ...) {\n";
  _cc_out << "va_list ap;\n";
  _cc_out << "terminalInfo = (AMSType**)new "
          << "char[numberOfTerminals * sizeof(AMSType*)];\n";
  _cc_out << "va_start(ap, numberOfTerminals);\n";
  _cc_out << "for(int i=0; i <numberOfTerminals; i++) {\n";
  _cc_out << "  terminalInfo[i] = va_arg(ap, AMSType*);\n";
  _cc_out << "}\n";
  
  _cc_out << "char *labelInfo = va_arg(ap, char *);\n" ;
  _cc_out << "va_end(ap);\n";     
  
  for(portelement = dynamic_cast<TyvisInterfaceDeclaration *>(get_local_port_clause()->first()); 
      portelement != NULL;
      portelement = dynamic_cast<TyvisInterfaceDeclaration *>(get_local_port_clause()->successor(portelement)) ) {
  
    switch(portelement->get_kind()) {
      case IIR_TERMINAL_INTERFACE_DECLARATION: {  
        _cc_out << "  setTerminalInfo(";
        (dynamic_cast<TyvisTerminalInterfaceDeclaration *>(portelement))->_publish_cc_lvalue(_cc_out, declarations);
        _cc_out << " , *(terminalInfo[" << terminal_index << "]));\n";
        terminal_index++;
      }
      break;
      default:
        break;
    } 
  }
  _cc_out << "boundedEntity->connectTerminals(numberOfTerminals";
  
  for(portelement = dynamic_cast<TyvisInterfaceDeclaration *>(get_local_port_clause()->first()); 
      portelement != NULL;
      portelement = dynamic_cast<TyvisInterfaceDeclaration *>(get_local_port_clause()->successor(portelement)) ) {
  
    switch(portelement->get_kind()) {
    case IIR_TERMINAL_INTERFACE_DECLARATION: {
      _cc_out << ", &";
     (dynamic_cast<TyvisTerminalInterfaceDeclaration *>(portelement))->_publish_cc_lvalue(_cc_out, declarations);
    }
    break;
    default:
      break;
    }
  }
  _cc_out << ", labelInfo);\n";
  _cc_out << "}\n";
}

void  
TyvisComponentDeclaration::_publish_cc_ams_form_characteristic_expressions(published_file &_cc_out) {
  CC_REF( _cc_out, "TyvisComponentDeclaration::_publish_cc_ams_form_characteristic_expressions" );
  _cc_out << "void\n";
  this->_publish_cc_binding_name(_cc_out.get_stream());
  _cc_out << "_elab::formCharacteristicExpressions() {\n";
  _cc_out << "  boundedEntity->formCharacteristicExpressions();\n}\n";
}

TyvisGenericList *
TyvisComponentDeclaration::_get_generic_list(){
  return dynamic_cast<TyvisGenericList *>(get_local_generic_clause());
}

TyvisPortList *
TyvisComponentDeclaration::_get_entity_port_clause() {
  if (!get_entity())
    return NULL;
  return dynamic_cast<TyvisPortList *>(get_entity()->get_port_clause());
}

TyvisPortList *
TyvisComponentDeclaration::_get_port_list(){
  return dynamic_cast<TyvisPortList *>(get_local_port_clause());
}

TyvisGenericList *
TyvisComponentDeclaration::_get_local_generic_clause() {
  return dynamic_cast<TyvisGenericList *>(get_local_generic_clause());
}

TyvisPortList *
TyvisComponentDeclaration::_get_local_port_clause() {
  return dynamic_cast<TyvisPortList *>(get_local_port_clause());
}

TyvisAttributeSpecificationList*
TyvisComponentDeclaration::_get_attribute_specification_list(){
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

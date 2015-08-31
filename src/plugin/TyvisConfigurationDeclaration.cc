
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

#include "TyvisArchitectureStatementList.hh"
#include "TyvisConfigurationDeclaration.hh"
#include "TyvisBlockConfiguration.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisGenericList.hh"
#include "TyvisIdentifier.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisPortList.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"

TyvisConfigurationDeclaration::TyvisConfigurationDeclaration() {
  set_configuration_declarative_part(new TyvisDeclarationList());
}

TyvisConfigurationDeclaration::~TyvisConfigurationDeclaration() {
  //Release the list memory
  delete get_configuration_declarative_part();
}

void
TyvisConfigurationDeclaration::_publish_cc_elaborate(){
  _set_currently_publishing_unit(CONFIGURATION_DECLARATION);
  _set_current_entity_name( _get_entity()->_get_mangled_declarator()->convert_to_string() );
  _set_current_configuration_name( _get_mangled_declarator()->convert_to_string() );
  _set_current_architecture_name( _get_configured_architecture()->_get_mangled_declarator()->convert_to_string() );
  _publish_cc_headerfile(_declarations);
  _publish_cc_ccfile(_declarations);
}

void
TyvisConfigurationDeclaration::_publish_cc_headerfile(PublishData *declarations) {
  const string filename = _get_cc_elaboration_class_name();
  //Publishing the .hh file
  published_header_file header_file( _get_library_directory(), 
				     filename,
				     this );
  Tyvis::_publish_cc_include( header_file, "tyvis/STDTypes.hh" );

  CC_REF( header_file, "TyvisConfigurationDeclaration::_publish_cc_headerfile" );

  header_file << "// Header files needed for this architecture\n";
  _publish_cc_headers( header_file );
  header_file << "\n// Forward reference classes needed for this architecture\n";

  _publish_cc_class_includes( header_file );
  header_file << "\n// Elaboration Class for the this architecture\n";
  _publish_cc_class( header_file, declarations );
}

void 
TyvisConfigurationDeclaration::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_headers" );

  _get_entity()->_publish_cc_include_decls( _cc_out );
  _get_block_configuration()->_publish_cc_headers( _cc_out );
  _get_configuration_declarative_part()->_publish_cc_headers( _cc_out );
}

void 
TyvisConfigurationDeclaration::_publish_cc_class_includes( published_file & ) {}

void
TyvisConfigurationDeclaration::_publish_cc_class( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_class" );

  _cc_out << "class " 
	  << _get_cc_elaboration_class_name() 
	  << " : public _savant_entity_elab {\n"
	  << "public:\n";

  //Publishing the constructor and destructor
  _cc_out << _get_cc_elaboration_class_name() << "();\n";

  TyvisEntityDeclaration* entitydecl = _get_entity();
  if(entitydecl->get_generic_clause()->size() > 0) {
    _cc_out << _get_cc_elaboration_class_name() << "(\n";
    entitydecl->_get_generic_clause()->_publish_generic_parameter_list( _cc_out, declarations );
    _cc_out << " );\n";
  }

  _cc_out << "~" << _get_cc_elaboration_class_name() << "();\n\n"
	  << "void instantiate(Hierarchy * hier, const string parent_base, const char *local_name);\n"
          << "std::string _base;\n"
          << "std::string get_base() {return(_base);}\n"
	  << "void createNetInfo();\n"
	  << "void connect(int, int, ...);\n"
	  << "void buildSignalTree();\n"
	  << "void partition() {}\n";

  _get_entity()->_get_generic_clause()->_publish_cc_elaborate( _cc_out, declarations );

  //Publishing the pointers to processes and objects used in this architecture
  _publish_cc_object_pointers( _cc_out );

  _cc_out << "};\n";
}

void 
TyvisConfigurationDeclaration::_publish_cc_object_pointers( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_object_pointers" );

  _cc_out << _get_configured_architecture()->_get_cc_elaboration_class_name()
	  << "* configuredEntity;\n";
}

void 
TyvisConfigurationDeclaration::_publish_cc_ccfile(PublishData *declarations){
  const string filename = _get_cc_elaboration_class_name();

  published_cc_file cc_file( _get_library_directory(), 
			     filename,
			     this );

  CC_REF( cc_file, "TyvisConfigurationDeclaration::_publish_cc_ccfile" );

  _publish_cc_include_elab( cc_file );
  _publish_cc_headerfiles_for_cc( cc_file );
  cc_file << "extern VHDLKernel *proc_array[];\n";
  _publish_cc_constructor( cc_file, declarations );
  _publish_cc_destructor( cc_file );
  _publish_cc_instantiate( cc_file );
  _publish_cc_createNetInfo( cc_file, declarations );
  _publish_cc_connect( cc_file, declarations );
  _publish_cc_getboundentityinfo( cc_file );
}

void
TyvisConfigurationDeclaration::_publish_cc_constructor( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_constructor" );

  _publish_cc_constructor_with_no_arguments( _cc_out, declarations );
  if (get_entity()->get_generic_clause()->size() > 0) {
    _publish_cc_constructor_with_arguments( _cc_out, declarations );
  }
}

void 
TyvisConfigurationDeclaration::_publish_cc_constructor_with_no_arguments( published_file &_cc_out, PublishData *declarations ) {
  int numGenericClause = get_entity()->get_generic_clause()->size();

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_constructor_with_no_arguments" );
  
  _cc_out << _get_cc_elaboration_class_name() << "::"
	  << _get_cc_elaboration_class_name() << "()";

  if( numGenericClause > 0 ){
    _cc_out << ":\n";
    _get_entity()->_get_generic_clause()->_publish_generic_init( _cc_out, declarations );
  }
  _cc_out << " {\n";
  _publish_cc_object_pointers_init( _cc_out, declarations );
  _cc_out << "}\n";
}

void
TyvisConfigurationDeclaration::_publish_cc_constructor_with_arguments( published_file &_cc_out, PublishData *declarations ) {
  int numGenericClause = get_entity()->get_generic_clause()->size();

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_constructor_with_arguments" );

  _cc_out << _get_cc_elaboration_class_name() << "::"
	  << _get_cc_elaboration_class_name() << "(\n";
  _get_entity()->_get_generic_clause()->_publish_generic_parameter_list( _cc_out, declarations );
  _cc_out << " )";

  if(numGenericClause > 0) {
    _cc_out << ":\n";
    _get_entity()->_get_generic_clause()->_publish_generic_init_by_arguments( _cc_out, declarations );
  }
  _cc_out << " {\n";
  _publish_cc_object_pointers_init( _cc_out, declarations );
  _cc_out << "}\n";
}

void
TyvisConfigurationDeclaration::_publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_object_pointers_init" );

  _cc_out << "configuredEntity = new "
	  << _get_configured_architecture()->_get_cc_elaboration_class_name() << "();\n";
  _get_block_configuration()->_publish_cc_object_pointers_init( _cc_out, declarations );
}

void
TyvisConfigurationDeclaration::_publish_cc_destructor( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_destructor" );

  _cc_out << _get_cc_elaboration_class_name()
	  << "::~"
	  << _get_cc_elaboration_class_name()
	  << "() {\n"
	  << "   delete configuredEntity;\n"
	  << "}\n";
}

void
TyvisConfigurationDeclaration::_publish_cc_instantiate( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_instantiate" );

  _cc_out << "void\n"
	  << _get_cc_elaboration_class_name()
    	  << "::instantiate(Hierarchy * hier, const string parent_base, const char *local_name)  {\n"
          << "  _base = hier->addFrame(parent_base, local_name);\n"
	  << "  if(configuredEntity != NULL) {\n"
	  << "    configuredEntity->instantiate(hier, _base, \"\");\n"
	  << "  }\n"
	  << "}\n";
}

void
TyvisConfigurationDeclaration::_publish_cc_createNetInfo( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_createNetInfo" );

  _cc_out << "void\n"
	  << _get_cc_elaboration_class_name()
	  << "::createNetInfo() {\n"
	  << "  if(configuredEntity != NULL) {"
	  << "    configuredEntity->createNetInfo();\n"
	  << "  }\n"
	  << "}\n";
}

void
TyvisConfigurationDeclaration::_publish_cc_connect( published_file &_cc_out,
						    PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_connect" );
  Tyvis::_publish_cc_include( _cc_out, "cstdarg", true );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/SignalNetinfo.hh" );
  _cc_out << "void" << NL()
	  << _get_cc_elaboration_class_name()
	  << OS("::connect(int inputsignals, int outputsignals, ...) {")
	  << "int noOfSignals = inputsignals + outputsignals;" << NL()
	  << "va_list ap;" << NL()
	  << "va_start(ap, outputsignals);" << NL()
	  << OS("for( int i=0; i <noOfSignals; i++ ){")
	  << "  addToFanOut( va_arg(ap, RValue*) );" << NL()
	  << CS("}")
	  << "va_end(ap);" << NL();
  
  //Pass on the output connection  inforamtion to its output signals
  TyvisEntityDeclaration* entitydecl = _get_entity();
  TyvisInterfaceDeclaration* portelement = NULL;
  IIR_Mode mode;
  int index = 0;

  _cc_out << OS("if( inputsignals > 0 ){") << NL();

  portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(entitydecl->get_port_clause()->first());
  while( portelement != NULL ){
    mode = portelement->get_mode();
    if(mode == IIR_IN_MODE ) {
      _cc_out << "setSourceInfo( configuredEntity->";
      portelement->_publish_cc_elaborate( _cc_out, declarations ); 
      _cc_out << "," << NL()
	      << "*(fanOutInfo[";
      _cc_out << index << "]) );" << NL();
      index++;
    }
    portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(entitydecl->get_port_clause()->successor(portelement));
  }
  _cc_out << CS("}") << NL();

  _cc_out << OS("if( outputsignals > 0 ){");

  portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(entitydecl->get_port_clause()->first());
  for(; portelement != NULL; ) {
    mode = portelement->get_mode();
// There may be a logic problem in this area SDW 3-13-2008
    if(mode == IIR_INOUT_MODE || mode == IIR_OUT_MODE) {
      _cc_out << "setSourceInfo( configuredEntity->";
      portelement->_publish_cc_elaborate( _cc_out, declarations ); 
      _cc_out << "," << NL()
	      << "*( fanOutInfo["
	      << index << "] ));" << NL();
      
      if (mode != IIR_OUT_MODE){
	_cc_out << "setSourceInfo( ";
	_cc_out << "configuredEntity->";
	portelement->_publish_cc_elaborate( _cc_out, declarations ); 
	_cc_out << "," << NL() 
		<< "*(fanOutInfo["
		<< index << "]));" << NL();
      }
      index++;
    }
    portelement = dynamic_cast<TyvisSignalInterfaceDeclaration *>(entitydecl->get_port_clause()->successor(portelement));
  }
  _cc_out << CS("}")
	  << "configuredEntity->connect(0,0);" << NL()
	  << CS("}");
}

void
TyvisConfigurationDeclaration::_publish_cc_getboundentityinfo( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_getboundentityinfo" );

  _cc_out << "void\n"
	  << _get_cc_elaboration_class_name()
	  << "::buildSignalTree() {\n"
	  << "  if(configuredEntity != NULL) {\n"
	  << "    configuredEntity->buildSignalTree();\n"
	  << "  }\n"
	  << "}\n";
}

void
TyvisConfigurationDeclaration::_publish_cc_binding_name( ostream& outstream ){
  //  CC_REF( _cc_out, "TyvisConfigurationDeclaration::_publish_cc_binding_name" );
  outstream << "SCFG" << *(_get_mangled_declarator()) << "_" << *(_get_entity()->_get_mangled_declarator())
	    << "_" << *(_get_configured_architecture()->_get_mangled_declarator());
}

void 
TyvisConfigurationDeclaration::_publish_cc_include_decls_prefix( ostream &os ){
  os << *(_get_mangled_declarator()) << "Cfg";
}

TyvisBlockConfiguration *
TyvisConfigurationDeclaration::_get_block_configuration() {
  return dynamic_cast<TyvisBlockConfiguration *>(get_block_configuration());
}

TyvisDeclarationList *
TyvisConfigurationDeclaration::_get_configuration_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>(get_configuration_declarative_part());
}

TyvisEntityDeclaration *
TyvisConfigurationDeclaration::_get_entity() {
  return dynamic_cast<TyvisEntityDeclaration *>(get_entity());
}

TyvisArchitectureDeclaration*
TyvisConfigurationDeclaration::_get_configured_architecture() {
  Tyvis* tmp_node = _get_block_configuration()->_get_block_specification();
  ASSERT(tmp_node->get_kind() == IIR_ARCHITECTURE_DECLARATION);
  TyvisArchitectureDeclaration* arch_ptr = (dynamic_cast<TyvisArchitectureDeclaration*>(tmp_node));  
  return arch_ptr;
}

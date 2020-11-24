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

#include "TyvisAboveAttribute.hh"
#include "TyvisAliasDeclaration.hh"
#include "TyvisBreakElement.hh"
#include "TyvisBreakList.hh"
#include "TyvisBreakStatement.hh"
#include "TyvisBlockStatement.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFileDeclaration.hh"
#include "TyvisFloatingPointLiteral.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIndexedName.hh"
#include "TyvisLabel.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisMonadicOperator.hh"
#include "TyvisProcedureCallStatement.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSelectedName.hh"
#include "TyvisSelectedNameByAll.hh"
#include "TyvisSequentialStatement.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSimpleName.hh"
#include "TyvisSliceName.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisVariableDeclaration.hh"
#include "TyvisWaitStatement.hh"
#include "PublishData.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
// From savant
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "savant/string_utils.hh"
#include "savant/language_processing_control.hh"
#include "savant/set.hh"
#include <sstream>

extern language_processing_control *lang_proc;

TyvisProcessStatement::TyvisProcessStatement() :
  declaration_collection( new PublishData() ){}

TyvisProcessStatement::~TyvisProcessStatement() { 
  delete declaration_collection;
  declaration_collection = 0;
}

void 
TyvisProcessStatement::_publish_cc( published_file &, PublishData * ){
  PublishData *_declarations = new PublishData();

  _set_current_process_statement(dynamic_cast<TyvisProcessStatement*>(this));
  
  _publish_cc_decls_header( _declarations );
  _publish_cc_decls_body( _declarations ); 
  _build_wait_list();
  _transmute_above_attributes();
  _publish_cc_process_header( _declarations );
  _publish_cc_process_cc_file( _declarations );
  _publish_cc_state( _declarations );

  _set_current_process_statement(NULL);
}

void
TyvisProcessStatement::_publish_cc_decls_header( PublishData *declarations ){
  // Publish the required stuff in the declarative part of the process.
  published_header_file process_decls_header_file( _get_work_library()->get_path_to_directory(),
						   _get_cc_process_class() + "_decls",
						   this );
  
  CC_REF( process_decls_header_file, 
          "TyvisProcessStatement::_publish_cc_decls_header()" );
  // Include the declarative region to which this process belongs
  
  Tyvis::_publish_cc_include( process_decls_header_file, _get_design_unit_name() + ".hh" );

  // Include all the stuff in the declarative part of process in this file
  get_process_declarative_part()->_publish_cc( process_decls_header_file, declarations );
}

void
TyvisProcessStatement::_publish_cc_decls_body( PublishData *declarations ){
  published_cc_file process_decls_cc_file( _get_work_library()->get_path_to_directory(),
					   _get_cc_process_class() + "_decls",
					   this );
  CC_REF( process_decls_cc_file, 
          "TyvisProcessStatement::_publish_cc_decls_body()" );

  Tyvis::_publish_cc_include( process_decls_cc_file, _get_current_entity_name() + ".hh" );
  _get_mangled_label()->_publish_cc_include( process_decls_cc_file );
  get_process_declarative_part()->_publish_cc_decl( process_decls_cc_file, declarations );
}

void
TyvisProcessStatement::_transmute_above_attributes(){
  IIR_Boolean _above_attribute_in_process =
    get_process_statement_part()->is_above_attribute_found();
  IIR_Boolean _above_attribute_in_wait_stmts = FALSE;
  TyvisAboveAttribute *current_above_attribute = NULL;

  TyvisSequentialStatement *stmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
  while ( stmt != NULL ) {
    stmt->_build_above_attribute_set(&_above_attribute_set);//VHDL AMS addition - jkm
    stmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(stmt));
  }

  TyvisWaitStatement *wait = _wait_stmt_list.first();
  while(wait != NULL) {
    _above_attribute_in_wait_stmts = _above_attribute_in_wait_stmts || wait->is_above_attribute_found();
    wait = _wait_stmt_list.successor(wait);
  }
  if( _above_attribute_in_wait_stmts == FALSE &&
      _above_attribute_in_process == TRUE) {
    current_above_attribute = _above_attribute_set.getElement();
    while(current_above_attribute != NULL) {
      wait = new TyvisWaitStatement();
      wait->wait_id = 0;
      TyvisDesignatorExplicit *current_designator = new TyvisDesignatorExplicit();
      current_designator->set_name(dynamic_cast<TyvisAttribute*>(current_above_attribute)->
				   _get_implicit_declaration());
      wait->get_sensitivity_list()->append(current_designator);
      _wait_stmt_list.append(wait);
      get_process_statement_part()->append(wait);
      current_above_attribute = _above_attribute_set.getNextElement();
    }
  }
  wait = NULL;
}

void
TyvisProcessStatement::_publish_cc_process_header( PublishData *declarations ){
  // generate code for the <process>.hh file
  published_header_file _cc_out( _get_work_library()->get_path_to_directory(),
                                 _get_cc_process_class(),
                                 this );
  CC_REF( _cc_out,
          "TyvisProcessStatement::_publish_cc_process_header()" );
  
  Tyvis::_publish_cc_include( _cc_out, "tyvis/VHDLProcess.hh" );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/STDTypes.hh" );
  _publish_cc_include_state( _cc_out );

  //redefine savantnow so that any initializations through savant now is 
  //performed
  _publish_cc_savantnow_no_process_ptr( _cc_out );
  
  _cc_out << "class " << _get_design_unit_name() << ";" << NL();
  
  _publish_cc_extern_declarations( _cc_out, declarations );
  _cc_out << "class " << _get_cc_process_class()
		      << OS( " : public VHDLProcess {" )
		      << "public:" << NL()
		      << _get_cc_process_class() << "( _savant_entity_elab *proc );" << NL()
		      << "~" << _get_cc_process_class() << "();" << NL()
		      << "void executeVHDL();" << NL()
		      << "State *allocateState();" << NL()
		      << "void initialize();" << NL();
  _cc_out.insert_comment( "type_info structures for types declared within this process" );

  get_process_declarative_part()->_publish_cc_constant_declarations( _cc_out, declarations );
  
  // End the .hh file later after searching for guard signal
  //  -----** This is done for guard signals
  CC_REF( _cc_out,
          "TyvisProcessStatement::_publish_cc_process_header()" );
  // This means the process is using the implicit guard signal
  _cc_out << "RValue *getGuard();" << NL();
  _cc_out << "RValue *getGuardExpression();" << NL();
  //  ------**
  
  _cc_out << CS( "};" );
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_process_header()" );
  _publish_cc_static_type_info_fn( _cc_out, declarations );
}


void
TyvisProcessStatement::_publish_cc_process_cc_file(PublishData *declarations){
  // generate code for the <process>.cc file
  published_cc_file _cc_out( _get_work_library()->get_path_to_directory(),
			     _get_cc_process_class(),
			     this );
  CC_REF( _cc_out,
          "TyvisProcessStatement::_publish_cc__cc_out()" );

  _publish_cc_include( _cc_out );
  if (lang_proc->processing_vhdl_ams()) {
    _cc_out << NL();
    _cc_out <<"extern int breakflag; "<< NL();
    _cc_out <<"extern int qsflag; "<< NL();
  }
  PublishedUnit _saved_publishing_unit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(PROCESS);

  // executeVHDL
  _cc_out << "void " << _get_cc_process_class(); 
  _cc_out << OS( "::executeVHDL() {" );
  _cc_out << "VHDLProcess *processPtr = this;";
  _cc_out << "\n#ifdef VHDLDBG\n"
	  << "  cout << getName() << \" executing VHDL at \" << getTimeNow() << endl;\n"
	  << "#endif" << NL();

  _cc_out << _get_cc_state_class() << " *currentState = static_cast<"
	  << _get_cc_state_class() <<" *>(getState());" << NL();

  get_process_statement_part()->_publish_cc_decl_for_loop_iterator( _cc_out, declarations );
  TyvisWaitStatement *wait = _wait_stmt_list.first();
  while( wait != 0 ){
    _cc_out << "if (currentState->waitLabel == "
	    << wait->wait_id 
	    << ") goto ";
    wait->_publish_cc_wait_label( _cc_out, declarations );
    _cc_out << ";" << NL();
    wait = _wait_stmt_list.successor(wait);
  }

  TyvisProcedureCallStatement *proc = 0;
  dl_list<TyvisProcedureCallStatement> _proc_stmt_list;
  get_process_statement_part()->_build_procedure_call_stmt_list(&_proc_stmt_list);
  // "goto"s for procedure call statements in the process.
  _cc_out << "if(currentState->waitLabel == ";
  _cc_out << OS( "WAITING_IN_PROC) {" );
  _cc_out << "currentState->stack.setCurrentToTop();" << NL();
  for (proc = _proc_stmt_list.first(); proc != NULL; 
       proc = _proc_stmt_list.successor(proc))  {
    _cc_out << "if(currentState->stack.getCurrentTop()->waitLabel == "
	    << proc << ") goto ";
    proc->_publish_cc_proc_label( _cc_out, declarations );
    _cc_out << ";" << NL();
  } // for
  _cc_out << CS( "}" );

  for (TyvisSequentialStatement* seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
       seqstmt != NULL;
       seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(seqstmt))) {
    ostringstream breakstr;
    TyvisIdentifier *new_identifier;
    if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
      ostringstream breakstringstream;
      breakstringstream << _get_cc_process_class() << "_breakSignal" ;
      string breakstring = breakstringstream.str();
      int    strlength   = breakstring.size();
      char  *breaklabel  = new char[strlength];
      strcpy(breaklabel,breakstring.c_str());
      new_identifier = dynamic_cast<TyvisIdentifier *>(IIRBase_Identifier::get( breaklabel, strlen(breaklabel), get_design_file()->get_class_factory() ));
      seqstmt->_set_process_stmt_label(new TyvisLabel);
      seqstmt->_get_process_stmt_label()->set_declarator(new_identifier);
    }
  }
  _cc_out << OS( "while(true){" );
  //const string prefix( "currentState->" );
  const string prefix( "" );
  _set_publish_prefix_string( prefix );
  
  get_process_statement_part()->_publish_cc( _cc_out, declarations );
  _set_publish_prefix_string( "" );
  
  _cc_out << CS( "} // end primary while loop" );
  _cc_out << CS( "} // end execute_VHDL" );
  // This piece of code is very tricky it has to come only after 
  // the publish_cc is called on the process statement part
  TyvisDeclaration *guardSignal = NULL;
  if (TyvisBlockStatement::guard_expr != NULL) {
    // This means an implicit guard signal exists in this scope
    // If the signal is used in this process then get it
    std::set<IIR_Declaration *> *decl_set = declarations->get_set(IIR_Declaration::SIGNAL);
    for (std::set<IIR_Declaration *>::iterator iter = decl_set->begin();
         iter != decl_set->end();
         iter++) {
      if (!IIRBase_TextLiteral::cmp((*iter)->get_declarator(), "guard")) {
        guardSignal = dynamic_cast<TyvisDeclaration *>(*iter);
	break;
      }
    }
  }

  // Back to publishing code in the cc file
  CC_REF( _cc_out,
          "TyvisProcessStatement::_publish_cc_process_cc_file()" );

  // <process> destructor
  _cc_out << _get_cc_process_class() << "::~" << _get_cc_process_class() << "() {}" 
	  << NL();

  // find out the number of things declared & used in this scope.

  // <process> constructor
  _cc_out << _get_cc_process_class() << "::" << _get_cc_process_class() 
	  << "( _savant_entity_elab *ptr)"
	  << ": VHDLProcess("
	  << "\"" << _get_cc_process_class() << "\", ptr )";
  
  get_process_declarative_part()->_publish_cc_constant_definitions( _cc_out, FALSE );  
  
  _cc_out << OS("{");
  get_process_declarative_part()->_publish_cc_global_constants_assignments( _cc_out, declarations );
  _set_currently_publishing_unit(_saved_publishing_unit);
  
  // All the files are actually opened in initialize() function.
  _cc_out << CS("}");

  _cc_out << "State *" << NL()
	  << OS( _get_cc_process_class() + "::allocateState(){" )
	  << "getProc()->copyGenericsToGlobals();" << NL()
	  << "return new " << _get_cc_state_class() << "(this);" << NL()
	  << CS( "}" );
  
  Tyvis::_publish_cc_include( _cc_out, "tyvis/GlobalControl.hh" );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/VCD.hh" );
  _cc_out << "void\n" << _get_cc_process_class() << "::initialize(){" << NL();
  _cc_out << "if (GlobalControl::getVCD()) GlobalControl::getVCD()->setBase(getProc()->get_base());\n";
  _cc_out << "VHDLProcess::initialize();\n";
  _cc_out << "}" << NL();
  
  _set_publish_prefix_string( prefix );
  // This means the implicit guard signal is used in this process
  _cc_out << "RValue*\n" << _get_cc_process_class() << "::getGuard()" << OS("{");
  if (guardSignal != NULL) {
    _cc_out << "  " << _get_cc_state_class() << " *currentState = static_cast<"
	    << _get_cc_state_class() << " *>(getState());\n\n"
	    << "  return &(";
    
    guardSignal->_publish_cc_lvalue( _cc_out, declarations );
    _cc_out <<");";
  }
  else{
    _cc_out << "return 0;" << NL();
  }
  _cc_out << CS("}");

  _cc_out << "RValue*\n"  << _get_cc_process_class() << "::getGuardExpression()" << OS("{");
  if( TyvisBlockStatement::guard_expr != 0 ){
    _cc_out << _get_cc_state_class() << "*currentState = static_cast<"
	    << _get_cc_state_class() << " *>(getState());" << NL();
    
    _cc_out <<"return ";
    TyvisBlockStatement::guard_expr->_publish_cc_rvalue( _cc_out, declarations );
  }
  else{
    _cc_out << "return 0;" << NL();
  }
  _cc_out << CS("}");

  _set_publish_prefix_string( "" );
}


void
TyvisProcessStatement::_publish_cc_state(PublishData *declarations){
  //Reset the code-generation get_attributes() set during publishing any of the
  //previously encountered process statements
  _clear(declarations);

  //All objects to be defined in _sate.hh file should not
  //be printed with prefix "state.current->"
  //So setting the flag so all object and Interface declarations
  //print as per the value of that flag
  IIR_Boolean tmp_value = _get_publish_object_without_prefix();
  _set_publish_object_without_prefix(TRUE);

  published_header_file header_file( _get_work_library()->get_path_to_directory(),
				     _get_cc_state_class(),
				     this );

  CC_REF( header_file, 
	  "TyvisProcessStatement::_publish_cc_state()");

  PublishedUnit _saved_publishing_unit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(PROCESS_STATE);

  //The following function adds the declarations in initializations of
  //visible declarations
  // This routine is not ok -- Probably we can remove it altogether
  _add_declarations_in_initializations(declarations);


  _publish_cc_headers( header_file );
  _publish_cc_state_class( header_file, declarations );

  published_cc_file cc_file( _get_work_library()->get_path_to_directory(),
			     _get_cc_state_class(),
			     this );  
  CC_REF( cc_file,
		"TyvisProcessStatement::_publish_cc_state( )" );

  _publish_cc_include_state( cc_file );
  _publish_cc_include( cc_file );
  
  //redefine savantnow so that any initializations through savant now is 
  //performed
  _publish_cc_savantnow_process_ptr( cc_file );
  
  _publish_cc_constructor( cc_file, declarations );
  _publish_cc_initstate( cc_file, declarations );
  _publish_cc_cleanstate( cc_file );
  _publish_cc_locatesignal( cc_file, declarations );
  _publish_cc_operator_equalto( cc_file, declarations );
  _publish_cc_state_file_methods( cc_file, declarations );

  //Resetting the flag value to the original value
  _set_publish_object_without_prefix(tmp_value);
  _set_currently_publishing_unit(_saved_publishing_unit);
}

void
TyvisProcessStatement::_publish_cc_headers( published_file &_cc_out ) {
  const string design_unit_name = _get_design_unit_name();
  CC_REF( _cc_out, 
		"TyvisProcessStatement::_publish_cc_headers( published_file &_cc_out )" );

  Tyvis::_publish_cc_include( _cc_out, "tyvis/Wait.hh" );
  Tyvis::_publish_cc_include( _cc_out, "tyvis/AccessObject.hh" );
  Tyvis::_publish_cc_include( _cc_out, _get_cc_process_class() + "_decls.hh" );
}

const string
TyvisProcessStatement::_get_cc_binding_name(){
  ostringstream os;
  if( _get_mangled_label() != NULL ) {
    _get_mangled_label()->_publish_cc_elaborate( os );
  }
  else {
    os << "ANON_PROCESS" << this;
  }
  return os.str();
}

void 
TyvisProcessStatement::_publish_cc_binding_name( ostream &os ){
  os << _get_cc_binding_name();
}

void
TyvisProcessStatement::_publish_cc_extern_declarations( published_file &,
							PublishData * ){}

void
TyvisProcessStatement::_publish_cc_read_or_write( published_file &_cc_out,
                                                         const string &functionName,
                                                         const string &streamName,
                                                         PublishData *declarations){
  TyvisDeclaration *decl;
  // Publish copy operator for constants in this state.
  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::CONSTANT);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    switch (decl->_get_declarative_region()->get_kind()) {
    case IIR_PACKAGE_DECLARATION:
    case IIR_PACKAGE_BODY_DECLARATION:
    case IIR_ARCHITECTURE_DECLARATION:
    case IIR_PROCESS_STATEMENT:
      break;
    default:
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << "." << functionName << "(" << streamName;
      _cc_out << ");\n";
      break;
    }
  }
  
  // Publish copy operator for signals in this state.
  decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if (decl->is_implicit_declaration() == FALSE) {
      _cc_out << *decl->_get_mangled_declarator()
              << "." << functionName << "(" << streamName
              << ");\n";
      decl->_publish_cc_implicit_signal_attributes_read_or_write( _cc_out, 
								  functionName, 
								  streamName,
                                                                  declarations);
    }
  }
  
  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_get_mangled_declarator()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "." << functionName << "(" << streamName;
    _cc_out << ");\n";
    decl->_publish_cc_implicit_signal_attributes_read_or_write( _cc_out, 
								functionName, 
								streamName,
                                                                declarations);
  }
  
  // Publish copy operator for variables in this state.
  decl_set = declarations->get_set(TyvisDeclaration::VARIABLE);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if (decl->_get_subtype()->is_access_type() == FALSE) {
      decl->_get_mangled_declarator()->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << "." << functionName << "(" << streamName;
      _cc_out << ");\n";
    }
  }
  
  // Publish copy operator for interface variables in this state - removed.
  decl_set = declarations->get_set(TyvisDeclaration::ATTRIBUTE);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "." << functionName << "(" << streamName << ");\n";
  }
  
  _cc_out << "\n";
}

const string
TyvisProcessStatement::_get_cc_state_class() const {
  return _get_cc_process_class() + "_state";
}

void
TyvisProcessStatement::_publish_cc_state_class( published_file &_cc_out,
						PublishData *declarations ) {
  TyvisDeclaration *decl = NULL;
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_class( published_file & )" );

  Tyvis::_publish_cc_include( _cc_out, "tyvis/VHDLKernel_state.hh" );
  _cc_out << "class " << _get_cc_process_class() << ";" << NL()
	  << "class " << _get_cc_state_class() <<" : public VHDLKernel_state " << OS("{")
	  << "public:" << NL()
	  << _get_cc_state_class() << "(" << _get_cc_process_class() << "*);" << NL()
	  << "~" << _get_cc_state_class() << "(){}" << NL()
	  << "void initState(_savant_entity_elab *, VHDLKernel * );" << NL()
	  << "void cleanState();" << NL()
	  << _get_cc_state_class() << "& operator=(const " << _get_cc_state_class() << "&);" << NL()
	  << "void copyState(const State *rhs){" << NL()
	  << "  *this = *(static_cast<const " << _get_cc_state_class() << " *>(rhs));" << NL()
	  << "}" << NL()
	  << "SignalBase* locateSig(int);" << NL()
	  << "unsigned int getStateSize() const {" << NL()
	  << "  return sizeof(" << _get_cc_state_class() << ");" <<  NL()
	  << "}" << NL();
  
  //Constants are made to publish first because,
  //other types of objects can be initialized using these constants
  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::CONSTANT);
  std::set<IIR_Declaration*>::iterator iter = decl_set->begin();
  while( iter != decl_set->end() ){
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    switch (decl->_get_declarative_region()->get_kind()) {
    case IIR_PACKAGE_DECLARATION:
    case IIR_PACKAGE_BODY_DECLARATION:
    case IIR_ARCHITECTURE_DECLARATION:
    case IIR_PROCESS_STATEMENT:
      break;
    default:
      decl->_publish_cc_decl( _cc_out , declarations );
      break;
    }
    iter++;
  }
  
  decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->is_implicit_declaration()) {
      TyvisDeclaration *decl_prefix = decl->_get_signal_prefix();
      if(!declarations->in_collection(decl_prefix)) {
	declarations->add_declaration(decl_prefix);
      }
    }
    else {
      decl->_publish_cc_decl( _cc_out , declarations );
      //This function publishes all the declarations that are in scope.
      decl->_publish_cc_implicit_signal_attributes( _cc_out, declarations );
    }
    iter++;
  }

  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_decl( _cc_out , declarations );
    //This function publishes all the declarations that are in scope.
    decl->_publish_cc_implicit_signal_attributes( _cc_out, declarations );
    iter++;
  }

  decl_set = declarations->get_set(TyvisDeclaration::VARIABLE);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    // Don't publish variables that are declared inside a function, inside a process.
    // They should only be used within the function they are declared.
    // TODO: Why are these part of the set?
    if (decl->_get_declarative_region()->get_kind() != IIR_FUNCTION_DECLARATION) {
      decl->_publish_cc_decl( _cc_out , declarations );
    }
    iter++;
  }
  
  // interface variables are no longer handled here - removed code
  decl_set = declarations->get_set(TyvisDeclaration::ALIAS);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_decl( _cc_out , declarations );
    iter++;
  }

  decl_set = declarations->get_set(TyvisDeclaration::ATTRIBUTE);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_decl( _cc_out , declarations );
    iter++;
  }

  decl_set = declarations->get_set(TyvisDeclaration::S_FILE);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){
    CC_REF( _cc_out,
	    "TyvisProcessStatement::_publish_cc_state_class( published_file & )" );
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_decl( _cc_out , declarations );
    iter++;
  }
  
  decl_set = declarations->get_set(TyvisDeclaration::QUANTITY);
  iter = decl_set->begin();
  while( iter != decl_set->end() ) {        
    TyvisDeclaration *implied_declaration = dynamic_cast<TyvisDeclaration *>(*iter);;
    if(decl->_get_implicit_declarations() != NULL &&
       decl->_get_implicit_declarations()->size() != 0) {
      for(implied_declaration = decl->_get_implicit_declarations()->getElement();
          implied_declaration != NULL;
          implied_declaration = decl->_get_implicit_declarations()->getNextElement()) {
        if(implied_declaration->is_signal() == TRUE) {
          implied_declaration->_publish_cc_decl(_cc_out, declarations);
	  implied_declaration->_publish_cc_implicit_signal_attributes( _cc_out, declarations);
        }
      }
    }
    iter++;
  }

  TyvisSequentialStatement *seqstmt =
    dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
  while( seqstmt != NULL ){
    if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
      ASSERT(seqstmt->_get_process_stmt_label() != NULL);
      string signalname = seqstmt->_get_process_stmt_label()->_to_string();
      _cc_out << "EnumerationType " << signalname << ";" << NL();
    }
    seqstmt =
      dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(seqstmt));
  }
  _cc_out <<  NL()
	  << "const int numWaits;" << NL()
	  << CS("};");  
}

void
TyvisProcessStatement::_publish_cc_constructor( published_file &_cc_out,
						PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_constructor( )" );
  // generate <process>_state constructor
  _cc_out << _get_cc_state_class() << "::" << _get_cc_state_class() << "("
	  << _get_cc_process_class() << " *processPtr )" << OS(":");
   savant::set<TyvisDeclaration> busSignals;

  _publish_cc_state_objects_init( _cc_out, &busSignals, declarations );
  // signal names could go here for human readability--not required.
  _cc_out << "numWaits(" << _wait_stmt_list.size() << ")" << CS("") << OS("{");
  // If the signal is of kind bus then set it 
  TyvisDeclaration *decl = busSignals.getElement();
  while( decl != 0 ){
    _cc_out << "  ";
    decl->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out <<".setBusKind();" << NL();
    decl = busSignals.getNextElement();
  }

  _cc_out << "  wait = new Wait[" << _wait_stmt_list.size()
	  << "];" << NL();
  
  //For a composite Resolved signals, certain data members namely
  // compositeResolvedSignal, and parentCompositeType of the kernel Signal
  //has to be initialized. The following function publishes them.
  _publish_cc_composite_resolved_signal_init( _cc_out, declarations );

  _cc_out << CS("}");
}

void
TyvisProcessStatement::_publish_cc_composite_resolved_signal_init( published_file &_cc_out, PublishData *declarations ) {
  TyvisDeclaration *decl = NULL;

  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->_is_composite_resolved_signal() == TRUE) {
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ".setParentCompositeType(&";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ");" << NL();
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ".setCompositeResolvedSignal(true);" << NL();
    }
  }

  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->_is_composite_resolved_signal() == TRUE) {
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ".setParentCompositeType(&";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ");" << NL();
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ".setCompositeResolvedSignal(true);" << NL();
    }
  }
}

void
TyvisProcessStatement::_publish_cc_initstate( published_file &_cc_out,
					      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_initstate()" );
  const string old_current_publish_name = _get_current_publish_name();
  Tyvis *oldNode = _get_current_publish_node();
  // generate_state::initState
  _cc_out << "void" << NL() 
	  << _get_cc_state_class() << "::initState" << OS("(")
	  <<"_savant_entity_elab *ptr," << NL()
	  << "VHDLKernel *processPointer" << CS(")") << OS("{");
  // We need to check if _current_architecture_name is NULL or not since
  // process statements in entity decl will not have an arch. In other
  // places i.e. in arch decls, blocks etc, this will be set.
  
  const string elabString = _get_current_publish_name() + _get_design_unit_name();

  _cc_out << elabString << "* proc = (" << elabString << " *) ptr;" << NL()
	  << _get_cc_process_class() << " *processPtr = (" << _get_cc_process_class()
	  << " *) processPointer;" << NL()
	  << "VHDLKernel_state::initState(ptr, processPointer);" << NL();
  for (TyvisSequentialStatement* seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
       seqstmt != NULL;
       seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(seqstmt))) {
    ostringstream breakstr;
    const string design_unit_name = _get_design_unit_name();
    if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
      abort();
      Tyvis::_publish_cc_include( _cc_out, "tyvis/SignalNetinfo.hh" );
      ASSERT(seqstmt->_get_process_stmt_label() != NULL);
      breakstr << _get_cc_process_class() << "_breakSignal" ;
      string breakstring = breakstr.str();
      _cc_out << "{" << NL()
              << "Signal *signal = " << "Signal (this->" << breakstring << ");" << NL()
              << "SignalNetinfo *signal_info = " << NL()
              << "(SignalNetinfo*) (*(ScalarType*)&proc->"
              << breakstring << "_info).object;" << NL()
              << "signal->fanDest = signal_info->obj_ids;" << NL()
              << "signal->fanout = signal_info->fanout;" << NL()
              << "signal->id = signal_info->id;" << NL()
              << "signal->type = EXPLICIT;" << NL()
              << "signal->sensitive = true;" << NL()
              << "signal->numAttributes = 0;" << NL()
              << "signal->attributeList = NULL;" << NL()
              << "signal->source = signal_info->source;" << NL()
              << "signal->name =\"" << breakstring << "\";" << NL()
              << "}" << NL();
    }
  }
  _set_current_publish_name( _get_cc_process_class() );
  TyvisDeclaration *decl = NULL;
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_initstate()" );
  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::CONSTANT);
  std::set<IIR_Declaration*>::iterator iter = decl_set->begin();
  while( iter != decl_set->end() ){
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ( decl->_get_declarative_region()->get_kind() != IIR_PACKAGE_DECLARATION && decl->_get_declarative_region()->get_kind() != IIR_PACKAGE_BODY_DECLARATION ){
      
      decl->_publish_cc_init( _cc_out, declarations );
    }
    iter++;
  }
  
  // Publish intialization for Signals and Signal Interfaces
  // Need this stuff for scoping of signals.
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_initstate()" );

  decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  iter = decl_set->begin();
  while( iter != decl_set->end()) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    _set_current_publish_node( _get_declarative_region() );
    if ( decl->is_implicit_declaration() == FALSE &&
	 decl->_get_declarative_region() != NULL &&
	 dynamic_cast<TyvisSubprogramDeclaration *>(decl->_get_declarative_region()) == NULL ){
      decl->_publish_cc_init( _cc_out, declarations );
    }
    iter++;
  }

  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    _set_current_publish_node( _get_declarative_region() );
    if( decl->is_implicit_declaration() == FALSE &&
	decl->_get_declarative_region() != NULL &&
	dynamic_cast<TyvisSubprogramDeclaration *>(decl->_get_declarative_region()) == NULL ){
      decl->_publish_cc_init( _cc_out, declarations );
    }
    iter++;
  }
  _set_current_publish_node( oldNode );
  
  // Publish initialization for Variables and Variable Interfaces
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_initstate()" );
  decl_set = declarations->get_set(TyvisDeclaration::VARIABLE);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_init( _cc_out, declarations );
    iter++;
  }

  TyvisDeclaration *implied_decl = NULL;
  decl_set = declarations->get_set(TyvisDeclaration::QUANTITY);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->_get_implicit_declarations() != NULL &&
       decl->_get_implicit_declarations()->size() != 0) {
      for(implied_decl = decl->_get_implicit_declarations()->getElement();
          implied_decl != NULL;
          implied_decl = decl->_get_implicit_declarations()->getNextElement()) {

        if(implied_decl->is_signal() == TRUE) {
	  PublishedUnit _temp_publishing_unit = _get_currently_publishing_unit();
	  _set_currently_publishing_unit(ABOVE_ATTRIBUTE);
          implied_decl->_publish_cc_init(_cc_out, declarations);
	  _set_currently_publishing_unit(_temp_publishing_unit);
	}
      }
    }
    iter++;
  }

  _publish_cc_wait_init( _cc_out, declarations );
  _cc_out << CS("}");
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_initstate()" );

  _set_current_publish_name( old_current_publish_name );
}

void
TyvisProcessStatement::_publish_cc_state_file_methods( published_file &_cc_out,
						       PublishData *declarations ){
  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::S_FILE);
  std::set<IIR_Declaration*>::iterator iter = decl_set->begin();
  while( iter != decl_set->end() ){        
    TyvisFileDeclaration *decl = dynamic_cast<TyvisFileDeclaration *>(*iter);
    decl->_publish_cc_shared_file_decl( _cc_out, declarations, _get_cc_state_class() );
    iter++;
  }
}

void
TyvisProcessStatement::_publish_cc_cleanstate( published_file &_cc_out ) {
  // generate <process>_state::cleanState
  _cc_out << "void" << NL();
  _cc_out << _get_cc_state_class() << "::cleanState() {" << NL();
  _cc_out << "//" << " Hey!!! cleanup is done in the signal's destructor" << NL();
  for(int i=0; i < _wait_stmt_list.size(); i++){ 
    _cc_out << "wait[" << i <<"].destructSensitivityList();" << NL();
  }

  _cc_out << "}" << NL();
}

void
TyvisProcessStatement::_publish_cc_locatesignal( published_file &_cc_out, PublishData *declarations ) {
  TyvisDeclaration *decl;

  // generate <process>_state::locateSig
  _cc_out << "SignalBase*" << NL()
	  << _get_cc_state_class() << "::locateSig(int sigId)" << OS("{")
	  << "SignalBase *ptr = NULL;" << NL();

  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  std::set<IIR_Declaration*>::iterator iter = decl_set->begin();
  while( iter != decl_set->end() ){
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_locatesignal( _cc_out, declarations );
    iter++;
  }

  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  iter = decl_set->begin();
  while( iter != decl_set->end() ){
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_locatesignal( _cc_out, declarations );
    iter++;
  }
  TyvisSequentialStatement *seqstmt =
    dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
  while( seqstmt != NULL ){
    const string design_unit_name = _get_design_unit_name();
    if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
      ASSERT(seqstmt->_get_process_stmt_label() != NULL);
      string breakstring( _get_cc_process_class() + "_breakSignal" );
      _cc_out << "if( sigId == " << breakstring << ".getId() )" << NL()
	      << "  return (SignalBase*)(&" << breakstring << ");" << NL();
    }
    seqstmt =
      dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(seqstmt));
  }
  _cc_out  <<"return ptr;" << NL()
	   << CS("}");
}

void
TyvisProcessStatement::_publish_ccprint( published_file &_cc_out ) {
  // generate <process>_state::print
  _cc_out << "void" << NL();
  _cc_out << _get_cc_state_class() << "::print(ostream& os) {" << NL();
  _cc_out << "  os << this;\n}\n\n" << NL();
}


void
TyvisProcessStatement::_publish_cc_operator_equalto( published_file &_cc_out, PublishData *declarations ) {
  TyvisDeclaration *decl;

  // generate <process>_state::operator=
  _cc_out << _get_cc_state_class() + " &" << NL();
  _cc_out << OS(_get_cc_state_class() + "::operator=( const " + _get_cc_process_class() + "_state &s ) {");
  _cc_out << "VHDLKernel_state::operator=(s);" << NL();

  // Publish copy operator for constants in this state.
  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::CONSTANT);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    switch (decl->_get_declarative_region()->get_kind()) {
    case IIR_PACKAGE_DECLARATION:
    case IIR_PACKAGE_BODY_DECLARATION:
    case IIR_PROCESS_STATEMENT:
    case IIR_ARCHITECTURE_DECLARATION:
      break;
    case IIR_ENTITY_DECLARATION:
      // TyvisConstantDeclaration's publish_cc_lvalue publishes a function, instead of an object
      // for entity and architecture declarative regions
      _cc_out << "this->" << *decl->_get_mangled_declarator();
      _cc_out << " = s." << *decl->_get_mangled_declarator();
      _cc_out << ";" << NL();
      break;
    default:
      _cc_out << "this->";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << " = s.";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ";" << NL();
      break;
    }
  }
  
  _cc_out  << NL();
  
  // Publish copy operator for signals in this state.
  decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if( decl->is_implicit_declaration() == FALSE ){
      _cc_out << "this->"
              << *decl->_get_mangled_declarator()
              << " = s."
              << *decl->_get_mangled_declarator()
              << ";" << NL();
      decl->_publish_cc_implicit_signal_attributes_copying( _cc_out, declarations );
    }
  }
  
  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    _cc_out << "this->"
            << *decl->_get_mangled_declarator()
            << " = s."
            << *decl->_get_mangled_declarator()
            << ";" << NL();
    decl->_publish_cc_implicit_signal_attributes_copying( _cc_out, declarations );
  }
  
  // Publish copy operator for variables in this state.
  decl_set = declarations->get_set(TyvisDeclaration::VARIABLE);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    // Don't publish variables that are declared within a function within the process.
    // They should only be defined within the function itself.
    if (decl->_get_declarative_region()->get_kind() != IIR_FUNCTION_DECLARATION) {
      _cc_out << "this->"
	      << *decl->_get_mangled_declarator()
	      << " = s."
	      << *decl->_get_mangled_declarator()
	      << ";" << NL();
    }
  }

  // Publish copy operator for interface variables in this state - removed.
  decl_set = declarations->get_set(TyvisDeclaration::ALIAS);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    _cc_out << "this->"
            << *decl->_get_mangled_declarator()
            << " = s."
            << *decl->_get_mangled_declarator()
            << ";" << NL();
  }
  
  decl_set = declarations->get_set(TyvisDeclaration::ATTRIBUTE);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    _cc_out << "this->";
    decl->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << " = s.";
    decl->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << ";" << NL();
  }
  
  // Publish copy operator for interface constants in this state.
  // This is not necessary since we directly replace the constant with the
  // value of the constant.

  _cc_out << "for( int i = 0; i < numWaits; i++) wait[i] = s.wait[i];" << NL();
  _cc_out << "return *this;" << NL()
	  << CS("}\n");
}


void
TyvisProcessStatement::_publish_createNetInfo( published_file &_cc_out,
					       PublishData *declarations ){
  savant::set<Tyvis> input_signal_list;
  _get_list_of_input_signals(&input_signal_list);

  string objectname = ", " + _get_cc_process_class();
  if (_is_currently_publishing_generate_for()) {
    objectname += "_elab_obj[i - generateMin]);";
  }
  else {
    objectname += "_elab_obj);";
  }
  
  Tyvis *currentSignal = input_signal_list.getElement();
  ASSERT ( _get_declarative_region() != NULL );
  while( currentSignal != NULL ){
    CC_REF( _cc_out, "TyvisProcessStatement::_publish_createNetInfo( published_file &_cc_out, PublishData *declarations )" );
    currentSignal->_publish_cc_addToFanOut( _cc_out,
					    const_cast<TyvisProcessStatement *>(this),
					    declarations );
    currentSignal = input_signal_list.getNextElement();
  }
  TyvisSequentialStatement *seqstmt =
    dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
  while( seqstmt != NULL ){
    ostringstream breakstr;
    if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
      ASSERT(seqstmt->_get_process_stmt_label() != NULL);
      breakstr << _get_cc_process_class() << "_breakSignal" ;
      string breakstring = breakstr.str();
      _cc_out << OS("{") << NL()
	      << "VHDLType* ptr = new EnumerationLiteral(std_standard_booleanTypeInfo());"
	      << "addChild(" << breakstring << ", *(ptr)" << objectname
	      << CS("};");
    }
    seqstmt =
      dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(seqstmt));
  }
  const string _saved_current_publish_name = _get_current_publish_name();
  _set_current_publish_name( objectname );
  Tyvis *_saved_current_publish_node      = _get_current_publish_node();

   savant::set<Tyvis> signal_source_set;
  _get_signal_source_info(&signal_source_set);
  
  Tyvis* sig_decl = signal_source_set.getElement();
  while( sig_decl != NULL ){
    CC_REF( _cc_out, "TyvisProcessStatement::_publish_createNetInfo( published_file &_cc_out, PublishData *declarations )" );
    if ( dynamic_cast<TyvisDeclaration *>(sig_decl) != NULL || sig_decl->is_name() ) {
      sig_decl->_publish_cc_addChild( _cc_out, declarations );
    }
    sig_decl = signal_source_set.getNextElement();
  }
  _set_current_publish_name( _saved_current_publish_name );
  _set_current_publish_node( _saved_current_publish_node );
}

void
TyvisProcessStatement::_publish_cc_static_type_info_fn( published_file &_cc_out,
					      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_type_info" );
  get_process_declarative_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_type_info" );
  get_process_statement_part()->_publish_cc_static_type_info_fn( _cc_out, declarations );
}

void
TyvisProcessStatement::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  get_process_statement_part()->_get_signal_source_info(siginfo);
}

void
TyvisProcessStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  get_process_statement_part()->_get_list_of_input_signals(list);
}

void
TyvisProcessStatement::_publish_cc_implicit_declarations( published_file &_cc_out,
                                                                 TyvisDeclaration *decl,
                                                                 PublishData *declarations) {
  ASSERT(decl->is_implicit_declaration() == FALSE);
  if( decl->_get_implicit_declarations() != NULL ){
    TyvisDeclaration* implied_declarations = decl->_get_implicit_declarations()->getElement();
    while (implied_declarations != NULL) {
      if (declarations->in_collection(implied_declarations)) {
	implied_declarations->_publish_cc_decl( _cc_out , declarations );
	implied_declarations->_publish_cc_implicit_signal_attributes( _cc_out, declarations );
      }
      implied_declarations = decl->_get_implicit_declarations()->getNextElement();
    }
  }
}

void
TyvisProcessStatement::_publish_cc_state_objects_init( published_file &_cc_out,
                                                       savant::set<TyvisDeclaration> *busSigs,
                                                       PublishData *declarations ){
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_objects_init()" );
  // Publish inititalization for constant
  TyvisDeclaration *decl = NULL;
  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::CONSTANT);
  for ( std::set<IIR_Declaration*>::iterator iter = decl_set->begin();
	iter != decl_set->end();
	iter++) {        
    CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_objects_init()" );
    dynamic_cast<TyvisDeclaration *>(*iter)->_publish_cc_state_object_init( _cc_out,
									    declarations );
  }

  // Publish inititalization for signals
  decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  for( std::set<IIR_Declaration*>::iterator iter = decl_set->begin();
       iter != decl_set->end();
       iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->is_implicit_declaration() == FALSE) {
      CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_objects_init()" );
      decl->_publish_cc_state_object_init( _cc_out, declarations );
      decl->_publish_cc_implicit_state_objects_init( _cc_out, declarations );
      if (decl->get_kind() == IIR_SIGNAL_DECLARATION) {
	if ((dynamic_cast<TyvisSignalDeclaration*>(decl))->get_signal_kind() == IIR_BUS_KIND) {
	  busSigs->add(decl);
	}
      }
    }
  }

  // Publish inititalization for interface signals
  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->is_implicit_declaration() == FALSE) {
      CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_objects_init()" );
      decl->_publish_cc_state_object_init( _cc_out, declarations );
      decl->_publish_cc_implicit_state_objects_init( _cc_out, declarations );
    }
  }

  // Publish inititalization for variable
  decl_set = declarations->get_set(TyvisDeclaration::VARIABLE);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_objects_init()" );
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    // Don't publish variables that are declared within a function within the process.
    // They should only be defined within the function itself.
    if (decl->_get_declarative_region()->get_kind() != IIR_FUNCTION_DECLARATION) {
      decl->_publish_cc_state_object_init( _cc_out, declarations );
    }
  }

  // Publish inititalization for alias declarations
  decl_set = declarations->get_set(TyvisDeclaration::ALIAS);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_objects_init()" );
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_state_object_init( _cc_out, declarations );
  }

  decl_set = declarations->get_set(TyvisDeclaration::ATTRIBUTE);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_objects_init()" );
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_publish_cc_state_object_init( _cc_out, declarations );
  }

//   decl_set = declarations->get_set(TyvisDeclaration::QUANTITY);
//   for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
//     TyvisDeclaration *implied_declaration = dynamic_cast<TyvisDeclaration *>(*iter);
//     _cc_out << NL();
//     if(decl->_get_implicit_declarations() != NULL &&
//        decl->_get_implicit_declarations()->size() != 0) {
//       for(implied_declaration = decl->_get_implicit_declarations()->getElement();
//           implied_declaration != NULL;
//           implied_declaration = decl->_get_implicit_declarations()->getNextElement()) {
//         if(implied_declaration->_is_signal() == TRUE) {
// 	  CC_REF( _cc_out,"TyvisProcessStatement::_publish_cc_state_objects_init()" );
//           PublishedUnit _temp_publishing_unit = _get_currently_publishing_unit();
//           _set_currently_publishing_unit(ABOVE_ATTRIBUTE);
//           implied_declaration->_publish_cc_state_object_init(_cc_out, declarations);
//           _cc_out << "," << NL();
//           implied_declaration->_publish_cc_implicit_state_objects_init(_cc_out, declarations);
//   	  _set_currently_publishing_unit(_temp_publishing_unit);
//         }
//       }
//     }
//   }

  for (TyvisSequentialStatement* seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
       seqstmt != NULL;
       seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(seqstmt))) {
    ostringstream breakstr;
    if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
      CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_state_objects_init()" );
      ASSERT(seqstmt->_get_process_stmt_label() != NULL);
      _cc_out << _get_cc_process_class() << "_breakSignal(ObjectBase::SIGNAL)," << NL();
    }
  }
}

void
TyvisProcessStatement::_publish_cc_wait_init( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_wait_init" );
  TyvisWaitStatement *wait;
  IIR_Boolean sens_list_newed = FALSE; // Flag used to indicate that the
  // new'ed memory should be deleted.
  for (wait = _wait_stmt_list.first();
       wait != NULL;
       wait = _wait_stmt_list.successor(wait)) {
    if (wait->get_sensitivity_list()->size() == 0) { 
      if (wait->get_condition_clause() != NULL) {
	wait->_get_condition_clause()->_build_sensitivity_list(wait->_get_sensitivity_list());
	sens_list_newed = TRUE;
      }
    }
    if(wait->get_sensitivity_list()->size() != 0) {
      int sig = 0;
      Tyvis *sens_sig = NULL;
      TyvisDesignator *current_signal_designator = NULL;
      current_signal_designator = dynamic_cast<TyvisDesignator *>(wait->get_sensitivity_list()->first());
      while (current_signal_designator != NULL) {
	ASSERT(current_signal_designator->get_kind()==IIR_DESIGNATOR_EXPLICIT);
	sens_sig = dynamic_cast<Tyvis *>(dynamic_cast<TyvisDesignatorExplicit *>(current_signal_designator)->get_name());

	_cc_out << "wait[" << wait->wait_id <<"].addSensitivityTo( dynamic_cast<SignalBase *>(" 
		<< "&";
	sens_sig->_publish_cc_declarator( _cc_out, declarations );
	_cc_out << "));" << NL(); 
	current_signal_designator = dynamic_cast<TyvisDesignator *>(wait->get_sensitivity_list()->successor( current_signal_designator ));
	sig++;
      }	// while
      if(sens_list_newed == TRUE) {
	current_signal_designator = dynamic_cast<TyvisDesignator *>(wait->get_sensitivity_list()->first());
	while(current_signal_designator != NULL) {
	  TyvisDesignator *next =  dynamic_cast<TyvisDesignator *>(wait->get_sensitivity_list()->successor(current_signal_designator));
	  wait->get_sensitivity_list()->remove( current_signal_designator );
	  delete current_signal_designator;
	  current_signal_designator = next;
	} // while
      }	// if(sens_list_newed == TRUE)
    }
  } // for
  _cc_out  << NL();
}

void
TyvisProcessStatement::_add_declarations_in_initializations(PublishData *declarations) {
  TyvisDeclaration *decl;
  //Constants are made to publish first because,
  //other types of objects can be initialized using these constants
  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::CONSTANT);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if ( decl->_get_declarative_region()->get_kind() != IIR_PACKAGE_DECLARATION && decl->_get_declarative_region()->get_kind() != IIR_PACKAGE_BODY_DECLARATION ){
      
      decl->_add_declarations_in_initializations( declarations );
    }
  }
  
  decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    if(decl->is_implicit_declaration() == FALSE) {
      decl->_add_declarations_in_initializations( declarations );
    }
  }

  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_add_declarations_in_initializations( declarations );
  }

  decl_set = declarations->get_set(TyvisDeclaration::VARIABLE);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_add_declarations_in_initializations( declarations );
  }

  decl_set = declarations->get_set(TyvisDeclaration::ALIAS);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_add_declarations_in_initializations( declarations );
  }
}

void
TyvisProcessStatement::_build_wait_list() {
  // This core of this method will only execute if the wait statement list
  // has no elements.  It will have no elements if 1) the method hasn't
  // been called yet, or 2) it has been called, but the process has no
  // wait statements (which is bad VHDL, but should cause no problems
  // here).  This check is to make the method idempotent.
  if ( 0 == _wait_stmt_list.size() ) {
    TyvisSequentialStatement *stmt = NULL;
 
    stmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
    while ( stmt != NULL ) {
      stmt->_build_wait_list((dl_list<TyvisWaitStatement>*)&_wait_stmt_list);
      stmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(stmt));
    }
  }
}

void
TyvisProcessStatement::_publish_cc_concurrent_stmt_init( published_file &_cc_out,
                                                         TyvisDeclarationList *,
                                                         PublishData * ){
  _cc_out << _get_cc_process_class() << "_elab_obj = NULL;" << NL();
}

const string
TyvisProcessStatement::_get_cc_process_class() const {
  string retval;
  if( _get_mangled_label() != NULL ){
    retval += _get_mangled_label()->_to_string();
  }
  else {
    retval += "ANON_PROCESS" + ptr_to_string( const_cast<const TyvisProcessStatement *>(this) );
  }

  return retval;
}

void
TyvisProcessStatement::_publish_cc_include( published_file &_cc_out,
					    const string &to_insert ){
  ostringstream file_name_stream;
  file_name_stream << *_get_mangled_declarator() << to_insert << ".hh";
  Tyvis::_publish_cc_include( _cc_out, file_name_stream.str() );
}

void
TyvisProcessStatement::_publish_cc_include_state( published_file &_cc_out ){
  _publish_cc_include( _cc_out, "_state" );
}

void
TyvisProcessStatement::_publish_cc_instantiate_call( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisProcessStatement::_publish_cc_instantiate_call" );
  _cc_out << _get_cc_binding_name() << "_elab_obj = new " << _get_cc_binding_name() << "(getArchitecture());" << NL();
  _set_number_of_processes( _get_number_of_processes() + 1 );
}

TyvisSequentialStatementList *
TyvisProcessStatement::get_process_statement_part() {
  return dynamic_cast<TyvisSequentialStatementList *>
    (IIRBase_ProcessStatement::get_process_statement_part());
}

// IIRBase Function Wrapper(s)
TyvisDeclarationList *
TyvisProcessStatement::get_process_declarative_part() {
  return dynamic_cast<TyvisDeclarationList *>
    (IIRBase_ProcessStatement::get_process_declarative_part());
}

void
TyvisProcessStatement::_publish_cc_ams_break_function_include(published_file &_cc_out) {
  _cc_out << "#include \"" << _get_cc_process_class() << ".hh\"\n";
}

void
TyvisProcessStatement::_publish_cc_ams_breakExpression_function_body(published_file &_cc_out, PublishData *declarations) {
  
  const string procname = _get_cc_process_class();
  const string old_current_publish_name = _get_current_publish_name();
  Tyvis *oldNode = _get_current_publish_node();
  TyvisBreakElement *element;
  
  _set_currently_publishing_unit(Tyvis::PROCESS);
  ASSERT(_get_mangled_label() != NULL);
  
  for (TyvisSequentialStatement* seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
       seqstmt != NULL;
       seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(seqstmt))) {
    if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
      element = dynamic_cast<TyvisBreakElement *>(dynamic_cast<TyvisBreakStatement *>(seqstmt)->get_break_list()->first());
      while (element != NULL) {
        _cc_out << "RealType *\n";
        _cc_out << "break_expr_func_" << element << "(VHDLKernelBase "
                << "*digitalProcess) {\n";
	_cc_out << procname << " *process =\n";
        _cc_out << "((" << procname << "*)digitalProcess);\n";
        if (( dynamic_cast<TyvisDeclaration *>(element->_get_expression()) != NULL )||
	    ( dynamic_cast<TyvisMonadicOperator *>(element->_get_expression()) != NULL )) {
          _cc_out << "RealType *ptr = new RealType(ObjectBase::VARIABLE," ;
          element->_get_expression()->_publish_cc_lvalue( _cc_out , declarations );
          _cc_out << ");" << NL();
          _cc_out << "return ptr;" << NL();
        }
        else if ( dynamic_cast<TyvisFloatingPointLiteral *>(element->_get_expression()) != NULL ) {
          _cc_out << "return (new ";
          element->_get_expression()->_publish_cc_lvalue( _cc_out , declarations );
          _cc_out << ");" << NL();
        }
        else {
          _cc_out << "return (";
          element->_get_expression()->_publish_cc_lvalue( _cc_out , declarations );
          _cc_out << ");" << NL();
        }
        _cc_out << "}" << NL() << NL();
        element = dynamic_cast<TyvisBreakElement *>(dynamic_cast<TyvisBreakStatement *>(seqstmt)->get_break_list()->successor(element));
      }
    }
  }
  _set_current_publish_node( oldNode );
  _set_current_publish_name( old_current_publish_name );
}

void
TyvisProcessStatement::_publish_cc_ams_breakExpression_function_prototype(published_file &_cc_out) {
  
  TyvisBreakElement *element;
  for (TyvisSequentialStatement* seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->first());
       seqstmt != NULL;
       seqstmt = dynamic_cast<TyvisSequentialStatement *>(get_process_statement_part()->successor(seqstmt))) {
    if (seqstmt->get_kind() == IIR_BREAK_STATEMENT) {
      element = dynamic_cast<TyvisBreakElement *>(dynamic_cast<TyvisBreakStatement *>(seqstmt)->get_break_list()->first());
      while (element != NULL) {
        _cc_out << "RealType *\n";
        _cc_out << "break_expr_func_" << element << "(VHDLKernelBase *);\n";
        _cc_out << NL();
        element = dynamic_cast<TyvisBreakElement *>(dynamic_cast<TyvisBreakStatement *>(seqstmt)->get_break_list()->successor(element));
      }
    }
  }
}

void
TyvisProcessStatement::_clear( PublishData *declarations ) {
  TyvisDeclaration *decl;

  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_clear();
  }

  decl_set = declarations->get_set(TyvisDeclaration::INTERFACE_SIGNAL);
  for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {        
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
    decl->_clear();
  }
}

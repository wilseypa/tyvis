
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

#include "TyvisDesignFile.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisIdentifier.hh"
#include "TyvisProcedureCallStatement.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisProcedureDeclaration.hh"
#include "TyvisSequentialStatementList.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisWaitStatement.hh"

#include "savant/set.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>
#include "savant/StandardPackage.hh"
#include "PublishData.hh"

void
TyvisProcedureDeclaration::_publish_cc_add_parameters( ostream &new_mangled_declarator ){
  if( _get_interface_declarations()->size() >= 1 ){
    TyvisInterfaceDeclaration *interface_element = 
      dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->first());
    while( interface_element != NULL ){
      ASSERT( interface_element->_get_subtype() != NULL );
      if( interface_element->_get_subtype()->_get_declaration() == NULL ){
	if( interface_element->_get_subtype()->_get_bottom_base_type() != NULL &&
	    interface_element->_get_subtype()->_get_bottom_base_type()->_get_declaration() != NULL){
	  new_mangled_declarator << *(interface_element->_get_subtype()->_get_bottom_base_type()->_get_declaration()->_get_declarator());
	}
      }
      else {
	new_mangled_declarator << *(interface_element->_get_subtype()->_get_declaration()->_get_declarator());
      }
      interface_element = 
        dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->successor(interface_element));
    }
  }
}

void
TyvisProcedureDeclaration::_publish_cc_procedure_stuff( published_file &_cc_out,
							PublishData *declarations ){
  CC_REF( _cc_out, "TyvisProcedureDeclaration::_publish_cc_procedure_stuff" );
  ASSERT(declarations != NULL); 
  TyvisWaitStatement *wait; 
  TyvisProcedureCallStatement *proc; 
  
  // Publish the subprograms and types within the declaratiove region of 
  // this subprogram, since these need to be published just once.  They 
  // should not be published while publishing the type conversion 
  // function / resolution function. 
  _get_subprogram_declarations()->_publish_cc_decl_subprograms_and_types( _cc_out,
									  declarations ); 
     
  // First collect the necessary information regarding the subprogram. 
  dl_list<TyvisWaitStatement> _wait_stmt_list; 
  _get_subprogram_body()->_build_wait_list((dl_list<TyvisWaitStatement> *) &_wait_stmt_list); 
  dl_list<TyvisProcedureCallStatement> _procedure_stmt_list; 
  _get_subprogram_body()->_build_procedure_call_stmt_list(&_procedure_stmt_list);     

  // Necessary statements for wait statement (in procedure) implementation.
  if(_wait_stmt_list.size() > 0) {
    _publish_cc_wait_init( _cc_out, &_wait_stmt_list, declarations );
  } // if
  _cc_out << "CallStack *callStack = processPtr->getCallStack();" << NL()
	  << OS("if( processPtr->getWaitLabel() == WAITING_IN_PROC ){")
	  << "callStack->setCurrentToNext();" << NL();
  // "goto"s for wait statements in the procedure.
  for (wait = dynamic_cast<TyvisWaitStatement *>(_wait_stmt_list.first());
       wait != NULL; 
       wait = dynamic_cast<TyvisWaitStatement *>(_wait_stmt_list.successor(wait)))  {
    _cc_out << "if( callStack->getCurrentTop()->waitLabel == "
	    << wait->wait_id << " )" << NL() << "  goto ";
    wait->_publish_cc_wait_label( _cc_out, declarations );
    _cc_out << ";" << NL();
  } 
  
  // "goto"s for procedure call statements in the procedure.
  for (proc = dynamic_cast<TyvisProcedureCallStatement *>(_procedure_stmt_list.first()); 
       proc != NULL; 
       proc = dynamic_cast<TyvisProcedureCallStatement *>(_procedure_stmt_list.successor(proc)))  {
    _cc_out << "if( callStack->getCurrentTop()->waitLabel == "
	    << proc << " )" << NL() << "  goto ";
    proc->_publish_cc_proc_label( _cc_out, declarations );
    _cc_out << ";" << NL();
  } // for
  _cc_out << CS("}");
}

void 
TyvisProcedureDeclaration::_publish_cc_decl( published_file &_cc_out,
					     PublishData *declarations ){
  CC_REF( _cc_out, "TyvisProcedureDeclaration::_publish_cc_decl" );
  if( contains_body() ){
    // We need a separate symbol table for a procedure statement to keep 
    // track of the declarations that must be saved (and restored later) 
    // when we execute a wait.
    PublishData *procedure_declarations = new PublishData(); 
    _copy_symbols_defined_in_enclosing_scope(procedure_declarations, declarations);  
    _publish_cc_subprogram_body( _cc_out, procedure_declarations );
    delete procedure_declarations;
  }
}

void
TyvisProcedureDeclaration::_publish_cc_wait_data( published_file &_cc_out,
						  dl_list<TyvisWaitStatement> *_wait_stmt_list,
						  PublishData *declarations){
  TyvisWaitStatement *wait;
  CC_REF( _cc_out, "TyvisProcedureDeclaration::_publish_cc_wait_data" );
  for (wait = dynamic_cast<TyvisWaitStatement *>(_wait_stmt_list->first()); 
       wait != NULL; 
       wait = dynamic_cast<TyvisWaitStatement *>(_wait_stmt_list->successor(wait)))  {
    wait->_publish_cc_proc_wait_data( _cc_out, declarations );
  }
}

void
TyvisProcedureDeclaration::_publish_cc_wait_init( published_file &_cc_out,
						  dl_list<TyvisWaitStatement> *_wait_stmt_list,
						  PublishData *declarations){
  IIR_Boolean sens_list_newed = FALSE; // Flag used to indicate that the
				       // new'ed memory should be deleted.

  CC_REF( _cc_out, "TyvisProcedureDeclaration::_publish_cc_wait_init" );

  _cc_out << "static Wait procedureWait[" << _wait_stmt_list->size() << "];" << NL()
	  << "if(procedureWait[0].getSensSize() == 0 )" << OS("{");
  for (TyvisWaitStatement *wait = _wait_stmt_list->first(); 
       wait != NULL; 
       wait = _wait_stmt_list->successor(wait))  {
    if (wait->_get_sensitivity_list()->size() == 0) { 
      if(wait->_get_condition_clause() != NULL) {
	wait->_get_condition_clause()->_build_sensitivity_list(wait->_get_sensitivity_list());
	sens_list_newed = TRUE;
      }
    }
  } // for
  _cc_out << CS("}");

  for (TyvisWaitStatement *wait = _wait_stmt_list->first(); 
       wait != NULL; 
       wait = _wait_stmt_list->successor(wait))  {
    if( wait->get_sensitivity_list()->size() != 0 ){
      TyvisDeclaration   *sens_sig;
      TyvisDesignator    *current_signal_designator;
      current_signal_designator = dynamic_cast<TyvisDesignator *>(wait->get_sensitivity_list()->first());
      while( current_signal_designator != NULL ){
	sens_sig = dynamic_cast<TyvisDeclaration *>(current_signal_designator->_get_name());
	ASSERT( sens_sig != 0 );
	ASSERT( sens_sig->is_signal() == TRUE );
	ASSERT( dynamic_cast<TyvisDeclaration *>(sens_sig) != NULL );

        _cc_out <<"  procedureWait[" << wait->wait_id << "].addSensitivityTo(" 
		<< "&(dynamic_cast<SignalBase &>(";
	sens_sig->_publish_cc_lvalue( _cc_out, declarations );
	_cc_out << ")));" << NL();	
	current_signal_designator = dynamic_cast<TyvisDesignator *>(wait->get_sensitivity_list()->successor(current_signal_designator));
      }	// while
      if(sens_list_newed == TRUE) {
	current_signal_designator = dynamic_cast<TyvisDesignator *>(wait->get_sensitivity_list()->first());
	while(current_signal_designator != NULL) {
	  TyvisDesignator *next =  dynamic_cast<TyvisDesignator *>(wait->get_sensitivity_list()->successor(current_signal_designator));
	  wait->get_sensitivity_list()->remove(current_signal_designator);
	  delete current_signal_designator;
	  current_signal_designator = next;
	} // while
      }	// if(sens_list_newed == TRUE)
    } // if
  } // For
}  

TyvisDeclarationList *
TyvisProcedureDeclaration::_get_subprogram_declarations() {
  return dynamic_cast<TyvisDeclarationList *>(get_subprogram_declarations());
}

TyvisTextLiteral *
TyvisProcedureDeclaration::_get_declarator() {
  return dynamic_cast<TyvisTextLiteral *>(get_declarator());
}

// Mangling of functions and procedures is done here as a constnat string
// "savant" is added as a suffix to the function name.
//
// This cannot be done at the subprogram level as for functions, the
// return type is added a prefix while for procedures it is not.

string
TyvisProcedureDeclaration::_mangle_declarator() {
  ostringstream newMangledDeclarator;
  if ( _need_to_mangle() ){
    newMangledDeclarator << *(_get_declarative_region()->_get_mangled_declarator());
    newMangledDeclarator << "_";
    _publish_cc_add_parameters( newMangledDeclarator );
  }
  
  //Mangling with the argument types
  if( dynamic_cast<TyvisStringLiteral*>(_get_declarator()) != NULL ){
    (dynamic_cast<TyvisStringLiteral*>(get_declarator()))->_convert_function_name(newMangledDeclarator);
  } 
  else {
    newMangledDeclarator << "savant" << *(_get_declarator());
  }
  return newMangledDeclarator.str();
}

void 
TyvisProcedureDeclaration::_get_signal_source_info( savant::set<Tyvis> *siginfo ){
  TyvisInterfaceDeclaration *decl = 
    dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->first());
  while( decl != NULL){
    if(decl->get_mode() == IIR_OUT_MODE ||
       decl->get_mode() == IIR_INOUT_MODE) {
      siginfo->add( decl );
    }
    decl =
      dynamic_cast<TyvisInterfaceDeclaration *>(get_interface_declarations()->successor(decl));
  }
}

bool
TyvisProcedureDeclaration::_need_to_mangle(){
  return ( _get_declarative_region() != NULL && 
           dynamic_cast<TyvisDeclaration *>(_get_declarative_region()));
}


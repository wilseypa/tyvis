
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

#include "TyvisDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisIdentifier.hh"
#include "TyvisDesignator.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisLabel.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "TyvisWaitStatement.hh"

#include "savant/StandardPackage.hh"
#include "savant/error_func.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include "PublishData.hh"
#include <sstream>
using std::ostringstream;

TyvisWaitStatement::TyvisWaitStatement() {
  set_sensitivity_list(new TyvisDesignatorList());
}

TyvisWaitStatement::~TyvisWaitStatement() {
  //Release the list memory
  delete get_sensitivity_list();
}

void 
TyvisWaitStatement::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {

  CC_REF( _cc_out, "TyvisWaitStatement::_publish_cc" );

  if(_is_currently_publishing_subprogram() == TRUE) {
    _publish_cc_procedure_wait( _cc_out, declarations );
    return;
  }
  _cc_out.open_scope( "executeWait(" );
  _cc_out << wait_id;
  if(get_timeout_clause() != NULL) {
    _cc_out << "," << NL();
    _get_timeout_clause()->_publish_cc_rvalue( _cc_out , declarations );
    _get_timeout_clause()->_add_decl_into_declaration_collection(declarations);
  }
  _cc_out.close_scope( ");" );
  _cc_out << NL() << "return;" << NL();

  _publish_cc_wait_label( _cc_out, declarations );
  _cc_out << ":" << NL();

  _cc_out << "if (!";
  if(_is_currently_publishing_subprogram() == TRUE) {
    _cc_out << "processPtr->";
  }
  _cc_out << "resumeWait(" << wait_id;
  if(_get_condition_clause() != NULL) {
    _cc_out << ", " << NL();
    _get_condition_clause()->_publish_cc_rvalue( _cc_out , declarations );
    _get_condition_clause()->_add_decl_into_declaration_collection(declarations);
  }
  _cc_out << ")) return;" << NL();
  _get_sensitivity_list()->_add_decl_into_declaration_collection(declarations);
}

void 
TyvisWaitStatement::_publish_cc_procedure_wait( published_file &_cc_out,
						PublishData *declarations ) {
  TyvisDeclaration *decl;
  const int _num_decls = 4;
  TyvisDeclaration::declaration_type _proc_decl;
  TyvisDeclaration::declaration_type _proc_decl_type[_num_decls] =
  {IIR_Declaration::VARIABLE, 
   IIR_Declaration::CONSTANT, 
   IIR_Declaration::INTERFACE_VARIABLE,
   IIR_Declaration::INTERFACE_CONSTANT};

  CC_REF( _cc_out, "TyvisWaitStatement::_publish_cc_procedure_wait" );

  _cc_out << "processPtr->setProcedureWait(&(procedureWait[" << wait_id 
	  << "]));" << NL()
	  << OS("{") 
	  << "StackElement *newElem = new StackElement;" << NL()
	  << "newElem->waitLabel = " << wait_id << ";" << NL()
	  << "int numSignals = 0;" << NL();
  register int i;

  // Save necessary declarations.
  for(i = 0; i < _num_decls; i++) {
    _proc_decl = _proc_decl_type[i];

    std::set<IIR_Declaration*> *decl_set = declarations->get_set(_proc_decl);
    for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
      decl = dynamic_cast<TyvisDeclaration *>(*iter);
      _cc_out << "if(!";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ".is_signal()) numSignals++;" << NL();
    } // for
  } // for

  _cc_out << "newElem->args = (RValue**) new "
	  << "RValue*[numSignals];" << NL()
	  << "numSignals = 0;" << NL();

  for(i = 0; i < _num_decls; i++) {
    _proc_decl = _proc_decl_type[i];
    std::set<IIR_Declaration*> *decl_set = declarations->get_set(_proc_decl);
    for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
    decl = dynamic_cast<TyvisDeclaration *>(*iter);
      _cc_out << "if(!";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ".is_signal())" << OS("{") 
              << "*newElem->args[numSignals++] = ";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ";" << NL()
	      << "newElem->numArgs++;" << NL()
	      << CS("}"); 
    } // for
  } // for

  _cc_out << "callStack->push(newElem);" << NL()
	  << CS("}");
  
  _cc_out << "processPtr->executeWait(WAITING_IN_PROC";
  if(_get_timeout_clause() != NULL) {
    _cc_out << ", ";
    _get_timeout_clause()->_publish_cc_rvalue( _cc_out , declarations );
  }
  _cc_out << ");" << NL()
	  << "return EXECUTE_WAIT_RETURN;" << NL();
  _publish_cc_wait_label( _cc_out, declarations );
  _cc_out << ":" << NL()
	  << "if(!processPtr->resumeWait(WAITING_IN_PROC";
  if(_get_condition_clause() != NULL) {
    _cc_out << ", ";
    _get_condition_clause()->_publish_cc_rvalue( _cc_out , declarations );
  }
  _cc_out << ")) "
	  << "return RESUME_WAIT_RETURN;" << NL();

  // Restore the required declarations.
  _cc_out << OS("{") 
	  << "int numSignals = 0;" << NL();
  for(i = 0; i < _num_decls; i++) {
    _proc_decl = _proc_decl_type[i];

    std::set<IIR_Declaration*> *decl_set = declarations->get_set(_proc_decl);
    for (std::set<IIR_Declaration*>::iterator iter = decl_set->begin(); iter != decl_set->end(); iter++) {
      decl = dynamic_cast<TyvisDeclaration *>(*iter);
      _cc_out << "if(!";
      decl->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ".is_signal())" << OS("{"); 
      //DRH - Verify implementation of callstack return - const references cannot be assigned
      //DRH decl->_publish_cc_lvalue( _cc_out , declarations );
      //DRH _cc_out << " = (const ";
      //DRH decl->_get_subtype()->_publish_cc_universal_type( _cc_out );
      //DRH _cc_out << " &) *(callStack->getCurrentTop()->args[numSignals]);" << NL()
      _cc_out << "delete (callStack->getCurrentTop()->args[numSignals++]);" << NL()
            << CS("}"); 
    } // for
  } // for
  _cc_out << "callStack->popAboveCurrent();" << NL()
	  << CS("}");
}

void 
TyvisWaitStatement::_publish_cc_wait_label( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisWaitStatement::_publish_cc_wait_label" );
  if (_get_label() != NULL) {
    _get_label()->_publish_cc_lvalue( _cc_out , declarations );
  }
  else {
    _cc_out << "wait";
    _cc_out << this;
  }
}

void 
TyvisWaitStatement::_publish_cc_wait_data( published_file &_cc_out,
                                           IIR_Char*,
                                           PublishData *declarations){

  CC_REF( _cc_out, "TyvisWaitStatement::_publish_cc_wait_data" );

  // insert all sigs in the senslist into this process's data structures
  if (_get_sensitivity_list()->size() != 0) {
    TyvisDeclaration *sig_decl;
    TyvisDesignator *sig_designator = dynamic_cast<TyvisDesignator *>(get_sensitivity_list()->first());
    while (sig_designator != NULL) {
      sig_decl = dynamic_cast<TyvisDeclaration*>((dynamic_cast<TyvisDesignatorExplicit*>(sig_designator))->get_name());
      ASSERT(sig_decl->is_signal() == TRUE );
      if( dynamic_cast<TyvisDeclaration *>(sig_decl) == NULL ) {
	sig_decl =dynamic_cast<TyvisDeclaration*>((dynamic_cast<TyvisDesignatorExplicit*>(sig_decl))->get_name());
      }

      if (!declarations->in_collection(sig_decl)) {
	declarations->add_declaration(sig_decl);
      }
      sig_designator = dynamic_cast<TyvisDesignator *>(get_sensitivity_list()->successor(sig_designator));
    }
  }
}

void
TyvisWaitStatement::_publish_cc_proc_wait_data( published_file &_cc_out, PublishData *declarations ) {
  const string old_prefix_string = _get_publish_prefix_string();
  // The wait condition and wait timeout clauses are published as
  // functions.  These functions are similar to the user-defined
  // functions, and hence we trick the code-generator by saying that we
  // are publishing a subprogram, whereas we are actually not.
  PublishedUnit _saved_publishing_unit = _get_currently_publishing_unit();

  CC_REF( _cc_out, "TyvisWaitStatement::_publish_cc_proc_wait_data" );

  _set_currently_publishing_unit(PROCEDURE);
  _set_publish_prefix_string("state->");

  // Redefine savantnow here.
  _publish_cc_savantnow_process_ptr( _cc_out );

  if( _get_condition_clause() != NULL ){
    _cc_out << "bool" << NL()
	    << "waitCond" << wait_id << "_" << this
	    << OS("(VHDLProcess *processPtr ){")
	    << this << "_state* state;" << NL()
	    << "state = (" << this << "_state*) processPtr->getState();" << NL()
	    << "return (SAVANT_BOOLEAN_TRUE  == ";
    _get_condition_clause()->_publish_cc_rvalue( _cc_out , declarations ); 
    _cc_out << ");" << NL()
	    << CS("}") << NL();
  }

  if (_get_timeout_clause() != NULL) {
    _cc_out << "PhysicalType" << NL()
	    << "waitTimeout" << wait_id << "_" << this
	    << OS("(VHDLProcess *processPtr) {")
	    << this << "_state* state;" << NL()
	    << "state = (" << this << "_state*) processPtr->getState();" << NL()
	    << "return (";
    _get_timeout_clause()->_publish_cc_rvalue( _cc_out , declarations );
    _cc_out << ");" << NL()
	    << "}" << NL();
  }

  // Restore the definition of savantnow.
  _publish_cc_savantnow_no_process_ptr( _cc_out );

  _set_publish_prefix_string(old_prefix_string);
  _set_currently_publishing_unit(_saved_publishing_unit);
}  

void 
TyvisWaitStatement::_publish_cc_wait_decl( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisWaitStatement::_publish_cc_wait_decl" );

  if (_get_condition_clause() != NULL) {
    _cc_out << "extern bool waitCond" << wait_id << "_" << this
	    << "(VHDLKernel *);" << NL();
  }
  if (_get_timeout_clause() != NULL) {
    _cc_out << "extern PhysicalType waitTimeout" << wait_id << "_" << this
	    << "(VHDLKernel *);" << NL();
  }
}

TyvisDesignatorList *
TyvisWaitStatement::_get_sensitivity_list() {
  return dynamic_cast<TyvisDesignatorList *>(get_sensitivity_list());
}

Tyvis*
TyvisWaitStatement::_get_condition_clause() {
  return dynamic_cast<Tyvis *>(get_condition_clause());
}

Tyvis *
TyvisWaitStatement::_get_timeout_clause() {
  return dynamic_cast<Tyvis *>(get_timeout_clause());
}

void
TyvisWaitStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list) {
  if( _get_condition_clause() != NULL ){
    _get_condition_clause()->_get_list_of_input_signals(list);
  }
  //_get_timeout_clause()->_get_list_of_input_signals(list);
  _get_sensitivity_list()->_get_list_of_input_signals(list);
}

void
TyvisWaitStatement::_build_wait_list(dl_list<TyvisWaitStatement>* list) {
  list->append(this);
  wait_id = list->size() - 1;
}

void
TyvisWaitStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_sensitivity_list()->_build_above_attribute_set(to_build);
  if (_get_condition_clause() != NULL) {
    _get_condition_clause()->_build_above_attribute_set(to_build);
  }
}

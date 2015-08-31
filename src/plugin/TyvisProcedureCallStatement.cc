
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

#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIndexedName.hh"
#include "TyvisInterfaceDeclaration.hh"
#include "TyvisInterfaceList.hh"
#include "TyvisLabel.hh"
#include "TyvisList.hh"
#include "TyvisProcedureCallStatement.hh"
#include "TyvisProcedureCallStatement.hh"
#include "TyvisProcedureDeclaration.hh"
#include "TyvisSimpleName.hh"
#include "TyvisTypeDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "savant/set.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/savant.hh"
#include "published_file.hh"
#include <sstream>

TyvisProcedureCallStatement::TyvisProcedureCallStatement() {
  set_actual_parameter_part(new TyvisAssociationList());
}

TyvisProcedureCallStatement::~TyvisProcedureCallStatement() {
  //Release the list memory
  delete get_actual_parameter_part();
}

// Note:
// 1. Default label for a procedure call statement is constructed by
//    concatenating the string "PL" with the string representation of the
//    pointer to that TyvisProcedureCallStatement instance.
// 2. The wait label for the procedure call statement is constructed by
//    taking the integer value of the "this" pointer of the
//    TyvisProcedureCallStatement instance.
void 
TyvisProcedureCallStatement::_publish_cc( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisProcedureCallStatement::_publish_cc" );

  _publish_cc_proc_label( _cc_out, declarations );
  _cc_out << OS( ":{" );
  ASSERT (_get_procedure_name() != NULL);

  TyvisProcedureDeclaration *proc_decl
    = dynamic_cast<TyvisProcedureDeclaration *>(get_procedure_name());

  ASSERT (proc_decl != NULL);

  _get_actual_parameter_part()->
    _publish_cc_necessary_temporaries_for_TC( _cc_out, proc_decl->_get_interface_declarations(), declarations);
  
  // Special handling is required for "writeline" call within a
  // subprogram.
  if(_is_currently_publishing_subprogram() == TRUE) {
    TyvisTextLiteral *declarator = (dynamic_cast<TyvisDeclaration *>( _get_procedure_name() ) )->_get_declarator();
    if(IIRBase_Identifier::cmp(declarator, "writeline") == 0 ||
       IIRBase_Identifier::cmp(declarator, "readline") == 0) {
      //_cc_out << "processPtr->";
      _get_procedure_name()->_publish_cc_rvalue( _cc_out , declarations );
      _cc_out << OS( "(processPtr," );
      if(_get_actual_parameter_part()->size() != 0) {
	_get_actual_parameter_part()->_publish_cc_rvalues( _cc_out, declarations, "," );
      }
      _cc_out << CS( ");" ) << CS( "}" );
      return;
    }
  } 

  // Assume being called from a process.
  _cc_out << OS( "switch(" );
  _get_procedure_name()->_publish_cc_subprogram_call_name( _cc_out, declarations );
  _cc_out << OS( "(processPtr" );

  if(_get_actual_parameter_part()->size() != 0) {
    _cc_out << ", " << NL();
    _get_actual_parameter_part()->
    _publish_cc_with_temporaries_for_TC( _cc_out, proc_decl->_get_interface_declarations(), declarations);
  }
  _cc_out << CS(")") << CS(")") << OS("{")
	  << OS( "case EXECUTE_WAIT_RETURN:" );
  if(_get_currently_publishing_unit() == PROCEDURE) {
    _cc_out << "{" << NL();
    _cc_out << "CallStack *callStack = processPtr->getCallStack();" << NL();
    _cc_out << "callStack->push(" << this << ", 0);";
  }
  else if(_get_currently_publishing_unit() == FUNCTION) {
    // Do Nothing.
  } else {
    _cc_out << "(static_cast<VHDLKernel_state *>(getState()))->stack.push(" << this << ", 0);";
  }
  _cc_out << NL();
  if(_get_currently_publishing_unit() == PROCEDURE) {
    _cc_out << "return EXECUTE_WAIT_RETURN;" << NL();
    _cc_out << "}" << NL();
  } else if(_get_currently_publishing_unit() == FUNCTION) {
    // Do Nothing.
  } else {
    _cc_out << "return;";
  }
  _cc_out << CS("") << OS( "case RESUME_WAIT_RETURN:" );
  if(_get_currently_publishing_unit() == PROCEDURE) {
    _cc_out << "return RESUME_WAIT_RETURN;";
  }
  else if(_get_currently_publishing_unit() == FUNCTION) {
    // Do Nothing.
  }
  else {
    _cc_out << "return;";
  }
  _cc_out << CS("");
  _cc_out << OS( "case NORMAL_RETURN:" );
  _get_actual_parameter_part()->
    _publish_cc_restore_from_temporaries( _cc_out, proc_decl->_get_interface_declarations(), declarations );
  _cc_out << "break;"
	  << CS("")
	  << CS("}")
	  << CS("}");
}

void
TyvisProcedureCallStatement::_publish_cc_proc_label( published_file &_cc_out,
						     PublishData *declarations ) {
  if (_get_label() != NULL) {
    _get_label()->_publish_cc_rvalue( _cc_out , declarations );
  }
  else {
    _cc_out << "PL";
    _cc_out << this;
  }
}


TyvisAssociationList *
TyvisProcedureCallStatement::_get_actual_parameter_part() {
  return dynamic_cast<TyvisAssociationList *>(get_actual_parameter_part());
}

Tyvis *
TyvisProcedureCallStatement::_get_procedure_name() {
  return dynamic_cast<Tyvis *>(get_procedure_name());
}

void
TyvisProcedureCallStatement::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_actual_parameter_part()->_get_list_of_input_signals(list);
}

void
TyvisProcedureCallStatement::_get_signal_source_info(  savant::set<Tyvis> *siginfo ){
  ASSERT(get_procedure_name()->get_kind() == IIR_PROCEDURE_DECLARATION);

  _get_actual_parameter_part()->_get_signal_source_info( siginfo );
}

void
TyvisProcedureCallStatement::_build_wait_list(dl_list<TyvisWaitStatement> * ){}

void 
TyvisProcedureCallStatement::_build_procedure_call_stmt_list(dl_list<TyvisProcedureCallStatement> *list) {
  list->append(dynamic_cast<TyvisProcedureCallStatement *>( this ));
}

void
TyvisProcedureCallStatement::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
  _get_actual_parameter_part()->_build_above_attribute_set(to_build);
}

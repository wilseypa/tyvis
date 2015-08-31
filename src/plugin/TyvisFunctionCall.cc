
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

#include "TyvisAssociationElement.hh"
#include "TyvisAssociationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisFunctionDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/StandardPackage.hh"
#include "published_file.hh"
#include <sstream>

TyvisFunctionCall::TyvisFunctionCall() 
{
  set_parameter_association_list(new TyvisAssociationList());
}

TyvisFunctionCall::~TyvisFunctionCall() {
  //Release the list memory
  delete get_parameter_association_list();
}

void
TyvisFunctionCall::_publish_cc_ams_function(published_file &_cc_out, PublishData *declarations) {
  TyvisTextLiteral *declarator = _get_implementation()->_get_declarator();
  
  if ((IIRBase_Identifier::cmp(declarator, "now") == 0)) {
    if(_get_currently_publishing_unit() == Tyvis::SIMULTANEOUS_STATEMENT) {
      _cc_out << "  new equationNode('T',0,node"
              << (int)(_stmt_node_index/2) << ");" << NL();
    }
  }
  else if ((IIRBase_Identifier::cmp(declarator, "exp") == 0)) {
    if(_get_currently_publishing_unit() == Tyvis::SIMULTANEOUS_STATEMENT) {
      _cc_out << "  equationNode *node"<<_stmt_node_index<<" = new equationNode('E',0,node"
              << (int)(_stmt_node_index/2) << ");" << NL();
    }
  }
  else if ((IIRBase_Identifier::cmp(declarator, "sin") == 0)) {
    if(_get_currently_publishing_unit() == Tyvis::SIMULTANEOUS_STATEMENT) {
      _cc_out << "  equationNode *node"<<_stmt_node_index<<" = new equationNode('S',0,node"
              << (int)(_stmt_node_index/2) << ");" << NL();
    }
  }
  if (((IIRBase_Identifier::cmp(declarator, "exp") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "log") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "cos") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "sqrt") == 0)) || 
      ((IIRBase_Identifier::cmp(declarator, "sin") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "tan") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "asin") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "acos") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "atan") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "atan2") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "pow") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "log10") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "sinh") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "cosh") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "tanh") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "ceil") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "floor") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "asinh") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "acosh") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "atanh") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "fabs") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "fmax") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "fmin") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "ldexp") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "frexp") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "erf") == 0)) ||
      ((IIRBase_Identifier::cmp(declarator, "rand") == 0))) {
    _publish_cc_ams_math_functions(declarator,_cc_out, declarations);
  }
  else {
    _cc_out << "((("; 
    _get_subtype()->_publish_cc_universal_type(_cc_out);
    _cc_out << "*)(&(";
    _get_implementation()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "_" << this << ".getVHDLData())))->val) ";
  }
}

void
TyvisFunctionCall::_publish_cc_ams_math_functions(TyvisTextLiteral *,
                                                  published_file &_cc_out,
                                                  PublishData *declarations) {
  TyvisAssociationElement *elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first());
  while (elem != NULL) {   
    elem->_get_actual()->_publish_cc_ams_function(_cc_out, declarations);
    elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->successor(elem));
    if (elem != NULL) {
      elem->_get_actual()->_publish_cc_ams_function(_cc_out, declarations);
    }
  }   
}

void
TyvisFunctionCall::_publish_cc_ams_function_call_in_simult_stmt(published_file &_cc_out, PublishData *declarations) {
  ostringstream retval_buf;
  string func_retval;
  
  TyvisTextLiteral *declarator = _get_implementation()->_get_declarator();
  
  if ((IIRBase_Identifier::cmp(declarator, "exp") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "log") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "sqrt") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "sin") == 0)) { 
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "cos") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "tan") == 0)) {
    // No need to do anything
  } 
  else if ((IIRBase_Identifier::cmp(declarator, "asin") == 0)) {  
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "acos") == 0)) {   
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "atan") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "atan2") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "pow") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "log10") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "sinh") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "cosh") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "tanh") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "ceil") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "floor") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "asinh") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "acosh") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "atanh") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "fabs") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "fmax") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "fmin") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "ldexp") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "frexp") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "erf") == 0)) {
    // No need to do anything
  }
  else if ((IIRBase_Identifier::cmp(declarator, "rand") == 0)) {
    // No need to do anything
  }
  else
  {
    ASSERT( _get_currently_publishing_unit() == Tyvis::ARCHITECTURE_DECL);
    ASSERT( _get_implementation() != NULL);
    
    _get_implementation()->_get_mangled_declarator()->print(retval_buf);
    retval_buf << "_" << this << ends;
    func_retval = retval_buf.str();
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << " " << func_retval << "(ObjectBase::VARIABLE, ";
    _get_implementation()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "(";
    /*
    if (_currently_publishing_simultaneous_stmt == TRUE) {
      _cc_out << "(VHDLKernel *)currentEquation->ckt";
    }
    */
    if ( _get_parameter_association_list()->size() != 0) {
      _cc_out << ", ";
      _get_parameter_association_list()->_publish_cc_lvalue( _cc_out , declarations );
    }
    _cc_out << "));" << NL();
  }
}

void
TyvisFunctionCall::_publish_cc_as_operator( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisFunctionCall::_publish_cc_as_operator" );

  ASSERT ( _get_implementation() != NULL );
  _get_implementation()->_publish_cc_lvalue( _cc_out , declarations );
  _cc_out << OS("(");
  _get_parameter_association_list()->_publish_cc_lvalue( _cc_out , declarations );
  if ((IIRBase_TextLiteral::cmp(_get_implementation()->_get_declarator(), "\"&\"") == 0) && 
      (_get_parameter_association_list()->_are_all_scalars() == TRUE)) {
    ASSERT ( _get_subtype() != NULL );
    _cc_out << "," << NL() << "(";
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << " *) NULL";
  }
  _cc_out << CS(")");
}

void 
TyvisFunctionCall::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisFunctionCall::_publish_cc_rvalue" );
  ASSERT(_get_implementation() != NULL);
  
  if( _get_implementation()->is_operator() &&
      _get_implementation()->is_implicit_declaration() ){
    _publish_cc_as_operator( _cc_out, declarations );
  }
  else{
    // Pass on the process ptr to any function that is being called from
    // this function.
    _get_implementation()->_publish_cc_rvalue( _cc_out , declarations );
    if(_get_parameter_association_list()->size() != 0) {
      //     parameter_association_list._publish_cc_subprogram_arguments( _cc_out );
      // If FunctionCall is in subprogram or process, we have access to processPtr - DRH
      if( _is_currently_publishing_subprogram() || _get_currently_publishing_unit() == PROCESS ){
        _cc_out << OS("(processPtr,");
      }
      else{ 
        _cc_out << OS("(NULL,"); 
      }
      _get_parameter_association_list()->_publish_cc_rvalues( _cc_out, declarations, "," );
      _cc_out << CS(")");
    }
    else{
      // If FunctionCall is in subprogram or process, we have access to processPtr
      if( _is_currently_publishing_subprogram() || _get_currently_publishing_unit() == PROCESS ){
        _cc_out << "(processPtr)";
      }
      else{
        _cc_out << OS("(NULL)");
      }
    }
  }
}

void 
TyvisFunctionCall::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisFunctionCall::_publish_cc_elaborate" );
  _publish_cc_lvalue( _cc_out , declarations );
}

void 
TyvisFunctionCall::_publish_cc_elaborate_arg( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisFunctionCall::_publish_cc_elaborate_arg" );

  ASSERT(get_parameter_association_list()->size() == 1);
  dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first())->_publish_cc_elaborate( _cc_out, declarations );
}

void 
TyvisFunctionCall::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisFunctionCall::_publish_cc_bounds" );

  ASSERT( _get_implementation() != NULL );
  ASSERT( _get_implementation()->get_kind() == IIR_FUNCTION_DECLARATION );

  (dynamic_cast<TyvisFunctionDeclaration *>(_get_implementation()))->_get_return_type()->_publish_cc_bounds( _cc_out, declarations );
}

void
TyvisFunctionCall::_publish_cc_first_objectParameter( published_file &_cc_out, PublishData *declarations ){

  CC_REF( _cc_out, "TyvisFunctionCall::_publish_cc_first_objectParameter" );

  // This function cannot be called on function call nodes with no
  // parameters. This function is for TCs that usually have a parameter
  ASSERT (get_parameter_association_list()->first() != NULL);

  dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first())->_publish_cc_first_objectParameter( _cc_out, declarations );
}

const string
TyvisFunctionCall::_get_cc_tyvis_type(){
  //  CC_REF( _cc_out, "TyvisFunctionCall::_publish_cc_tyvis_type" );
  ASSERT( _get_implementation() != NULL );
  return _get_implementation()->_get_subtype()->_get_cc_tyvis_type();
}

void
TyvisFunctionCall::_publish_cc_headers( published_file &_cc_out )
{
  // Nothing needed here.
}

void
TyvisFunctionCall::_add_decl_into_declaration_collection(PublishData *declarations) {
  TyvisAssociationElement *elem = 
    dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first());

  while (elem != NULL) {
    if (elem->_get_actual() != NULL) {
      elem->_get_actual()->_add_decl_into_declaration_collection(declarations);
    }
    elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->successor(elem));
  }
}

TyvisSubprogramDeclaration*
TyvisFunctionCall::_get_implementation() {
  return dynamic_cast<TyvisSubprogramDeclaration *>(get_implementation());
}

TyvisAssociationList *
TyvisFunctionCall::_get_parameter_association_list() {
  return dynamic_cast<TyvisAssociationList *>(get_parameter_association_list());
}

IIR_Mode
TyvisFunctionCall::_get_mode() {
  ASSERT(get_parameter_association_list()->size() == 1);
  ASSERT(dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first())->is_signal() == TRUE);

  return dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first())->_get_actual()->_get_mode();
}

void
TyvisFunctionCall::_build_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  _get_parameter_association_list()->_build_sensitivity_list(sensitivity_list);
}

IIR_Boolean 
TyvisFunctionCall::_is_globally_static_primary(){
  IIR_Boolean retval = TRUE;

  ASSERT( get_implementation() != NULL );
  ASSERT( get_implementation()->get_kind() == IIR_FUNCTION_DECLARATION );
  if ( (dynamic_cast<TyvisFunctionDeclaration *>(get_implementation()))->get_pure() == IIR_IMPURE_FUNCTION ){
    retval = FALSE;
  }
  
  if( _get_parameter_association_list()->_is_globally_static_for_function() == FALSE ){
    retval = FALSE;
  }

  return retval;
}

void
TyvisFunctionCall::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  _get_parameter_association_list()->_get_list_of_input_signals(list);

  // There isn't any real type checking - it was all done during the
  // semantic_transform of the indexed_name.
}

void
TyvisFunctionCall::_set_stmt_node_index(IIR_Int32 *index, bool _is_right_child, bool &reducibleFlag) {
  if (_is_right_child == true) {
   _stmt_node_index = 2 * (*index)+ 1;
  }
  else {
   _stmt_node_index = 2 * (*index);
  }
  TyvisAssociationElement *elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first());
  while(elem != NULL) {
    elem->_get_actual()->_set_stmt_node_index(&_stmt_node_index, false, reducibleFlag);
    elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->successor(elem));
    if (elem!=NULL) {
     elem->_get_actual()->_set_stmt_node_index(&_stmt_node_index, true, reducibleFlag);
    }
  }
}

void
TyvisFunctionCall::_set_stmt_qty_index( IIR_Int32 *index,
                                               savant::set<TyvisDeclaration> *quantity_set,
                                               PublishData *declarations,
                                               PublishData *arch_declarations) {
  TyvisAssociationElement *elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first());
  while (elem != NULL) {
    elem->_get_actual()->_set_stmt_qty_index(index, quantity_set, declarations, arch_declarations);
    elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->successor(elem));
  }
} 

void
TyvisFunctionCall::_flush_stmt_index() {
  TyvisAssociationElement *elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first());
  while (elem != NULL) {
    elem->_get_actual()->_flush_stmt_index();
    elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->successor(elem));
  }
}

void
TyvisFunctionCall::_set_stmt_signal_index(IIR_Int32 *index,
                                                  savant::set<TyvisDeclaration> *signal_set) {
  
  TyvisAssociationElement *elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->first());
  while (elem != NULL) {
    elem->_get_actual()->_set_stmt_signal_index(index, signal_set);
    elem = dynamic_cast<TyvisAssociationElement *>(get_parameter_association_list()->successor(elem));
  }
}

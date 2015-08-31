
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
#include "TyvisAssociationElementByExpression.hh"
#include "TyvisAssociationList.hh"
#include "TyvisAttribute.hh"
#include "TyvisContributionAttribute.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDotAttribute.hh"
#include "TyvisFloatingPointLiteral.hh"
#include "TyvisFunctionCall.hh"
#include "TyvisIdentifier.hh"
#include "TyvisIndexedName.hh"
#include "TyvisLabel.hh"
#include "TyvisMultiplicationOperator.hh"
#include "TyvisReferenceAttribute.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "TyvisSimpleSimultaneousStatement.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisTerminalDeclaration.hh"
#include "TyvisTypeDefinition.hh"

#include "savant/dl_list.hh"
#include "published_file.hh"
#include <sstream>
#include <iostream>
#include "published_file.hh"
#include "published_header_file.hh"
#include "published_cc_file.hh"
#include "savant/set.hh"
#include "savant/resolution_func.hh"
#include "savant/error_func.hh"
#include "savant/error_func.hh"

TyvisSimpleSimultaneousStatement::TyvisSimpleSimultaneousStatement() {
  _free_quantity_present = false;
  enclosingRegion = NULL;
  _stmt_qty_index = 0;
  _stmt_signal_index = 0;
  _stmt_node_index = 1;
}

void
TyvisSimpleSimultaneousStatement::_publish_cc_characteristic_expressions(TyvisArchitectureStatement::SimultaneousIfPublishingPart current_part,
                                                                                published_file &_cc_out,
                                                                                PublishData *declarations) {
  TyvisDeclaration *is_Q2S = NULL;
  is_Q2S  = _unique_signals.getElement();

  for ( TyvisDeclaration *temp  = _unique_qtys.getElement();
	temp != NULL;
	temp = _unique_qtys.getNextElement() ) {
    if ( temp->get_kind() == IIR_FREE_QUANTITY_DECLARATION ) {
      _free_quantity_present = true;
    }
  }
  if (_is_free_quantity_present()) {
    // For simultaneous Statements without signals..
    if (is_Q2S == NULL) {
      // this is a freeEquation ...
      _cc_out << "  equation = new freeEquation(";
      _cc_out << " this, parent_equation,";
      
      switch (current_part) {
      case TyvisArchitectureStatement::None: {
        _cc_out << " -1 ,\"";
      }
	break;
      case TyvisArchitectureStatement::IF_PART: {
        _cc_out << " YES,\"";
      }
	break;
      case TyvisArchitectureStatement::ELSE_PART: {
        _cc_out << " NO,\"";
      }           
	break;
      default: {
        cerr << "Wrong enumeration for publishing simultaneous "
             << "if/elsif statement!!" << endl;
        cerr << "Aborting VHDL-AMS to C++ code generation ..." << endl;
        abort();
      }
	break;
      }
      _publish_cc_ams_equation_constructor(current_part,_cc_out, declarations);
    }
    else {  
      _cc_out << "  equation = new freeEquation(\"";
      _publish_cc_ams_equation_constructor(current_part,_cc_out, declarations);
    }
  }
  else {    
    // Simple sim statements without signal
    if (is_Q2S == NULL) {
      _cc_out << "equation = new branchEquation(this, parent_equation ,";
      switch (current_part) {
      case TyvisArchitectureStatement::None: {
        _cc_out << " -1 ,\"";
      }
	break;
      case TyvisArchitectureStatement::IF_PART: {
        _cc_out << " YES,\"";
      }
	break;
      case TyvisArchitectureStatement::ELSE_PART: {
        _cc_out << " NO,\"";
      }
	break;
      default: {
        cerr << "Wrong enumeration for publishing simultaneous "
             << "if/elsif statement!!" << endl;
        cerr << "Aborting VHDL-AMS to C++ code generation ..." << endl;
        abort();
      }
	break;
      }
      _publish_cc_ams_equation_constructor(current_part,_cc_out, declarations);
      _cc_out <<NL();
    }
    else {
      _cc_out << "equation = new branchEquation(\"";
      _publish_cc_ams_equation_constructor(current_part,_cc_out, declarations);
    }
  }
  _publish_cc_implicit_reference_equations(_cc_out, declarations);
  _publish_cc_implicit_contribution_equations(_cc_out, declarations);
  _publish_cc_implicit_differential_equations(_cc_out, declarations);
}

void
TyvisSimpleSimultaneousStatement::_publish_cc_ams_function_prototype(published_file &_cc_out, PublishData *declarations) {
  _publish_cc( _cc_out, declarations );
   
  _cc_out << "equationNode *" << NL();
  _cc_out << "ams_rhs_expr_";
  _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );
  _cc_out << "( component *, bool& );" << NL();
}

void
TyvisSimpleSimultaneousStatement::_publish_cc_ams_function_body(published_file &_cc_out, PublishData *declarations, PublishData *arch_declarations) {
 
  Tyvis *left_expr = _get_left_expression();
  Tyvis *right_expr = _get_right_expression();
    
  left_expr->_set_stmt_qty_index(&_stmt_qty_index, &_unique_qtys, declarations, arch_declarations);
  right_expr->_set_stmt_qty_index(&_stmt_qty_index, &_unique_qtys, declarations, arch_declarations);
  
  left_expr->_set_stmt_signal_index(&_stmt_signal_index, &_unique_signals);
  right_expr->_set_stmt_signal_index(&_stmt_signal_index, &_unique_signals);

  bool reducibleFlag = true; // all SSS are assumed reducible until they encounter a attr class
  left_expr->_set_stmt_node_index(&_stmt_node_index, false, reducibleFlag);
  right_expr->_set_stmt_node_index(&_stmt_node_index, true, reducibleFlag);

  // the following function call ensures that all the generic
  // parameters that are appearing in this particular statement
  // are declared as static variables in the function that we are publishing

  left_expr->_build_generic_parameter_set(&_unique_generic_constants);
  right_expr->_build_generic_parameter_set(&_unique_generic_constants);
  
  PublishedUnit _temp_publishing_unit = _get_currently_publishing_unit();
  _set_currently_publishing_unit(SIMULTANEOUS_STATEMENT);

  _cc_out << "equationNode *" << NL();
  _cc_out << "ams_rhs_expr_";
  _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );
  _cc_out << "( component *currentEquation, bool &reducible ) {" << NL();
  
  left_expr->_publish_cc_ams_function_call_in_simult_stmt(_cc_out);
  right_expr->_publish_cc_ams_function_call_in_simult_stmt(_cc_out);
  
  //  to publish the generics in the component if any.
  for(TyvisDeclaration *current_generic_constant = _unique_generic_constants.getElement();
      current_generic_constant != NULL;
      current_generic_constant = _unique_generic_constants.getNextElement()) {
  
    current_generic_constant->_get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << " "
            << *current_generic_constant->_get_mangled_declarator()
            << " = (("<< _get_design_unit_name()
	    << " *)currentEquation->getEnclosingBlock())->"
            << *current_generic_constant->_get_mangled_declarator()
            << "_info;" << NL();
  }

  _cc_out << "  equationNode *";
  _cc_out << "node1 = new equationNode('O',2,NULL);" << NL();

  left_expr->_publish_cc_ams_function(_cc_out, declarations);
  right_expr->_publish_cc_ams_function(_cc_out, declarations);

  _cc_out <<"  reducible = ";
  if ( reducibleFlag == true ) {
   _cc_out << "true;" << NL();
  }
  else {
   _cc_out << "false;" << NL();
  }
  _cc_out <<"  return node1;" << NL();
  _cc_out <<" }" << NL();

  left_expr->_flush_stmt_index();
  right_expr->_flush_stmt_index();
        
  _set_currently_publishing_unit(_temp_publishing_unit);

}

void
TyvisSimpleSimultaneousStatement::_publish_cc_implicit_reference_equations(published_file &_cc_out, PublishData *declarations) {
  TyvisReferenceAttribute *current_reference_attribute = NULL;
  dl_list<TyvisReferenceAttribute> reference_quantity_list;
    
  Tyvis *lhs_expr = _get_left_expression();
  if( lhs_expr->_reference_quantity_found() == TRUE ) {
    lhs_expr->_build_reference_quantity_list(&reference_quantity_list);
  }  
  Tyvis *rhs_expr = _get_right_expression();
  if( rhs_expr->_reference_quantity_found() == TRUE ) {
    rhs_expr->_build_reference_quantity_list(&reference_quantity_list);
  }
  for(current_reference_attribute = reference_quantity_list.first();
      current_reference_attribute != NULL;
      current_reference_attribute =
        reference_quantity_list.successor(current_reference_attribute)) {
    _cc_out << "  equation = new freeEquation( this,parent_equation,-1,\"referEqn\",referFunction";
    _cc_out << ", &(";
    
    dynamic_cast<TyvisAttribute *>(current_reference_attribute)->_get_implicit_declaration()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "), ";
    if(current_reference_attribute->_get_prefix()->is_interface() == FALSE) {
      _cc_out << "&(";
      current_reference_attribute->_get_prefix()->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ")";
    }
    else {
      _cc_out << "(";
      current_reference_attribute->_get_prefix()->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ")"; 
    }
    _cc_out << ");" << NL();
  }
}

void
TyvisSimpleSimultaneousStatement::_publish_cc_implicit_contribution_equations(published_file &_cc_out, PublishData *declarations) {
  TyvisContributionAttribute *current_contribution_attribute = NULL;
  dl_list<TyvisContributionAttribute> contribution_quantity_list;
  Tyvis *lhs_expr = _get_left_expression();
  if( lhs_expr->_contribution_quantity_found() == TRUE ) {
    lhs_expr->_build_contribution_quantity_list(&contribution_quantity_list);
  }  
  Tyvis *rhs_expr = _get_right_expression();
  if( rhs_expr->_contribution_quantity_found() == TRUE ) {
    rhs_expr->_build_contribution_quantity_list(&contribution_quantity_list);
  } 
  for(current_contribution_attribute = contribution_quantity_list.first();
      current_contribution_attribute != NULL;
      current_contribution_attribute =
        contribution_quantity_list.successor(current_contribution_attribute)) {
    _cc_out << "  equation = new freeEquation( this,parent_equation,-1,\"contrEqn\",contribFunction";
    _cc_out << ", &(";
    
    dynamic_cast<TyvisAttribute *>(current_contribution_attribute)->_get_implicit_declaration()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "), ";
    if(current_contribution_attribute->_get_prefix()->is_interface() == FALSE) {
      _cc_out << "&(";
      current_contribution_attribute->_get_prefix()->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ")";
    }
    else {
      _cc_out << "(";
      current_contribution_attribute->_get_prefix()->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ")";
    }
    _cc_out << ");" << NL();
  }
}

void
TyvisSimpleSimultaneousStatement::_publish_cc_implicit_differential_equations( published_file &_cc_out, PublishData *declarations ) {
  TyvisDotAttribute *current_dot_attribute = NULL;
  dl_list<TyvisDotAttribute> differential_quantity_list;
  
  Tyvis *lhs_expr = _get_left_expression(); 
  if( lhs_expr->_differential_quantity_found() == TRUE ) {
    lhs_expr->_build_differential_quantity_list(&differential_quantity_list);
     }
  
  Tyvis *rhs_expr = _get_right_expression();
  if( rhs_expr->_differential_quantity_found() == TRUE ) {
         rhs_expr->_build_differential_quantity_list(&differential_quantity_list);
     }
  
  for(current_dot_attribute = differential_quantity_list.first();
      current_dot_attribute != NULL;
      current_dot_attribute =
	differential_quantity_list.successor(current_dot_attribute)) {
    
    _cc_out << "  equation = new differentialEquation( parent_equation,\"";
    _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "\", &(";
    dynamic_cast<TyvisAttribute *>(current_dot_attribute)->_get_implicit_declaration()->_publish_cc_lvalue( _cc_out , declarations );
    _cc_out << "), ";
    if(current_dot_attribute->_get_prefix()->is_interface() == FALSE) {
      _cc_out << "&(";
      current_dot_attribute->_get_prefix()->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ")";
    }
    else {
      _cc_out << "(";
      current_dot_attribute->_get_prefix()->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ")";
    }
    _cc_out << ");" << NL();
  }
}

void
TyvisSimpleSimultaneousStatement::_publish_cc_ams_equation_constructor( SimultaneousIfPublishingPart,
                                                                        published_file &_cc_out,
                                                                        PublishData *declarations){
  IIR_Int32 temp = _unique_qtys.size();
  TyvisDeclaration *current_set_qty = NULL;
  
  TyvisDeclaration *is2_Q2S = NULL;
  is2_Q2S  = _unique_signals.getElement();
      
  _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );
    
  _cc_out << "\", ";
  _cc_out << "ams_rhs_expr_";
  _get_mangled_label()->_publish_cc_lvalue( _cc_out , declarations );

  if(is2_Q2S != NULL) {
    _cc_out << ",NULL,NULL";
  }  
  current_set_qty = _unique_qtys.getElement();
  _cc_out << ", "<<_unique_qtys.size();
  if(_unique_qtys.size() > 0 ) {
    _cc_out << ", ";
  }
  for( current_set_qty = _unique_qtys.getElement();
       current_set_qty != NULL;
       current_set_qty = _unique_qtys.getNextElement() ) {
     
    if( temp > 0 ) {
      _cc_out << "&(";
      current_set_qty->_publish_cc_lvalue( _cc_out , declarations );
      _cc_out << ") ";
      if( temp > 1 ) {
        _cc_out << ", ";
      }
      temp--;
    }
  } 
  _cc_out << " );" << NL();
           
  // add all the signals that appeared in this simultaneous statement
  /* for(current_set_signal = _unique_signals.getElement();
      current_set_signal != NULL;
      current_set_signal = _unique_signals.getNextElement() ) {
    
   //_cc_out << "equationSignal = new signalDS(&(";
   //current_set_signal->_publish_cc_elaborate(_cc_out, declarations);
   // the signals in the stmt are now published as
   // heap allocated objects in the architecture class.
   // hence the leading & has been removed.
   _cc_out << "), (";
   current_set_signal->_publish_cc_lvalue( _cc_out , declarations );
   _cc_out << ") );\n";
   _cc_out << "equation->addSignal(equationSignal);\n";
   _cc_out << "add(equation);\n";
  } */  
}

IIR_Boolean
TyvisSimpleSimultaneousStatement::_is_simultaneous_statement() {
  return TRUE;
}

void
TyvisSimpleSimultaneousStatement::_publish_cc( published_file &, PublishData * ) {
  //  cgen_sym_tab_ptr = cgen_sym_tab;
}

Tyvis *
TyvisSimpleSimultaneousStatement::_get_left_expression() {
  return dynamic_cast<Tyvis *>(get_left_expression());
}

Tyvis *
TyvisSimpleSimultaneousStatement::_get_right_expression() {
  return dynamic_cast<Tyvis *>(get_right_expression());
}

IIR_Boolean
TyvisSimpleSimultaneousStatement::_is_free_quantity_present() {
  return _free_quantity_present;
}

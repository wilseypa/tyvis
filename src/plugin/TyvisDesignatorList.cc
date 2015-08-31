
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

#include "TyvisDesignatorList.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisDesignatorExplicit.hh"
#include "TyvisLabel.hh"
#include "TyvisSimpleName.hh"
#include "TyvisTextLiteral.hh"
#include "savant/error_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>

void
TyvisDesignatorList::_publish_cc_elements( published_file &_cc_out, PublishData *declarations ) {
  TyvisDesignator* element = dynamic_cast<TyvisDesignator *>(first());

  while(element != NULL) {
    element->_publish_cc_designator( _cc_out, declarations );
    _cc_out << "_";
    element = dynamic_cast<TyvisDesignator *>(successor(element));
  }
}

void
TyvisDesignatorList::_add_decl_into_declaration_collection( PublishData *declarations ) {
  TyvisDesignator* element = dynamic_cast<TyvisDesignator *>(first());
  while(element != NULL) {
    element->_add_decl_into_declaration_collection(declarations);
    element = dynamic_cast<TyvisDesignator *>(successor(element));
  }
}

void
TyvisDesignatorList::_get_list_of_input_signals(  savant::set<Tyvis> *list ){
  TyvisDesignator *element = dynamic_cast<TyvisDesignator *>(first());
  while( element != NULL) {
    element->_get_list_of_input_signals(list);
    element = dynamic_cast<TyvisDesignator *>(successor(element));
  }
}

// Modified by SK to copy the names from the argument to the list.  This
// may be better done by blindly copying the elements, but this involves
// some check.
void
TyvisDesignatorList::_add_signals_to_sensitivity_list(TyvisDesignatorList* sensitivity_list) {
  TyvisDesignator* sig_desig = dynamic_cast<TyvisDesignator *>(sensitivity_list->first());
  while(sig_desig != NULL) {
    ASSERT(sig_desig->get_kind() == IIR_DESIGNATOR_EXPLICIT);
    ASSERT(sig_desig->_get_name() != NULL);
    this->append(sig_desig);
    sig_desig = dynamic_cast<TyvisDesignator *>(sensitivity_list->successor(sig_desig));
  }
}

IIR_Boolean
TyvisDesignatorList::_is_component_in_the_list(TyvisTextLiteral* component_name) {
  TyvisDesignator* designator = dynamic_cast<TyvisDesignator *>(first());
  while(designator != NULL) {
    switch(designator->get_kind()) {
    case IIR_DESIGNATOR_EXPLICIT:
      ASSERT( dynamic_cast<TyvisDeclaration *>(designator->_get_name()) != NULL );
      if( IIRBase_TextLiteral::cmp(designator->_get_name()->_get_declarator(), component_name) == 0) {
	return TRUE;
      }
      break;
    case IIR_DESIGNATOR_BY_ALL:
    case IIR_DESIGNATOR_BY_OTHERS:
      return TRUE;
      break;

    default:
      break;
    }	// switch(designator->get_kind())
    designator = dynamic_cast<TyvisDesignator *>(successor(designator));
  } // while(designator != NULL)
  return FALSE;
}

void
TyvisDesignatorList::_build_above_attribute_set(savant::set<TyvisAboveAttribute> *to_build) {
 TyvisDesignator* element = dynamic_cast<TyvisDesignator *>(first());

  while(element != NULL) {
    element->_build_above_attribute_set(to_build);
    element = dynamic_cast<TyvisDesignator *>(successor(element));
  }
}

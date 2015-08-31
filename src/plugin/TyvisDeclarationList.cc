
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

#include "PublishData.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisBlockStatement.hh"
#include "TyvisConfigurationSpecification.hh"
#include "TyvisDeclaration.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDisconnectSpecification.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisFileDeclaration.hh"
#include "TyvisLibraryClause.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisNatureDeclaration.hh"
#include "TyvisScalarNatureDefinition.hh"
#include "TyvisSelectedName.hh"
#include "TyvisSubprogramDeclaration.hh"
#include "TyvisTerminalDeclaration.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisTypeDeclaration.hh"
#include "TyvisUseClause.hh"
#include "savant/error_func.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <sstream>
#include <set>
using std::set;

void 
TyvisDeclarationList::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  IIR_Boolean has_sub_prog = FALSE;

  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc" );

  // First collect all the includes needed for this declartive region
  for (TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(first());
       decl != NULL;decl = dynamic_cast<TyvisDeclaration *>(successor(decl))) {
    switch (decl->get_kind()) {
    case IIR_PROCEDURE_DECLARATION:
    case IIR_FUNCTION_DECLARATION:
      has_sub_prog = TRUE;
      break;
    default:
      break;
    }
  }

  // If the declarative region has sub programs then include these stuff
  if (has_sub_prog == TRUE) {
    _publish_cc_include( _cc_out, "tyvis/VHDLProcess.hh" );
  }
  // Walk through the collected list and publish the headers
  for (TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(first());
       decl != NULL;decl = dynamic_cast<TyvisDeclaration *>(successor(decl))) {
    switch( decl->get_kind() ){
    case IIR_COMPONENT_DECLARATION:
      decl->_publish_cc_include( _cc_out );
      break;
    case IIR_ATTRIBUTE_SPECIFICATION:
      _cc_out << "extern ";
      decl->_publish_cc_decl( _cc_out , declarations );
      break;
    case IIR_PROCEDURE_DECLARATION:
    case IIR_FUNCTION_DECLARATION:
      (dynamic_cast<TyvisSubprogramDeclaration *>(decl))->_publish_cc_prototype( _cc_out, declarations );
      break;
    case IIR_USE_CLAUSE:
      decl->_publish_cc_headers( _cc_out );
      break;
    case IIR_CONSTANT_DECLARATION:
      _publish_cc_include( _cc_out, "tyvis/Variable.hh" );
      if (decl->get_declarative_region()->get_kind() == IIR_ENTITY_DECLARATION
	  || decl->get_declarative_region()->get_kind() == IIR_ARCHITECTURE_DECLARATION) {
	_cc_out << "extern Variable& " << *decl->_get_mangled_declarator() << "Func();" << NL();
      } else {
	_cc_out << "extern Variable " << *decl->_get_mangled_declarator() << ";" << NL();
      }
      break;
    default:
      break;
    }
  }
}

void
TyvisDeclarationList::_publish_cc_constant_declarations( published_file &_cc_out,
							 PublishData *declarations ){
  PublishedUnit oldUnit = _get_currently_publishing_unit();
  for (TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(first());
       decl != NULL;decl = dynamic_cast<TyvisDeclaration *>(successor(decl))) {
    if( decl->is_constant() ){
      CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_constant_declaration" );
      decl->_publish_cc_decl( _cc_out, declarations );
    }
  }
  
  _set_currently_publishing_unit(oldUnit);
}

IIR_Boolean
TyvisDeclarationList::_publish_cc_constant_definitions( published_file &_cc_out,
                                                        PublishData *declarations,
                                                        IIR_Boolean firstDeclFlag ){
  PublishedUnit oldUnit = _get_currently_publishing_unit();
  IIR_Boolean localFirstDecl = firstDeclFlag;

  for (TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(first());
       decl != NULL;decl = dynamic_cast<TyvisDeclaration *>(successor(decl))) {
    switch (decl->get_kind()) {
    case IIR_CONSTANT_DECLARATION:
      if( localFirstDecl == TRUE ){
	_cc_out << " : ";
      }
      else {
	_cc_out << "," << NL();
      }
      localFirstDecl = FALSE;
      _cc_out << *decl->_get_mangled_declarator();
      decl->_publish_cc_constructor_args( _cc_out, declarations );

      break;
      
    default:
      break;
    }
  }
  
  _set_currently_publishing_unit(oldUnit);
  
  return localFirstDecl;
}

void 
TyvisDeclarationList::_publish_cc_decl( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_decl" );

  for (TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(first());
       decl != NULL;decl = dynamic_cast<TyvisDeclaration *>(successor(decl))) {
    switch (decl->get_kind()) {
     case IIR_CONSTANT_DECLARATION:
       decl->_publish_cc_decl_with_constructor_args( _cc_out, declarations );
       break;
       
    case IIR_COMPONENT_DECLARATION:
      decl->_publish_cc_elaborate( _cc_out, declarations );
      CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_decl" );
      break;
      
    case IIR_ATTRIBUTE_SPECIFICATION:
      dynamic_cast<TyvisAttributeSpecification*>(decl)->_publish_cc_decl_with_constructor( _cc_out, declarations );
      break;

    case IIR_PROCEDURE_DECLARATION:
    case IIR_FUNCTION_DECLARATION:
      decl->_publish_cc_decl( _cc_out, declarations );
      break;

    case IIR_QUANTITY_DECLARATION:
    case IIR_FREE_QUANTITY_DECLARATION:
    case IIR_BRANCH_QUANTITY_DECLARATION:
    case IIR_SOURCE_QUANTITY_DECLARATION:
      break;
      
    case IIR_NATURE_DECLARATION: {
      if ( dynamic_cast<TyvisNatureDeclaration *>(decl)->get_nature()->get_kind() != 
	   IIR_ARRAY_SUBNATURE_DEFINITION ){
        _cc_out << "AMSType ";
        dynamic_cast<TyvisNatureDeclaration*>(decl)->_get_reference_terminal()->_publish_cc_lvalue(_cc_out, declarations);
        _cc_out << "(ObjectBase::TERMINAL, \"";
        dynamic_cast<TyvisNatureDeclaration*>(decl)->_get_reference_terminal()->_publish_cc_lvalue(_cc_out, declarations);
        _cc_out << "\");" << NL();
      }
      else { 
	// found an array nature declaration.
	ostringstream err;                                                                 
	err << "TyvisDeclarationList::_publish_cc_decl. Unimplemented declaration type: " << decl->get_kind_text();
	report_error( decl, err.str() );
	abort();
      }
    }
      break;

    case IIR_SIGNAL_DECLARATION:
    case IIR_VARIABLE_DECLARATION:
    case IIR_SIGNAL_INTERFACE_DECLARATION:
    case IIR_VARIABLE_INTERFACE_DECLARATION:
    case IIR_FILE_DECLARATION:
    case IIR_CONFIGURATION_SPECIFICATION:
    case IIR_TYPE_DECLARATION:
    case IIR_SUBTYPE_DECLARATION:
    case IIR_ALIAS_DECLARATION:
    case IIR_ATTRIBUTE_DECLARATION:
    case IIR_USE_CLAUSE:
    case IIR_TERMINAL_DECLARATION:
    default:
      break;
    }
  }
}

void 
TyvisDeclarationList::_publish_cc_package_declarations( published_file &_cc_out,
							PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_package_declarations" );
  _publish_cc_list( _cc_out, declarations, 
		    &Tyvis::_publish_cc_package_decl,
		    NO_SEPARATOR, "" );
}

void 
TyvisDeclarationList::_publish_cc_package_definitions( published_file &_cc_out,
						       PublishData *declarations ){
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_package_definitions" );
  _publish_cc_list( _cc_out, declarations, 
		    &Tyvis::_publish_cc_package_definition,
		    NO_SEPARATOR, "" );
}

void 
TyvisDeclarationList::_publish_cc_decl_subprograms_and_types( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_decl_subprograms_and_types" );

  TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(first());
  while( decl != NULL ){
    if( dynamic_cast<TyvisSubprogramDeclaration *>(decl) != NULL ){
      decl->_publish_cc_decl( _cc_out , declarations );
    }
    if( decl->is_type() == TRUE ){
      decl->_publish_cc_static_type_info_fn( _cc_out, declarations );
    }
    decl = dynamic_cast<TyvisDeclaration *>(successor(decl));
  }
}  

void 
TyvisDeclarationList::_publish_cc_prototype( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_prototype" );

  TyvisDeclaration *decl = dynamic_cast<TyvisDeclaration *>(first());
  while( decl != NULL ){
    if( dynamic_cast<TyvisSubprogramDeclaration *>(decl) != NULL ){
      (dynamic_cast<TyvisSubprogramDeclaration *>(decl))->_publish_cc_prototype( _cc_out, declarations );
    }
    decl = dynamic_cast<TyvisDeclaration *>(successor(decl));
  }
}  

void
TyvisDeclarationList::_publish_cc_constants( published_file &_cc_out, PublishData *declarations ){
  TyvisDeclaration *current_declaration = dynamic_cast<TyvisDeclaration *>(first());
  
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_constants" );
  
  while( current_declaration != NULL ){
    if( current_declaration->is_constant() == TRUE ){
      current_declaration->_publish_cc_decl( _cc_out, declarations );
    }
    current_declaration = dynamic_cast<TyvisDeclaration *>(successor(current_declaration));
  }
}

void
TyvisDeclarationList::_publish_cc_global_constants_assignments( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_global_constants_assignments" );
  
  for (TyvisDeclaration *current_declaration = dynamic_cast<TyvisDeclaration *>(first());
       current_declaration != NULL;
       current_declaration = dynamic_cast<TyvisDeclaration *>(successor(current_declaration))) {
    if (current_declaration->get_kind() == IIR_CONSTANT_DECLARATION) {
      if( _get_value() != 0 ){
        _cc_out << "::"
                << *current_declaration->_get_mangled_declarator()
                << " = ";
        _get_value()->_publish_cc_rvalue( _cc_out, declarations );
      }
    }
  }
}

void
TyvisDeclarationList::_publish_cc_file_objects( published_file &_cc_out,
						PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_file_objects" );
  std::set<IIR_Declaration*> *decl_set = declarations->get_set(TyvisDeclaration::S_FILE);
  std::set<IIR_Declaration*>::iterator iter = decl_set->begin();
  while( iter != decl_set->end() ){        
    TyvisFileDeclaration *decl = dynamic_cast<TyvisFileDeclaration *>(*iter);
    decl->_publish_cc_shared_file_decl( _cc_out, declarations, "" );
    iter++;
  }
}

void
TyvisDeclarationList::_publish_cc_file_objects_init( published_file &,
						     PublishData * ){}

IIR_Boolean
TyvisDeclarationList::_publish_cc_constants_init( published_file &_cc_out, 
                                                         PublishData *declarations,
                                                         IIR_Boolean firstDecl) {
  IIR_Boolean     flag = FALSE;
  
  CC_REF( _cc_out, "TyvisDeclarationList::_publish_cc_constants_init" );

  for (TyvisDeclaration *current_declaration = dynamic_cast<TyvisDeclaration *>(first());
       current_declaration != NULL;
       current_declaration = dynamic_cast<TyvisDeclaration *>(successor(current_declaration))) {
    switch (current_declaration->get_kind()) {
    case IIR_CONSTANT_DECLARATION:
      if( firstDecl == TRUE ){
	_cc_out << " : ";
	firstDecl = FALSE;
      }
      else {
	_cc_out << "," << NL();
      }
      _cc_out << *current_declaration->_get_mangled_declarator();
      current_declaration->_publish_cc_constructor_args( _cc_out, declarations );
      flag = TRUE;
      break;
    default:
      break;
    }
  }
  return firstDecl;
}

bool
TyvisDeclarationList::_contains_foreign_attribute(){
  bool retval = false;
   savant::set<TyvisAttributeSpecification> *foreign_attributes = _find_foreign_attributes();
  if( foreign_attributes != 0 ){
    retval = true;
    delete foreign_attributes;
  }
  return retval;
}

 savant::set<TyvisAttributeSpecification> *
TyvisDeclarationList::_find_foreign_attributes(){
   savant::set<TyvisAttributeSpecification> *retval = 0;

  for (TyvisDeclaration *current = dynamic_cast<TyvisDeclaration *>(first());
       current != NULL;current = dynamic_cast<TyvisDeclaration *>(successor(current))) {
    if( current->_is_foreign_attribute_specification() == TRUE ){
      if( retval == 0 ){
	retval = new  savant::set<TyvisAttributeSpecification>;
      }
      ASSERT( current->get_kind() == IIR_ATTRIBUTE_SPECIFICATION );
      retval->add(dynamic_cast<TyvisAttributeSpecification *>(current) );
    }
  }
  return retval;
}

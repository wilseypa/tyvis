
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

#include "TyvisGenericList.hh"
#include "TyvisConstantInterfaceDeclaration.hh"
#include "TyvisTypeDefinition.hh"
#include "TyvisIdentifier.hh"
#include "published_file.hh"

void
TyvisGenericList::_publish_cc_elaborate_as_pointers( published_file &_cc_out,
						     PublishData *declarations,
						     IIR_Boolean publishDefinition ){
  TyvisConstantInterfaceDeclaration *i =
    dynamic_cast<TyvisConstantInterfaceDeclaration *>(first()); 
  while( i != NULL ){
    i->_publish_cc_elaborate_as_pointers( _cc_out, declarations, publishDefinition );
    i = dynamic_cast<TyvisConstantInterfaceDeclaration *>(successor(i));
  }
}

void
TyvisGenericList::_publish_cc_copy_generics_to_globals( published_file &_cc_out,
							PublishData * ) {
  CC_REF( _cc_out, "TyvisGenericList::_publish_cc_copy_generics_to_globals" );;
  TyvisConstantInterfaceDeclaration *genericClause =
    dynamic_cast<TyvisConstantInterfaceDeclaration *>(first());
  ASSERT ( genericClause != NULL );
  if (genericClause->_get_declarative_region()->get_kind() !=
      IIR_ENTITY_DECLARATION) {
    _cc_out << "if( enclosingScope != NULL )" << OS("{")
	    << "enclosingScope->copyGenericsToGlobals();" << NL()
	    << CS("}");
  }
  do {
    _cc_out << "::"
            << *genericClause->_get_mangled_declarator()
            // Func() is the TyvisConstantInterfaceDeclaration global wrapper function
            << "Func().updateVal( this->"
            << *genericClause->_get_mangled_declarator()
            << ".readVal() );"
	    << NL();

    genericClause = dynamic_cast<TyvisConstantInterfaceDeclaration *>(successor(genericClause));
  } while (genericClause != NULL);
}


void
TyvisGenericList::_publish_generic_parameter_list( published_file &_cc_out,
						   PublishData * ){
  TyvisConstantInterfaceDeclaration *current_generic =
    dynamic_cast<TyvisConstantInterfaceDeclaration *>(first());
  while( current_generic != NULL ) {
    CC_REF( _cc_out, "TyvisGenericList::_publish_generic_parameter_list" );
    _cc_out << "const RValue &" 
	    << *current_generic->_get_mangled_declarator() << "_arg";
    current_generic =
      dynamic_cast<TyvisConstantInterfaceDeclaration*>(successor(current_generic));
    if( current_generic != NULL ){
      _cc_out << ",";
    }
    _cc_out << NL();
  }
}

void
TyvisGenericList::_publish_generic_parameters_notypes( published_file &_cc_out, PublishData *declarations ){
  TyvisConstantInterfaceDeclaration *i = dynamic_cast<TyvisConstantInterfaceDeclaration *>(first());
  while( i != NULL ) {
    CC_REF( _cc_out, 
		  "TyvisGenericList::_publish_generic_parameters_notypes" );
    i->_get_mangled_declarator()->_publish_cc_elaborate( _cc_out, declarations );
    _cc_out << "_arg";
    i = dynamic_cast<TyvisConstantInterfaceDeclaration *>(successor(i));
    if(i!= NULL) {
      _cc_out << ",";
    }
    _cc_out << NL();
  }
}

void
TyvisGenericList::_publish_generic_init( published_file &_cc_out, PublishData *declarations ){
  TyvisConstantInterfaceDeclaration* i = NULL;
  i = dynamic_cast<TyvisConstantInterfaceDeclaration *>(first()); 
  while( i != NULL ) {
    if(_get_currently_publishing_unit() == Tyvis::ENTITY_DECL) {
      i->_get_subtype()->_publish_cc_elaborate( _cc_out, declarations );
      _cc_out << " ";
    }
    
    i->_get_mangled_declarator()->_publish_cc_elaborate( _cc_out, declarations );
    i->_publish_cc_constructor_args( _cc_out, declarations );
    i = dynamic_cast<TyvisConstantInterfaceDeclaration *>(successor(i));
    if(_get_currently_publishing_unit() == Tyvis::ENTITY_DECL) {
      _cc_out << ";";
    }
    else {
      if(i != NULL) {
	_cc_out << ",";
      }
    }
    _cc_out << NL();
  } 
}

void
TyvisGenericList::_publish_generic_init_by_arguments( published_file &_cc_out,
						      PublishData *declarations ){
  CC_REF( _cc_out, "TyvisGenericList::_publish_generic_init_by_arguments" );  
  TyvisConstantInterfaceDeclaration *i =
    dynamic_cast<TyvisConstantInterfaceDeclaration *>(first()); 
  while( i != NULL ){
    _cc_out << *i->_get_mangled_declarator() << "(" <<
    	    *i->_get_mangled_declarator() << "_arg)";
    i = dynamic_cast<TyvisConstantInterfaceDeclaration *>(successor(i));
    if(i != NULL) {
      _cc_out << "," << NL();
    }
  }
}


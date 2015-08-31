
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

#include "Tyvis.hh"
#include "TyvisArraySubtypeDefinition.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisIdentifier.hh"
#include "TyvisTypeDeclaration.hh"
#include "TyvisAttribute.hh"
#include "TyvisIntegerLiteral.hh"
#include "TyvisFunctionDeclaration.hh"
#include "published_file.hh"
#include "TyvisScalarNatureDefinition.hh"

IIR_Boolean 
TyvisArraySubtypeDefinition::_is_globally_static(){
  IIR_Boolean retval = TRUE;

  if( is_unconstrained_array_type() == TRUE ){
    retval = FALSE;
  }
  else{
    TyvisScalarTypeDefinition *current_index = _get_index_subtype();
    if( current_index->_is_globally_static() == FALSE ){
      retval = FALSE;
    }
    TyvisTypeDefinition *current_element_type = _get_element_subtype();
    ASSERT( current_element_type != NULL );
    while( dynamic_cast<TyvisArrayTypeDefinition *>(current_element_type) && 
	   current_element_type->is_element() == FALSE ){
      TyvisArrayTypeDefinition *as_array_type = dynamic_cast<TyvisArrayTypeDefinition *>(current_element_type);
      ASSERT( as_array_type->_get_index_subtype() != NULL );
      if( as_array_type->_get_index_subtype()->_is_globally_static() == FALSE ){
	retval = FALSE;
	break;
      }
      current_element_type = as_array_type->_get_element_subtype();
      ASSERT( current_element_type != NULL );
    }
  }

  return retval;
}

TyvisFunctionDeclaration * 
TyvisArraySubtypeDefinition::_get_resolution_function() { 
  return dynamic_cast<TyvisFunctionDeclaration *>(get_resolution_function()); 
}

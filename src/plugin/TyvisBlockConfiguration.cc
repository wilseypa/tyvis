
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
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisBlockConfiguration.hh"
#include "TyvisComponentConfiguration.hh"
#include "TyvisConfigurationItemList.hh"
#include "TyvisConfigurationDeclaration.hh"
#include "TyvisConcurrentGenerateIfStatement.hh"
#include "TyvisConcurrentGenerateForStatement.hh"
#include "TyvisDeclarationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisIndexedName.hh"
#include "TyvisIntegerSubtypeDefinition.hh"
#include "TyvisLabel.hh"
#include "TyvisSliceName.hh"
#include "TyvisStatement.hh"

#include "savant/StandardPackage.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "published_file.hh"
#include <sstream>

#include <iostream>
using std::cerr;

TyvisBlockConfiguration::TyvisBlockConfiguration() {
  set_use_clause_list(new TyvisDeclarationList());
  set_configuration_item_list(new TyvisConfigurationItemList());  
}

TyvisBlockConfiguration::~TyvisBlockConfiguration(){
  //Release the list memory
  delete get_use_clause_list();
  delete get_configuration_item_list();
}

void
TyvisBlockConfiguration::_publish_cc_headers( published_file &_cc_out ) {

  CC_REF( _cc_out, "TyvisBlockConfiguration::_publish_cc_headers" );

  _get_block_specification()->_publish_cc_include( _cc_out );
  _get_use_clause_list()->_publish_cc_headers( _cc_out );
  _get_configuration_item_list()->_publish_cc_headers( _cc_out );
}

void
TyvisBlockConfiguration::_publish_cc_object_pointers_init( published_file &_cc_out, PublishData *declarations ) {

  CC_REF( _cc_out, "TyvisBlockConfiguration::_publish_cc_object_pointers_init" );

  _cc_out << "{" << _get_block_specification()->_get_cc_elaboration_class_name()
	  << " *enclosingArch = configuredEntity;\n"
	  << _get_block_specification()->_get_cc_elaboration_class_name()
	  << " *enclosingArch1 = enclosingArch;\n"
	  << "{\n";
  _get_configuration_item_list()->_publish_cc_configure_block( _cc_out, declarations );
  _cc_out << "};\n";
  _cc_out << "};\n";
}

void
TyvisBlockConfiguration::_publish_cc_configure_block( published_file &_cc_out,
                                                      PublishData *declarations,
                                                      IIR_Boolean ){
  TyvisConcurrentGenerateIfStatement  *tempGenerateIfStmt;
  TyvisConcurrentGenerateForStatement *tempGenerateForStmt;
  TyvisSliceName                      *tempSlice;
  TyvisLabel                          *mangled_label = NULL;

  CC_REF( _cc_out, "TyvisBlockConfiguration::_publish_cc_configure_block" );

  if((_get_block_specification()->_is_block_statement() == TRUE) ||
     (_get_block_specification()->_is_concurrent_generate_statement() == TRUE)) {
    
    if (_get_block_specification()->get_kind() == IIR_LABEL) {
      if ((dynamic_cast<TyvisLabel *> (_get_block_specification()))->get_statement()->get_kind() == IIR_CONCURRENT_GENERATE_IF_STATEMENT) {
	tempGenerateIfStmt = dynamic_cast<TyvisConcurrentGenerateIfStatement *>( (dynamic_cast<TyvisLabel *>(_get_block_specification()))->get_statement());
	ASSERT ( tempGenerateIfStmt->get_kind() == IIR_CONCURRENT_GENERATE_IF_STATEMENT );
	tempGenerateIfStmt->_publish_cc_generate_condition( _cc_out, declarations );
      }
      if ((dynamic_cast<TyvisLabel *> (get_block_specification()))->get_statement()->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT) {
	tempGenerateForStmt = dynamic_cast<TyvisConcurrentGenerateForStatement *>( (dynamic_cast<TyvisLabel *>(get_block_specification()))->get_statement());
	ASSERT(tempGenerateForStmt->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT);
	tempGenerateForStmt->_publish_cc_generate_for_loop_with_zero( _cc_out,
                                                                      declarations,
								      "generateConstant" );
      }
    }
    else if (get_block_specification()->get_kind() == IIR_SLICE_NAME) {
      tempSlice = dynamic_cast<TyvisSliceName *>(_get_block_specification());
      ASSERT ( tempSlice->get_kind() == IIR_SLICE_NAME );
      tempSlice->_publish_cc_slice_name_as_for_loop( _cc_out, declarations );
    }
    else if (get_block_specification()->get_kind() == IIR_INDEXED_NAME) {
      (dynamic_cast<TyvisIndexedName *>(_get_block_specification()))->_publish_cc_index_name_as_for_loop( _cc_out, declarations );
    }
    else {
      // The case we have hit on so far is a slice name
      // Don't know if anything else is going to pop up. So will put a
      // check for some extraneous case that we have not hit upon so far.
      
      cerr << "Warning : " << _get_block_specification()->get_kind_text()
	   << " Not yet handled in block configurations\n";
    }
      
    _cc_out << "{\n"
	    << _get_block_specification()->_get_cc_elaboration_class_name()
	    << " *enclosingArch = enclosingArch1->";
    
    if (_get_block_specification()->get_kind() == IIR_LABEL) {
      mangled_label = (dynamic_cast<TyvisLabel*>(get_block_specification()))->_get_statement()->_get_mangled_label();
    }
    else if (_get_block_specification()->get_kind() == IIR_SLICE_NAME) {
      mangled_label = dynamic_cast<TyvisLabel *>((dynamic_cast<TyvisSliceName *>(get_block_specification()))->get_prefix());
      mangled_label = mangled_label->_get_statement()->_get_mangled_label();
    }
    else {
      cerr << "Warning : " << _get_block_specification()->get_kind_text()
	   << " Not yet handled in block configurations\n";
    }

    ASSERT ( mangled_label != NULL );
    ASSERT ( mangled_label->get_kind() == IIR_LABEL );
    
    mangled_label->_publish_cc_elaborate( _cc_out.get_stream() );
    _cc_out << "_elab_obj;\n"
	    << _get_block_specification()->_get_cc_elaboration_class_name()
	    << " *enclosingArch1 = enclosingArch;\n";
  }


  if (_get_block_specification()->get_kind() == IIR_SLICE_NAME) {
    _get_configuration_item_list()->_publish_cc_configure_block( _cc_out, declarations, TRUE );
  }
  else if (_get_block_specification()->get_kind() == IIR_LABEL && (dynamic_cast<TyvisLabel *>(_get_block_specification()))->get_statement()->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT){
    _get_configuration_item_list()->_publish_cc_configure_block( _cc_out, declarations, TRUE );
  }
  else {
    _get_configuration_item_list()->_publish_cc_configure_block( _cc_out, declarations, FALSE );
  }
  
  if((_get_block_specification()->_is_block_statement() == TRUE) ||
     (_get_block_specification()->_is_concurrent_generate_statement() == TRUE)) {
    _cc_out << "};\n";
  }
}

Tyvis*
TyvisBlockConfiguration::_get_block_specification() {
  return dynamic_cast<Tyvis *>(get_block_specification());
}

TyvisDeclarationList *
TyvisBlockConfiguration::_get_use_clause_list() {
  return dynamic_cast<TyvisDeclarationList *>(get_use_clause_list());
}

TyvisConfigurationItemList *
TyvisBlockConfiguration::_get_configuration_item_list() {
  return dynamic_cast<TyvisConfigurationItemList *>(get_configuration_item_list());
}

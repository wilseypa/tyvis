
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

#include "TyvisAttributeSpecification.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisLabel.hh"
#include "TyvisTextLiteral.hh"
#include "TyvisStatement.hh"
#include "published_file.hh"
#include <sstream>

TyvisLabel::TyvisLabel() {
  set_attributes(new TyvisAttributeSpecificationList());
}

TyvisLabel::~TyvisLabel() {
  //Release the list memory
  delete get_attributes();
}

void 
TyvisLabel::_publish_cc_elaborate( published_file &_cc_out, PublishData * ){
  _publish_cc_elaborate( _cc_out.get_stream() );
}

void 
TyvisLabel::_publish_cc_elaborate( ostream &_cc_out ){
  _cc_out << *_get_mangled_declarator();
}

void 
TyvisLabel::_publish_cc_lvalue( published_file &_cc_out, PublishData * ) {
  _cc_out << *_get_mangled_declarator();
}

void
TyvisLabel::_publish_cc_binding_name(ostream& outstream) {
  _get_statement()->_publish_cc_binding_name(outstream);
}

void
TyvisLabel::_publish_cc_include( published_file &_cc_out ){
  ostringstream file_name_stream;
  if (get_statement() == NULL) {
    file_name_stream << *(_get_mangled_declarator()) << ".hh";
  }
  else {
    _get_statement()->_publish_cc_class_name(file_name_stream);
    file_name_stream << ".hh";
  }
  
  Tyvis::_publish_cc_include( _cc_out, file_name_stream.str() );
}

TyvisList *
TyvisLabel::_get_statement_list(){
  ASSERT( _get_statement() != NULL );
  return _get_statement()->_get_statement_list();
}

TyvisAttributeSpecificationList *
TyvisLabel::_get_attributes() {
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

IIR_Boolean
TyvisLabel::_is_block_statement() {
  return _get_statement()->_is_block_statement();
}

IIR_Boolean
TyvisLabel::_is_concurrent_generate_statement() {
  return _get_statement()->_is_concurrent_generate_statement();
}


TyvisStatement* 
TyvisLabel::_get_statement() {
  return dynamic_cast<TyvisStatement *>(get_statement());
}

TyvisAttributeSpecificationList* 
TyvisLabel::_get_attribute_specification_list() {
  return dynamic_cast<TyvisAttributeSpecificationList *>(get_attributes());
}

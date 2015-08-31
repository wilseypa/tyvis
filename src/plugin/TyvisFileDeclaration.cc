
// Copyright (c) The University of Cincinnati.
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

#include "TyvisAttribute.hh"
#include "TyvisAttributeSpecification.hh"
#include "TyvisAttributeSpecificationList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisFileDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLabel.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisPackageBodyDeclaration.hh"
#include "TyvisProcessStatement.hh"
#include "TyvisSimpleName.hh"
#include "TyvisTypeDefinition.hh"
#include "published_file.hh"
#include <sstream>
#include "PublishData.hh"

void
TyvisFileDeclaration::_publish_cc_get_file_signature( published_file &_cc_out,
						      const string &className ){
  _publish_cc_include( _cc_out, "tyvis/FileObject.hh" );
  _cc_out << "FileObject &" << NL();
  if( className != "" ){
    _cc_out << className << "::";
  }
  _cc_out << *_get_mangled_declarator() << "Object()";
}

void
TyvisFileDeclaration::_publish_cc_package_decl( published_file &_cc_out,
						PublishData *declarations ){
  _publish_cc_decl( _cc_out, declarations );
}

void
TyvisFileDeclaration::_publish_cc_package_definition( published_file &_cc_out,
						      PublishData *declarations ){
  _publish_cc_shared_file_decl( _cc_out, declarations );
}

void
TyvisFileDeclaration::_publish_cc_decl( published_file &_cc_out,
					PublishData * ){
  _publish_cc_get_file_signature( _cc_out, "" );
  _cc_out << ";" << NL();
}

void 
TyvisFileDeclaration::_publish_cc_lvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisFileDeclaration::_publish_cc" );

  ASSERT (declarations != NULL);

  if( (!declarations->in_collection(this)) &&
      _get_declarative_region()->get_kind() != IIR_PACKAGE_DECLARATION &&
      _get_declarative_region()->get_kind() != IIR_PACKAGE_BODY_DECLARATION &&
      _get_declarative_region()->get_kind() != IIR_ARCHITECTURE_DECLARATION ){
    declarations->add_declaration( this );
  }

  if (((_get_declarative_region()->get_kind() == IIR_ARCHITECTURE_DECLARATION) ||
       ( dynamic_cast<TyvisConcurrentStatement *>(_get_declarative_region()) )) &&
      (_get_declarative_region()->get_kind() != IIR_PROCESS_STATEMENT)) {
    ostringstream prefix;
    TyvisProcessStatement* process_ptr = _get_current_process_statement();
    // Need a different scope for getting files in architectures and blocks etc..
    if( _get_currently_publishing_unit() == PROCESS ){
      prefix << "(("
	     << process_ptr->_get_declarative_region()->_get_cc_elaboration_class_name()
	     << " *)getProc())->";
    }
    else {
      prefix << "((" << *(process_ptr->_get_mangled_label()) << " *) processPtr)->";
    }
    
    process_ptr->_publish_cc_scoping_prefix( prefix, _get_declarative_region(), process_ptr );
    _set_publish_prefix_string(prefix.str());
  }
  else {
    _set_scoping_prefix();
  }
  
  //  ASSERT(_get_file_logical_name() != NULL);

  // Ditto, this breaks process statements
  if (//(_get_declarative_region()->_is_iir_concurrent_statement() == FALSE) &&
      (_get_declarative_region()->get_kind() != IIR_PACKAGE_DECLARATION) &&
      (_get_declarative_region()->get_kind() != IIR_PACKAGE_BODY_DECLARATION)) {
    _publish_cc_prefix_string( _cc_out );
  }
  //DRH - () not needed in procedure declaration _cc_out << *_get_mangled_declarator() << "Object()";
  _cc_out << *_get_mangled_declarator() << "Object";
  if (_get_declarative_region()->get_kind() != IIR_PROCEDURE_DECLARATION) {
    _cc_out << "()";
  }

  _reset_scoping_prefix();
}

void
TyvisFileDeclaration::_publish_cc_shared_file_decl( published_file &_cc_out,
						    PublishData *declarations,
						    const string &className ) {
  _publish_cc_get_file_signature( _cc_out, className );
  _cc_out << OS("{")
	  << "static FileObject *" << *_get_mangled_declarator() << " = new "
	  << _get_cc_object_type();
  _publish_cc_constructor_args( _cc_out, declarations );
  _cc_out << ";" << NL()
	  << "return *" << *_get_mangled_declarator() << ";"
	  << CS("}");
}

void
TyvisFileDeclaration::_publish_cc_constructor_args( published_file &_cc_out,
						    PublishData *declarations ){
  CC_REF( _cc_out, "TyvisFileDeclaration::_publish_cc_constructor_args" );
  _cc_out << OS("(");
  _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
  if ( _get_file_logical_name() != NULL) { // the file declaration is not yet associated to a file
    _cc_out << "," << NL();
    //DRH - need mangled_declarator when not a string
    //_cc_out << *_get_file_logical_name() << "," << NL();
    if( _get_file_logical_name()->get_kind() != IIR_STRING_LITERAL ){
      _cc_out << *_get_declarative_region()->_get_mangled_declarator() << "_" << _get_mangling_prefix();
    }
    _cc_out <<  *_get_file_logical_name() << "," << NL();
    _get_file_open_expression()->_publish_cc_rvalue( _cc_out, declarations );
  }
  _cc_out << CS(")");
}

void
TyvisFileDeclaration::_publish_cc_state_object_init( published_file &_cc_out,
						     PublishData *declarations ){
  _publish_cc_declarator_with_constructor_args( _cc_out, declarations );
}

void
TyvisFileDeclaration::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisFileDeclaration::_publish_cc_include( published_file &_cc_out ){
  _publish_cc_include( _cc_out, "tyvis/FileObject.hh" );
}


IIR_Boolean
TyvisFileDeclaration::_is_shared_file() {
  ASSERT (_get_declarative_region() != NULL);
  Tyvis *declRegn = _get_declarative_region();

  if ((dynamic_cast<TyvisPackageDeclaration *>(declRegn) != NULL) ||
      (dynamic_cast<TyvisPackageBodyDeclaration *>(declRegn) != NULL) ||
      (declRegn->get_kind() == IIR_ARCHITECTURE_DECLARATION)  ||
      (dynamic_cast<TyvisConcurrentStatement *>(declRegn) != NULL)) {
    return TRUE;
  }

  return  FALSE;
}

Tyvis *
TyvisFileDeclaration::_get_file_logical_name() {
  return dynamic_cast<Tyvis *>(get_file_logical_name());
}

Tyvis *
TyvisFileDeclaration::_get_file_open_expression() {
  return dynamic_cast<Tyvis *>(get_file_open_expression());
}

const string
TyvisFileDeclaration::_get_cc_object_type() {
  return "FileObject";
}

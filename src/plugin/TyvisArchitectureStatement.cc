
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

#include "TyvisArchitectureStatement.hh"
#include "TyvisArchitectureStatementList.hh"
#include "TyvisAssociationList.hh"
#include "TyvisComponentInstantiationStatement.hh"
#include "TyvisDeclaration.hh"
#include "TyvisDesignFile.hh"
#include "TyvisEnumerationSubtypeDefinition.hh"
#include "TyvisLabel.hh"
#include "TyvisPackageDeclaration.hh"
#include "TyvisSignalDeclaration.hh"
#include "TyvisSimpleSimultaneousStatement.hh"

#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/StandardPackage.hh"
#include "published_file.hh"
#include <sstream>

#include <iostream>
using std::cerr;

TyvisArchitectureStatement::TyvisArchitectureStatement() {
  enclosing_path = "";
}

TyvisArchitectureStatement::~TyvisArchitectureStatement() {
}

void 
TyvisArchitectureStatement::_publish_cc_state( published_file & ){
  _report_undefined_fn("_publish_cc_state( _cc_out )");
}

void 
TyvisArchitectureStatement::_publish_cc_headerfiles_for_cc_generate_statement( published_file &_cc_out ){
  const string temp = _get_current_architecture_name();
  const string old_current_name = _get_current_publish_name();
  TyvisComponentInstantiationStatement *compInst = NULL;

  CC_REF( _cc_out, "TyvisArchitectureStatement::_publish_cc_headerfiles_for_cc_generate_statement" );
  
  TyvisArchitectureStatement *arch_stmt = NULL;
  Tyvis *stmt = dynamic_cast<Tyvis *>(_get_statement_list()->first());

  while( stmt != NULL ) {
    arch_stmt = dynamic_cast<TyvisArchitectureStatement *>(stmt);
    ASSERT( arch_stmt != NULL );

    switch( arch_stmt->get_kind() ){
    case IIR_PROCESS_STATEMENT:
      arch_stmt->_publish_cc_headerfiles_for_cc_default( _cc_out );
      break;

    case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
    case IIR_BLOCK_STATEMENT:
      _set_current_publish_name( "SG" );
      arch_stmt->_publish_cc_headerfiles_for_cc_default( _cc_out );
      _set_current_architecture_name( temp );
      _set_current_publish_name( old_current_name );
      break;

    case IIR_COMPONENT_INSTANTIATION_STATEMENT:{
      const string include_file_name = arch_stmt->_get_cc_elaboration_class_name() + ".hh";
      Tyvis::_publish_cc_include( _cc_out, include_file_name );
      compInst = dynamic_cast<TyvisComponentInstantiationStatement *>(arch_stmt);
      ASSERT ( compInst->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT );
      if (compInst->_get_configuration() != NULL) {
	compInst->_get_configuration()->_publish_cc_headers( _cc_out );
      }
      break;
    }
    default:
      cerr << "ERROR! TyvisConcurrentGenerateForStatement::"
	   << "_publish_cc_headerfiles_for_cc( _cc_out ): unknown conc_statement "
	   << "type |" << arch_stmt->get_kind_text() << "|\n";
    }

    stmt = dynamic_cast<Tyvis *>(_get_statement_list()->successor( stmt ));
  }
}

void
TyvisArchitectureStatement::_publish_cc_scoping_prefix( ostream &os, 
                                                        Tyvis *finalScope, 
                                                        Tyvis *currentScope ){
  if(currentScope == finalScope){
    return;
  }

  if((dynamic_cast<TyvisPackageDeclaration *>(finalScope) != NULL) ||
     (finalScope->get_kind() == IIR_PACKAGE_BODY_DECLARATION)) {
    return;
  }

  if ( (currentScope->get_kind() == IIR_BLOCK_STATEMENT) || 
       (currentScope->get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT) || 
       (currentScope->get_kind() == IIR_CONCURRENT_GENERATE_IF_STATEMENT) &&
       (finalScope != currentScope) ) {      
    ASSERT ( _get_declarative_region() != 0 );
    os << "enclosingScope->";
    dynamic_cast<Tyvis *>(_get_declarative_region())->_publish_cc_scoping_prefix( os,
										  finalScope,
										  _get_declarative_region() );
  }
  else if (currentScope->get_kind() == IIR_COMPONENT_INSTANTIATION_STATEMENT) {
    ASSERT (currentScope->_get_declarative_region() != NULL);
    currentScope->_get_declarative_region()->_publish_cc_scoping_prefix( os, finalScope, _get_declarative_region());
  }
}

void
TyvisArchitectureStatement::_publish_cc_binding_name(ostream& os) {
  //  CC_REF( os, "TyvisArchitectureStatement::_publish_cc_binding_name" );
  os << *_get_mangled_label();
}

void 
TyvisArchitectureStatement::_publish_cc_enclosing_stmt_to_architecture_path(ostream& outstream) {
  //  CC_REF( _cc_out, "TyvisArchitectureStatement::_publish_cc_enclosing_stmt_to_architecture_path" );
  
  outstream << _get_enclosing_stmt_to_architecture_path();
}

void
TyvisArchitectureStatement::_publish_cc_concurrent_stmt_init( published_file &_cc_out,
                                                              TyvisDeclarationList *,
                                                              PublishData *declarations) {
  if (_is_simultaneous_statement() != TRUE) {
    ASSERT((_is_concurrent_generate_statement() == TRUE) ||
     	 (_is_block_statement() == TRUE));
    TyvisAssociationList* generic_map = _get_generic_map_aspect();

    CC_REF( _cc_out, "TyvisArchitectureStatement::_publish_cc_concurrent_stmt_init" );
  
    //_cc_out << *(_get_mangled_label());
    _get_mangled_label()->_publish_cc_elaborate( _cc_out.get_stream() );
    _cc_out << "_elab_obj = new "
	  << _get_cc_elaboration_class_name()
	  << OS("(this");
    if(generic_map != NULL) {
      if (generic_map->size() > 0) {
        _cc_out << "," << NL();
        //_current_publish_node = this;
        generic_map->_publish_cc_generic_map_aspect_for_conc_stmts( _cc_out, declarations );
        //_current_publish_node = NULL;
      }
    }
    _cc_out << CS(");");
  }
}

void
TyvisArchitectureStatement::_publish_cc_instantiate_call( published_file &_cc_out, PublishData * ) {
  CC_REF( _cc_out, "TyvisArchitectureStatement::_publish_cc_instantiate_call" );
  if( _is_simultaneous_statement() != TRUE ){
    switch( get_kind() ) {
    case IIR_PROCESS_STATEMENT:
      _cc_out << "Hierarchy *newHier;" << NL()
	      << "newHier = new Hierarchy;" << NL()
	      << "hier->add_block(\"";
      _cc_out.get_stream() << dynamic_cast<TyvisLabel *>(get_label());
      _cc_out << "\", newHier);" << NL();
      break;
    default:
      break;
    }
    _get_mangled_label()->_publish_cc_elaborate( _cc_out.get_stream() );
    _cc_out << "_elab_obj->instantiate(hier, _base, \"" << *(_get_label());
    _cc_out << "\" );" << NL();
  }
}

void
TyvisArchitectureStatement::_publish_cc_class_generate_stmt( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisArchitectureStatement::_publish_cc_class_generate_stmt" );

  _cc_out << "class " << _get_cc_elaboration_class_name() << " : public _savant_entity_elab {\n"
	  << "public:\n\n"
	  << _get_cc_elaboration_class_name() << "(";
  ASSERT ( _get_declarative_region() != NULL );
  _get_declarative_region()->_publish_cc_class_name( _cc_out.get_stream() );
  _cc_out << " *);\n";

  _cc_out << "~" << _get_cc_elaboration_class_name() << "();\n"
	  << _get_declarative_region()->_get_cc_elaboration_class_name()
  	  << "* enclosingScope;\n";
  
  _cc_out << "void instantiate(Hierarchy * hier, const string parent_base, const char *local_name);\n";
  _cc_out << "std::string _base;\n";
  _cc_out << "std::string get_base() {return(_base);}\n";

  _cc_out << "void createNetInfo();\n";
  _cc_out << "void connect(int, int, ...);\n";
  _cc_out << "Value getGenerateConstant(int);\n";
  _cc_out << "void resetGenerateConstant();\n";

  _publish_cc_signals( _cc_out, _get_declaration_list(), declarations );
  ASSERT( _get_statement_list()->get_kind() == IIR_ARCHITECTURE_STATEMENT_LIST );
  _publish_cc_object_pointers( _cc_out, dynamic_cast<TyvisArchitectureStatementList *>(_get_statement_list()), declarations);

  if( get_kind() == IIR_CONCURRENT_GENERATE_FOR_STATEMENT ){
    _cc_out << "static int generateConstant;\n";
    _cc_out << "static ArrayInfo::ArrayDirn_t generateDirection;\n";
  }
  
  _cc_out << "};\n\n";
}

void
TyvisArchitectureStatement::_publish_cc_characteristic_expressions( TyvisArchitectureStatement::SimultaneousIfPublishingPart,
                                                                           published_file & ){
  _report_undefined_fn("_publish_cc_characteristic_expressions()");
}


void 
TyvisArchitectureStatement::_publish_createNetInfo( published_file &, PublishData * ){
  _report_undefined_fn("_publish_createNetInfo( published_file &_cc_out, PublishData *declarations )");
}

void
TyvisArchitectureStatement::_publish_cc( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisArchitectureStatement::_publish_cc" );
  switch(get_kind()) {
  case IIR_COMPONENT_INSTANTIATION_STATEMENT:
    break;
  case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    break;
  default:
    cerr <<  "_publish_cc_lvalue( _cc_out, declarations ) not defined for " << get_kind_text() << "\n";
    break;
  }
}

Tyvis * 
TyvisArchitectureStatement::_transmute() {
  return this;
}

// For example We have the following code:
// architecture a of ent e is
// begin
// Ablk: block
// begin
// Bblk: block
// begin
// Cblk: block
// begin
// end block;
// end block;
// end block;
// end a
// _get_enclosing_stmt_to_architecture_path() of Ablk gives NULL;
// _get_enclosing_stmt_to_architecture_path() of Bblk gives Ablk;
// _get_enclosing_stmt_to_architecture_path() of Cblk gives Ablk_Bblk;
string 
TyvisArchitectureStatement::_get_enclosing_stmt_to_architecture_path() {
  string retval = "";

  ASSERT(_get_declarative_region() != NULL);
  if( _get_declarative_region()->get_kind() != IIR_ARCHITECTURE_DECLARATION && 
      _get_declarative_region()->get_kind() != IIR_ENTITY_DECLARATION ) {
    if( _get_enclosing_path() == "" ){
      ASSERT( dynamic_cast<TyvisArchitectureStatement *>(_get_declarative_region()) != 0 );
      ASSERT( dynamic_cast<TyvisArchitectureStatement *>(_get_declarative_region())->_get_label() != 0 );
      retval = dynamic_cast<TyvisArchitectureStatement *>(_get_declarative_region())->_get_label()->_to_string();
    }
  }

  return retval;
}

Tyvis *
TyvisArchitectureStatement::_get_declarative_region(){
  return dynamic_cast<Tyvis *>(get_declarative_region());
}

const string &
TyvisArchitectureStatement::_get_enclosing_path() const {
  return enclosing_path;
}

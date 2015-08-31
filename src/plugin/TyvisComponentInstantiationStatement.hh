#ifndef TYVIS_COMPONENTINSTANTIATIONSTATEMENT_HH
#define TYVIS_COMPONENTINSTANTIATIONSTATEMENT_HH

// Copyright (c) 1996-2010 The University of Cincinnati.
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

#include "TyvisConcurrentStatement.hh"
#include "savant/IIRBase_ComponentInstantiationStatement.hh"

class TyvisAssociationElement;
class TyvisConfigurationSpecification;
class TyvisDeclarationList;
class TyvisComponentDeclaration;

class TyvisComponentInstantiationStatement : public virtual TyvisConcurrentStatement, public virtual IIRBase_ComponentInstantiationStatement {
public:
  TyvisComponentInstantiationStatement();
  ~TyvisComponentInstantiationStatement();

  void _publish_cc_binding_name( ostream& outstream );
  void _publish_createNetInfo( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_elaborate_add( published_file &_cc_out, 
				  TyvisAssociationElement* actual_clause, 
				  TyvisLabel* label,
                                  PublishData *declarations);

  void _publish_cc_elaborate_addChild( published_file &_cc_out,
				       TyvisAssociationElement* actual_clause, 
				       TyvisLabel* label,
                                       PublishData *declarations);

  void _publish_cc_elaborate_upType( published_file &_cc_out,
				     TyvisAssociationElement* actual_clause, 
				     TyvisLabel* label,
                                     PublishData *declarations);

  void _publish_cc_elaborate_downType( published_file &_cc_out,
				       TyvisAssociationElement* actual_clause, 
				       TyvisLabel* label,
                                       PublishData *declarations);

  /** Publish the full call to "connect". */
  void _publish_cc_connect_call( published_file &_cc_out, PublishData *declarations );

  /**
     Used by _publish_cc_connect_call.
  */
  void _publish_connect( published_file &_cc_out, PublishData *declarations );
  void _publish_connect_terminals( published_file &_cc_out, PublishData *declarations );
  void _publish_form_characteristic_expressions( published_file & );

  void _publish_cc_concurrent_stmt_init( published_file &_cc_out,
					 TyvisDeclarationList *decl_list,
					 PublishData *declarations );

  /**
     This transmute function only applies for direct entity instantiation.
     In this case it synthesizes a configuration specification and a
     component declaration and then "standard" code generation occurs.
  */
  Tyvis *_transmute();

  TyvisConfigurationSpecification* _get_configuration_specification(TyvisDeclarationList* decl_list);
  TyvisConfigurationSpecification* _get_configuration_specification_from_any_scope(TyvisDeclarationList* decl_list);

  // Helper Functions
  TyvisAssociationList *_get_generic_map_aspect();  
  TyvisAssociationList *_get_port_map_aspect();
  Tyvis                *_get_configuration();
  Tyvis                *_get_instantiated_unit();
protected:
  TyvisConfigurationSpecification *
  _build_implicit_configuration_specification(TyvisLibraryUnit *, TyvisComponentDeclaration *);
  TyvisComponentDeclaration *
  _build_implicit_component_declaration(TyvisEntityDeclaration *, const string &componentName );
  
  void _add_to_configuration_specification(TyvisDeclarationList *, TyvisComponentDeclaration *, TyvisComponentInstantiationStatement *);

private:

};
#endif

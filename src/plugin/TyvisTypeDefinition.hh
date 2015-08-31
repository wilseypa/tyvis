#ifndef TYVIS_TYPEDEFINITION_HH
#define TYVIS_TYPEDEFINITION_HH

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
#include "savant/IIRBase_TypeDefinition.hh"

class TyvisScalarTypeDefinition;
class TyvisElementDeclarationList;

class TyvisTypeDefinition : public virtual Tyvis, public virtual IIRBase_TypeDefinition {
public:
  TyvisTypeDefinition();
  ~TyvisTypeDefinition();

  void _add_decl_into_declaration_collection(PublishData *declarations);

  // c++ code generation methods
  virtual void _publish_cc_left( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_right( published_file &_cc_out, PublishData *declarations );

  /**
     Returns the type name associated with this type definition.  Typically
     it will be something like "std_standard_integer".
  */
  virtual const string _get_cc_type_name();

  /** Returns the name of the typeInfo for this type, including the parens
      - i.e it returns a function call.  For example, for
      std.standard.integer this would return
      std_standard_integerTypeInfo() */
  virtual void _publish_cc_type_info_xxx( published_file &_cc_out, PublishData *declarations );

  /**
     This method gets used during type publishing by array types for
     publishing indexes and elements types with it's own name.
  */
  virtual void _publish_cc_type_info_xxx( published_file &_cc_out,
					  PublishData *declarations,
					  const string &type_name );

  virtual void _publish_cc_type_string( published_file & );

  virtual void _publish_cc_base_type_name( published_file &_cc_out );
  virtual void _publish_cc_decl_type_attributes( published_file &_cc_out );
  virtual void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );

  /** This method, currently only defined for scalar, range, and array
      types, publishes a Tyvis ArrayInfo structure corresponding this
      type. */
  virtual void _publish_cc_array_info( published_file &_cc_out );

  virtual void _publish_cc_decl_predefined_procedures_prototypes( published_file &_cc_out );

  virtual void _publish_cc_necessary_decl_in_state( published_file &_cc_out );
  virtual void _publish_cc_init_fields_for_signals( published_file &_cc_out );

  virtual void _publish_cc_subelement_type( published_file &_cc_out );
  virtual void _publish_cc_anonymous_type_name( ostream &os );

  /**
     If the derived class needs to do some prequisite publishing, for
     instance building a list of enumeration literals or something like
     that, then it can override this method.  By default it does nothing.
  */
  virtual void _publish_cc_type_info_prereq( published_file &,
					     PublishData *,
					     const string & ){}
  virtual void _publish_cc_static_type_info_fn( published_file &_cc_out, 
						PublishData *declarations,
						const string &functionName = "" );
  /**
     This call is overridden in the various TypeDefinition classes.  They
     define the proper arguments used to initialize the tyvis::TypeInfo
     that their definition corresponds to.
  */
  virtual void _publish_cc_type_info_args( published_file &_cc_out,
					   PublishData *declarations,
					   const string &type_name );

  virtual void _publish_cc_type_info_scoping_prefix( published_file &_cc_out,
                                                     PublishData *declarations,
						     const IIR_Boolean commaFlag = TRUE);

  virtual void _publish_cc_object_type_info( published_file &_cc_out,
                                             PublishData *declarations,
					     const IIR_Boolean commaFlag = TRUE, 
					     const char *additionalSuffix = NULL, 
					     const IIR_Boolean temporaryFlag = FALSE);

  /**
     A VHDL object (variable, signal, interface, etc.) is being
     constructed.  This call asks the subtype to add its contribution to
     the publishing.
  */
  virtual void _publish_cc_constructor_args( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_init_val( published_file &, PublishData * );

  virtual void _publish_cc_temporary_type_info( published_file &_cc_out, 
                                                PublishData *declarations,
						char * = NULL, 
						char * = NULL );

  virtual void _publish_cc_parent_type_name( published_file &_cc_out );
 
  virtual void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_headers( published_file &_cc_out );

  using Tyvis::_publish_cc_include;
  /** Publishes the appropriate include for this type. */
  virtual void _publish_cc_include( published_file &_cc_out );

  /** Publishes the resolution function id for resolved types, "-1" for
      types that aren't resolved. */
  void _publish_cc_resolution_function_id( published_file &_cc_out, PublishData *declarations );

  /** This method sets the resolution function for types that are
     allowed to have them.  An internal error will be generated if
     called on file or access types.  */ 
 void _publish_cc_set_resolution_function( published_file &_cc_out, PublishData *declarations );

  /** These functions publishes the code needed in the initializtion of any
      signal These functions unrolls the composite type to their individual
      non composite types */
  virtual void _publish_cc_init_signal( published_file &_cc_out );  
  
  /** This function publishes the code needed for the initializaion of any
      quantity */
  virtual void _publish_cc_init_quantity( published_file &_cc_out, PublishData *declarations );

  TyvisTextLiteral *_get_mangled_declarator();

  virtual Tyvis *_get_direction();
  virtual IIR_Int32 get_num_indexes();

  virtual TyvisScalarTypeDefinition *_get_index_subtype();
  virtual TyvisScalarTypeDefinition *_get_resolved_index_subtype();
  virtual TyvisTypeDefinition *_get_element_subtype();
  virtual TyvisTypeDefinition *_get_base_type();
  virtual TyvisTypeDefinition *_get_resolved_base_type();

  virtual IIR_Boolean _is_single_dimensional_array_type(){ return FALSE; }
  virtual IIR_Boolean _is_kernel_type();
  virtual IIR_Boolean _is_bit_type() { return FALSE; }
  virtual IIR_Boolean _is_boolean_type() { return FALSE; }
  virtual IIR_Boolean _is_resolved_type();
  virtual IIR_Boolean _is_anonymous();

  virtual TyvisElementDeclarationList *_get_element_declarations(){ return NULL; }
  virtual TyvisFunctionDeclaration *_get_resolution_function();
  virtual Tyvis* _get_elaboration_class();

  //The following function return TRUE if the elements of the
  //array type or any field of record type is an access type
  virtual IIR_Boolean _has_access_type() { return FALSE; }

  /** Returns what the code generator expects as a parent type.  If there
     is none, returns "this". */
  TyvisTypeDefinition *_get_parent_type();

  // Helper Functions
  Tyvis                           *_get_base_type_left();
  Tyvis                           *_get_base_type_direction();
  Tyvis                           *_get_base_type_right();  
  TyvisTypeDefinition            *_get_bottom_base_type();
  TyvisDeclaration               *_get_declaration();
  TyvisTypeDefinition            *_get_type_mark();
protected:
  virtual void _publish_cc_type_info_xxx_dynamic( published_file &_cc_out,
						  PublishData *declarations,
						  const string &type_name );

  virtual void _publish_cc_type_info_xxx_static( published_file &_cc_out,
						 PublishData *declarations,
						 const string &type_name );

private:
  class name_manager {
  public:
    name_manager();
    ~name_manager();

    bool contains_name( const string &check_for ) const;
    void add_name( const string &to_add );

  private:
    class impl;
    impl *_impl;
  };

  name_manager _names;

};
#endif

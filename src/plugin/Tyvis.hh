#ifndef TYVIS_HH
#define TYVIS_HH

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

#include "savant/IIRBase.hh"
using std::string;

extern bool no_mangling;

class symbol_table;
class include_manager;
class published_file;
class break_handler;

namespace savant {
  template <class type> class set;
}

template <class type> class dl_list;

class TyvisAboveAttribute;
class TyvisArchitectureDeclaration;
class TyvisArchitectureStatementList;
class TyvisAssociationList;
class TyvisAttributeSpecification;
class TyvisAttributeSpecificationList;
class TyvisConcurrentStatement;
class TyvisConstantDeclaration;
class TyvisContributionAttribute;
class TyvisDeclaration;
class TyvisDeclarationList;
class TyvisDesignator;
class TyvisDesignatorList;
class TyvisDesignFile;
class TyvisDotAttribute;
class TyvisEntityDeclaration;
class TyvisFunctionDeclaration;
class TyvisGenericList;
class TyvisIdentifier;
class TyvisIntegerLiteral;
class TyvisInterfaceList;
class TyvisLabel;
class TyvisLibraryDeclaration;
class TyvisLibraryUnit;
class TyvisList;
class TyvisNatureDeclaration;
class TyvisNatureDefinition;
class TyvisPackageDeclaration;
class TyvisPortList;
class TyvisProcessStatement;
class TyvisReferenceAttribute;
class TyvisSimultaneousStatement;
class TyvisStatement;
class TyvisTypeDeclaration;
class TyvisTypeDefinition;
class TyvisTextLiteral;
class published_file;
class PublishData;

/**
   The following type is used by the publishing functions to differentiate
   between publishing a node's declaration, and a node's definition.  For
   example, a signal inserts a SignalNetinfo instance into it's class
   definition, but then it also has to define itself (through constructor
   arguments and the initializatin list) sometime later.  We want to call
   the same method to do both, and we signal which we're doing with this
   enum.
*/
typedef enum { DECLARATION, DEFINITION } cc_publish_mode;

/** This class is the base of the SAVANT extensions to the IIR.  Due to the
    design of the IIR, lots of things have been put in here that seem like
    they might be better suited somewhere else.  Unfortunately, it is often
    the case that IIR is the closest common ancestor that one can find to
    insert a virtual method into. */
class Tyvis : public virtual IIRBase {
public:
  Tyvis();
  ~Tyvis() {}
  
  /** The following function is used for building the sensitivity list
      while doing transmogrification. */
  virtual void _build_sensitivity_list(TyvisDesignatorList* sensitivity_list);
  virtual void _build_contribution_quantity_list(dl_list<TyvisContributionAttribute> *);
  virtual void _build_differential_quantity_list(dl_list<TyvisDotAttribute> *);
  virtual void _build_generic_parameter_set( savant::set<TyvisDeclaration> *);
  virtual void _build_reference_quantity_list(dl_list<TyvisReferenceAttribute> *);

  virtual TyvisAttributeSpecification *_get_attribute_specification(Tyvis*);
  virtual TyvisAttributeSpecificationList *_get_attribute_specification_list( );

  virtual void _set_stmt_node_index(IIR_Int32 *, bool, bool&);
  virtual void _set_stmt_qty_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *, PublishData *, PublishData *); 
  virtual IIR_Int32 _get_stmt_qty_index();
  virtual IIR_Int32 _get_stmt_signal_index();  
  virtual void _set_stmt_signal_index(IIR_Int32 *,  savant::set<TyvisDeclaration> *);
  virtual void _flush_stmt_index();
  
  virtual TyvisDeclaration *_find_in_implicit_list( const string );
  virtual  savant::set<TyvisDeclaration> *_get_implicit_declarations( );
  virtual void _set_implicit_declarations(  savant::set<TyvisDeclaration> * );

  virtual IIR_Boolean _reference_quantity_found();
  virtual IIR_Boolean _contribution_quantity_found();
  virtual IIR_Boolean _differential_quantity_found();

  TyvisIdentifier                        *_get_file_name();
  TyvisLibraryDeclaration                *_get_work_library( );
  virtual TyvisFunctionDeclaration       *_get_resolution_function();
  virtual TyvisDeclaration               *_get_package_declaration();
  virtual TyvisPortList                  *_get_port_list();
  virtual TyvisDeclaration               *_get_prefix_declaration();
  virtual TyvisGenericList               *_get_generic_list();
  virtual Tyvis                           *_get_component_name( );
  virtual TyvisLibraryUnit               *_get_entity_aspect();
  virtual TyvisDeclarationList           *_get_declaration_list();
  virtual TyvisList                      *_get_statement_list();
  virtual TyvisTextLiteral               *_get_declarator();
  
  virtual void _set_passed_through_out_port(IIR_Boolean val);
  virtual void _get_list_of_input_signals(  savant::set<Tyvis> *list );
  virtual void _get_signal_source_info(  savant::set<Tyvis> *siginfo );

  IIR_Boolean _is_currently_publishing_subprogram();
  virtual IIR_Boolean _is_globally_static(){ return _is_globally_static_primary(); }
  virtual IIR_Boolean _is_globally_static_primary();
  virtual IIR_Boolean _is_longest_static_prefix();
  virtual IIR_Boolean _is_static_expression() { return FALSE;}
  virtual IIR_Boolean _is_composite_resolved_signal() { return FALSE; }
  virtual IIR_Boolean _is_kernel_type() { return FALSE;}
  virtual IIR_Boolean _is_simultaneous_statement() { return FALSE; }
  virtual IIR_Boolean _is_resolved_signal() { return FALSE; }

  /** _is_resolved_type : TRUE if the type defined contain resolution function */
  virtual IIR_Boolean _is_resolved_type();

  /** Warning!!!!!!  It is not always right to typecast to
      TyvisConcurrentGenerate*Statement if this function returns true.  The
      reason being, this will return true, if it is a concurrent
      generate*statement or if the label is that of a block statement. */
  virtual IIR_Boolean _is_concurrent_generate_statement() { return FALSE;}

  /** Warning!!!!!!  It is not always right to typecast to
      TyvisBlockStatement if this function returns true.  The reason being,
      this will return true, if it is a block statement or if the label is
      that of a block statement. */
  virtual IIR_Boolean _is_block_statement() { return FALSE;}

  /** Returns a STL string representing this node, if defined.  Relies on
      the "_print" method being defined for the node. */
  const string _to_string( );

  virtual Tyvis  *_transmute();

  /** This function copies the identifiers declared in outer scope and accessed
      somewhere within nested scope(s) to outer scopes till it reaches the
      scope where the object was declared. */
  void _copy_symbols_defined_in_enclosing_scope(PublishData *current, PublishData *outer);

  /** The following function adds the constant declarations that Initialize
      variables or signals into code generators symbol table. */
  virtual void _add_decl_into_declaration_collection(PublishData *declarations);

  //@{
  /**
     LValue methods.
  */
  /** Publish C++ appropriate for the lvalue of an assignment.  Often this
      will be a name of some sort. */
  virtual void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  /**
     Publish this lvalue which is the target of a variable assignment
     statement.
  */
  void _publish_cc_variable_lvalue( published_file &_cc_out, PublishData *declarations );
  /**
     Publish this lvalue which is the target of a signal assignment
     statement.
  */
  void _publish_cc_signal_lvalue( published_file &_cc_out, PublishData *declarations );
  //@}

  /** Publish C++ appropriate for getting the rvalue of an assignment. */ 
  virtual void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  virtual string _get_cc_rvalue();

  virtual void _publish_cc_init( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_init_for_ams(published_file &);

  /** Publish the name of this object as would be accessed from it's
      context. */
  virtual void _publish_cc_object_name( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_wait_data( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_data( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_decl( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_decl_with_constructor_args( published_file &_cc_out,
						       PublishData *declarations );
  virtual void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );

  /** This function publishes the range of the type if it is scalar type
      and publishes the range of indices, if it is an array type */
  virtual void _publish_cc_range( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_value( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_universal_value( published_file &_cc_out, PublishData *declarations );

  /** This method is defined for all the classes in the system that have an
      elaboration class associated with them.  This class is a reminisent of the
      days before we ever did any form of name mangling in the system and hence
      the useage "binding_name".  The class publishes necessary association for
      the elaboration classes (such as <architecture>_<entity>,
      <block>_<architecture>, <generate_statement>_<architecture> etc.) in the
      system.  This method is overloaded by the following child classes:
      TyvisArchitectureDeclaration, TyvisComponentDeclaration,
      TyvisComponentInstantiationStatement, TyvisConcurrentStatement,
      TyvisConfigurationDelcaration, TyvisEntityDeclaration,
      TyvisProcessStatement.  These methods also add predefined prefixes to
      the binding-names (such as SEA, SC, SG, SCFG etc.) to improve quick
      recognition of the type of elaboration class in the generated code.

      This method is also overloaded by the following classes to ease/assist
      code-generation: TyvisFunctionDeclaration, TyvisLabel,
      TyvisName, TyvisPackageBodyDeclaration,
      TyvisPackageDeclaration.
      
      @param  outstream  The output stream into which the binding-name
      needs to be published (_cc_out by default) */
  virtual void _publish_cc_binding_name( ostream &outstream );

  /** This method publishes an include appropriate for a generated file to
      include whatever this node is.  If this method isn't overridden in
      the leaf node calling this will generate a runtime error.  */
  virtual void _publish_cc_include( published_file &_cc_out );

  /** This method publishes an include appropriate for a generated file to
      include whatever this node is.  If this method isn't overridden in
      the leaf node calling this will generate a runtime error.  */
  virtual void _publish_cc_include_decls( published_file &_cc_out );

  /** This method publishes an include for whatever this node is.  (If it's
      defined for this node, generates a runtime error if not.  */
  virtual void _publish_cc_include_elab( published_file &_cc_out );

  /** Generate an include for the VHDL file name passed in.
     
      @param os The stream to publish the include to.
      @param file_to_include the file to include
      @param system_include - is this a system include that needs angle
      braces, or a user include that needs quotes? */
  virtual void _publish_cc_include( published_file &os,
				    const string &file_to_include,
				    bool system_include = false );

  /** This method is/can be used to publish the full name of the C++ class
      associated with a concurrent statements (Note: Only concurrent statements
      in the system have a unique class associated with them).  This method uses
      Tyvis::_publish_cc_binding_name( published_file &_cc_out ) to
      publish the class name. 

      @param The output stream to which the class name needs to be published
      into (it is _cc_out by default).  */
  virtual void _publish_cc_class_name( ostream &os );

  virtual void _publish_cc_subprogram_arg( published_file &_cc_out );
  virtual void _publish_cc_constructor_args( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_sigdest( published_file &_cc_out );
  virtual void _publish_cc_declarator( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_for_index( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_condition( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_headers( published_file &_cc_out );
  virtual void _publish_cc_ams_function(published_file &, PublishData *declarations);
  virtual void _publish_cc_ams_function_call_in_simult_stmt(published_file & ){
    // do nothing
  }
  /**
    * Used to create the circuit hierarchy. Empty implementation because not every node needs
    * an implementation.
    */
  virtual void _publish_cc_main(published_file & ){}

  /** This is the default behaviour for _publish_cc_headerfiles_for_cc( published_file &_cc_out ).
      It has to be called explicitly.  It's defined here as both
      declarations and statements call it. */
  void _publish_cc_headerfiles_for_cc_default( published_file &_cc_out );

  virtual void _publish_cc_direction( published_file &_cc_out );
  virtual string _get_cc_direction();

  //Works for only one case. These two functions need a lot of work
  virtual void _publish_cc_name_elaborate( published_file &_cc_out, PublishData *declarations );

  /** This function publishes the constructor of signals in the constructor
      of the state. */
  virtual void _publish_cc_state_object_init( published_file &, PublishData * ){ }
  virtual void _publish_cc_state_l_object_init( published_file &, PublishData * ){ }

  /** This method publishes code for this node to act as an initializer for
      a constant or variable.  By default, it calls "_publish_cc". */
  virtual void _publish_cc_initialization_value( published_file &_cc_out,PublishData *declarations );

  /** This function specifies the type of object instatiated in simulation
      kernel. */
  virtual void _publish_cc_universal_type( published_file &_cc_out );

  /** Publishes c++ for kernel types.  Right now this is only for scalar
      types. 
      @param os Optional stream to publish into.  Defaults to _cc_out. */
  virtual const string _get_cc_tyvis_type();
  virtual void _publish_cc_kernel_type( ostream &os );
  virtual void _publish_cc_kernel_type( published_file &pf );

  /** This function is implemented recursively in different nodes to handle
      type conversion in procedure calls. The actual can be nest function
      calls and the actual nested with them is extracted and published. */
  virtual void _publish_cc_first_objectParameter( published_file &_cc_out, PublishData *declarations ); 

  /** The following function is used for code generation that does runtime 
      elaboration. */
  virtual void _publish_cc_copy_generics_to_globals( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_addChild( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_addToFanOut( published_file &_cc_out, TyvisProcessStatement *process, PublishData *declarations );
  void _publish_cc_prefix_string( published_file &_cc_out );

  virtual void _publish_cc_scoping_prefix( ostream &os, Tyvis *, Tyvis * );

  /** The following function is used for code generation that does runtime 
      elaboration. */
  void _publish_cc_dump( published_file &_cc_out );

  /** Called on expressions to publish array indexes - what goes between
      the [] in the C++. */
  virtual void _publish_cc_array_index( published_file &_cc_out, PublishData *declarations );

  /**
     This node is the procedure_name in an IIR_ProcedureCall or
     IIR_FunctionCall statement.  Publish the c++ function name that needs
     to be called.
  */
  virtual void _publish_cc_subprogram_call_name( published_file &_cc_out, PublishData *declarations );

  /** This method returns the c++ class name used for the elaboration of
      the node it is called on.  It relies on _publish_cc_binding_name for
      most of it's work. */
  virtual const string _get_cc_elaboration_class_name();

  /** Publish the type of the object, as required by the type constructor.  */
  virtual const string _get_cc_object_type();

  /** For the "get_instantiated_unit()" of an
      IIR_ComponentInstantiationStatment, return the architecture (or
      null.)

      @see IIR_ComponentInstantiationStatement#get_instantiated_unit */
  virtual TyvisArchitectureDeclaration *_get_cc_instantiated_architecture();

  /** For the "get_instantiated_unit()" of an
      IIR_ComponentInstantiationStatment, return the entity (or null.)

      @see IIR_ComponentInstantiationStatement#get_instantiated_unit */
  virtual TyvisEntityDeclaration *_get_cc_instantiated_entity();

  virtual void _publish_cc_type_info_xxx( published_file &_cc_out,
					  PublishData *declarations );

  /**
     Publish the method that defines the TypeInfo for this node.  Mostly
     relevant to IIR_Declarations and IIR_TypeDefinitions, but some other
     nodes might need to do special things as well.  (For loops need to
     publish type infos for their parameters, for example.)
  */
  virtual void _publish_cc_static_type_info_fn( published_file &_cc_out,
						PublishData *declarations );
  
  /**
     Publish the C++ for this node in the context of what needs to be
     defined to declare it as part of a package.
  */
  virtual void _publish_cc_package_decl( published_file &_cc_out,
					 PublishData *declarations );

  virtual void _publish_cc_package_definition( published_file &_cc_out,
					       PublishData *declarations );


  /** This method is used to obtain the declarator corresponding to this IIR
      node.  This method ALWAYS returns only the mangled declarator .ie. the
      declarator whose name hase been prefixed with the declarators of its
      enclosing scope.  This method is overloaded in the following derived
      classes: TyvisDeclaration, TyvisName, TyvisSimpleName,
      TyvisStatement, and TyvisTypeDefinition.  By default this method
      reports "undefined_function" when invoked on a Tyvis hierarchy that does
      not overload this virtual method.

      @return a TyvisTextLiteral that points to the mangled declarator.  The
      user MUST NOT delete or modify the pointer returned my this method.  */
  virtual TyvisTextLiteral *_get_mangled_declarator();
  
  void _report_undefined_fn(char *);

  TyvisTypeDefinition *_get_subtype();
  
  void _group_component_instantiations(TyvisArchitectureStatementList* conc_stmt_list,
				       int blockSize);

  /**
     For items that live in a declarative region, return it.  Calling this
     on things that don't live in a declarative region will generate a runtime error.
  */
  virtual Tyvis *_get_declarative_region();

  /** This method has been introduced to remove unneccessary type
      castings. */
  virtual IIR_Mode _get_mode();

  virtual IIR_Boolean _is_published_attribute_in_state(SignalAttribute);
  virtual void _add_published_attribute_in_state(SignalAttribute);
  virtual IIR_Boolean _is_published_attribute_in_constructor(SignalAttribute);
  virtual void _add_published_attribute_in_constructor(SignalAttribute);
  virtual IIR_Boolean _is_published_attribute_in_initstate(SignalAttribute);
  virtual void _add_published_attribute_in_initstate(SignalAttribute);

  /** Builds the set of above attributes.*/ 
  virtual void _build_above_attribute_set(savant::set<TyvisAboveAttribute> *);

  IIR_Boolean _is_currently_publishing_generate_for();

  Tyvis *_get_current_declarative_region();

  IIR_Boolean get_is_currently_publishing_simultaneous_stmt();
  IIR_Boolean get_is_currently_publishing_simultaneous_if_stmt();

  void set_is_currently_publishing_simultaneous_stmt( bool );
  void set_is_currently_publishing_simultaneous_if_stmt( bool );

  Tyvis *_get_value();
protected:
  void _publish_cc_class_includes( published_file &_cc_out, 
				   TyvisArchitectureStatementList* conc_stmt_list);

  void _publish_cc_signals( published_file &_cc_out, TyvisDeclarationList* decl_list, PublishData *declarations);

  void _publish_cc_object_pointers( published_file &_cc_out, TyvisArchitectureStatementList* conc_stmt_list, PublishData *declarations);
  virtual void _publish_cc_object_pointers_init( published_file &_cc_out,
						 TyvisArchitectureStatementList* conc_stmt_list,
						 TyvisDeclarationList* decl_list,
                                                 PublishData *declarations);

  void _publish_cc_destructor( published_file &_cc_out, TyvisArchitectureStatementList* conc_stmt_list);

  void _publish_cc_anonymous_drivers( published_file &_cc_out, TyvisDeclarationList *, PublishData *);
  void _publish_cc_reference_info( published_file &_cc_out, const char *method_info, int line_number );
  void _publish_make_reference_info( published_file &_cc_out, char *method_info, int line_number );

  /** Redefine the savantnow macro to use a process pointer to access the
      time. */
  void _publish_cc_savantnow_process_ptr( published_file &_cc_out );

  /** Redefine the savantnow macro to not use a process pointer to access
      the time. */
  void _publish_cc_savantnow_no_process_ptr( published_file &_cc_out );

private:
  /** Do the actual publishing of the savantnow macro. */
  void _publish_cc_savantnow( published_file &_cc_out, const string &prefix );

  IIR_Boolean _currently_publishing_simultaneous_stmt;
  IIR_Boolean _currently_publishing_simultaneous_if_stmt;
};

TyvisTypeDefinition *_get_aggregate_iterator_subtype();
void _set_aggregate_iterator_subtype( TyvisTypeDefinition * );

//@{
/** global functions - deprecated */

string &_get_current_publish_name();
void _set_current_publish_name( const string );
void _remove_current_publish_name( const string );
string * _get_full_current_publish_name();
void _add_current_publish_name( string new_name );
void _remove_current_publish_name();

int _get_aggregate_iterator_counter();
void _set_aggregate_iterator_counter( int );

Tyvis *_get_current_publish_node();
void _set_current_publish_node( Tyvis * );

string &_get_current_elab_name();
void _set_current_elab_name( const string );

string &_get_current_configuration_name();
void _set_current_configuration_name( const string );

string &_get_current_architecture_name();
void _set_current_architecture_name( const string );

string &_get_current_entity_name();
void _set_current_entity_name( const string );

IIR_Int32 _get_number_of_processes();
void _set_number_of_processes( IIR_Int32 );

string &_get_current_suffix_name();
void _set_current_suffix_name( const string );

/** While Publishing Multidimensonal Arrays/Composite types, index level
    has to be kept track of,across IF nodes and hence the following
    declaration. */
IIR_Int32 _get_index_level();
void _set_index_level( IIR_Int32 );

/** In case of resolution function and type conversion function publishing,
    certain special handling is required for "return" statements.  This
    static value is used to indicate that a possible type conversion
    function or a resolution function is being published. */
IIR_Boolean _get_allocate_return_value();
void _set_allocate_return_value( IIR_Boolean );

string &_get_current_another_name();
void _set_current_another_name( const string );

/** This is a string that will be prepended to ALL declarations while hey
    are published.  This will help us in publishing different perfixes in
    different contxts.  The wrapper methods for this variable follow. */
string &_get_publish_prefix_string();
void _set_publish_prefix_string( const string );

IIR::PublishedUnit _get_currently_publishing_unit();
void _set_currently_publishing_unit( IIR::PublishedUnit );

/** The following variable is set either true or false.  If the variable is
    set true, the objects are published without the string
    "state.current->" as prefix. This has been done to reduce The number of
    functions to be written in all the nodes, one publishing with
    "state.current->" as prefix and one without it. */
IIR_Boolean _get_publish_object_without_prefix();
void _set_publish_object_without_prefix( IIR_Boolean );

TyvisProcessStatement *_get_current_process_statement();
void _set_current_process_statement( TyvisProcessStatement * );
//@}
/** CC_REF

    Description: Will conditionally dump scram source code reference to the
    code generator section involved in publish-cc code generation.

    Parameters:
    char *	f
     	Should hold a string containing the Tyvis subclass and
     	function name in class::function type of format.
	
	Generates reference according to: \
	<Tyvisclass>::<function>(<line#>) on <userfile>(userline#)	*/

#ifndef CC_REF
#if (!defined(NO_DEVELOPER_ASSERTIONS) && !defined(NO_GENERATE_CC_REFERENCES))
#define	CC_REF( file, string ) _publish_cc_reference_info( file, string, __LINE__ )
#else /* CC_REF */
#define CC_REF( file, string )
#endif
 
#endif /* CC_REF */

published_file &
operator<<(published_file &pf, IIR &is );

#endif

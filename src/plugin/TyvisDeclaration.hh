#ifndef TYVIS_DECLARATION_HH
#define TYVIS_DECLARATION_HH

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
#include "savant/IIRBase_Declaration.hh"

class TyvisAttribute;
class TyvisGenericList;

class TyvisDeclaration : public virtual Tyvis, public virtual IIRBase_Declaration {
public:  
  TyvisDeclaration();
  ~TyvisDeclaration();

  void _publish_cc_declarator( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_object_name( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_declarative_region( published_file &_cc_out );
  virtual void _publish_cc_init_signal( published_file &_cc_out );
  virtual void _publish_cc_init_function( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_locatesignal( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_constant_object_init( published_file &_cc_out );
  virtual void _publish_cc_implicit_signal_type( published_file &_cc_out );
  virtual void _publish_cc_implicit_signal_attributes( published_file &_cc_out,
						       PublishData *declarations );
  virtual void _publish_cc_implicit_state_objects_init( published_file &_cc_out,
							PublishData *declarations );
  /**
     Publish a forward declaration for this node.
  */
  virtual void _publish_cc_forward_decl( published_file &_cc_out );
  virtual void _publish_cc_decl_with_constructor_args( published_file &_cc_out,
						       PublishData *declarations );
  virtual void _publish_cc_implicit_signal_attributes_copying( published_file &_cc_out,
							       PublishData *declarations );
  virtual void _publish_cc_implicit_signal_attributes_read_or_write( published_file &_cc_out,
								     const string &functionName, 
								     const string &streamName,
                                                                     PublishData *declarations);

  virtual void _publish_cc_necessary_copying( published_file &_cc_out );
  virtual void _publish_cc_read_or_write( published_file &_cc_out,
					  const string &functionName,
					  const string &streamName);


  virtual void _publish_cc_signal_net_info( published_file &_cc_out,
					    cc_publish_mode mode,
					    PublishData *declarations );

  void _publish_cc_addChild( published_file &_cc_out,
			     PublishData *declarations );

  virtual void _publish_cc_include_path( ostream &os );
  virtual void _publish_cc_library_file_name( ostream &os );
  virtual void _publish_cc_locally_static_value( published_file &_cc_out,
						 PublishData *declarations );

  virtual TyvisPortList *_get_port_list(){ return NULL; }
  void _get_list_of_input_signals(  savant::set<Tyvis> *list );
  void _get_signal_source_info(  savant::set<Tyvis> *siginfo);

  void _build_sensitivity_list(TyvisDesignatorList *sensitivity_list);

  virtual IIR_Boolean _is_resolved_signal();
  virtual IIR_Boolean _is_composite_resolved_signal();
  virtual IIR_Boolean _is_signal_in_outerscope();

  /** Returns true if this is a foreign attribute specification, false
     otherwise.  */
  virtual IIR_Boolean _is_foreign_attribute_specification( ){ return FALSE; }

  /** Since we can now have implicit declarations due to attributes, we
      need to be able to tell if THIS declaration is the result of an
      attribute. */
  TyvisAttribute *_get_attribute_name();

  TyvisAttributeSpecification* _get_attribute_specification(Tyvis*);

  TyvisDeclaration* _get_implicit_declaration_for_attribute ( TyvisAttribute *findAttrib );
  /** There is a set of implicit declarations associated with some
      declarations.  For instance, type declarations defining a file type
      implictly declare "read" and "write" procedures.  These methods are
      used to access the set of implicit declarations.  If there are none
      associated with this declaration, this method will return NULL. */
  savant::set<TyvisDeclaration> *_get_implicit_declarations();
  void _set_implicit_declarations(  savant::set<TyvisDeclaration> * );

  /** Since an implicit declaration has its prefix a signal declaration
      This function extracts that declaration from the implied declaration.
      It returns NULL if this isn't an implicit declaration. */
  virtual TyvisDeclaration* _get_signal_decl();

  /** An implicit signal declaration's prefix is a signal; this function
      returns the implicit signal declaration's prefix.  This is not an
      equivalent of _get_signal_decl().  This is only is used in the code
      generation phase. */
  virtual TyvisDeclaration *_get_signal_prefix();

  virtual TyvisDeclaration *_get_prefix_declaration();
  virtual TyvisGenericList *_get_generic_list(){ return NULL; }
  virtual IIR_Int32 get_num_indexes();

  virtual void _add_declarations_in_initializations( PublishData *declarations );
  virtual TyvisDesignatorList *_get_instantiation_list();
  virtual TyvisNatureDefinition *_get_nature(){ return NULL; }

  void _reset_scoping_prefix();
  void _set_scoping_prefix();

  void _clear();

  // Helper Functions
  Tyvis              *_get_value();
  TyvisTextLiteral  *_get_declarator();

  virtual IIR_Boolean _have_added_driver() {
    return FALSE;
  }

  virtual void _set_driver_addition_flag(IIR_Boolean);

  /** This method is invoked by TyvisDeclaration::_mangle_declarator()
      method to provide a predefined prefix for the IIR trees derived from
      IIR_Declaration.  This method is overloaded by
      TyvisAliasDeclaration and TyvisObjectDeclaration.  The idea
      of having a mangling prefix is to provide a simple means of
      identifying different kinds of declarations in the generated code by
      merely looking at the mangling prefix.  Usually the mangling prefix
      is an UPPER case character (since everything else is in lowercase,
      this one will stand out clearly)

      @return A const IIR_Char* that points to the mangling prefix.  The
      calling routines will not/cannot delete this pointer.  Hence, calls
      to this method (and overloaded instances) must ensure that they do
      not leak memory.  */
  virtual const IIR_Char *_get_mangling_prefix();

  /** This method is provided as an accessor method.  It basically returns
      a pointer to the mangledDeclarator (a class member object) that gets
      set by a call to TyvisDeclaration::_set_mangled_declarator().

      @return A IIR_Identifier* that points to the mangled declarator for
      "this" IIR hierarchy.  The caller MUST NOT modify or delete the
      pointer returned by a call to this method.  */
  TyvisTextLiteral *_get_mangled_declarator();

  /** This method is used to obtain a name mangled declarator for all IIR
      hierarchies derived from this class.  Mangling of names is "lazy"
      .ie. mangling is done during the first class to this method (for each IIR
      tree).  The mangled name is stored in a member object (mangledDeclarator)
      and is used in subsequent calls to this method.  It uses
      TyvisDeclaration::_mangle_declarator() method to perform the
      mangling.

      @return An IIR_TextLiteral* that points to the mangled declarator
      correcponding to "this" IIR hierarchy.  The user MUST NOT modify or
      delete the pointer returned by a call to this method.  */
  virtual TyvisTextLiteral *_get_my_mangled_declarator();

  /** This method is provided to set the pointer to the mangled declarator
      for "this" IIR hierarchy.  The parameter (char*) passed to this
      method is stored internally as an identifier [created by a call to
      TyvisIdentifier::get().  The mangled declarator can be accessed by a
      call to TyvisDeclaration::_get_mangled_declarator().

      @param Pointer to the mangled declarator. The parameter is not
             modified or deleted by this method.  */ 
  void _set_mangled_declarator( const string );

  /** This method is used by TyvisDeclaration::_get_declarator() to
      mangle the declarator corresponding to "this" IIR hierarchy.  This
      method iteratively prefixes the declarator for the IIR hiearchy with
      the declarators of all the enclosing scopes .ie. declarative regions
      (obtained by a call to _get_declarative_region()).  The method sets
      the mangled declarator by a call to the
      TyvisDeclaration::_set_mangled_declarator() method.  This method
      also makes use of the TyvisDeclaration::_get_mangling_prefix()
      method to obtain a prefix for mangling in an attempt to make the
      declarators more readable/distinguishable.  */
  virtual string _mangle_declarator();

  IIR_Boolean _is_published_attribute_in_constructor(SignalAttribute);
  void _add_published_attribute_in_constructor(SignalAttribute);

  IIR_Boolean _is_published_attribute_in_state(SignalAttribute);
  void _add_published_attribute_in_state(SignalAttribute);

  IIR_Boolean _is_published_attribute_in_initstate(SignalAttribute);
  void _add_published_attribute_in_initstate(SignalAttribute);
  
  /** This array contains the enum of all the get_attributes() of "this" signal
      for which code generation has been done. */
  IIR_Boolean *implemented_attributes_in_constructor;
  IIR_Boolean *implemented_attributes_in_state;
  IIR_Boolean *implemented_attributes_in_initstate;

  Tyvis *_get_declarative_region();


  //@{
  /**
     These methods apply to packages and package bodies.  IIR_Declaration
     is the common root so they appear here.
  */
  /** Returns the name of this package as it should be referred to in the
      generated code. */
  string _get_cc_package_name();
  string _get_cc_package_body_name();
  //@}
protected:

private:
  TyvisIdentifier   *mangledDeclarator;

  /** This is the set of implicit declarations associated with this
      declaration. */
  savant::set<TyvisDeclaration> *implicit_declarations;

};
#endif

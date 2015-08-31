#ifndef TYVIS_ATTRIBUTE_HH
#define TYVIS_ATTRIBUTE_HH

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

#include "TyvisName.hh"
#include "savant/IIRBase_Attribute.hh"

class TyvisFreeQuantityDeclaration;
class TyvisBranchQuantityDeclaration;
class TyvisSignalDeclaration;

class TyvisAttribute : public virtual TyvisName, public virtual IIRBase_Attribute {
public:
  TyvisAttribute();
  ~TyvisAttribute();

  void _add_decl_into_declaration_collection(PublishData *declarations);
  /** This function defines the necessary variables that are needed in the 
      _state.hh file */
  virtual void _publish_cc_necessary_decl_in_state( published_file &_cc_out );

  virtual void _publish_cc_name_in_caps( published_file &_cc_out );
  virtual void _publish_cc_attrib_type( published_file &_cc_out );
  virtual void _publish_cc_necessary_decl_init( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_init_val( published_file &_cc_out );
  virtual void _publish_cc_sigtype( published_file &_cc_out );
  virtual void _publish_cc_init_fields_for_signals( published_file &_cc_out );
  virtual void _publish_cc_implicit_signal_declaration( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_implicit_signal_state_object_init( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_type_info_xxx( published_file &_cc_out,
					  PublishData *declarations );
  virtual void _publish_cc_universal_value( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  virtual void _publish_cc_as_for_loop( published_file &_cc_out, 
                                        PublishData *declarations,
					const char * = "generateConstant" );
  
  void _publish_cc_left( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_right( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_value( published_file &_cc_out, PublishData *declarations );
  
  virtual void _publish_cc_headers( published_file &_cc_out );

  virtual void _publish_cc_necessary_copying( published_file &_cc_out );

  void _publish_cc_necessary_attribute_copying( published_file &_cc_out,
                                                SignalAttribute attrib, 
                                                const char *objectSuffix,
                                                PublishData *declarations);

  virtual void _publish_cc_read_or_write( published_file &_cc_out, 
					  const string &,
					  const string & );

  virtual void _publish_cc_attribute_read_or_write( published_file &_cc_out,
						    SignalAttribute attrib, 
						    const string &, 
						    const string &,
						    const string &,
                                                    PublishData *declarations);

  const string _get_cc_signal_attribute_type_suffix();
  const string _get_cc_signal_attribute_subelement_type();
  const string _get_cc_signal_attribute_subelement_type( SignalAttribute attrib );

  void _publish_cc_declarator( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_signal_attribute_suffix( published_file &_cc_out );

  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_locally_static_value( published_file &_cc_out,
						 PublishData *declarations );

  TyvisDeclaration *_get_implicit_declaration();
  void _set_implicit_declaration( TyvisDeclaration * );
  virtual TyvisDeclaration *
  _build_implicit_declaration( TyvisTextLiteral *,
			       TyvisTypeDefinition * ) = 0;
  /**
     Get the name that this attribute uses in the code generator.
  */
  virtual const string &_get_cc_attribute_name() const = 0;

  void _get_list_of_input_signals(  savant::set<Tyvis> *list) ;  

  void _build_sensitivity_list(TyvisDesignatorList*);
  TyvisSignalDeclaration *_build_signal_declaration( TyvisTextLiteral *, TyvisTypeDefinition * );
  TyvisConstantDeclaration *_build_constant_declaration( TyvisTextLiteral *,
							 TyvisTypeDefinition * );
  TyvisTypeDeclaration *_build_type_declaration( TyvisTextLiteral *,
						 TyvisTypeDefinition * );
  TyvisFunctionDeclaration *_build_function_declaration( TyvisTextLiteral *,
							 TyvisTypeDefinition *return_type,
							 TyvisTypeDefinition *argument_type = 0 );

  IIR_Boolean _is_globally_static_primary();
  virtual IIR_Boolean _is_signal_attribute() const { return FALSE; }
  virtual IIR_Boolean _is_type_attribute() const { return FALSE; }
  /**
     Does the code generator need to publish a suffix for this attribute?
  */
  virtual IIR_Boolean _needs_suffix_published(){ return FALSE; }

  virtual Tyvis *_get_suffix( );

  TyvisTextLiteral *_get_attribute_name();

  TyvisTypeDefinition* _get_explicit_signal_type();


protected:
  /**
     Helper function for the derived classes.
  */
  TyvisTextLiteral *_build_attribute_name( const string &attribute_string );

  void _publish_cc_necessary_decl_in_state_last_event( published_file &_cc_out, PublishData *declarations );
  
  void _publish_cc_signal_attribute( published_file &_cc_out,
				     const char *attributeString, 
                                     PublishData *declarations,
				     IIR_Boolean checkSuffixFlag = FALSE);

  void _publish_cc_necessary_signal_decl_in_state( published_file &_cc_out,
						   SignalAttribute attrib,
						   const char *objectSuffix,
                                                   PublishData *declarations);

  void _publish_cc_signal_attribute_subelement_type_suffix( published_file &_cc_out,
							    SignalAttribute attrib);

  void _publish_cc_necessary_signal_state_object_init( published_file &_cc_out,
						       SignalAttribute attrib, 
						       const char *objectSuffix, 
                                                       PublishData *declarations,
						       IIR_Boolean implicitSignal = FALSE);  

  void _publish_cc_necessary_signal_init( published_file &_cc_out,
					  SignalAttribute attrib,
					  const char *objectSuffix, 
                                          PublishData *declarations,
					  IIR_Boolean implicitSignal = FALSE);

  void _publish_cc_default_initial_universal_value( published_file &_cc_out,
						    SignalAttribute attrib,
                                                    PublishData *declarations);

  void _publish_cc_signal_attribute_subelement_type_info( published_file &_cc_out, 
							  SignalAttribute,
                                                          PublishData *declarations);

  TyvisDeclaration *_get_actual_signal_declaration();

  void _check_and_add_necessary_decls(PublishData *declarations);

  /** This method takes a predefined attribute of a terminal or a free
      quantity (in VHDL-AMS) and builds an IIR_FreeQuantityDeclaration.*/
  TyvisFreeQuantityDeclaration *_build_free_quantity_declaration( TyvisTextLiteral *,
								  TyvisTypeDefinition *);
 
  /** This method takes a predefined attribute of a branch quantity
      (in VHDL-AMS) and builds an IIR_BranchQuantityDeclaration. */
  TyvisBranchQuantityDeclaration *_build_branch_quantity_declaration( TyvisTextLiteral *,
								      TyvisTypeDefinition *);

  void _publish_cc_default_array_suffix( published_file &_cc_out );



  void _publish_cc_rvalue_dynamic( published_file &_cc_out,
				   PublishData *declarations );

private:
  /**
     Publishes the prefix for _publish_cc_rvalue.
  */
  void _publish_cc_prefix( published_file &_cc_out,
			   PublishData *declarations );
  /**
     Publishes the suffix for _publish_cc_rvalue.
  */
  void _publish_cc_suffix( published_file &_cc_out,
			   PublishData *declarations );

  bool _can_publish_static_value();

  TyvisTextLiteral *my_attribute_name;
  
  TyvisDeclaration *implicit_declaration;
};
#endif

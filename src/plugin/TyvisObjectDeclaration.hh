#ifndef TYVIS_OBJECTDECLARATION_HH
#define TYVIS_OBJECTDECLARATION_HH

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

#include "TyvisDeclaration.hh"
#include "savant/IIRBase_ObjectDeclaration.hh"

class TyvisObjectDeclaration : public virtual TyvisDeclaration,
			       public virtual IIRBase_ObjectDeclaration {
public:
  TyvisObjectDeclaration();
  ~TyvisObjectDeclaration();

  void _publish_cc_first_objectParameter( published_file &_cc_out, PublishData *declarations );

  string _get_cc_rvalue();
  void _publish_cc_rvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_lvalue( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_left( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_right( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_object_base( published_file &_cc_out, PublishData *declarations );
  
  using TyvisDeclaration::_publish_cc_include;
  void _publish_cc_include( published_file &_cc_out );

  /** Publishes the C++ type of this object followed by a "base" argument
      name.  So "foo :integer" in VHDL would come out "IntegerType foo" in
      the C++.  */
  void _publish_cc_type_and_object_name( published_file &_cc_out, PublishData *declarations );

  /** Publish the arguments of the constructor of the
      variable/signal/constant declaration. */
  void _publish_cc_constructor_args( published_file &_cc_out, PublishData *declarations );

  /** Publish the object declaration with arguments to the constructor:
      Variable <mangled_declarator>( <args> )
  */
  void _publish_cc_decl_with_constructor_args( published_file &_cc_out,
					       PublishData *declarations );

  /** Publish the object declaraor with arguments to the constructor:
      <mangled_declarator>( <args> )
  */
  virtual
  void _publish_cc_declarator_with_constructor_args( published_file &_cc_out,
						     PublishData *declarations );

  void _publish_cc_decl( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_universal_type( published_file &_cc_out );
  void _publish_cc_range( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_bounds( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file &_cc_out );
  void _publish_cc_state_object_init( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_state_l_object_init( published_file &_cc_out, PublishData *declarations );

  /**
     Publish the SignalNetInfo for this object, which should be either an
     IIR_SignalDeclaration or an IIR_SignalInterfaceDeclaration.
  */
  void _publish_cc_signal_net_info( published_file &, cc_publish_mode, PublishData * );

  /**
     Publish a typeinfo for this object, if this object declared it.
     Sometimes the subtype indication of an object will impose additional
     constraints on the base type.  When these subtypes are declared inline
     like this, we need to publish a typeinfo function for the new subtype.
  */
  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations );

  /**
     Used by IIRScram_SignalDeclaration and
     IIRScram_SignalInterfaceDeclaration to publish signal initalization
     code.  In particular, attribute initialization code gets published
     here.
  */
  void _publish_cc_init_signal( published_file &_cc_out, PublishData *declarations );


  TyvisAttributeSpecificationList *_get_attribute_specification_list();

  TyvisDeclaration* _get_package_declaration() { return NULL; }
  virtual IIR_Boolean _is_globally_static();
  IIR_Int32 get_num_indexes();
  
  void _add_declarations_in_initializations(PublishData *declarations);
  virtual Tyvis *_get_value() { return NULL; }
  void _add_decl_into_declaration_collection(PublishData *declarations);
  TyvisAttributeSpecificationList * _get_attributes();
  virtual const IIR_Char *_get_mangling_prefix();
protected:

private:

};
#endif

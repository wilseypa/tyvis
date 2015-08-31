#ifndef TYVIS_SUBPROGRAMDECLARATION_HH
#define TYVIS_SUBPROGRAMDECLARATION_HH

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
#include "savant/IIRBase_SubprogramDeclaration.hh"

class TyvisSequentialStatementList;

class TyvisSubprogramDeclaration : public virtual TyvisDeclaration,
				   public virtual IIRBase_SubprogramDeclaration {
public:
  TyvisSubprogramDeclaration();
  ~TyvisSubprogramDeclaration();

  using TyvisDeclaration::_publish_cc_include;
  /** get the "#include"s that this subprogram needs */
  void _publish_cc_include( published_file &_cc_out );
  void _publish_cc_rvalue( published_file &_cc_out,
			   PublishData *declarations );

  /** Publish the prototype required for the package class declaration. */
  void _publish_cc_prototype( published_file &_cc_out,
			      PublishData *declarations );

  void _publish_cc_package_decl( published_file &_cc_out,
				 PublishData *declarations );

  void _publish_cc_package_definition( published_file &_cc_out,
				       PublishData *declarations );

  /** Publish the declarative part of this subprogram */
  void _publish_cc_declarations( published_file &_cc_out,
				 PublishData *declarations );

  /** Subprograms need to access the state of the processes. For convinience
      we redfine them */
  void _publish_cc_define_current_state( published_file &_cc_out );

  virtual void _publish_cc_type_conversion_function_name( published_file &_cc_out,
							  PublishData *declarations );

  /** This method publishes default file_close for files in this
      subprogram */
  virtual void _publish_cc_implicit_file_close( published_file &_cc_out,
						PublishData *declarations );

  void _publish_cc_static_type_info_fn( published_file &_cc_out,
					PublishData *declarations );

  void _publish_cc_subprogram_call_name( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_subprogram_body( published_file &_cc_out, PublishData *declarations );

  TyvisAttributeSpecificationList* _get_attribute_specification_list();

  /** Checks for the conditions under which this function may be used as a
      resolution function.  If so, extra code has to be published. */
  virtual IIR_Boolean _is_possible_resolution_function() { return FALSE; }

  /** Checks for the conditions under which this function may be used as a
      type conversion function.  If so, extra code has to be published. */
  virtual IIR_Boolean _is_possible_type_conversion_function() {return FALSE;}
  
  /** Returns the return type of the subprogram if it is a function, else,
      returns NULL. */
  virtual TyvisTypeDefinition* _get_return_type();
    
  /** This method returns the minimum number of arguments required by this
      subprogram... */
  IIR_Int32 _num_subprogram_args();
  IIR_Boolean _found_return_statement( );

  void _add_decl_into_declaration_collection(PublishData *) {}

  // Helper Functions
  TyvisInterfaceList           *_get_interface_declarations();
  TyvisSequentialStatementList *_get_subprogram_body();
  TyvisDeclarationList         *_get_subprogram_declarations();

protected:
  /** Publishes just the actual prototype. */
  void _publish_cc_function_prototype( published_file &_cc_out,
				       PublishData *declarations,
				       bool publish_semicolon );



private:
  /** Is this an implicit file function like "open_file", "close_file", etc? */
  IIR_Boolean _is_implicit_file_operation();
  
  /** Is this a non-file implicit operation like "deallocate" */
  IIR_Boolean _is_implicit_operation();

  /** Does the publishing for such functions. */
  void _publish_cc_implicit_file_operation_call_name( published_file & );
  
  /** Does the publishing for such functions - provides control for non standard ops. */
  void _publish_cc_implicit_operation_call_name( published_file & );
};
#endif

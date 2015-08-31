#ifndef TYVIS_INTERFACELIST_HH
#define TYVIS_INTERFACELIST_HH

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

#include "TyvisDeclarationList.hh"
#include "savant/IIRBase_InterfaceList.hh"

class TyvisInterfaceDeclaration;

class TyvisInterfaceList : public virtual TyvisDeclarationList,
			   public virtual IIRBase_InterfaceList {
public:
  void _publish_cc_decl( published_file &(_cc_out), PublishData *declarations );

  /**
     This prints out RValue and LValue declarations for subprogram
     arguments.  These have names like "_" + _get_mangled_declarator().
     These are used with type info from the parameter list to initialize
     variables.  See _publish_cc_decl_subprogram_arg_vars.
  */
  void _publish_cc_decl_subprogram_args( published_file &_cc_out, 
                                         PublishData *declarations);

  /**
     This prints out local variable declarations for all of the arguments
     passed in.  Our notion of type info is applied to these variables.
  */
  void _publish_cc_decl_subprogram_arg_vars( published_file &_cc_out, 
					     PublishData *declarations);

  using TyvisDeclarationList::_publish_cc_list;
  /**
     A version with interface declarations in the signature.
  */
  void _publish_cc_list( published_file &_cc_out,
			 PublishData *declarations,
			 void (TyvisInterfaceDeclaration::*)(published_file &,PublishData *),
			 SeparatorMode separatorMode,
			 const string &separator );

protected:

private:

};
#endif

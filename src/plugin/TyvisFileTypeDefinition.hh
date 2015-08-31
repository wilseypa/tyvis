#ifndef TYVIS_FILETYPEDEFINITION_HH
#define TYVIS_FILETYPEDEFINITION_HH

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

#include "TyvisTypeDefinition.hh"
#include "savant/IIRBase_FileTypeDefinition.hh"

class TyvisFileTypeDefinition : public virtual TyvisTypeDefinition, public virtual IIRBase_FileTypeDefinition {
public:
  void _publish_cc_headers( published_file &_cc_out );
  const string _get_cc_tyvis_type();
  void _publish_cc_data_members( published_file &_cc_out );

  IIR_Boolean _is_globally_static(){ return TRUE; }

  void _publish_cc_type_info_args( published_file &_cc_out,
				   PublishData *declarations,
				   const string &type_name );

protected:

private:
  TyvisTypeDefinition *_get_type_mark();

};
#endif

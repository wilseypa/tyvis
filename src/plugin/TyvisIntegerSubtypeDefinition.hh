#ifndef TYVIS_INTEGERSUBTYPEDEFINITION_HH
#define TYVIS_INTEGERSUBTYPEDEFINITION_HH

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

#include "TyvisIntegerTypeDefinition.hh"
#include "savant/IIRBase_IntegerSubtypeDefinition.hh"

class TyvisIntegerSubtypeDefinition : public virtual TyvisIntegerTypeDefinition,
				      public virtual IIRBase_IntegerSubtypeDefinition {
public:
  void _publish_cc_universal_type( published_file &_cc_out );
  
  //The following function is used to publish the range of the type.
  //And this is useful in publishing ranges speficied in aggregartes.
  //It is intended to be used for aggregate initialization only.
  void _publish_cc_universal_value( published_file &_cc_out, PublishData *declarations );

  TyvisFunctionDeclaration* _get_resolution_function();

protected:

private:

};
#endif

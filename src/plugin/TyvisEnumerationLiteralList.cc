
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

#include "TyvisEnumerationLiteralList.hh"
#include "TyvisEnumerationLiteral.hh"
#include "TyvisTextLiteral.hh"
#include "published_file.hh"

void 
TyvisEnumerationLiteralList::_publish_cc( published_file &_cc_out, PublishData *declarations  ) {
  _publish_cc_rvalues( _cc_out , declarations, "," );
}

int
TyvisEnumerationLiteralList::_get_ordinal_position( TyvisEnumerationLiteral *to_find ){
  ASSERT( to_find != 0 );

  int retval = -1;
  int counter = 0;
  TyvisEnumerationLiteral *literal = dynamic_cast<TyvisEnumerationLiteral *>(first());
  while (literal != NULL) {
    if( IIRBase_TextLiteral::cmp( literal->_get_declarator(),
                                  to_find->_get_declarator()) == 0 ){
      retval = counter;
      break;
    }
    else{
      counter++;
    }
    literal = dynamic_cast<TyvisEnumerationLiteral *>(successor(literal));
  }

  return retval;
}

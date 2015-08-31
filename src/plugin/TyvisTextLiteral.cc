
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

#include "TyvisTextLiteral.hh"
#include "TyvisAccessSubtypeDefinition.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisName.hh"
#include "savant/resolution_func.hh"
#include "published_file.hh"
#include <cctype>
#include <sstream>
  
void 
TyvisTextLiteral::_publish_cc_rvalue( published_file &pf, PublishData * ) {
  _print_cc( pf.get_stream() );
}

ostream &
TyvisTextLiteral::_print_cc( ostream &os ) {
  for( int i = 0; i < get_text_length(); i++ ){
    if ((i > 0) && (i < (get_text_length() - 1))) {
      if ((operator[](i) == '\"')){
        os << "\\\"";
      } else if (operator[](i) == '\\') {
	// If the character is a backslash output two backslashes
	os << "\\\\";
      } else if ((operator[](i) == '%') && (operator[](i + 1) == '%')) {
	os << operator[](i);
	i++;
      } else {
        os << operator[](i);
      }
    }
    else {
      // If the string's delimiters are percentages instead of quotes
      if (operator[](i) == '%') {
	os << '\"';
      } else {
	os << operator[](i);
      }
    }
  }
  return os;
}

string
TyvisTextLiteral::get_string( ) {
  ostringstream ostr;
  _print_cc(ostr);
  return (ostr.str());
}

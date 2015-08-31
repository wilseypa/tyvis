
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
#include "TyvisBitStringLiteral.hh"
#include "TyvisArrayTypeDefinition.hh"
#include "TyvisScalarTypeDefinition.hh"

#include "published_file.hh"
#include "savant/error_func.hh"
#include <sstream>

void
TyvisBitStringLiteral::_publish_cc_rvalue( published_file &_cc_out, PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisBitStringLiteral::_publish_cc" );

  _cc_out << OS("StringLiteral(");
  if( _get_subtype() != 0 ){
    _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
    _cc_out << ",";
  }
  _publish_cc_initialization_value( _cc_out, declarations );
  _cc_out << CS(")");
}

void
TyvisBitStringLiteral::_publish_cc_initialization_value( published_file &_cc_out, PublishData * ) {
  IIR_Int32 length = get_text_length();
  IIR_Char base_specifier = (*this)[0];
  string value_string;

  CC_REF( _cc_out, "TyvisBitStringLiteral::_publish_cc_initialization_value" );

  switch(base_specifier) {
  case 'b':
  case 'B':
    // We have to new the cstringere since this will be deleted at the end.
    value_string = get_text();
    break;
  case 'o':
  case 'O':
    value_string = _convert_octal_to_binary();
    length = value_string.length();
    break;
  case 'x':
  case 'X':
    value_string = _convert_hex_to_binary();
    length = value_string.length();
    break;
  default:
    ostringstream err;
    err << "TyvisBitStringLiteral::_publish_cc_initialization_value( published_file &_cc_out, PublishData *declarations ):"
	<< " unrecognized base specifier `" << base_specifier << "'";
    report_error( this, err.str() );
    abort();
  }

  _cc_out << "\"";
  for( IIR_Int32 i = 0; i < length; i++) { 
    if(value_string[i] == '1' || value_string[i] == '0') {
      _cc_out << value_string[i];
    }
  }
  _cc_out << "\"";
}

void 
TyvisBitStringLiteral::_publish_cc_condition( published_file &_cc_out, PublishData *declarations ){
  _publish_cc_rvalue( _cc_out, declarations );
}

void
TyvisBitStringLiteral::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {
  TyvisScalarTypeDefinition *index_type = NULL;
  TyvisArrayTypeDefinition *array_type = NULL;

  CC_REF( _cc_out, "TyvisBitStringLiteral::_publish_cc_range" );

  if ( _get_subtype()->is_array_type() == TRUE && _get_subtype()->_is_single_dimensional_array_type() == TRUE ){
    array_type = dynamic_cast<TyvisArrayTypeDefinition *>(_get_subtype());
    index_type = array_type->_get_index_subtype();
    if ( index_type != NULL && index_type->get_left() != NULL && index_type->get_right() != NULL ){
      index_type->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
      if ( index_type->is_ascending_range() == TRUE){
	_cc_out << ", ArrayInfo::to, ";
      } else {
	_cc_out << ", ArrayInfo::downto, ";
      }
      index_type->_get_base_type_right()->_publish_cc_value( _cc_out, declarations );
    } else {
      IIR_Int32 length = _get_effective_length();
      _cc_out << "0, ArrayInfo::to, " << (length - 1);
    }
  } else {
    IIR_Int32 length = _get_effective_length();
    _cc_out << "0, ArrayInfo::to, " << (length - 1);
  }
}

void
TyvisBitStringLiteral::_publish_cc_bounds( published_file &_cc_out, PublishData *declarations ) {
  TyvisScalarTypeDefinition *index_type = NULL;
  TyvisArrayTypeDefinition  *array_type = NULL;

  CC_REF( _cc_out, "TyvisBitStringLiteral::_publish_cc_bounds" );
  
  if ( _get_subtype()->is_array_type() == TRUE && 
       _get_subtype()->_is_single_dimensional_array_type() == TRUE ){
    array_type = dynamic_cast<TyvisArrayTypeDefinition *>(_get_subtype());
    index_type = array_type->_get_index_subtype();
    if (index_type != NULL ){
      index_type->_publish_cc_array_info( _cc_out, declarations );
    } else {
      _cc_out << "nullInfo()";
    }
  } else {
    _cc_out << "nullInfo()";
  }
}

void
TyvisBitStringLiteral::_publish_cc_state_object_init( published_file &_cc_out,
						      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisBitStringLiteral::_publish_cc_state_object_init" );

  _cc_out << " ";
  if( _get_subtype() ) {
    _cc_out << "*";
  }
  _cc_out << "( new ";
  _publish_cc_lvalue( _cc_out, declarations );
  _cc_out << " ),";
}

void
TyvisBitStringLiteral::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void
TyvisBitStringLiteral::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisBitStringLiteral::_publish_cc_headers" );

  if(_get_subtype() != NULL) {
    _get_subtype()->_publish_cc_headers( _cc_out );
  }
}

const string
TyvisBitStringLiteral::_convert_octal_to_binary() {
  ostringstream bitStream;
  string bitString;
  IIR_Int32 i;
  IIR_Int32 length = get_text_length();

  // Omit the first character since it is B, X, or O
  for(i = 1; i < length; i++) {
    switch((*this)[i]) {
    case '0':
      bitStream << "000"; break;
    case '1':
      bitStream << "001"; break;
    case '2':
      bitStream << "010"; break;
    case '3':
      bitStream << "011"; break;
    case '4':
      bitStream << "100"; break;
    case '5':
      bitStream << "101"; break;
    case '6':
      bitStream << "110"; break;
    case '7':
      bitStream << "111"; break;
    default:
      // The code-generator assumes that all the error-checking has been
      // done by the front-end, and hence no error is reported here.
      break;
    } // switch
  } // for
  // The return memory is newed here.  It is the caller's responsibility
  // to deallocate this memory.
  bitString = bitStream.str();
  return bitString;
}

      
const string
TyvisBitStringLiteral::_convert_hex_to_binary() {
  ostringstream bitStream;
  string bitString;
  IIR_Int32 i;
  IIR_Int32 length = get_text_length();

  // Omit the first character since it is B, X, or O
  for(i = 1; i < length; i++) {
    switch((*this)[i]) {
    case '0':
      bitStream << "0000"; break;
    case '1':
      bitStream << "0001"; break;
    case '2':
      bitStream << "0010"; break;
    case '3':
      bitStream << "0011"; break;
    case '4':
      bitStream << "0100"; break;
    case '5':
      bitStream << "0101"; break;
    case '6':
      bitStream << "0110"; break;
    case '7':
      bitStream << "0111"; break;
    case '8':
      bitStream << "1000"; break;
    case '9':
      bitStream << "1001"; break;
    case 'a':
    case 'A':
      bitStream << "1010"; break;
    case 'b':
    case 'B':
      bitStream << "1011"; break;
    case 'c':
    case 'C':
      bitStream << "1100"; break;
    case 'd':
    case 'D':
      bitStream << "1101"; break;
    case 'e':
    case 'E':
      bitStream << "1110"; break;
    case 'f':
    case 'F':
      bitStream << "1111"; break;
    default:
      // The code-generator assumes that all the error-checking has been
      // done by the front-end, and hence no error is reported here.
      break;
    } // switch
  } // for
  // The return memory is newed here.  It is the caller's responsibility
  // to deallocate this memory.
  bitString = bitStream.str();
  return bitString;
}


IIR_Int32
TyvisBitStringLiteral::_get_effective_length() {
  IIR_Int32 length = _get_num_digits();
  IIR_Char base_specifier = (*this)[0];
  
  // Get the length of the string as a bit string.
  switch(base_specifier) {
  case 'b':
  case 'B':
    break;
  case 'o':
  case 'O':
    length *= 3;		// One octal digit = 3 bits.
    break;
  case 'x':
  case 'X':
    length *= 4;		// One hex digit = 4 bits.
    break;
  default:
    ostringstream err;
    err << "TyvisBitStringLiteral::_publish_cc_range( published_file &_cc_out, PublishData *declarations ):"
	<< " unrecognized base specifier `" << base_specifier << "'";
    report_error( this, err.str() );
    abort();
  }
  return length;
}


IIR_Int32
TyvisBitStringLiteral::_get_num_digits() {
  IIR_Int32 length = get_text_length();
  IIR_Int32 num_digits = 0;

  // We omit the bit specifier and the two quotes.
  for( int i = 2; i < length-1; i++ ){
    if((*this)[i] != '_') {
      num_digits++;
    }
  }
  return num_digits;
}

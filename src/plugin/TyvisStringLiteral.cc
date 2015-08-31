
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

#include "TyvisDesignFile.hh"
#include "TyvisStringLiteral.hh"
#include "TyvisScalarTypeDefinition.hh"
#include "TyvisAccessSubtypeDefinition.hh"
#include "TyvisArraySubtypeDefinition.hh"

#include "savant/StandardPackage.hh"
#include "savant/savant.hh"
#include "savant/set.hh"
#include "published_file.hh"
#include <cctype>

void
TyvisStringLiteral::_publish_cc_rvalue( published_file &_cc_out,
					PublishData *declarations ) {
  _publish_cc_include( _cc_out, "tyvis/StringLiteral.hh" );
  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc" );
  // The following is just to make the generated code slightly prettier,
  // it's an optimization and not necessary.  (I.e., the else clause will
  // always work.)
  if( !_get_subtype() ||
      _get_subtype() == dynamic_cast<TyvisTypeDefinition *>(get_design_file()->get_standard_package()->get_string_type()) ){
    _cc_out << "VHDL_STRING(";
    _print_cc( _cc_out.get_stream() );
    _cc_out << ")";
  }
  else{
    _cc_out << "StringLiteral" << OS("(");
    //TODO DRH - need to process null string better, static for now 
    if( _get_subtype() != 0 ){
      if ( IIRBase_TextLiteral::cmp(dynamic_cast<TyvisStringLiteral*>(this), "null")){
	if ( _get_subtype()->_get_cc_type_name() == "left" ) {
	  // HACK: Publishing the base type is NOT correct here, but publishing the subtype
	  // was creating bogus "leftTypeInfo()" objects. Needs investigating.
	  _get_subtype()->_get_base_type()->_publish_cc_type_info_xxx( _cc_out, declarations );
	} else {
	  _get_subtype()->_publish_cc_type_info_xxx( _cc_out, declarations );
	}
        _cc_out << ", " << NL();
	_print_cc( _cc_out.get_stream() );
      }
      else{
	_cc_out << "std_standard_stringTypeInfo(), \"\"" << NL();	
      }
    }
    _cc_out << CS(")");
  }
}

void
TyvisStringLiteral::_publish_cc_range( published_file &_cc_out, PublishData *declarations ) {
  int i;
  IIR_Int32 length = get_text_length();
  int funkyChars = 0;
  TyvisTypeDefinition *index_type = _get_subtype()->_get_resolved_index_subtype();

  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_range" );

  if (index_type->is_scalar_type() == TRUE) {
    index_type->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << "," << NL();
  }
  //   _cc_out << "1, to, ";
  // A string can be confined within quotes or percetages but if we 
  // have percentages as delimiters of strings we cannot have quotes inside
  if ((*this)[0] != '%') {
    for( i = 1; i < length-1; i++ ){
      // Check for two consecutive double quotes.
      if (((*this)[i] == '\"') && ((*this)[(i + 1)] == '\"')) {	//"
	i++;
	funkyChars++;
      }
    }
  } else {
    // A string whose delimiters are percentages
    for( i = 1; i < length-1; i++ ){
      // Check for two consecutive double quotes.
      if (((*this)[i] == '%') && ((*this)[(i + 1)] == '%')) {	
	i++;
	funkyChars++;
      }
    }
  }

  if (index_type->_get_base_type_direction()->is_ascending_range() == TRUE) {
    _cc_out << "ArrayInfo::to," << NL();
    index_type->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " + ";
  }
  else {
    _cc_out << " ArrayInfo::downto," << NL();
    index_type->_get_base_type_left()->_publish_cc_value( _cc_out, declarations );
    _cc_out << " - ";
  }
  _cc_out << (length - funkyChars - 3); //omit quotes
}

void 
TyvisStringLiteral::_publish_cc_state_object_init( published_file &_cc_out,
						   PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_state_object_init" );

  _cc_out << " ";
  if( dynamic_cast<TyvisAccessTypeDefinition *>(_get_subtype()) == NULL ) {
    _cc_out << "*";
  }
  _cc_out << "(new ";
  _publish_cc_rvalue( _cc_out , declarations );
  _cc_out << ")," << NL();
}

void
TyvisStringLiteral::_publish_cc_state_l_object_init( published_file &_cc_out,
						     PublishData *declarations ) {
  _publish_cc_state_object_init( _cc_out, declarations );
}

void 
TyvisStringLiteral::_publish_cc_condition( published_file &_cc_out, PublishData *declarations ) {
  ASSERT( _get_current_publish_node() != NULL );
  //_current_publish_node is set during conditions in case statements
  TyvisTypeDefinition* type = _get_current_publish_node()->_get_subtype();

  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_condition" );

  ASSERT(type != NULL);
  _publish_cc_rvalue( _cc_out , declarations );
}

void 
TyvisStringLiteral::_publish_cc_initialization_value( published_file &_cc_out,
						      PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_initialization_value" );
  _publish_cc_rvalue( _cc_out, declarations );
}

void 
TyvisStringLiteral::_publish_cc_without_quotes( published_file &_cc_out ) {
  IIR_Int32 length = get_text_length();
  register int i;

  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_without_quotes" );

  if((*this)[0] == '\'') {
    i = 1;
  } else {
    i = 0;
  }

  for (; i < length - 1; i++) { 
    _cc_out << (*this)[i];
  }

  if((*this)[length - 1] != '\'') {
    _cc_out << (*this)[length-1];
  }
}

void 
TyvisStringLiteral::_publish_cc_function_name( published_file &_cc_out ) {
  IIR_Int32 length = get_text_length();
  IIR_Char c;

  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_function_name" );

  _cc_out << "savant";
  for (int i = 1; i < length-1; i++) { 
    c = (*this)[i];
    if(isalnum(c)) {
      _cc_out << c;
    } 
    else {
      switch(c) {
      case '=':	
	_cc_out << "Equal"; break;
      case '/':	
	if(i+1 < length-1 && (*this)[i+1] == '=') {
	  _cc_out << "NotEqual"; i++; 
	} else {
	  _cc_out << "Divide";
	}	  
	break;
      case '<':	
	if(i+1 < length-1 && (*this)[i+1] == '=') {
	  _cc_out << "LessThanOrEqual"; i++; 
	} else {
	  _cc_out << "LessThan";
	}	  
	break;
      case '>':	
	if(i+1 < length-1 && (*this)[i+1] == '=') {
	  _cc_out << "GreaterThanOrEqual"; i++; 
	} else {
	  _cc_out << "GreaterThan";
	}	  
	break;
      case '+':	_cc_out << "Plus"; break;
      case '-':	_cc_out << "Minus"; break;
      case '&':	_cc_out << "Concatenate"; break;
      case '*':	
	if(i+1 < length-1 && (*this)[i+1] == '*') {
	  _cc_out << "Pow"; i++; 
	} else {
	  _cc_out << "Multiply";
	}	  
	break;
      default:
	cerr << "TyvisStringLiteral::_publish_cc_function_name( published_file &_cc_out )::"
	     << "Unknown operator `" << c << "\'\n";
	abort();
      }	// switch
    } // else 
  } // for
}

void 
TyvisStringLiteral::_publish_cc_universal_value( published_file &_cc_out,
						 PublishData *declarations ) {
  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_universal_value" );
  _publish_cc_rvalue( _cc_out, declarations );
}

void
TyvisStringLiteral::_publish_cc_bounds( published_file &_cc_out, PublishData * ) {

  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_bounds" );

  _cc_out << "nullInfo()";
}

void 
TyvisStringLiteral::_publish_cc_headers( published_file &_cc_out ) {
  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_headers" );

  if(_get_subtype() != NULL) {
    _get_subtype()->_publish_cc_headers( _cc_out );
  }
}

void 
TyvisStringLiteral::_publish_cc_variable_name( published_file &_cc_out ) {
  IIR_Int32 length = get_text_length();
  register int i;

  CC_REF( _cc_out, "TyvisStringLiteral::_publish_cc_variable_name" );

  if((*this)[0] == '\'') {
    _cc_out << "_";
    i = 1;
  } else {
    i = 0;
  }

  for (; i < length-1; i++) { 
    if(isalnum((*this)[i])) {
      _cc_out << (*this)[i];
    } else {
      _cc_out << (unsigned int)(*this)[i];
    }
  }

  if((*this)[length-1] != '\'') {
    if(isalnum((*this)[i])) {
      _cc_out << (*this)[length-1];
    } else {
      _cc_out << (unsigned int)(*this)[length-1];
    }
  }
  else {
    _cc_out << "_";
  }
}

//This function is used to creat objects of corresponding type 
//when string literals are compared with objects of VHDLTypes
const string
TyvisStringLiteral::_convert_function_name( IIR_Boolean _is_unary_operator ){
  int length = get_text_length();
  char c;

  string retval = "savant";
  for( int i = 1; i < length-1; i++ ){ 
    c = (*this)[i];
    if (i == 1) {
      c = toupper(c);
    }
    
    if(isalnum(c)) {
      retval += c;
    } 
    else {
      switch(c) {
      case '=':	
	retval += "Equal"; break;
      case '/':	
	if(i+1 < length-1 && (*this)[i+1] == '=') {
	  retval += "NotEqual"; i++; 
	} 
	else {
	  retval += "Divide";
	}	  
	break;
      case '<':	
	if(i+1 < length-1 && (*this)[i+1] == '=') {
	  retval += "LessThanOrEqual"; i++; 
	} 
	else {
	  retval += "LessThan";
	}	  
	break;
      case '>':	
	if(i+1 < length-1 && (*this)[i+1] == '=') {
	  retval += "GreaterThanOrEqual"; i++; 
	} 
	else {
	  retval += "GreaterThan";
	}	  
	break;
      case '+':
	if (_is_unary_operator == TRUE)  {
	  retval += "Unary";
	}
	retval += "Plus";
	break;
      case '-':
	if (_is_unary_operator == TRUE)  {
	  retval += "Unary";
	}
	retval += "Minus";
	break;
      case '&':	retval += "Concatenate"; break;
      case '*':	
	if(i+1 < length-1 && (*this)[i+1] == '*') {
	  retval += "Pow"; i++; 
	} 
	else {
	  retval += "Multiply";
	}	  
	break;
      default:
	cerr << "TyvisStringLiteral::_publish_cc_function_name( published_file &_cc_out )::"
	     << "Unknown operator `" << c << "\'\n";
	abort();
      }	// switch
    } // else 
  } // for
  return retval;
}

// Copyright (c) 2001 Clifton Labs, Inc.
// All rights reserved. 

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING,
// MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.


// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.

// Author: Dale E. Martin          dmartin@cliftonlabs.com

//---------------------------------------------------------------------------

#include "published_header_file.hh"
#include "savant/IIR.hh"


published_header_file::published_header_file( const string directory,
					      const string name_base,
					      IIR *published_node,
					      ios::openmode mode ) :
  published_file( directory,
		  name_base + get_file_suffix(),
		  mode ), 
  base_name( name_base ){
  publish_standard_header( published_node );
}

published_header_file::~published_header_file(){
  file_close();
}

const string 
published_header_file::get_file_suffix(){
  static const string suffix = ".hpp";
  return suffix;
}

void
published_header_file::publish_headers( ofstream &to_file ){
  to_file << get_generic_header();

  /*
    This is a hack.  If we're appending onto an existing file, we need to
    undefine the existing CPP variable so we see this next part of the
    file.
  */
  if( get_mode() & ios::app ){
    to_file << "#undef " << base_name << "_HPP\n";
  }

  to_file << "#ifndef " << base_name << "_HPP\n"
	  << "#define " << base_name << "_HPP\n";
}

void 
published_header_file::publish_footers( ofstream &to_file ){
  to_file << "\n#endif\n";
}

const string
published_header_file::get_generic_header(){
  return "// This file was automatically generated - don't edit it by hand\n";
}

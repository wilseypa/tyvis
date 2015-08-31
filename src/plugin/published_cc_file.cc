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

#include "published_cc_file.hh"
#include "savant/IIR.hh"

published_cc_file::published_cc_file( const string &directory,
				      const string &name_base,
				      IIR *published_node,
				      ios::openmode mode ) :
  published_file( directory, 
		  name_base + get_file_suffix(),
		  mode ){
  publish_standard_header( published_node );
}

published_cc_file::~published_cc_file(){
  file_close();
}

const string 
published_cc_file::get_file_suffix(){
  static const string suffix = ".cc";
  return suffix;
}

void 
published_cc_file::publish_headers( ofstream & ){}

void 
published_cc_file::publish_footers( ofstream & ){}


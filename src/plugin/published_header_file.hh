#ifndef PUBLISHED_HEADER_FILE_HH
#define PUBLISHED_HEADER_FILE_HH

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

#include "published_file.hh"

class published_header_file : public published_file {
public:
  published_header_file( const string publish_dir,
			 const string name_base,
			 IIR *published_node,
			 ios::openmode mode = ios::trunc | ios::out );

  ~published_header_file();

protected:
  virtual void publish_headers( ofstream &to_file );
  virtual void publish_footers( ofstream &to_file );

private:
  static const string get_file_suffix();
  const string base_name;
  
  const string get_generic_header();
  
};

#endif

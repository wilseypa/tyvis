
#ifndef PUBLISHED_FILE_HH
#define PUBLISHED_FILE_HH

// Copyright (c) 2001-2010 Clifton Labs, Inc.
// All rights reserved. 

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING,
// MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include <string>
#include <fstream>

class include_manager;
class IIR;

using std::ios;
using std::ofstream;
using std::ostream;
using std::ifstream;
using std::string;
using std::ostream;

/** This class represents a published file to the code generator.  It has
    methods to insert #includes, can write a standard header into a
    published file, and stuff like that. */
class published_file {

public:
  /** When we add an include, we actually just store it in an internal
      list, and it's not until the file gets written that it gets dumped
      into the output file.  */
  virtual void add_include( const string file_to_include, bool system_include );

  /** Starts a new block in the published code. */
  virtual void start_block();
  /** Ends the current block. */
  virtual void end_block();
  /** Opens a new scope, increasing the indent and publishing the string
      passed in. */
  virtual void open_scope( const string &withString );
  /** Closes the scope, decreasing the indent and publishing the string
     passed in. */
  virtual void close_scope( const string &withString );
  /** Ends the current class definition. */
  virtual void end_class();
  /** Ends the C++ statement by inserting a ; and a newline.  Inserts
      appropriate indention. */
  virtual void end_statement();

  /** Ends the current line.  Inserts appropriate indention. */
  virtual void new_line();

  /** Inserts the comment text in the published file.  The proper prefix
      (e.g. # for a Makefile) will be prepended. */
  virtual void insert_comment( const string &comment_text );

  /** Inserts the default number of spaces. */
  void indent(){ indent( indention ); }

  ostream &get_stream();

protected:
  /**  Constructor      

       @param output_directory This parameter will be prepended to the file
         name when opening the file.  Must be non-null.
     
       @param file_name The name of the file to open.  Must be non-null.

       @param mode Optional mode to open the file with, defaults to ios::out
         | ios::trunc.  Can be any ios:: value - it's passed down to the
         fstream we're derived from.
  */
  published_file( const string &output_directory,
		  const string &file_name,
		  ios::openmode = ios::trunc | ios::out );

  /** Destructor - closes the file - writes it, etc. */
  virtual ~published_file( );

  /** Takes a file name in prefix, and adds the suffix "suffix".  Memory
      returned is allocated via new and should be freed via delete []. */
  static const string concat( const string prefix, 
				   const string suffix );

  /** Takes a directory name in dir, and adds the filename.  Memory
      returned is allocated via new and should be freed via delete [].*/
  static const string full_path( const string dir, 
				      const string filename );

  virtual void publish_headers( ofstream &to_file ) = 0;
  virtual void publish_footers( ofstream &to_file ) = 0;

  /** This method does all of the dirty work of closing a file.  It flushes
      the contents file, creates a temporary file of the headers +
      contents, and then compares it with the existing file (if there is
      one), overwriting if they're different. */
  void file_close();

  /** Returns the mode with which this file was opened.  This is a hack to
      support file appending and should go away if/when file appending
      does. */
  int get_mode(){ return mode; }

  /** Publishes the header at the top of the file. */
  void publish_standard_header( IIR *published_node );

private:
  /** Inserts spaces spaces. */
  void indent( unsigned int spaces );

  /** This method returns a contents file name for the file passed in. */
  static const string get_contents_file_name( const string for_file );

  /** Suffix of a contents file. */
  static const string get_contents_suffix();
  /** Suffix of a tmp file. */
  static const string get_tmp_suffix();

  /** Duplicates #input_file into #output_file. */
  static void duplicate_file( ifstream &input_file,
			      ofstream &output_file );

  /** This is the name that we will ultimately end up with.  Due to how
      this file is generated - includes added independently, etc, we'll
      have some tempory files prior to getting this name. */
  const string file_name;

  /** As things get written this file, we cache them into a
      "contents_file".  When we get closed, we write headers out, and then
      copy in the contents of contents file. */
  const string contents_file_name;

  /** This is the mode that we were opened with. */
  const int mode;

  unsigned int indention;

  /** This method builds the temp file from the headers and contents file
      It returns the temporary file's name. */
  const string construct_temporary_file();

  /** This method copies the contents file into the stream passed in. */ 
  void publish_contents_file( ofstream &to_file );

  include_manager *my_include_manager;

  ofstream contents_file;
};

#include "publish_modifiers.hh"

inline published_file &
operator<<( published_file &pf, const string &str ){
  pf.get_stream() << str;
  return pf;
}

inline published_file &
operator<<( published_file &pf, double d ){
  pf.get_stream() << d;
  return pf;
}

inline published_file &
operator<<( published_file &pf, char c ){
  pf.get_stream() << c;
  return pf;
}

inline published_file &
operator<<( published_file &pf, int i ){
  pf.get_stream() << i;
  return pf;
}

inline published_file &
operator<<( published_file &pf, unsigned int ui ){
  pf.get_stream() << ui;
  return pf;
}

inline published_file &
operator<<( published_file &pf, long l ){
  pf.get_stream() << l;
  return pf;
}

inline published_file &
operator<<( published_file &pf, char *p ){
  pf.get_stream() << p;
  return pf;
}

inline published_file &
operator<<( published_file &pf, const char *p ){
  pf.get_stream() << p;
  return pf;
}

inline published_file &
operator<<( published_file &pf, void *p ){
  pf.get_stream() << p;
  return pf;
}

inline published_file &
operator<<( published_file &pf, const new_line & ){
  pf.new_line();

  return pf;
}

inline published_file &
operator<<( published_file &pf, const open_scope &os ){
  pf.open_scope( os.get_prefix() );

  return pf;
}

inline published_file &
operator<<( published_file &pf, const close_scope &os ){
  pf.close_scope( os.get_prefix() );

  return pf;
}
#endif

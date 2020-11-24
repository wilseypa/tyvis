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

#include <FileManager.h>

#include "published_file.hh"
#include <savant/savant.hh>
#include <savant/include_manager.hh>
#include <savant/error_func.hh>
#include <savant/IIR_Identifier.hh>
#include <StringUtilities.h>

published_file::published_file( const string &output_directory,
				const string &init_file_name,
				ios::openmode mode ) :
  file_name( full_path( output_directory, init_file_name )), 
  contents_file_name( get_contents_file_name( file_name ) ), 
  mode( mode ),
  indention(0),
  my_include_manager( new include_manager ){
  // We can't explicitly call the ofstream constructor due to ordering
  // problems.
  contents_file.open( contents_file_name.c_str(), mode );

  ASSERT( contents_file.good() );
}

void
published_file::publish_standard_header( IIR *published_node ){
  ASSERT( published_node != 0 );
  // The embedded endl in here is on purpose - we want the file to flush.
  insert_comment( "This file was published as the result of processing the IIR " );
  insert_comment( string("node ") + published_node->get_kind_text() );
  if( published_node->get_file_name() ){
    insert_comment( "from the VHDL file " + published_node->get_file_name()->convert_to_string() );
    insert_comment( "line " + intToString(published_node->get_line_number()) + "." );
    insert_comment( "It's probably best not to be editing this file by hand since" );
    insert_comment( "it was automatically generated.\n\n" );
  }
}

published_file::~published_file(){
  delete my_include_manager;
}

void
published_file::file_close(){
  // Insure that we end with a newline so we don't get a compiler warning.
  *this << "\n\n";
  /**
     Commit the contents file to disk
  */
  contents_file.flush();
  /** Create a temp file. */
  const string temp_file_name = construct_temporary_file();
  /**
     Now we're going to ram the headers in, the contents, and any footers.
  */
  contents_file.close();
  
  /**
     So, now we can whack the contents file.
  */
  FileManager::instance()->unlink( contents_file_name );

  if( FileManager::instance()->checkFileStatus( file_name ) == FileManager::OK ){
    // Then the named file existed, so let's compare it with the file we just created
    if( FileManager::instance()->fileCompare( temp_file_name, file_name ) == 0){
      // The files are identical so we'll just unlink the temp file
      FileManager::instance()->unlink( temp_file_name );
    }
    else{
      // We'll whack the original file.
      FileManager::instance()->unlink( file_name );      
      // Now we'll move the temp file to the real filename
      FileManager::instance()->rename( temp_file_name, file_name );
    }
  }
  else{
    FileManager::FileStatus status = 
      FileManager::instance()->rename( temp_file_name, file_name );
    if( status != FileManager::OK ){
      const string error = "There was an error trying to rename |" + temp_file_name + "| to |"
	+ file_name + "|.";
      report_error( error );
    }
  }
}

const string 
published_file::get_contents_file_name( const string for_file ){
  string retval = for_file + get_contents_suffix();

  return retval;
}

const string 
published_file::construct_temporary_file(){
  // First we need to open a temp file.
  const string retval = file_name + get_tmp_suffix();
  ofstream temp_file( retval.c_str() );
  
  if( mode & ios::app ){
    ifstream orig_file( file_name.c_str() );
    duplicate_file( orig_file, temp_file );
  }

  // Publish the headers.
  publish_headers( temp_file );

  // Dump the includes
  my_include_manager->publish_includes( temp_file );

  // Publish the contents file into it.
  publish_contents_file( temp_file );
  // Publish footers
  publish_footers( temp_file );

  return retval;
}

void 
published_file::publish_contents_file( ofstream &outfile ){
  ifstream contents_file( contents_file_name.c_str() );
  duplicate_file( contents_file, outfile );
}

const string 
published_file::get_contents_suffix(){
  const string retval = ".contents";
  return retval;
}

const string 
published_file::get_tmp_suffix(){
  const string retval = ".tmp";
  return retval;
}

const string 
published_file::concat( const string prefix, const string suffix ){
  const string filename = prefix + suffix;
  return filename;
}

const string 
published_file::full_path( const string dir, const string filename ){
  int len = dir.size();
  string with_slash;
  if( dir[ len ] != '/' ){
    with_slash = dir + "/";
  }
  else{
    with_slash = dir;
  }

  const string retval = with_slash + filename;

  return retval;
}

void 
published_file::duplicate_file( ifstream &input_file,
				ofstream &output_file ){
  const int size = 4096;
  char buf[size];
  int count = 0;

  // This should ram the whole file into the output buf.
  while( input_file.eof() == false && input_file.fail() == false ){
    input_file.read( buf, size );
    count = input_file.gcount();
    output_file.write( buf, count );
  }
  output_file.flush();
  input_file.close();
}


void
published_file::add_include( const string include_file_name, bool system_include ){
  if( include_file_name != 
      FileManager::instance()->baseName( file_name ) ){
    my_include_manager->add_include( include_file_name, system_include );
  }
}

void
published_file::insert_comment( const string &comment_text ){
  *this << "// " << comment_text;
  new_line();
}

void 
published_file::start_block(){
  open_scope( "{" );
}

void
published_file::end_block(){
  close_scope( "}" );
}

void
published_file::open_scope( const string &withString ){
  *this << withString;
  indention += 2;
  new_line();
}

void
published_file::close_scope( const string &withString ){
  if( indention >= 2 ){
    indention -= 2;
  }
  else{
    cerr << "Asked to close a scope when the indention was already at " << indention 
	 << endl;
    abort();
  }
  new_line();
  *this << withString;
  new_line();
}

void
published_file::end_class(){
  close_scope( "};" );
}

void
published_file::indent( unsigned int spaces ){
  for( unsigned int i = 0; i < spaces; i++ ){
    *this << " ";
  }
}

void
published_file::new_line(){
  *this << "\n";
  indent( indention );
}

void
published_file::end_statement(){
  *this << ";\n";
  indent();
}

ostream &
published_file::get_stream(){
  return contents_file;
}

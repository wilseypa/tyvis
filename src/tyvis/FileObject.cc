// Copyright (c) Ohio Board of Regents and the University of Cincinnati.  
// All Rights Reserved.

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

#include "FileObject.hh"
#include "AccessTypeInfo.hh"
#include "AccessObject.hh"
#include "std_textioPkg.hh"
#include "FileTypeInfo.hh"
#include <FileManager.h>
#include <cstdio>

using std::ios;
using std::fstream;

const int maxExtraInFilesInProcedure = 5;
const int maxExtraOutFilesInProcedure = 5;

FileObject::FileObject( const FileTypeInfo &initTypeInfo ) :
  ObjectBase( initTypeInfo, "" ),
  fileHandle(0),
  openKind( SAVANT_READ_MODE ){}

FileObject::FileObject(const FileObject &that) :
  ObjectBase( that ),
  fileHandle( that.fileHandle ),
  openKind( that.openKind ){}

FileObject::FileObject( const FileTypeInfo &initTypeInfo,
			const string &fileName, 
			const EnumerationLiteral &mode ) :
  ObjectBase( initTypeInfo, fileName ),
  openKind( mode ){
  fileHandle = new std::fstream();
  openFile(fileHandle, fileName, mode);
}

FileObject::FileObject( const FileTypeInfo &initTypeInfo,
			const CompositeLiteral &fileName,
			const EnumerationLiteral &mode ) :
  ObjectBase( initTypeInfo, fileName.toString() ),
  openKind( mode ){
  fileHandle = new std::fstream();
  openFile(fileHandle, fileName.toString(), mode);
}

FileObject::~FileObject(){
  if( fileHandle && *fileHandle ){
    fileHandle->flush();
    fileHandle->close();
  }
}

void
FileObject::print(ostream& ) const {
  abort();
}

string
FileObject::nextToken(){
  ASSERT( fileHandle != 0 );
  ASSERT( *fileHandle );
  string buf;
  *fileHandle >> buf;
  return buf;
}

std::ios::openmode
FileObject::getOpenMode( const RValue &mode ){
  ios::openmode retval = ios::in;
  switch( mode.getIntValue() ){
  case 0:{
    retval = ios::in;
    break;
  }
  case 1:{
    retval = ios::out;
    break;
  }
  case 2:{
    retval = ios::out | ios::app;
    break;
  }
  default:{
    cerr << "Unknown file_open_kind |" << mode.toString() << "| in FileObject::getOpenMode"
	 << ", aborting!";
    abort();
  }
  }  
  return retval;
}


void
FileObject::openFile( std::fstream* stream, const string &fileName, 
		      const RValue &mode ){
  FileManager::instance()->open( stream, fileName, getOpenMode(mode) );
  if( stream == NULL || !stream->good() ){
    char message[255];
    snprintf( message, sizeof(message), "Error opening %s", fileName.c_str() );
    perror( message );
  }
}

void
FileObject::openFile( std::fstream* stream, const Slice &fileName, 
		      const RValue &mode ){
  return openFile( stream, fileName.toString(), mode );
}

void
FileObject::open( const string &fileName, const RValue &openMode ){
  if ((fileHandle == NULL) || !fileHandle->is_open()) {
    fileHandle = new std::fstream();
    openFile(fileHandle, fileName, openMode);
  } else {
    cerr << "file_open called on " << fileName << " which was already open - aborting()" << endl;
    abort();
  }
}

void
FileObject::open( const string &fileName, const RValue &openMode, LValue &status ){
  // Need a bit more work here to support MODE_ERROR
  if( fileHandle != 0 ){
    status.updateVal( STATUS_ERROR );
  }
  else{
    open( fileName, openMode );
    if( fileHandle == 0 || !*fileHandle ){
      status.updateVal( NAME_ERROR );
    }
    else{
      status.updateVal( OPEN_OK );
    }
  }
}

void
FileObject::close(){
  cerr << "closing file." << endl;
  fileHandle->flush();
  fileHandle->close();
}

void
FileObject::read( LValue &readInto ){
  readInto.updateVal( getFileTypeInfo().getElementTypeInfo().value(*this) );
}

void
FileObject::write( const RValue &value ){
  ASSERT( fileHandle != 0 );
  *fileHandle << value.toString() << "\n";
  fileHandle->flush();
}

RValue *
FileObject::clone() const{
  return new FileObject(*this);
}

void
FileObject::writeline( LValue &line ){
  ASSERT( fileHandle != 0 );
  (*fileHandle) << line.toString() << endl;
}

void
FileObject::readline( AccessObject &line ){
  ASSERT( fileHandle != 0 );
  string buffer;
  *fileHandle >> buffer;
  line.updateVal( VHDL_STRING(buffer) );
}

void
FileObject::readline( AccessObject &line, int &count){
  readline( line );
  count = line.length().getIntValue();
}

void 
FileObject::readline( LValue &line ){
  ASSERT( fileHandle != 0 );
  char char_buffer[255];
  fileHandle->getline(char_buffer, 255);
  line.updateVal( VHDL_STRING(char_buffer) );
}

const EnumerationLiteral &
FileObject::getFileOpenKind() {
  return openKind;
}

void
FileObject::setFileOpenKind(const EnumerationLiteral& kind){
  openKind = kind;
}

FileObject&
FileObject::operator=(const FileObject &val) {
  ObjectBase::operator=( val );
  openKind = val.openKind;
  fileHandle = val.fileHandle;

  return *this;
}

RValue &
FileObject::operator=(const RValue &val) {
  return operator=(dynamic_cast<const FileObject &>(val));
}

bool
FileObject::operator==( const RValue &val ) const {
  return (fileHandle == (dynamic_cast<const FileObject &>(val).fileHandle ));
}
bool
FileObject::operator!=( const RValue &val ) const {
  return !operator==(val);
}
bool
FileObject::operator<( const RValue & ) const {
  abort();
}
bool
FileObject::operator<=( const RValue & ) const {
  abort();
}
bool
FileObject::operator>( const RValue & ) const {
  abort();
}
bool
FileObject::operator>=( const RValue & ) const {
  abort();
}

UniversalInteger
FileObject::endfile(){
  ASSERT( fileHandle != 0 );
  // if there is a newline character in the stream, remove it before testing for EOF
  if (fileHandle->peek() == '\n') {
    fileHandle->ignore();
  };
  return fileHandle->peek() == EOF;
}


const RValue &
FileObject::readVal() const {
  return *this;
}

RValue &
FileObject::readVal(){
  return *this;
}

void
FileObject::updateVal( const RValue & ){
  abort();
}

const FileTypeInfo &
FileObject::getFileTypeInfo() const {
  ASSERT( dynamic_cast<const FileTypeInfo *>(&getTypeInfo()) != 0 );
  return dynamic_cast<const FileTypeInfo &>(getTypeInfo());
}

void
FileObject::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "openKind:";
  openKind.debugPrint( os, indent+5);

  return;
}

int
savantfile_open( VHDLKernel *,
		 LValue &fileObject,
		 const RValue &fileName,
		 const RValue &openMode ){
  dynamic_cast<FileObject &>(fileObject.readVal()).open( fileName.toString(), 
							 openMode );
  return NORMAL_RETURN;
}

int
savantfile_open( VHDLKernel *,
		 FileObject &fileObject,
		 const StringLiteral fileName,
		 const EnumerationLiteral openMode ){
  fileObject.open( fileName.toString(), openMode );
  return NORMAL_RETURN;
}

int
savantfile_open( VHDLKernel *,
		 LValue &fileOpenStatus,
		 LValue &fileObject,
		 const RValue &fileName,
		 const RValue &openMode ){
  dynamic_cast<FileObject &>(fileObject.readVal()).open( fileName.toString(), 
							 openMode,
							 fileOpenStatus );
  return NORMAL_RETURN;
}

int
savantfile_close( VHDLKernel *, LValue &fileObject ){
  dynamic_cast<FileObject &>(fileObject.readVal()).close();
  return NORMAL_RETURN;
}

int
savantread( VHDLKernel *, LValue &fileObject, LValue &readValue ){
  dynamic_cast<FileObject &>(fileObject.readVal()).read( readValue );
  return NORMAL_RETURN;
}

int
savantread( VHDLKernel *, LValue &fileObject, LValue &readValue, LValue &length ){
  dynamic_cast<FileObject &>(fileObject.readVal()).read( readValue );
  length.updateVal( UniversalInteger(readValue.length(UniversalInteger(1))) );
  return NORMAL_RETURN;
}

int
savantwrite( VHDLKernel *, LValue &fileObject, const RValue &toWrite ){
  dynamic_cast<FileObject &>(fileObject.readVal()).write( toWrite );
  return NORMAL_RETURN;
}

Value
savantendfile( VHDLKernel *, LValue &fileObject ){
  return dynamic_cast<FileObject &>(fileObject.readVal()).endfile();
}

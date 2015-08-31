#ifndef FILETYPE_HH
#define FILETYPE_HH

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

#include "tyvis/tyvis-config.h"
#include "tyvis/ObjectBase.hh"
#include "tyvis/EnumerationLiteral.hh"

#include <fstream>
using std::fstream;
using std::ios;

class AccessObject;
class FileTypeInfo;
class SimulationStream;

class FileObject : public ObjectBase {
public:
  FileObject(const FileTypeInfo &);

  FileObject(const FileObject &);

  FileObject( const FileTypeInfo &,
	      const string &fileName,
	      const EnumerationLiteral &openMode );

  FileObject( const FileTypeInfo &,
	      const CompositeLiteral &,
	      const EnumerationLiteral &openMode );
  
  virtual ~FileObject();

  ObjectBase* getObject() const {
    return NULL;
  }

  virtual void print(ostream& = cout) const;
  
  RValue *clone() const;

  //@{
  /** Support for std.textio */
  void writeline( LValue &line );
  void readline(  AccessObject &);
  void readline(  AccessObject &, int &numRead );
  void readline(  LValue &line );
  //@}

  //@{
  /** Support for implicit operations */
  void open( const string &fileName, const RValue &openMode );
  void open( const string &fileName, const RValue &openMode, LValue &status );
  void close();
  void read( LValue &readInto );
  void write( const RValue &value );
  UniversalInteger endfile();
  //@}

  int getHandle() const;
  const EnumerationLiteral &getFileOpenKind();
  void setFileOpenKind(const EnumerationLiteral &);

  RValue &operator=(const RValue &);
  FileObject &operator=(const FileObject &);

  bool operator==( const RValue & ) const;
  bool operator!=( const RValue & ) const;
  bool operator>( const RValue & ) const;
  bool operator>=( const RValue & ) const;
  bool operator<( const RValue & ) const;
  bool operator<=( const RValue & ) const;

  const RValue &readVal() const;
  RValue &readVal();
  void updateVal( const RValue &newValue );

  const FileTypeInfo &getFileTypeInfo() const;

  /** Pulls out whitespace. */
  string nextToken();

protected:
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:  
  static fstream &openFile( const string &fileName, 
			    const RValue &openMode );
  
  static fstream &openFile( const Slice &fileName, 
			    const RValue &openMode );

  /** Turns a VHDL open_kind into an ios::openmode. */
  static ios::openmode getOpenMode( const RValue &mode );

  /** We hold a pointer to this guy, but it is NOT ours to delete. */
  fstream *fileHandle;
  EnumerationLiteral openKind;
};

//@{
/** The following methods support the implicit file operations defined in
    section 3.4.1 of the '93 LRM. */

/** Implicit file_open operator. */
int
savantfile_open( VHDLKernel *processPtr,
		 LValue &fileObject,
		 const RValue &openMode );

/** Implicit "file_open" operator. */
int
savantfile_open( VHDLKernel *processPtr,
		 FileObject &fileObject,
		 const StringLiteral fileName,
		 const EnumerationLiteral openMode );

/** Implicit "file_open" operator. */
int
savantfile_open( VHDLKernel *processPtr,
		 LValue &fileOpenStatus,
		 LValue &fileObject,
		 const RValue &fileName,
		 const RValue &openMode );

/** Implicit "file_close" operator. */
int
savantfile_close( VHDLKernel *processPtr, LValue &fileObject );

/** Implicit "read" operator. */
int savantread( VHDLKernel *processPtr, LValue &fileObject, LValue &readValue );

/** Implicit "read" operator for unconstrained Array types. */
int savantread( VHDLKernel *processPtr, LValue &fileObject, LValue &readValue, LValue &length );

/** Implicit "write" operation. */
int savantwrite( VHDLKernel *processPtr, LValue &fileObject, const RValue &toWrite );

/** Implicit "endfile" operation. */
Value savantendfile( VHDLKernel *processPtr, LValue &fileObject );

//@}

#endif

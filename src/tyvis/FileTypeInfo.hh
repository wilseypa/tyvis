#ifndef FILE_TYPE_INFO_HH
#define FILE_TYPE_INFO_HH

// Copyright (c) Clifton Labs, Inc.  All Rights Reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "tyvis/tyvis-config.h"
#include "tyvis/TypeInfo.hh"

class SubElementFactory;

class FileTypeInfo : public TypeInfo {
public:
  FileTypeInfo( const TypeInfo &elementTypeInfo );
  FileTypeInfo( const FileTypeInfo &that );
  ~FileTypeInfo();

  TypeInfo *clone() const;
  bool operator==( const TypeInfo &that ) const;

  RValue *create( const SubElementFactory *parent ) const;

  /**
     Take the value passed in and convert it to a string for us, in terms
     of the element info.
  */
  string writeToString( const RValue &value ) const;

  const TypeInfo &getElementTypeInfo() const;
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  const TypeInfo *myElementTypeInfo;
};

#endif

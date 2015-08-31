#ifndef ELEMENT_INFO_HH
#define ELEMENT_INFO_HH

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
#include <string>
using std::string;

class RValue;
class Element;

class ElementInfo : public TypeInfo {
public:
  ElementInfo( const TypeInfo &initTypeInfo,
	       const string &initIdentifier );

  ElementInfo( const ElementInfo &that );

  TypeInfo *clone() const;

  ElementInfo &operator=( const ElementInfo & that );

  bool operator==( const TypeInfo &that ) const;

  ~ElementInfo();

  /**
     Return this element's identifier.
  */
  const string &getIdentifier() const;

  const TypeInfo &getTypeInfo() const;

  virtual const ScalarTypeInfo &range( const RValue &index = defaultIndex() ) const;
  virtual const ScalarTypeInfo &reverse_range( const RValue &index = defaultIndex() ) const;

  RValue *create( const SubElementFactory *factory ) const;

  bool isCompositeType() const;
  bool isScalarType() const;
  bool isPhysicalType() const;
  bool isCharacterType() const;

  const TypeInfo & readVal() const {return *myTypeInfo;}

  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  const TypeInfo *myTypeInfo;
  string myIdentifier;
};

#endif

#ifndef SCALAR_TYPE_INFO_HH
#define SCALAR_TYPE_INFO_HH

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
#include "tyvis/TypeInfo.hh"
#include "tyvis/StringLiteral.hh"

/** Holds the basic information related to scalar types; left, direction
    and right. */
class ScalarTypeInfo :  public TypeInfo {
public:
  bool operator==( const TypeInfo &that ) const;

  bool isScalarType() const { return true; }

  /** Returns the left bound of this ScalarTypeInfo. */
  const RValue &left( const RValue & = defaultIndex() ) const { return *myLeft; }
  void setLeft( const RValue &newLeft );

  /** Returns the direction of this ScalarTypeInfo. */
  ArrayInfo::ArrayDirn_t getDirection() const { return myDirn; }
  void setDirection( ArrayInfo::ArrayDirn_t newDirection );

  /** Returns the right bound of this ScalarTypeInfo. */
  const RValue &right( const RValue & = defaultIndex() ) const { return *myRight; }
  void setRight( const RValue &newRight );

  ScalarTypeInfo();

  /** Constructor that clones the left and right values passed in. */
  ScalarTypeInfo( const RValue &initLeft, ArrayInfo::ArrayDirn_t, const RValue &initRight );

  /** Constructor that assumes ownership of left and right values passed
      in. */
  ScalarTypeInfo( const RValue *initLeft, ArrayInfo::ArrayDirn_t, const RValue *initRight );

  /** Deep copies the ScalarTypeInfo passed in. */
  ScalarTypeInfo(const ScalarTypeInfo &ri);

  ScalarTypeInfo& operator=(const ScalarTypeInfo &that );

  TypeInfo& operator=(const TypeInfo &that ) {
    operator=(static_cast<const ScalarTypeInfo &>(that));
    return *this;
  }

  virtual ~ScalarTypeInfo() = 0;

  //@{
  /** The following methods are in support of type attributes. */
  StringLiteral image( const RValue &imageOf ) const;
  virtual const RValue &high( const RValue &index = defaultIndex() ) const;
  virtual const RValue &low( const RValue &index = defaultIndex() ) const;
  virtual const EnumerationLiteral &ascending( const RValue &index = defaultIndex() ) const;
  RangeIterator iterator( const RValue &index = defaultIndex() ) const;
  virtual UniversalInteger length( const RValue &index = defaultIndex() ) const;
  //@}

  virtual const ScalarTypeInfo &range( const RValue &index = defaultIndex() ) const;
  virtual const ScalarTypeInfo &reverse_range( const RValue &index = defaultIndex() ) const;

  virtual int storageIndex( int elementIndex ) const;
  virtual void debugPrintDetails( ostream &os, int indent ) const;

protected:
  
private:
  const RValue *myLeft;
  ArrayInfo::ArrayDirn_t myDirn;
  const RValue *myRight;
};

#endif

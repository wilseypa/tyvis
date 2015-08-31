#ifndef ARRAY_TYPE_INFO_HH
#define ARRAY_TYPE_INFO_HH

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
#include "tyvis/CompositeTypeInfo.hh"
#include "tyvis/RangeIterator.hh"
#include <vector>
using std::vector;
class ScalarTypeInfo;

/** 
    This holds info about the array type.
*/
class ArrayTypeInfo : public CompositeTypeInfo {
public:
  ArrayTypeInfo();

  /**
     Main constructor.  Does not take ownership
  */
  ArrayTypeInfo( const TypeInfo &initElementTypeInfo,
		 const ScalarTypeInfo &initBound );


  ArrayTypeInfo( const ArrayTypeInfo &that );

  ~ArrayTypeInfo();

  bool operator==( const TypeInfo &that ) const;
  TypeInfo *clone() const;
 
  bool isCompositeType() const { return true; }
  bool isConstrained() const;
  bool isUnconstrained() const;
  bool isCharacterType() const;

  int numDimensions() const;

  ArrayTypeInfo &operator=(const ArrayTypeInfo&);

  const TypeInfo &getElementTypeInfo( const RValue &index = defaultIndex() ) const;
  const ScalarTypeInfo &getIndexTypeInfo( const RValue &index = defaultIndex() ) const;

  virtual RValue *create( const SubElementFactory * ) const;

  string getSubFieldName( const string &prefix, int d ) const;
  const TypeInfo &getFieldInfo( int fieldIndex ) const;

  void constructFields( vector<RValue *> &elements, 
			const SubElementFactory *subElementFactory,
			int numElements = -1 ) const;

  virtual int storageIndex( int elementIndex ) const;

  //@{
  /**
     Array type attributes.
  */
  const RValue &left( const RValue &dimension = defaultIndex() ) const;
  const RValue &right( const RValue &dimension = defaultIndex() ) const;
  const RValue &low( const RValue &dimension = defaultIndex() ) const;
  const RValue &high( const RValue &dimension = defaultIndex() ) const;

  virtual const ScalarTypeInfo &range( const RValue &index = defaultIndex() ) const;
  virtual const ScalarTypeInfo &reverse_range( const RValue &index = defaultIndex() ) const;

  UniversalInteger length( const RValue &index = defaultIndex() ) const;
  //@}

  using TypeInfo::value;
  Value value( const string &initValue ) const;
  virtual void debugPrintDetails( ostream &os, int indent ) const;

protected:
  
private:
  /** When the initialization string passed in is in the form of an aggregate,
      this method is used to do the processing. */
  Value valueFromAggregate( const string &initValue ) const;
  /** When the initialization string passed in is in the form of a string,
      this method is used to do the processing. */
  Value valueFromString( const string &initValue ) const;

  friend class CompositeLiteral;

  const ScalarTypeInfo *myIndexTypeInfo;
  const TypeInfo *myElementTypeInfo;
};

#endif

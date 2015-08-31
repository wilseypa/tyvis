#ifndef COMPOSITE_LITERAL_HH
#define COMPOSITE_LITERAL_HH

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
#include "tyvis/LValue.hh"
#include "tyvis/UniversalInteger.hh"

class TyvisVarArgs;
class CompositeTypeInfo;
class SubElementFactory;
class ArrayTypeInfo;
class RecordTypeInfo;
class ScalarTypeInfo;

class CompositeLiteral : public LValue {
public:
  CompositeLiteral();
  CompositeLiteral( const CompositeTypeInfo &initTypeInfo );

  CompositeLiteral( const TypeInfo &initTypeInfo,
		    const RValue &initValue );

  CompositeLiteral( const CompositeTypeInfo &initTypeInfo,
		    TyvisVarArgs &initValue );
  
  CompositeLiteral( TyvisVarArgs &initValue );


  CompositeLiteral( const SubElementFactory &subElementFactory );
  CompositeLiteral( const ArrayTypeInfo &typeInfo,
		    const string &initValue );
  CompositeLiteral( const ArrayTypeInfo &typeInfo,
		    const RValue &initValue );
  CompositeLiteral( const CompositeLiteral &that );

  /**
     Constructor called when building slices.  This CompositeLiteral will
     hold pointers to the members of "that", myTypeInfo will be cloned and
     deleted when we destructed.
  */
  CompositeLiteral( CompositeLiteral &that,
		    const ScalarTypeInfo &sliceSpecifier );

  ~CompositeLiteral();

  int getIntValue() const;
  LONG getInt64Value() const;
  double getDoubleValue() const;

  virtual void vcdPrint( ostream &os ) const {os << "INVALID";}

  bool isScalarType() const { return false; }
  bool isCompositeType() const { return true; }

  RValue &operator=( const RValue &that );
  CompositeLiteral &operator=( const CompositeLiteral &that );

  bool operator==( const RValue & ) const;
  bool operator!= (const RValue & ) const;
  bool operator> (const RValue &) const;
  bool operator>= (const RValue &) const;
  bool operator< (const RValue &) const;
  bool operator<= (const RValue &) const;

  const TypeInfo &getTypeInfo() const;
  const TypeInfo &getElementTypeInfo( const RValue &index = defaultIndex() ) const;
  const ScalarTypeInfo &getIndexTypeInfo( const RValue &index = defaultIndex() ) const;

  const RValue &operator[]( int index ) const;
  RValue &operator[]( int index );
  const RValue &operator[]( const RValue &index ) const;
  RValue &operator[]( const RValue &index );

  RValue &getField( int index );
  const RValue &getField( int index ) const;
  const RValue &getField( const RValue &index ) const;
  RValue &getField( const RValue &index );

  void clearElements();
  void voidElements();
  /**
     Append an element to this array.
  */
  void append( const RValue &newElement );
  /**
     Aggregate uses this method since it typically is handling pointers
     that it is going to give us to manage.
  */
  void append( RValue *newElement );


  Value vhdlUnaryPlus( ) const;
  Value vhdlUnaryMinus( ) const;
  
  Value vhdlPlus( const RValue & ) const;
  Value vhdlMinus( const RValue & ) const;
  Value vhdlMultiply( const RValue & ) const;
  Value vhdlDivide( const RValue & ) const;
  Value vhdlMod( const RValue & ) const;
  Value vhdlRem( const RValue & ) const;
  Value vhdlPow( const RValue & ) const;
  Value vhdlAnd( const RValue &rhs) const; 
  Value vhdlOr( const RValue &rhs) const; 
  Value vhdlNand( const RValue &rhs) const; 
  Value vhdlNor( const RValue &rhs) const;
  Value vhdlXor( const RValue &rhs) const; 
  Value vhdlXnor( const RValue &rhs) const; 
  Value vhdlNot() const; 
  Value vhdlSll( const RValue &rhs ) const;
  Value vhdlSrl( const RValue &rhs ) const;
  Value vhdlSla( const RValue &rhs ) const;
  Value vhdlSra( const RValue &rhs ) const;
  Value vhdlRol( const RValue &rhs ) const;
  Value vhdlRor( const RValue &rhs ) const;
  Value vhdlConcatenate( const RValue &rhs ) const;

  const RValue &readVal() const;
  RValue &readVal();

  bool isPhysicalType() const { return false; }
  const string &getUnit() const;
  LONG getValue() const;
  int getUnits() const;
  const EnumerationLiteral &vhdlEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlNotEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlGreaterThan( const RValue & ) const;
  const EnumerationLiteral &vhdlGreaterThanEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlLessThan( const RValue & ) const;
  const EnumerationLiteral &vhdlLessThanEqual( const RValue & ) const;

  RValue &all();
  const RValue &all() const;
  void updateVal( const RValue & );

  SignalNetinfo &addToFanOut( VHDLKernel * );
  //DRH
  SignalNetinfo &addChild( SignalNetinfo &, VHDLKernel * );
  
  //@{
  /** Attributes */
  Value left( const RValue &param = defaultIndex() ) const;
  Value right( const RValue &param = defaultIndex() ) const;
  Value high( const RValue &param = defaultIndex() ) const;
  Value low( const RValue &param = defaultIndex() ) const;
  const ScalarTypeInfo &range( const RValue &index = defaultIndex() ) const;
  const ScalarTypeInfo &reverse_range( const RValue &index = defaultIndex() ) const;
  Value length( const RValue &index = defaultIndex() ) const;
  Value ascending( const RValue &index = defaultIndex() ) const;
  //@}

protected:
  /**
     Performs a logical shift on this array.  Positive amounts shift left,
     negative amounts shift right.  Assumes the CompositeLiteral is one dimensional.
  */
  void shiftLogical( int shiftAmount );

  static const UniversalInteger &defaultIndex();

  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  /**
     We've been asked for our index type info and we have typeInfo.  We
     need to figure out our index type based on both the typeinfo and our
     size.
  */
  const ScalarTypeInfo &divineIndexTypeInfo() const;

  /**
     Returns the array index for the element index and the dimension passed
     in.
  */
  int storageIndex( int elementIndex ) const;

  void cloneElements( const vector<RValue *> &thatElements );

  static void buildElements( const CompositeTypeInfo &arrayTypeInfo,
			     vector<RValue *> &elements,
			     const SubElementFactory *subElementFactory,
			     int numElements = -1 );

  /**
     If this CompositeLiteral owns its elements then it will destroy them.
  */
  void destroyElements();

  /**
     Used by string literals to build up a proper representation of a VHDL
     string literal.
  */
  void insertElements( const string &initElements );

  /**
     Used for non-string Aggregates.
  */
  void insertElements( const RValue &initElements );

  /**
     @param localElements My local vector of elements.  Will be filled with
     pointers back to this slice's "parent" elements.
     @param targetElements The elements of the object that we are slicing
     from.
     @param sliceSpecifier The specification for which elements we are
     slicing.
  */
  void sliceElements( CompositeLiteral &that );

  static CompositeTypeInfo *buildSliceTypeInfo( const CompositeLiteral &that,
						const ScalarTypeInfo &sliceSpecifier );

  const CompositeTypeInfo *myTypeInfo;
  /**
     Our index type info can be decoupled from myTypeInfo.  When we
     calculate it we store it here.  Generally it should be recalculated
     every time as we don't maintain it.
   */
  mutable const ScalarTypeInfo *indexTypeInfo;

  /// The list of objects contained by this composite type
  vector<RValue *> myElements;
  vector<RValue *> myCleanUps;
  bool deleteMembers;
  const SubElementFactory *myFactory;

  /**
     Performs a dyadic operation on the array, with the array passed in as
     the argument.
  */
  Value bitOperation( const RValue &rhs,
		      Value (EnumerationLiteral::*operation)(const RValue &) const
		      ) const;
};

#endif

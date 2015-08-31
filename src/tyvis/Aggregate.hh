#ifndef AGGREGATE_HH
#define AGGREGATE_HH

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
#include "tyvis/CompositeLiteral.hh"

class ArrayTypeInfo;
class SubElementFactory;
class TyvisVarArgs;
class SerializedInstance;
    
class Aggregate : public CompositeLiteral {
public:
  /**@name Public Class Methods of Aggregate. */
  //@{

  /** This is the default constructor.  However, this constructor is rather evil
      because it let's the user build Aggregate object's that may not have
      sufficient information (such as TypeInfo data) for their working.  Right
      now, it is not very clear if this constructor is essential.  It should be
      removed if it is not necessary.
  */
  Aggregate(); 

  /** This is the basic constructor for the array type.  This constructor should
      be used by all applications to build default Aggregate objects.

      @param objType  The type of the object (variable/signal etc.).
      @param typeInfo The AggregateInfo structure associated with this object.
      @param resolveFnId Resolution function id, if any (-1 for invalid).
  */
  Aggregate( const ArrayTypeInfo &typeInfo );

  //@{
  /**
     Constructor taking TyvisVarArgs...
  */
  Aggregate( const ArrayTypeInfo &typeInfo,
	     TyvisVarArgs &initElements );

  Aggregate( TyvisVarArgs &initElements );

  /**
     Constructor used by file operations.
  */
  Aggregate( const ArrayTypeInfo &typeInfo,
	     const string &initValue );

  /**
     Constructor used by function calls.
  */
  Aggregate( const ArrayTypeInfo &typeInfo,
	     const RValue &initValue );

  //@}

  /**
     This constructor is for initializing with an associated Object.  This
     is necessary for composite types, for example, because a Variable
     needs to have Variable subelements, and a Signal needs to have Signal
     subelments.  This is the constructor that gets called in that case.     
  */
  Aggregate( const SubElementFactory &subElementFactory );

  /** Copy constructor  */
  Aggregate( const Aggregate &that );

  /**
     Constructor used when constructing slices.
  */
  Aggregate( Aggregate &that,
	     const ScalarTypeInfo &sliceSpecifier );
  
  /// The destructor
  virtual ~Aggregate();

  /** This method is used to print a Aggregate object.  This method should not
      be used to dump Aggregate objects to achieve texio support.  This method
      is just a utility method.  More for debugging and maybe for tracing
      signals.  In any case, the use of textio methods is encouraged.

      @param os The output stream to which the data must be dumped.
  */
  virtual void print(ostream& os) const;


  /** The string representation for the elements for the given Aggregate.  This
      method is valid only for one-dimensional Aggregates whose elements are of
      EnumerationLiteral.

      @return The "c" string representation for the elements of the Aggregate object
  */
  string toString() const;

  /** Returns a clone of the object.  All the fields, dimensions, and elements
      are duplicated.  The return value is a fresh/new duplicate and the caller
      must ensure that the return value is deleted (otherwise, memory will leak)

      @return A pointer to the clone of the object.
  */
  virtual RValue *clone() const;

  void serialize( SerializedInstance *addTo ) const;
  static const string &getAggregateDataType();
  const string &getDataType() const;

private:

  static TyvisVarArgs stringToVarArgs( const ArrayTypeInfo &typeInfo, const string &initValue );

  /**
     Prints one dimension - used by "print".
  */
  void printDimension( ostream &os, int dimension ) const;
};

#endif

#ifndef SLICE_HH
#define SLICE_HH

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
#include "tyvis/CompositeLiteral.hh"
#include "tyvis/ArrayTypeInfo.hh"

#include <vector>
using std::vector;
class ScalarType;
class SignalBase;
class AccessObject;
class UniversalInteger;
class PhysicalLiteral;
class ObjectBase;
class SubElementFactory;

/** The class Slice

    This class represents a one-dimensional array, or a slice.
*/    
    
class Slice : public CompositeLiteral {
public:
  /**
     Operator= for slices.
  */
  Slice &operator=( const Slice &that );

  /// The destructor
  virtual ~Slice() = 0;

  bool isUnconstrained();

protected:
  /**@name Public Class Methods of Slice. */
  //@{

  Slice(); 

  /** This is the basic constructor for the array type.  This constructor should
      be used by all applications to build default Slice objects.

      @param objType  The type of the object (variable/signal etc.).
      @param typeInfo The SliceInfo structure associated with this object.
      @param resolveFnId Resolution function id, if any (-1 for invalid).
  */
  Slice( const ArrayTypeInfo &typeInfo );

  Slice( const ArrayTypeInfo &typeInfo,
	 const string &initValue );

  Slice( const SubElementFactory &subElementFactory );

  /** A constructor for creating temporary objects.  This constructor is
      typically used by the frontend code-generator to build  temporary array
      slices.
 
      @param objType The type objects contained by this object (signal/variable)
      @param source The source Slice object.
  */
  Slice( const Slice &that );

  /**
     Constructor used when constructing slices.
  */
  Slice( Slice &that, const ScalarTypeInfo &sliceSpecifier );

private:

};
#endif


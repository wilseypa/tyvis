#ifndef PHYSICAL_TYPE_INFO_HH
#define PHYSICAL_TYPE_INFO_HH

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
#include "tyvis/ScalarTypeInfo.hh"
#include "tyvis/UnitDeclaration.hh"
#include "tyvis/Types.hh"

#include <string>
using std::string;

class IntegerTypeInfo;

class PhysicalTypeInfo : public ScalarTypeInfo {
public:
  PhysicalTypeInfo();
  PhysicalTypeInfo( const PhysicalTypeInfo &pi );

  PhysicalTypeInfo( const IntegerTypeInfo &range,
		    const vector<UnitDeclaration> &units );

  PhysicalTypeInfo( const IntegerTypeInfo &range );

  PhysicalTypeInfo( const vector<UnitDeclaration> &units );

  ~PhysicalTypeInfo(){}
  
  TypeInfo *clone() const;

  PhysicalTypeInfo& operator=(const PhysicalTypeInfo &);

  bool isPhysicalType() const { return true; }

  const vector<UnitDeclaration> &getUnits() const;

  const string &getPrimaryUnit() const;

  void addUnits( const vector<UnitDeclaration> &unitList );

  RValue *create( const SubElementFactory *subElementFactory ) const;

  static const PhysicalLiteral getMax( const PhysicalTypeInfo &typeInfo );
  static const PhysicalLiteral getMin( const PhysicalTypeInfo &typeInfo );
  static const PhysicalLiteral &getZeroTime();
  static const PhysicalLiteral &getInfinityTime();

  LONG getBaseMultiplier( const string &unitName ) const;

  Value value( const string &initValue ) const;
  Value value( FileObject &file ) const;

  Value vhdlPlus    ( const RValue &, const RValue & ) const;
  Value vhdlMinus   ( const RValue &, const RValue & ) const;
  Value vhdlUnaryPlus  ( const RValue & ) const;
  Value vhdlUnaryMinus  ( const RValue & ) const;
  Value vhdlMultiply( const RValue &, const RValue & ) const;
  Value vhdlDivide  ( const RValue &, const RValue & ) const;
  Value vhdlMod     ( const RValue &, const RValue & ) const;
  Value vhdlRem     ( const RValue &, const RValue & ) const;
  Value vhdlPow     ( const RValue &, const RValue & ) const;

  /**
     These are the implementations of type attributes that are specific to
     PhysicalTypeInfo.  (Most of the PhysicalTypeInfo type attributes
     are implemented in ScalarTypeInfo.)
  */
  Value val( const RValue &expression ) const;
  Value succ( const RValue &expression ) const;
  Value pred( const RValue &expression ) const;
  //@}
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  vector<UnitDeclaration> myUnits;
};

#endif

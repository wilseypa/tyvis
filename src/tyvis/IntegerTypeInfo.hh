#ifndef INTEGER_TYPE_INFO_HH
#define INTEGER_TYPE_INFO_HH

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

class IntegerLiteral;
class SubElementFactory;

class IntegerTypeInfo : public ScalarTypeInfo {
public:
  IntegerTypeInfo();
  IntegerTypeInfo(int, ArrayInfo::ArrayDirn_t, int);
  IntegerTypeInfo(const IntegerTypeInfo &ri);
  IntegerTypeInfo( const RValue &left, 
		   ArrayInfo::ArrayDirn_t dirn, 
		   const RValue &right );

  ~IntegerTypeInfo(){}

  /**
     Clone this TypeInfo.
  */
  TypeInfo *clone() const;

  RValue *create( const SubElementFactory *subElementFactory ) const;

  IntegerTypeInfo &operator=(const IntegerTypeInfo &ri);

  static const IntegerLiteral &getMax();
  static const IntegerLiteral &getMin();

  Value vhdlPlus( const RValue &, const RValue & ) const;
  Value vhdlMinus   ( const RValue &, const RValue & ) const;
  Value vhdlMultiply( const RValue &, const RValue & ) const;
  Value vhdlDivide  ( const RValue &, const RValue & ) const;
  Value vhdlMod     ( const RValue &, const RValue & ) const;
  Value vhdlRem     ( const RValue &, const RValue & ) const;
  Value vhdlPow     ( const RValue &, const RValue & ) const;

  //@{
  /**
     These are the implementations of type attributes that are specific to
     IntegerTypeInfo.  (Most of the IntegerTypeInfo type attributes are
     implemented in ScalarTypeInfo.)
  */
  Value val( const RValue &expression ) const;
  Value succ( const RValue &expression ) const;
  Value pred( const RValue &expression ) const;
  //@}

  using TypeInfo::value;
  Value value( const string &initValue ) const;
  virtual void debugPrintDetails( ostream &os, int indent ) const;

protected:
  
private:
};

#endif

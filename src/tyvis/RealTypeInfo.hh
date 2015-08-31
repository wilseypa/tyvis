#ifndef REAL_TYPE_INFO_HH
#define REAL_TYPE_INFO_HH

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
#include "tyvis/UniversalReal.hh"

class RealTypeInfo : public ScalarTypeInfo {
public:
  RealTypeInfo();
  RealTypeInfo(const RealTypeInfo &ri);
  RealTypeInfo(double, ArrayInfo::ArrayDirn_t, double);
  RealTypeInfo( const RValue &left, ArrayInfo::ArrayDirn_t, const RValue &right );
  ~RealTypeInfo() {}

  TypeInfo *clone() const;
  
  RealTypeInfo& operator=(const RealTypeInfo &ri);
  TypeInfo& operator=(const TypeInfo& ti) {
    return operator=((const RealTypeInfo &) ti);
  }
  Value vhdlPlus( const RValue &, const RValue & ) const;
  Value vhdlMinus   ( const RValue &, const RValue & ) const;
  Value vhdlMultiply( const RValue &, const RValue & ) const;
  Value vhdlDivide  ( const RValue &, const RValue & ) const;
  Value vhdlMod     ( const RValue &, const RValue & ) const;
  Value vhdlRem     ( const RValue &, const RValue & ) const;
  Value vhdlPow     ( const RValue &, const RValue & ) const;

  RValue *create( const SubElementFactory *subElementFactory ) const;

  using TypeInfo::value;
  Value value( const string &initValue ) const;
  virtual void debugPrintDetails( ostream &os, int indent ) const;

protected:

private:
};

#endif

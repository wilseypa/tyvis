#ifndef INTEGER_LITERAL_HH
#define INTEGER_LITERAL_HH

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
#include "tyvis/VHDLData.hh"
#include <warped/SerializedInstance.h>
#include <warped/DeserializerManager.h>
#include <iostream>
using std::cout;

class UniversalReal;
class AccessObject;
class ScalarTypeInfo;

class IntegerLiteral : public VHDLData {
public:

  IntegerLiteral();
  IntegerLiteral(const ScalarTypeInfo &initTypeInfo);

  IntegerLiteral(int i) : val( i ){}
  IntegerLiteral(LONG i) : val( i ){}
  IntegerLiteral(char i) : val( i ){}
  IntegerLiteral(bool i) : val( i ){}
  IntegerLiteral(double i) : val( (int)i ){}

  IntegerLiteral(const RValue &i) : VHDLData(),
				    val( i.getIntValue() ){}

  bool isScalarType() const { return true; }

  operator int() const { return val; }

  bool operator==( const RValue &compareTo ) const;
  bool operator!=( const RValue &compareTo ) const;
  bool operator > ( const RValue &compareTo ) const;
  bool operator >= ( const RValue &compareTo ) const;
  bool operator < ( const RValue &compareTo ) const;
  bool operator <= ( const RValue &compareTo ) const;
  
  int getIntValue() const { return val; }
  LONG getInt64Value() const { return (LONG)getIntValue(); }
  double getDoubleValue() const { return (double)getIntValue(); }

protected:
  ~IntegerLiteral(){}
  virtual void debugPrintDetails( ostream &os, int indent ) const;

  int val;

private:
};

#endif

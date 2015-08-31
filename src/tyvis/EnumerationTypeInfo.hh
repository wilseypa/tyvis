#ifndef ENUMERATION_TYPE_INFO_HH
#define ENUMERATION_TYPE_INFO_HH

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
#include <vector>
#include <string>
using std::vector;
using std::string;

class SubElementFactory;
class IntegerTypeInfo;

class EnumerationTypeInfo : public ScalarTypeInfo {
public:
  EnumerationTypeInfo();
  EnumerationTypeInfo( const vector<string> &imageMap );
  EnumerationTypeInfo( const IntegerTypeInfo &range,
		       const vector<string> &imageMap );
  ~EnumerationTypeInfo();
  EnumerationTypeInfo( const EnumerationTypeInfo &that );
  const vector<string> &getImageMap() const;

  bool operator==( const TypeInfo &that ) const;
  TypeInfo *clone() const;

  /**
     Get the index of the literal passed in.  Returns -1 if the item is not
     a literal of this type.
  */
  int getIndex( const string &value ) const;

  /**
     Get the string value of a literal, given the integer value.
  */
  string getLiteral( int value ) const;

  EnumerationTypeInfo& operator=(const EnumerationTypeInfo &ei);
  TypeInfo &operator=(const TypeInfo &ti) {
    return operator=( static_cast<const EnumerationTypeInfo &>(ti) );
  }

//   int getCharacterOffset() const;
  
  RValue *create( const SubElementFactory *parent ) const;
  
  bool isCharacterType() const;

  Value vhdlPlus( const RValue &, const RValue & ) const;
  Value vhdlMinus   ( const RValue &, const RValue & ) const;
  Value vhdlMultiply( const RValue &, const RValue & ) const;
  Value vhdlDivide  ( const RValue &, const RValue & ) const;
  Value vhdlMod     ( const RValue &, const RValue & ) const;
  Value vhdlRem     ( const RValue &, const RValue & ) const;
  Value vhdlPow     ( const RValue &, const RValue & ) const;
  Value vhdlNot     ( const RValue & ) const;
  Value vhdlXor     ( const RValue &, const RValue & ) const;
  Value vhdlXnor    ( const RValue &, const RValue & ) const;

  /**
     These are the implementations of type attributes that are specific to
     EnumerationTypeInfo.  (Most of the EnumerationTypeInfo type attributes
     are implemented in ScalarTypeInfo.)
  */
  Value val( const RValue &expression ) const;
  Value succ( const RValue &expression ) const;
  Value pred( const RValue &expression ) const;
  //@}

  using TypeInfo::value;
  virtual Value value( const string &initValue ) const;
  virtual void debugPrintDetails( ostream &os, int indent ) const;
  const vector<string> *getImage() const {return imageMap;}

protected:
  
private:
  const vector<string> *imageMap;

  static int calcRight( int numElements );
};

#endif

#ifndef ACCESSVARIABLE_HH
#define ACCESSVARIABLE_HH

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
#include "tyvis/vhdl.hh"
#include "tyvis/ObjectBase.hh"
#include "tyvis/ArrayInfo.hh"
#include "tyvis/Types.hh"
#include <string.h>
#include <iostream>

using std::cout;

class AccessVariable : public ObjectBase {
public:
  AccessVariable( const TypeInfo &initTypeInfo );

  AccessVariable( const TypeInfo &initTypeInfo,
		  const string &initName );

  AccessVariable( const AccessVariable & );

  static const string &getAccessVariableType(){
    static const string accessVariableType = "AccessVariable";
    return accessVariableType;
  }

  const string &getDataType() const {
    return getAccessVariableType();
  }

  void serialize( SerializedInstance * ) const {
    abort();
  }

  AccessVariable &operator=(const AccessVariable & );
  AccessVariable &operator=( const string &newVal );

  void print( ostream &os ) const;
  int size() const { return val.size(); }
  void advance( int amount ){ curPos += amount; }

  int getIntValue() const { abort(); return 0; }
  LONG getInt64Value() const { abort(); return 0; }
  double getDoubleValue() const { abort(); return 0; }

  void setVal( const string &newVal ){
    curPos = 0;
    val = newVal;
  }
  
  const string &getVal(){
    return val;
  }

  bool operator==( const RValue &compareTo ) const {
    return ( dynamic_cast<const AccessVariable &>( compareTo ).curPos == curPos &&
	     dynamic_cast<const AccessVariable &>( compareTo ).val == val );
  }

  bool operator!=( const RValue &compareTo ) const {
    return !( dynamic_cast<const AccessVariable &>( compareTo ).curPos == curPos &&
	      dynamic_cast<const AccessVariable &>( compareTo ).val == val );
  }

  bool operator>( const RValue &compareTo ) const{
    return val > dynamic_cast<const AccessVariable &>( compareTo ).val;
  }

  bool operator>=( const RValue &compareTo ) const {
    return val >= dynamic_cast<const AccessVariable &>( compareTo ).val;
  }

  bool operator<( const RValue &compareTo ) const {
    return val < dynamic_cast<const AccessVariable &>( compareTo ).val;
  }

  bool operator<=( const RValue &compareTo ) const {
    return val <= dynamic_cast<const AccessVariable &>( compareTo ).val;
  }

  inline void printstr(ostream& os) {
    os << val;
  }
  
  /**
     Eats whitespace starting at currentPos.  Returns true if we're still
     inside the string at the end, false otherwhise.
  */
  bool eatwhite(){
    for( int i = curPos; i < size(); i++ ){
      while( val[i] == ' ' || val[i] == '\t' ){
	curPos++;
      }
    }
    return curPos < size();
  }

  void append( const string &toAppend ){
    val += toAppend;
  }

  void reset();

  const RValue &readVal() const;
  void updateVal( const RValue &newValue );
  RValue &operator=(const RValue &);
  RValue *clone() const;

protected:

  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:

  int curPos; // pointer into val indicating, e.g., the current read position
  string val;
};

#endif

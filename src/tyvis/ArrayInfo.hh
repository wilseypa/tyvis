#ifndef ARRAY_INFO_HH
#define ARRAY_INFO_HH

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
#include <Serializable.h>
#include <SerializedInstance.h>
#include <DeserializerManager.h>
#include <iostream>
using std::ostream;
using std::cerr;
using std::endl;

class RValue;

class ArrayInfo : public Serializable {
  friend ostream& operator<<(ostream&, const ArrayInfo&);
public:
  enum ArrayDirn_t {to = 1, downto = -1};
  
  ArrayInfo();
  ArrayInfo(int, ArrayInfo::ArrayDirn_t, int);
  ArrayInfo(const RValue &left, ArrayInfo::ArrayDirn_t, const RValue &right);
  ArrayInfo(const ArrayInfo&);
  int length() const;
  bool operator==(const ArrayInfo&) const;
  bool operator!=(const ArrayInfo&) const;
  bool is_null_range() const;
  bool isAscending() const { return (dirn() == to); }

  ArrayInfo &operator=(const ArrayInfo&);
  bool contains(const int) const;

  int storageIndex(const int) const;
  
  /** The following function returns the actual index specified from the
      vhdl code from the positional index.  The positional index starts
      from 0 onwards.  That is, 0 is converted to left() and so on. */
  int actualIndex(int) const;

  int left() const { return lbound; }
  int right() const { return rbound; }
  ArrayInfo::ArrayDirn_t dirn() const { return direction; }
  
  int rightof(int i) const {
    if (i == rbound) {
      cerr << "ArrayInfo::rightof: can't take rightof right bound!\n("
	   << lbound << " " << (int) direction << " " << rbound << ")" << endl;
      abort();
    }
    return (i + (int) direction);
  }

  static const string &getArrayInfoType(){
    static const string arrayInfoType = "ArrayInfo";
    return arrayInfoType;
  }

  const string &getDataType() const {
    return getArrayInfoType();
  }
  
  void serialize( SerializedInstance *serializeInto ) const {
    serializeInto->addInt( lbound );
    serializeInto->addInt( rbound );
    serializeInto->addInt( direction );
  }
  
  static Serializable *deserialize( SerializedInstance *si ){
    int lBound = si->getInt();
    int rBound = si->getInt();
    ArrayDirn_t dDirection = ArrayDirn_t( si->getInt() );
    return new ArrayInfo( lBound, dDirection, rBound );
  }

  static void registerDeserializer(){
    DeserializerManager::instance()->registerDeserializer( getArrayInfoType(),
							   &ArrayInfo::deserialize );
  }

protected:
  
private:
  int lbound;
  int rbound;
  ArrayDirn_t direction;
};

extern ostream& operator<<(ostream& os, const ArrayInfo::ArrayDirn_t &);

const ArrayInfo &defaultInfo();
const ArrayInfo &others();
const ArrayInfo &nullInfo();

#endif

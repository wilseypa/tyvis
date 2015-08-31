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

#include "ArrayInfo.hh"
#include "RValue.hh"

const ArrayInfo &defaultInfo(){
  static ArrayInfo _defaultInfo(0, ArrayInfo::to, -1);
  return _defaultInfo;
}

const ArrayInfo &others(){
  static ArrayInfo _others(1, ArrayInfo::to, 0);
  return _others;
}

const ArrayInfo &nullInfo(){
  static ArrayInfo _nullInfo(0, ArrayInfo::to, -1);
  return _nullInfo;
}

ArrayInfo::ArrayInfo() : lbound(0), rbound(0), direction(ArrayInfo::to) {}


ArrayInfo::ArrayInfo(int l, ArrayInfo::ArrayDirn_t d, int r) : lbound(l), rbound(r), direction(d){}

ArrayInfo::ArrayInfo(const RValue &l, ArrayInfo::ArrayDirn_t d, const RValue &r) 
  : lbound(l.getIntValue()), rbound(r.getIntValue()), direction(d){}

ArrayInfo::ArrayInfo(const ArrayInfo& ai):
  Serializable(),
  lbound(ai.lbound), 
  rbound(ai.rbound),
  direction(ai.direction){}

// Returns the correct size, or 0 if null range.
int 
ArrayInfo::length() const {
  int retval = 0;
  if(direction == ArrayInfo::to) {
    if( rbound - lbound >=0 ){
      retval = rbound-lbound + 1;
    }
  } 
  else {
    if( lbound - rbound >= 0 ){
      retval = lbound - rbound + 1;
    }
  }    
  return retval;
}


bool
ArrayInfo::operator==( const ArrayInfo &a ) const {
  return (rbound == a.rbound && lbound == a.lbound && direction == a.direction);
}

bool
ArrayInfo::is_null_range() const{
  return ((direction == ArrayInfo::to) ? (lbound > rbound): (lbound < rbound));
}

bool
ArrayInfo::operator!=(const ArrayInfo& a) const {
  return !operator==(a);
}


ArrayInfo&
ArrayInfo::operator=(const ArrayInfo& a) {
  lbound    = a.lbound;
  rbound    = a.rbound;
  direction = a.direction;
  
  return *this;
}


bool
ArrayInfo::contains(const int index) const {
  if (direction == ArrayInfo::to && index >= lbound && index <= rbound) {
    return true;
  }
  
  if ((direction == ArrayInfo::downto) &&
      (index <= lbound && index >= rbound)) {
    return true;
  }
  
  if ((direction != ArrayInfo::to) &&
      (direction != ArrayInfo::downto)) {
    cerr << "Invalid direction! " << *this << endl;
    abort();
  }
  
  return false;
}

int
ArrayInfo::storageIndex( const int arrayIndex ) const {
  ASSERT( contains(arrayIndex) );

  int retval = -1;
  if( direction == to ) {
    retval = arrayIndex - lbound;
  } 
  else {
    retval = lbound - arrayIndex;
  }
  
  return retval;
}

int
ArrayInfo::actualIndex(int positionalIndex) const {
  int retval;
  
  if (direction == ArrayInfo::to) {
    retval = lbound + positionalIndex;
  } else {
    retval = lbound - positionalIndex;
  }
  
  return retval;
}

ostream&
operator<<(ostream& os, const ArrayInfo::ArrayDirn_t& ad) {
  switch (ad) {
  case ArrayInfo::to:
    os << "to";
    break;
  case ArrayInfo::downto:
    os << "downto";
    break;
  default:
    os << "(invalid ArrayDirn)";
    break;
  }
  return os;
}


ostream&
operator<<(ostream&os, const ArrayInfo& ai) {
  os << "(" << ai.lbound << " " << ai.direction << " " << ai.rbound << ")";
  return os;
}

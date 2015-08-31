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

#include "IntegerTypeInfo.hh"
#include "UniversalInteger.hh"
#include "AccessObject.hh"
#include "Alias.hh"
#include "Value.hh"
#include <iostream>
#include <typeinfo>

using std::cerr;
using std::endl;

static void undefined( const char *method, const RValue &it ){
  cerr << method << " is undefined for " << it.getKindText() << endl;
  abort();
}

#define ALLOC_CHUNK 200
static vector<vector<void *> > *free_blocks;
static vector<unsigned int> *free_len;
static unsigned int max_size = 0;
// static unsigned long total = 0;

void * RValue::operator new(size_t nBytes_in) 
{
  unsigned size;
  void * Ptr;
  unsigned int nBytes = nBytes_in+7;
  nBytes >>= 3;
  nBytes <<= 3;
  if (max_size < nBytes) {
    if (max_size == 0) {
      free_len = new vector<unsigned int>;
      free_blocks = new vector<vector<void *> >;
    }
    free_blocks->resize(nBytes+1);
    free_len->resize(nBytes+1,0);
    max_size = nBytes;
  }
  size = (*free_len)[nBytes];
  if (size > 0) {
    Ptr = (*free_blocks)[nBytes][size-1];
    (*free_len)[nBytes] = size-1;
  }
  else {
    ASSERT (size == 0);
    Ptr = static_cast<void *>(new char[nBytes*ALLOC_CHUNK]);
    if ((*free_blocks)[nBytes].size() < ALLOC_CHUNK)
      (*free_blocks)[nBytes].resize(ALLOC_CHUNK);
    for ( int i=1 ; i<ALLOC_CHUNK ; ++i ){
      (*free_blocks)[nBytes][size++] = static_cast<void *>(static_cast<char *>(Ptr) + nBytes*i);
    }
//    total += ALLOC_CHUNK*nBytes;
    (*free_len)[nBytes] = size;
//    cout << "Total allocation for RValues = " << total << endl;
  }
//  void *Ptr = static_cast<void *>(new char[nBytes_in]);

  return Ptr;
} 

void RValue::operator delete(void *p, size_t nBytes_in) 
{  
  unsigned int nBytes = nBytes_in+7;
  nBytes >>= 3;
  nBytes <<= 3;
  if ((*free_blocks)[nBytes].size() <= (*free_len)[nBytes])
    (*free_blocks)[nBytes].resize((*free_len)[nBytes]+ALLOC_CHUNK);
  (*free_blocks)[nBytes][(*free_len)[nBytes]++] = p;
//  delete p;

  return;
}

RValue::~RValue(){}

void
RValue::debugPrint(ostream &os, int indent) const {
  os << NL(indent);
  os << "RValue";
  if (getName() != "")
    os << " \"" << getName() << "\" with";
  os << " TypeID: " << typeid(*this).name();
  os << NL(indent) << "Value: '" << toString() << "'";
  if (typeid(*this) != typeid(AccessObject)){
    os << NL(indent) << "Attributes:";
    if (isCompositeType())
      os << " Composite";
    if (isScalarType())
      os << " Scalar";
    if (isPhysicalType())
      os << " Physical";
    if (is_signal())
      os << " Signal";
  }
  debugPrintDetails(os, indent);
  os << NL(0);
}

const string &
RValue::getName() const {
  static string nullName = "";
  return nullName;
}

RValue &
RValue::operator=( const RValue & ){
  undefined( "RValue::operator=", *this );
  return *this;
}

const string 
RValue::getKindText() const {
  return typeid(*this).name();
}

Value
RValue::left( const RValue & ) const {
  undefined( "RValue::left", *this );
  return *this;
}

Value
RValue::right( const RValue & ) const {
  undefined( "RValue::right", *this );
  return *this;
}

Value
RValue::high( const RValue & ) const {
  undefined( "RValue::high", *this );
  return *this;
}

Value
RValue::low( const RValue & ) const {
  undefined( "RValue::low", *this );
  return *this;
}

const ScalarTypeInfo &
RValue::range( const RValue & ) const {
  undefined( "RValue::range", *this );
  return *new IntegerTypeInfo( -1, ArrayInfo::to, 0 );
}

const ScalarTypeInfo &
RValue::reverse_range( const RValue & ) const {
  undefined( "RValue::reverse_range", *this );
  return *new IntegerTypeInfo( -1, ArrayInfo::to, 0 );
}

Value
RValue::length( const RValue & ) const {
  undefined( "RValue::length", *this );
  return *this;
}

Value
RValue::ascending( const RValue & ) const {
  undefined( "RValue::ascending", *this );
  return *this;
}

Value
RValue::vhdlNot() const {
  undefined( "RValue::ascending", *this );
  return *this;
}

Value
RValue::vhdlAnd( const RValue & ) const {
  undefined( "RValue::vhdlAnd", *this );
  return *this;
}

Value
RValue::vhdlOr( const RValue & ) const {
  undefined( "RValue::vhdlOr", *this );
  return *this;
}

Value
RValue::vhdlNand( const RValue & ) const {
  undefined( "RValue::vhdlNand", *this );
  return *this;
}

Value
RValue::vhdlNor( const RValue & ) const {
  undefined( "RValue::vhdlNor", *this );
  return *this;
}

Value
RValue::vhdlXor( const RValue & ) const {
  undefined( "RValue::vhdlXor", *this );
  return *this;
}

Value
RValue::vhdlXnor( const RValue & ) const {
  undefined( "RValue::vhdlXnor", *this );
  return *this;
}

Value
RValue::vhdlSll( const RValue & ) const {
  undefined( "RValue::vhdlSll", *this );
  return *this;
}

Value
RValue::vhdlSrl( const RValue & ) const {
  undefined( "RValue::vhdlSrl", *this );
  return *this;
}

Value
RValue::vhdlSla( const RValue & ) const {
  undefined( "RValue::vhdlSla", *this );
  return *this;
}

Value
RValue::vhdlSra( const RValue & ) const {
  undefined( "RValue::vhdlSra", *this );
  return *this;
}

Value
RValue::vhdlRol( const RValue & ) const {
  undefined( "RValue::vhdlRol", *this );
  return *this;
}

Value
RValue::vhdlRor( const RValue & ) const {
  undefined( "RValue::vhdlRor", *this );
  return *this;
}

const string &
RValue::NL(const int indent) const {
  static string nextLine;
  int i;
  nextLine = "\n";
  for (i=0 ; i<indent ; ++i){
    nextLine += " ";
  }
  return nextLine;
}

// In UniversalInteger.cc to prevent circular dependency hell.
// const RValue &defaultIndex();

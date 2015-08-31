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

#include "TypeInfo.hh"
#include "std_standardPkg.hh"
#include "UniversalReal.hh"
#include "FileObject.hh"
#include "EnumerationLiteral.hh"
#include <string>
#include <typeinfo>

using std::string;

#define ALLOC_CHUNK 200
static vector<vector<void *> > *free_blocks;
static vector<unsigned int> *free_len;
static unsigned int max_size = 0;
// static unsigned long total = 0;

void * TypeInfo::operator new(size_t nBytes_in) 
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
//    cout << "Total allocation for TypeInfos = " << total << endl;
  }
//  void *Ptr = static_cast<void *>(new char[nBytes_in]);

  return Ptr;
} 

void TypeInfo::operator delete(void *p, size_t nBytes_in) 
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

TypeInfo &
TypeInfo::getNullTypeInfo(){
  static TypeInfo NULL_TYPE_INFO;
  return NULL_TYPE_INFO;
}

ArrayInfo::ArrayDirn_t
TypeInfo::getDirection() const {
  unimplementedFn( "TypeInfo::getDirection()" );
  return ArrayInfo::to;
}

TypeInfo *
TypeInfo::clone() const {
  unimplementedFn( "TypeInfo::clone() const" );
  return 0;
}

bool
TypeInfo::operator==( const TypeInfo & ) const{
  unimplementedFn( "TypeInfo::operator==( const TypeInfo &that ) const" );
  return false;
}



RValue *
TypeInfo::create( const SubElementFactory * ) const {
  unimplementedFn("TypeInfo::create( const SubElementFactory * ) const");
  return NULL;
}

const vector<string> &
TypeInfo::getImageMap() const{
  unimplementedFn("TypeInfo::TypeInfo::getImageMap() const");
  static vector<string> retval;
  return retval;
}


vector<string>
TypeInfo::createImageMap( const char *elements[] ){
  vector<string> retval;
  if( elements != 0 ){
    int i = 0;
    const char *current = elements[i];
    do {
      retval.push_back( current );
      current = elements[++i];
    } while( current != 0 );
  }
  return retval;
}

string 
TypeInfo::getKindText() const {
  return typeid(*this).name();
}

void
TypeInfo::debugPrint(ostream &os, int indent) const {
  os << NL(indent);
  os << "TypeInfo TypeID: " << getKindText() << "  at address: " << this;
  debugPrintDetails(os, indent);
  return;
}

const TypeInfo &
TypeInfo::getFieldInfo( int ) const {
  unimplementedFn("TypeInfo::getFieldInfo( int fieldIndex ) const");
  return *this;
}

string
TypeInfo::getSubFieldName( const string &, int  ) const {
  unimplementedFn("TypeInfo::getSubFieldName()");
  return "";
}

bool 
TypeInfo::isCharacterType() const {
  // Overidden in enumeration type
  return false;
}

Value
TypeInfo::vhdlPlus( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlPlus( const RValue &, const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlMinus( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlMinus( const RValue &, const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlMultiply( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlMultiply( const RValue &, const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlDivide( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlDivide( const RValue &, const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlMod( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlMod( const RValue &, const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlRem( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlRem( const RValue &, const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlPow( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlPow( const RValue &, const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlNot( const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlNot( const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlXor( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlXor( const RValue &, const RValue & ) const");
  return defaultIndex();
}

Value
TypeInfo::vhdlXnor( const RValue &, const RValue & ) const {
  unimplementedFn("TypeInfo::vhdlXnor( const RValue &, const RValue & ) const");
  return defaultIndex();
}

const RValue &
TypeInfo::left( const RValue & ) const {
  unimplementedFn("TypeInfo::TypeInfo::left( const RValue &dimension ) const");
  return defaultIndex();
}

const RValue &
TypeInfo::right( const RValue & ) const {
  unimplementedFn("TypeInfo::TypeInfo::right( const RValue &dimension ) const");
  return defaultIndex();
}

const RValue &
TypeInfo::high( const RValue & ) const {
  unimplementedFn("TypeInfo::TypeInfo::high( const RValue &dimension ) const");
  return defaultIndex();
}

const RValue &
TypeInfo::low( const RValue & ) const {
  unimplementedFn("TypeInfo::TypeInfo::low( const RValue &dimension ) const");
  return defaultIndex();
}

const EnumerationLiteral &
TypeInfo::ascending( const RValue &index ) const {
  unimplementedFn("TypeInfo::ascending() const");
  return EnumerationLiteral::getEnumerationTrue();
}

Value
TypeInfo::value( const StringLiteral &stringLit ) const {
  return value( stringLit.toString() );
}

Value
TypeInfo::value( const string & ) const {
  unimplementedFn("TypeInfo::value( const string &initValue ) const");
  return UniversalInteger(0);
}

Value
TypeInfo::value( FileObject &file ) const {
  return value( file.nextToken() );
}

UniversalInteger
TypeInfo::pos( const RValue &expression ) const {
  return UniversalInteger( expression.getIntValue() );
}

Value
TypeInfo::val( const RValue &expression ) const {
  unimplementedFn("TypeInfo::val( const RValue &expression ) const");
  return defaultIndex();
}

Value
TypeInfo::succ( const RValue &expression ) const {
  unimplementedFn("TypeInfo::val( const RValue &expression ) const");
  return defaultIndex();
}

Value
TypeInfo::pred( const RValue &expression ) const {
  unimplementedFn("TypeInfo::pred( const RValue &expression ) const");
  return defaultIndex();
}

Value
TypeInfo::leftof( const RValue &expression ) const {
  if( getDirection() == ArrayInfo::to ){
    return pred( expression );
  }
  else{
    return succ( expression );
  }
}

Value
TypeInfo::rightof( const RValue &expression ) const {
  if( getDirection() == ArrayInfo::to ){
    return succ( expression );
  }
  else{
    return pred( expression );
  }
}

const ScalarTypeInfo &
TypeInfo::range( const RValue &index ) const{
  abort();
}

const ScalarTypeInfo &
TypeInfo::reverse_range( const RValue &index ) const {
  abort();
}


UniversalInteger 
TypeInfo::length( const RValue &index ) const {
  unimplementedFn("TypeInfo::length( const RValue &index ) const");
  return defaultIndex();
}

const UniversalInteger &
TypeInfo::defaultIndex(){
  static UniversalInteger i(1);
  return i;
}

void
TypeInfo::unimplementedFn( const string &functionName ) const {
  cerr << functionName << " called on a(n) " << getKindText() 
       << " - aborting!" << endl;
  abort();
}

const string &
TypeInfo::NL(const int indent) const {
  static string nextLine;
  int i;
  nextLine = "\n";
  for (i=0 ; i<indent ; ++i){
    nextLine += " ";
  }
  return nextLine;
}
 
/**
  This is here solely for detection of the library when searching via
  "configure".
*/
extern "C" {
  int isTyvis(){ return 0; }
}

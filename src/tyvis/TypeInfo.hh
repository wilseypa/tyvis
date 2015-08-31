#ifndef TYPEINFO_HH
#define TYPEINFO_HH

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
#include "tyvis/UniversalInteger.hh"
#include "tyvis/ArrayInfo.hh"
#include "tyvis/RangeIterator.hh"
#include "tyvis/Value.hh"
#include <vector>
using std::vector;

class FileObject;
class ObjectBase;
class RValue;
class StringLiteral;
class SubElementFactory;

/**
   The root of all TypeInfo classes.
*/

class TypeInfo {
public:
  void * operator new (size_t);
  void operator delete (void *, size_t);

  TypeInfo() {}
  virtual ~TypeInfo() {}
  
  virtual TypeInfo& operator=(const TypeInfo &) {
    cerr << "Error - TypeInfo::operator=() called.\n";
    return *this;
  }

  virtual ArrayInfo::ArrayDirn_t getDirection() const;

  /**
     Clone this TypeInfo.
  */
  virtual TypeInfo *clone() const;

  /**
     This is here for testing.
  */
  virtual bool operator==( const TypeInfo &that ) const;

  /**
     This method is used to create an object of the appropriate type given
     that we are the typeInfo.  The arguments are the (possibly not fully
     constructed) parent and the kind of object that we want for any
     subelements.  For scalar types these arguments are completely ignored.
  */
  virtual RValue *create( const SubElementFactory *subElementFactory = 0 ) const;

  virtual bool isCompositeType() const { return false; }
  virtual bool isScalarType() const { return false; }
  virtual bool isPhysicalType() const { return false; }

  virtual const TypeInfo & readVal() const {return *this;}
  virtual const TypeInfo &getFieldInfo( int fieldIndex ) const;

  /**
     Returns the field name of a subelement in a composite type.  Record
     types return the name of a field; array types return
     "<prefix>(fieldNumber)".
  */
  virtual string getSubFieldName( const string &prefix, int d ) const;

  /**
     Returns true if this is an enumeration type whose elements all belong
     to the character enumeration.
  */
  virtual bool isCharacterType() const;
  
  /**
     If this is an EnumerationTypeInfo, this will return the image map.
  */
  virtual const vector<string> &getImageMap() const;

  static vector<string> createImageMap( const char *elements[] );
  static TypeInfo &getNullTypeInfo();

  /**
     A method to assist in debugging.
  */
  string getKindText() const;

  virtual void debugPrint( ostream &, int = 0 ) const;
  
  //@{
  /**
     Each of these operators evaluates the RValues passed in the context of
     this typeinfo, and returns the result.  The caller is responsble for
     deleting that result!
  */
  virtual Value vhdlPlus    ( const RValue &, const RValue & ) const;
  virtual Value vhdlMinus   ( const RValue &, const RValue & ) const;
  virtual Value vhdlMultiply( const RValue &, const RValue & ) const;
  virtual Value vhdlDivide  ( const RValue &, const RValue & ) const;
  virtual Value vhdlMod     ( const RValue &, const RValue & ) const;
  virtual Value vhdlRem     ( const RValue &, const RValue & ) const;
  virtual Value vhdlPow     ( const RValue &, const RValue & ) const;
  virtual Value vhdlNot     ( const RValue & ) const;
  virtual Value vhdlXor     ( const RValue &, const RValue & ) const;
  virtual Value vhdlXnor    ( const RValue &, const RValue & ) const;
  //@}

  //@{
  /**
     Type attributes.
  */
  /**
     For scalar types returns the left element of the range.  For array
     types, returns the left element of the index passed in.
  */
  virtual const RValue &left( const RValue &index = defaultIndex() ) const;

  /**
     For scalar types returns the right element of the range.  For array
     types, returns the right element of the index passed in.
  */
  virtual const RValue &right( const RValue &index = defaultIndex() ) const;

  /**
     For scalar types, returns the high element of the range.
  */
  virtual const RValue &high( const RValue &index = defaultIndex() ) const;
  /**
     For scalar types, returns the low element of the range.
  */
  virtual const RValue &low( const RValue &index = defaultIndex() ) const;
  /**
     Returns either the boolean literal "true" or "false", indicating
     whether this is an ascending range or not.
  */
  virtual const EnumerationLiteral &ascending( const RValue &index = defaultIndex() ) const;
  /**
     Using this TypeInfo as context, convert the string passed in into a
     literal.
  */
  virtual Value value( const StringLiteral &initValue ) const;
  virtual Value value( const string &initValue ) const;
  virtual Value value( FileObject &file ) const;

  /**
     Returns the position of the literal passed in.  For example, if I had
     enumeration type "red", "white", "blue", this would return
     UniversalInteger(2) for the EnumerationLiteral representing "white".
  */
  virtual UniversalInteger pos( const RValue &expression ) const;
  /**
     Returns the value corresponding to the ordinal position passed in.
     For example, if I had enumeration type "red", "white", "blue", this
     would return the EnumerationLiteral representing "white" for
     UniversalInteger(2).
  */
  virtual Value val( const RValue &expression ) const;
  /**
     Returns the literal succeeding that passed in.  Pass in "1", get back
     "2".
  */
  virtual Value succ( const RValue &expression ) const;
  /**
     Returns the literal succeeding that passed in.  Pass in "2", get back
     "1".
  */
  virtual Value pred( const RValue &expression ) const;
  
  /**
     Returns the literal to the leftof that passed in.  Pass in "2", get
     back either "1" or "3" depending on the direction of the type.
  */
  virtual Value leftof( const RValue &expression ) const;
  /**
     Returns the literal to the rightof that passed in.  Pass in "2", get
     back either "1" or "3" depending on the direction of the type.
  */
  virtual Value rightof( const RValue &expression ) const;

  virtual const ScalarTypeInfo &range( const RValue &index = defaultIndex() ) const;
  virtual const ScalarTypeInfo &reverse_range( const RValue &index = defaultIndex() ) const;
  
  /**
     The 'length attribute
  */
  virtual UniversalInteger length( const RValue &index = defaultIndex() ) const;
  //@}

  static const UniversalInteger &defaultIndex();

protected:
  void unimplementedFn( const string & ) const;
  virtual void debugPrintDetails( ostream &, int ) const {}
  const string & NL(const int ) const;
  
private:
};

#endif

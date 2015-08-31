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

#include "EnumerationTypeInfo.hh"
#include "EnumerationLiteral.hh"
#include "std_standardPkg.hh"

#include <string>
using std::string;

int 
EnumerationTypeInfo::calcRight( int numElements ){
  return numElements - 1;
}

//The constructors for EnumerationTypeInfo
EnumerationTypeInfo::EnumerationTypeInfo() :
  ScalarTypeInfo(),
  imageMap(0){}

EnumerationTypeInfo::EnumerationTypeInfo( const vector<string> &initImageMap ) :
  ScalarTypeInfo( UniversalInteger(0).clone(),
		  ArrayInfo::to,
		  UniversalInteger(calcRight( initImageMap.size() )).clone() ),
  imageMap( &initImageMap ){}

EnumerationTypeInfo::EnumerationTypeInfo( const IntegerTypeInfo &range,
					  const vector<string> &initImageMap ) :
  ScalarTypeInfo( range ),
  imageMap( &initImageMap ){}

EnumerationTypeInfo::~EnumerationTypeInfo(){
  // The image map belongs to some static method somewhere.
}

EnumerationTypeInfo::EnumerationTypeInfo(const EnumerationTypeInfo &that) :
  ScalarTypeInfo( that ),
  imageMap( that.imageMap ){}

int
EnumerationTypeInfo::getIndex( const string &value ) const {
  int retval = -1;
  int count = 0;
  for( vector<string>::const_iterator i = imageMap->begin();
       i < imageMap->end();
       i++, count++ ){
    if( *i == value ){
      retval = count;
      break;
    }
  }
  return retval;
}

string
EnumerationTypeInfo::getLiteral( int value ) const {
  string retval;
  if( value > (int)imageMap->size() - 1 ){
    cerr << "Trying to access element " << value << " of an image map with " 
	 << imageMap->size() << " elements in it." << endl;
    abort();
  }
  else{
    retval = (*imageMap)[value];
  }
  return retval;
}

const vector<string> &
EnumerationTypeInfo::getImageMap() const{
  return *imageMap;
}

bool
EnumerationTypeInfo::operator==( const TypeInfo &that ) const {
  bool retval = ScalarTypeInfo::operator==( that );
  if( retval ){
    if( getImageMap().size() == that.getImageMap().size() ){
      for( unsigned int i = 0;
	   i < imageMap->size();
	   i++ ){
	if( (*imageMap)[i] != that.getImageMap()[i] ){
	  retval = false;
	  break;
	}
      }
    }
    else{
      retval = false;
    }
  }
  return retval;
}

TypeInfo *
EnumerationTypeInfo::clone() const {
  return new EnumerationTypeInfo( *this );
}

EnumerationTypeInfo&
EnumerationTypeInfo::operator=(const EnumerationTypeInfo &ei) {
  ScalarTypeInfo::operator=( static_cast<const ScalarTypeInfo &>(ei) );
  imageMap = ei.imageMap;
  return *this;
}

RValue *
EnumerationTypeInfo::create( const SubElementFactory * ) const {
  return new EnumerationLiteral( *this );
}

bool
EnumerationTypeInfo::isCharacterType() const {
  bool retval = false;
  vector<string>::const_iterator i = imageMap->begin();
  while( i < imageMap->end() ){
    if( (*i).length() == 3 ){
      // The LRM says a character type is one that contains at least one
      // character literal.
      if( (*i)[0] == '\'' && (*i)[2] == '\'' ){
	retval = true;
	break;
      }
    }
    i++;
  }
  return retval;
}

Value
EnumerationTypeInfo::vhdlPlus( const RValue &left, const RValue &right ) const {
  const EnumerationLiteral &leftLit = dynamic_cast<const EnumerationLiteral &>(left.readVal());
  return Value(leftLit.vhdlPlus(right));
}

Value
EnumerationTypeInfo::vhdlMinus( const RValue &left, const RValue &right ) const {
  const EnumerationLiteral &leftLit = dynamic_cast<const EnumerationLiteral &>(left.readVal());
  return Value(leftLit.vhdlMinus(right));
}

Value
EnumerationTypeInfo::vhdlMultiply( const RValue &left, const RValue &right ) const {
  const EnumerationLiteral &leftLit = dynamic_cast<const EnumerationLiteral &>(left.readVal());
  return Value(leftLit.vhdlMultiply(right));
}

Value
EnumerationTypeInfo::vhdlDivide( const RValue &left, const RValue &right ) const {
  const EnumerationLiteral &leftLit = dynamic_cast<const EnumerationLiteral &>(left.readVal());
  return Value(leftLit.vhdlDivide(right));
}

Value
EnumerationTypeInfo::vhdlMod( const RValue &left, const RValue &right ) const {
  const EnumerationLiteral &leftLit = dynamic_cast<const EnumerationLiteral &>(left.readVal());
  return Value(leftLit.vhdlMod(right));

}

Value
EnumerationTypeInfo::vhdlRem( const RValue &left, const RValue &right ) const {
  const EnumerationLiteral &leftLit = dynamic_cast<const EnumerationLiteral &>(left.readVal());
  return Value(leftLit.vhdlRem(right));
}

Value
EnumerationTypeInfo::vhdlPow( const RValue &left, const RValue &right ) const {
  const EnumerationLiteral &leftLit = dynamic_cast<const EnumerationLiteral &>(left.readVal());
  return Value(leftLit.vhdlPow(right));
}

Value
EnumerationTypeInfo::vhdlNot( const RValue &obj ) const {
  const EnumerationLiteral &objLit = dynamic_cast<const EnumerationLiteral &>(obj.readVal());
  return Value(objLit.vhdlNot());
}

Value
EnumerationTypeInfo::vhdlXor( const RValue &lhs, const RValue &rhs ) const {
  const EnumerationLiteral &lhsLit = dynamic_cast<const EnumerationLiteral &>(lhs.readVal());
  return Value(lhsLit.vhdlXor(rhs));
}

Value
EnumerationTypeInfo::vhdlXnor( const RValue &lhs, const RValue &rhs ) const {
  const EnumerationLiteral &lhsLit = dynamic_cast<const EnumerationLiteral &>(lhs.readVal());
  return Value(lhsLit.vhdlXnor(rhs));
}

Value
EnumerationTypeInfo::val( const RValue &expression ) const {
  return Value( EnumerationLiteral( *this, expression ) );
}

Value
EnumerationTypeInfo::succ( const RValue &expression ) const {
  return Value( EnumerationLiteral( *this, expression.getIntValue() + 1 ) );
}

Value
EnumerationTypeInfo::pred( const RValue &expression ) const {
  return Value( EnumerationLiteral( *this, expression.getIntValue() - 1 ) );
}

Value
EnumerationTypeInfo::value( const string &initValue ) const {
  return Value( EnumerationLiteral( *this, initValue ) );
}

void
EnumerationTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  ScalarTypeInfo::debugPrintDetails(os, indent);
  os << NL(indent) << "vector<string> *imageMap";
  if (imageMap) {
    os << " (size = ";
    os << imageMap->size() << "):" << NL(indent);
    int j=imageMap->size();
    if (j > 20)
      j = 20;
    for (int i=0 ; i<j ; ++i) {
      if (i > 0)
        os << ", ";
      os << (*imageMap)[i];
    }
  }
  else {
    os << " is null";
  }
  return;
}

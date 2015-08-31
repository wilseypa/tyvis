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

#include "AccessVariable.hh"

void 
AccessVariable::print(ostream& os) const {
  os << val;
}

AccessVariable::AccessVariable( const TypeInfo &initTypeInfo ) :
  ObjectBase( initTypeInfo, "" ),
  curPos(0),
  val(""){}

AccessVariable::AccessVariable( const TypeInfo &initTypeInfo,
				const string &initName ) :
  ObjectBase( initTypeInfo, initName ){}

AccessVariable::AccessVariable( const AccessVariable &that ) : 
  ObjectBase( that ),
  curPos( that.curPos ),
  val( that.val ){}

AccessVariable &
AccessVariable::operator=( const AccessVariable &that ) {
  ObjectBase::operator=( that );
  curPos = that.curPos;
  val = that.val;
  return *this;
}

AccessVariable &
AccessVariable::operator=( const string &newVal ){
  curPos = 0;
  val = newVal;

  return *this;
}

void 
AccessVariable::reset(){
  val = "";
  curPos = 0;
}

const RValue &
AccessVariable::readVal() const{
  cerr << "AccessVariable::readVal() const called!" << endl;
  abort();
  return *this;
}

void
AccessVariable::updateVal( const RValue & ){
  cerr << "AccessVariable::updateVal() const called!" << endl;
  abort();
}

RValue &
AccessVariable::operator=(const RValue &){
  cerr << "AccessVariable::operator= called!" << endl;
  abort();
  return *this;
}

RValue *
AccessVariable::clone() const {
  return new AccessVariable( *this );
}

void
CompositeLiteral::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "val = " << val <<  "  curPos = " << curPos;
  ObjectBase::debugPrintDetails( os, indent);

  return;
}

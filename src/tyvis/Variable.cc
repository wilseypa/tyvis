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

#include "Variable.hh"
#include "TypeInfo.hh"
#include "SubElementFactory.hh"
#include "AccessObject.hh"
#include <iostream>
using std::cout;
using std::endl;

Variable::Variable( const TypeInfo &initTypeInfo,
		    const string &initName ) :
  ObjectBase( SubElementFactory( initTypeInfo, this, initName, SubElementFactory::VARIABLE ) ){}

Variable::Variable( const TypeInfo &initTypeInfo,
		    const string &initName,
		    const RValue &initValue ) :
  ObjectBase( SubElementFactory( initTypeInfo, this, initName, SubElementFactory::VARIABLE ) ){
  updateVal(initValue);
}

Variable::Variable( const Variable &that ) :
  ObjectBase( that ){}

Variable::Variable() : 
  ObjectBase(){}

Variable::Variable( const RValue &that ) :
  ObjectBase( SubElementFactory( that.getTypeInfo(), this, that.toString(), SubElementFactory::VARIABLE ) ){
  updateVal( that );
}

Variable::~Variable(){}; 

Variable &
Variable::operator=( const Variable &that ){
  ObjectBase::operator=( that );
  return *this;
}

Variable *
Variable::clone() const {
  return new Variable( *this );
}

ObjectBase *
Variable::createSubelement( const TypeInfo &typeInfo, int fieldIndex ){
  ASSERT( typeInfo.isCompositeType() );
  return new Variable( typeInfo.getFieldInfo(fieldIndex),
		       typeInfo.getSubFieldName( "foo", fieldIndex ) );
  
}

void
Variable::assign( const LValue &destObject,
		  const RValue &newValue ){ 
  if (dynamic_cast<const AccessObject *>(&newValue))
    const_cast<LValue &>(destObject).updateVal( dynamic_cast<const AccessObject *>(&newValue)->all() );
  else
    const_cast<LValue &>(destObject).updateVal( newValue.readVal() );
}

void 
Variable::assign( LValue &dest,
		  const RValue &src, 
		  const ArrayInfo &dinfo,
		  const ArrayInfo &sinfo ){
  
  if( dinfo != defaultInfo() || sinfo != defaultInfo() ){
    abort();
  }
  for( int i = 0;
       i < dest.length(UniversalInteger(1)).getIntValue();
       i++ ){
    dest.getField(i) = src.getField(i);
  }
}

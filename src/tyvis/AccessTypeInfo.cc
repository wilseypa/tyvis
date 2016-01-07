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

#include "AccessTypeInfo.hh"
#include "AccessObject.hh"

AccessTypeInfo::AccessTypeInfo( const TypeInfo &designatedType ) :
  TypeInfo(),
  designatedType( designatedType.clone() ){}

AccessTypeInfo::AccessTypeInfo( const AccessTypeInfo &that ) :
  TypeInfo( that ),
  designatedType( that.getDesignatedType().clone() ){}

AccessTypeInfo::~AccessTypeInfo(){
  delete designatedType;
  designatedType = 0;
}

RValue *
AccessTypeInfo::create( const SubElementFactory * ) const {
  return new AccessObject( *this );
}

TypeInfo *
AccessTypeInfo::clone() const {
  return new AccessTypeInfo( *this );
}

bool
AccessTypeInfo::operator==( const TypeInfo &that ) const {
  bool retval = false;
  if( dynamic_cast<const AccessTypeInfo *>( &that ) ){
    const AccessTypeInfo *thatAccessType = dynamic_cast<const AccessTypeInfo *>( &that );
    retval = ( *designatedType == *thatAccessType->designatedType );
  }
  return retval;
}


const TypeInfo &
AccessTypeInfo::getDesignatedType() const {
  ASSERT( designatedType != 0 );
  return *designatedType;
}

int
savantdeallocate( VHDLKernel *ker,  LValue &ptr ){
/* DNS this does not work.  Leads to a corrupt stack.  I think that the deallocate
   method may be the problem as it causes the object to delete itself.  Commenting out
   for now as I suspect this will never be much of a memory leak.
  if (dynamic_cast<AccessObject *>(&ptr))
    dynamic_cast<AccessObject &>(ptr).deallocate();
  else if (dynamic_cast<AccessObject *>(&ptr.readVal())) {
    dynamic_cast<AccessObject &>(ptr.readVal()).deallocate();
  }
*/

  return NORMAL_RETURN;
}

void
AccessTypeInfo::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "TypeInfo *designatedType: ";
  if (designatedType)
    designatedType->debugPrint(os, indent+2);
  else
    os << " is null";
  return;
}

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

#include <warped/SerializedInstance.h>
#include <warped/DeserializerManager.h>
#include "VHDLVTime.hh"

const VHDLVTime &
VHDLVTime::getVHDLVTimeZero(){
  static const VHDLVTime ZERO = VHDLVTime( 0, 0 );
  return ZERO;
}

const VHDLVTime &
VHDLVTime::getVHDLVTimePositiveInfinity(){ 
  static const VHDLVTime PINFINITY = VHDLVTime( getWarped64Max(), getWarped32Max() );
  return PINFINITY; 
}

void 
VHDLVTime::serialize( SerializedInstance *serialized ) const {
  serialized->addInt64( getMajor() );
  serialized->addInt( getMinor() );
}

Serializable *
VHDLVTime::deserialize( SerializedInstance *serialized ){
  warped64_t major = serialized->getInt64();
  int minor = serialized->getInt();
  VHDLVTime *retval = new VHDLVTime( major,
			     minor );
  return retval;
}


void 
VHDLVTime::registerDeserializer(){
  DeserializerManager::instance()->registerDeserializer( getVHDLVTimeDataType(),
							 &deserialize );
}

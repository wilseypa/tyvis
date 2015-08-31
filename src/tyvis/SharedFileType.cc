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

#include "SharedFileObject.hh"
#include "SharedFileEvent.hh"
#include <warped/SimulationManager.h>

SignalBase*
SharedFileState::locateSig(int) {
  cerr << "Error : SharedFileState::locateSig(int) called.\n"
       << "      : This method should never get called.\n";

  return NULL;
}

void
SharedFileState::cleanState() {}

unsigned int
SharedFileState::getStateSize(){
  return sizeof(SharedFileState);
}
  
SharedFileObject::SharedFileObject( const char *const name, bool alias ) :
  VHDLType(alias),
  VHDLKernel(name),
  openKind( EnumerationLiteral::READ_MODE ),
  fileName( name ){ }

SharedFileObject::SharedFileObject(const SharedFileObject &sft) :
  VHDLType((const VHDLType &) sft),
  VHDLKernel((const VHDLKernel &) sft),
  openKind( sft.openKind ),
  fileName( sft.getFileName() ){}


SharedFileObject::SharedFileObject(const char* name,
			       const EnumerationLiteral mode) :
  VHDLType(false),
  VHDLKernel(name),
  openKind( mode ),
  fileName( name ){}

SharedFileObject::SharedFileObject(const Array& name,
			       const EnumerationLiteral mode) :
  VHDLType(false),
  VHDLKernel(name.getString()),
  openKind( mode ),
  fileName( name.getString() ){}

void
SharedFileObject::initialize() {
  openFile(fileName, get_fileOpenKind());
}

SharedFileObject::~SharedFileObject() {}

Type
SharedFileObject::get_kind() const {
  return SHARED_FILE_TYPE;
}

ObjectBase::ObjectType
SharedFileObject::getKind() const {
  return ObjectBase::VARIABLE;
}

void
SharedFileObject::print(ostream &) const {}

VHDLType*
SharedFileObject::clone() const{
  cerr << "Error: SharedFileObject::clone() const called.\n"
       << "     : This method should never get called.\n";
  
  return NULL;
}


int
SharedFileObject::get_fileHandle() const {
  return SHARED_FILE_HANDLE;
}

EnumerationLiteral&
SharedFileObject::get_fileOpenKind() {
  return openKind;
}

void
SharedFileObject::set_fileOpenKind(const EnumerationLiteral& kind) {
  openKind = kind;
}

SharedFileObject&
SharedFileObject::operator=(const SharedFileObject &val) {
  VHDLKernel::operator=( val );
  
  openKind = val.openKind;
  
  return *this;
}

VHDLType&
SharedFileObject::operator=(const RValue &val) {
  return operator=(dynamic_cast<const SharedFileObject &>(val));
}

bool
SharedFileObject::operator==( const RValue &val ) const {
  return (get_fileHandle() == (dynamic_cast< const SharedFileObject&>(val).get_fileHandle()));
}
bool
SharedFileObject::operator!=( const RValue &val ) const {
  return !operator==( val );
}
bool
SharedFileObject::operator<( const RValue & ) const {
  abort();
}
bool
SharedFileObject::operator<=( const RValue & ) const {
  abort();
}
bool
SharedFileObject::operator>( const RValue & ) const {
  abort();
}
bool
SharedFileObject::operator>=( const RValue & ) const {
  abort();
}

State*
SharedFileObject::allocateState() {
  return new SharedFileState();
}

void
SharedFileObject::executeVHDL( VHDLKernel_state &,
			     Block &,
			     Block & ){
  cerr << "Error - SharedFileObject::executeVHDL() called.\n"
       << "      - This method should never get called.\n";
  abort();
}

void
SharedFileObject::executeProcess() {
  while (haveMoreEvents() == true) {
    const VHDLEvent *event = dynamic_cast<const VHDLEvent *>(getEvent());
    ASSERT( event != 0 );
    //    ASSERT ( event->getEventKind() == VHDLEvent::SHARED_FILE_EVENT );
    const SharedFileEvent *sfe = dynamic_cast<const SharedFileEvent *>(event);
    if (sfe->isReadRequest() == true) {
      // Read a file from our file...
      int lineSize = 0;
      char *rawData = getline(*fileHandles[0], lineSize);
      string data;
      if( rawData != 0 ){
	data.insert( 0, rawData, lineSize );
      }
      SharedFileEvent *newEvent = new SharedFileEvent( getTimeNow(),
						       getTimeNow(),
						       getName(),
						       sfe->getSender(),
						       data );
						   

      getObjectHandle( newEvent->getReceiver() )->receiveEvent( newEvent );
    }
    else {
      *fileHandles[0] << sfe->getData() << endl;
    }
  }
}

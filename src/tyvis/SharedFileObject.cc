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
#include "FileTypeInfo.hh"
#include <warped/SimulationManager.h>

SignalBase*
SharedFileState::locateSig(int) {
  cerr << "Error : SharedFileState::locateSig(int) called.\n"
       << "      : This method should never get called.\n";

  return NULL;
}

void
SharedFileState::cleanState() {}
  
SharedFileObject::SharedFileObject( const FileTypeInfo &initTypeInfo, const string &name ) :
  ObjectBase(initTypeInfo, name),
  VHDLKernel(name),
  openKind( SAVANT_READ_MODE ),
  fileName( name ){ }

SharedFileObject::SharedFileObject(const SharedFileObject &that) :
  ObjectBase(that),
  VHDLKernel(that),
  openKind( that.openKind ),
  fileName( that.getFileName() ){}


SharedFileObject::SharedFileObject( const FileTypeInfo &initTypeInfo,
				    const string &name,
				    const EnumerationLiteral mode) :
  ObjectBase( initTypeInfo, name ),
  VHDLKernel(name),
  openKind( mode ),
  fileName( name ){}

SharedFileObject::SharedFileObject( const FileTypeInfo &initTypeInfo,
				    const ArrayLiteral &name,
				    const EnumerationLiteral mode) :
  ObjectBase( initTypeInfo, name.toString() ),
  VHDLKernel(name.toString()),
  openKind( mode ),
  fileName( name.toString() ){}

void
SharedFileObject::initialize() {
  openFile(fileName, get_fileOpenKind());
}

SharedFileObject::~SharedFileObject() {}

void
SharedFileObject::print(ostream &) const {}

void vcdPrint( ostream &os ) const {os << "INVALID";}

RValue *
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

RValue &
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

const RValue &
SharedFileObject::readVal() const {
  cerr << "Error, SharedFileObject::readVal() " << endl;
  abort();
  return SAVANT_BOOLEAN_FALSE; // keep the compiler quiet
}

void
SharedFileObject::updateVal( const RValue & ){
  cerr << "Error, SharedFileObject::updateVal( const RValue & ) " << endl;
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
						       VHDLKernel::getName(),
						       sfe->getSender(),
						       data );
						   

      getObjectHandle( newEvent->getReceiver() )->receiveEvent( newEvent );
    }
    else {
      *fileHandles[0] << sfe->getData() << endl;
    }
  }
}

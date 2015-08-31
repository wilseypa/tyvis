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

#include "FanDest.hh"
#include "SignalNetinfo.hh"
#include "SourceInfo.hh"
#include "SourceData.hh"
#include "TypeConvert.hh"
#include "SourceTypeConvert.hh"
#include "SubElementFactory.hh"
#include "TyvisVarArgs.hh"
#include "UniversalInteger.hh"
#include "Signal.hh"
#include "VCD.hh"
#include <iostream>

static const bool debug = false;

SignalNetinfo::SignalNetinfo( const TypeInfo &initTypeInfo,
			      const string &signalName ) :
  ObjectBase( SubElementFactory( initTypeInfo,
				 this,
				 signalName + "<netInfo>",
				 SubElementFactory::NETINFO )),
  id( globalSignalId++ ),
  driverAddedFlag( false ),
  source( 0 ),
  sourceBaseDeleteFlag( false ){
  if( debug ){
    cerr << signalName << " constructed, id = " << id << endl;
  }
}

SignalNetinfo::SignalNetinfo( const SignalNetinfo &sigIn,
                              const IntegerTypeInfo &tiIn ) :
  ObjectBase( const_cast<SignalNetinfo &>(sigIn), tiIn ),
  id( -2 ),
  driverAddedFlag( false ),
  source( 0 ),
  sourceBaseDeleteFlag( false ) {
  if( debug ){
    cerr << endl << "Slice subset of SignalNetinfo constructed, id = " << id << endl;
    tiIn.debugPrint(cerr);
  }
}

SignalNetinfo::SignalNetinfo( const ArrayTypeInfo &tiIn,
                              TyvisVarArgs &siIn ) :
  ObjectBase( tiIn, siIn ),
  id( -2 ),
  driverAddedFlag( false ),
  source( 0 ),
  sourceBaseDeleteFlag( false ) {
}

SignalNetinfo::SignalNetinfo() :
  ObjectBase(), 
  id( globalSignalId++ ), 
  driverAddedFlag( false ),
  source( 0 ),
  sourceBaseDeleteFlag( false ){
  if( debug ){
    cerr << "anonymous SignNetinfo constructed, id = " << id << endl;
  }
}

SignalNetinfo::~SignalNetinfo() {
  if (id == -2)
    voidElements();
  if ( getSourceBaseDeleteFlag() && source != NULL ){
    delete source ;
    source = 0;
  }
}

SignalNetinfo&
SignalNetinfo::addToFanOut( const RValue* ptr ){
  return addToFanOut(*ptr);
}

SignalNetinfo&
SignalNetinfo::addToFanOut( const RValue& ptr ){
  if (dynamic_cast<const SignalNetinfo *>(&ptr)) {
    return addToFanOut(dynamic_cast<const SignalNetinfo &>(ptr));
  }
  return *this;
}

SignalNetinfo&
SignalNetinfo::addToFanOut( const SignalNetinfo* ptr ){
  return addToFanOut(*ptr);
}

SignalNetinfo&
SignalNetinfo::addChild( SignalNetinfo &sig, VHDLKernel *ptr ){
  ObjectBase *sourceSignal = static_cast<ObjectBase *>(new Signal( getTypeInfo(), getName() )); 
  if (isCompositeType()) {
    for( int i = 0; i < length().getIntValue(); i++ ){
      dynamic_cast<SignalNetinfo &>(getField(i)).addChild(dynamic_cast<SignalNetinfo &>(sig.getField(i)), ptr);
    }
  }
  if (sig.getSource() == NULL) {
    sig.setSource( new SourceData );
    sig.getSource()->addChild(sourceSignal, ptr);
  }
  else {
    if( sig.getSource()->getNumChildren() == 0 ){
      SourceBase *source = sig.getSource();
      if( dynamic_cast<SourceData *>(source) ){
        if( ptr == SourceBase::ANONYMOUS_PROCESS_ID) {
          cerr << "Warning !! _savant_entity_elab::addChild() called with"
               << "ANONYMOUS_PROCESS_ID but this is not the first guy to be"
               << " added" << endl;
          abort();
        }
        else {
          if (source->isAnonymousDriver() == true) {
            sourceSignal = sig.getSource()->getData();
            sig.setSource( new SourceData );
            sig.getSource()->addChild(sourceSignal, ptr);
          }
          else {
            sig.setSource( new SourceInfo );
            sig.getSource()->addChild( source->getData(),
                                                  source->getSourceId() );
            sig.getSource()->addChild(sourceSignal, ptr);
          }
        }
        delete source;
      }
      else if ( dynamic_cast<SourceInfo *>(source) ){
        sig.getSource()->addChild(sourceSignal, ptr);
      }
      else if ( dynamic_cast<SourceTypeConvert *>(source) ){
        sig.getSource()->addChild(source);
        sig.getSource()->addChild(sourceSignal, ptr);
      }
    }
    else if (ptr != SourceBase::ANONYMOUS_PROCESS_ID) {
      // Check if there is an anonymous driver already present in the tree
      // is so then delete that guy and add the new guy.
      if (sig.getSource()->getNumChildren() == 1) {
        // Check if the first guy is an anonymous driver. If so pitch him
        // after copying the default initial value.
        ASSERT ( dynamic_cast<SourceInfo *>(sig.getSource()) );
        SourceInfo *tempSourceInfo = (SourceInfo *) sig.getSource();
        if (tempSourceInfo->getChild(0)->isAnonymousDriver() == true) {
          ASSERT( dynamic_cast<SourceData *>(tempSourceInfo->getChild(0)) );
          ObjectBase *tempData =
            dynamic_cast<ObjectBase *>(tempSourceInfo->getChild(0)->getData());
          // tempSourceInfo->child = NULL;
          delete tempSourceInfo;
          sig.setSource( new SourceInfo );
          sig.getSource()->addChild(tempData, ptr);
        }
        else {
          sig.getSource()->addChild(sourceSignal, ptr);
        }
      }
      else {
        sig.getSource()->addChild(sourceSignal, ptr);
      }
    }
    if ( ptr != SourceBase::ANONYMOUS_PROCESS_ID){
      sig.setDriverAddedFlag();
    }
  }
}

SignalNetinfo &
SignalNetinfo::addToFanOut( const SignalNetinfo &sig ){
  if( isCompositeType() ){
    for( int i = 0; i < length().getIntValue(); i++ ){
      ASSERT( dynamic_cast<SignalNetinfo *>( &getField(i) ) != 0 );
      ASSERT( dynamic_cast<const SignalNetinfo *>( &sig.getField(i) ) != 0 );
      dynamic_cast<SignalNetinfo &>(getField(i)).addToFanOut( dynamic_cast<const SignalNetinfo &>(sig.getField(i)) );
    }    
  }
  else{
    fanOut.addAll( sig.getFanOut() );
  }
  return *this;
}

SignalNetinfo&
SignalNetinfo::addToFanOut( VHDLKernel *fanOutTo ){
  if( isCompositeType() ){
    for( int i = 0; i < length().getIntValue(); i++ ){
      ASSERT( dynamic_cast<SignalNetinfo *>( &getField(i) ) != 0 );
      dynamic_cast<SignalNetinfo &>(getField(i)).addToFanOut( fanOutTo );
    }
  }
  else{
    fanOut.add( fanOutTo->getName(), getId() );
  }

  return *this;
}

SignalNetinfo &
SignalNetinfo::addToFanOut( const string &processName, int destSigId ){
  if( debug ){
    cerr << getName() << "::addToFanout(" << processName << ", " 
	 << destSigId << ") called!" << endl;
  }

  if( isCompositeType() ){
    for( int i = 0; i < length().getIntValue(); i++ ){
      ASSERT( dynamic_cast<SignalNetinfo *>( &getField(i) ) != 0 );
      dynamic_cast<SignalNetinfo &>(getField(i)).addToFanOut( processName, destSigId );
    }
  }
  else{
    fanOut.add( processName, destSigId );
  }
  return *this;
}

void 
SignalNetinfo::setResolutionFunctionId(int resolutionFnId) {
  if(source == NULL) {
    source = new SourceInfo;
  }
  else if( dynamic_cast<SourceData *>(source) != 0 ){
    SourceBase *newSrc = new SourceInfo;
    newSrc->addChild(source);
    source = newSrc;
  }
  source->setResolutionFnId(resolutionFnId);
}

void 
SignalNetinfo::setTypeConversionFunctionId(int typeConversionFnId) {
  if(source == NULL) {
    source = new SourceInfo;
  }
  else if( dynamic_cast<SourceData *>(source) != 0 ){
    SourceBase *newSrc = new SourceInfo;
    newSrc->addChild(source);
    source = newSrc;
  }
  source->setUpTypeConversionFnId(typeConversionFnId);
}

const RValue & 
SignalNetinfo::readVal() const {
  cerr << "ERROR: SignalNetinfo::readVal const called." << endl;
  abort();
  return *(new UniversalInteger(false)); // Just to keep CC from shouting.
}

RValue & 
SignalNetinfo::readVal(){
  cerr << "ERROR: SignalNetinfo::readVal called." << endl;
  abort();
  return *(new UniversalInteger(false)); // Just to keep CC from shouting.
}

void
SignalNetinfo::updateVal(const RValue &){
  cerr << "ERROR: SignalNetinfo::updateVal called." << endl;
  abort();
}

RValue & 
SignalNetinfo::operator=( const RValue &s ){
  return operator=((const SignalNetinfo &) s);
}

SignalNetinfo &
SignalNetinfo::operator=(const SignalNetinfo &that ) {
  ObjectBase::operator=( that );

  fanOut = that.fanOut;
  source  = that.source;
  sourceBaseDeleteFlag = false;

  additionalDrivers = that.additionalDrivers;
  
  return (*this);
}

ObjectBase* 
SignalNetinfo::clone() const {
  SignalNetinfo *retval = new SignalNetinfo;
  *retval = *this;
  retval->setSourceBaseDeleteFlag(false);
  return retval;
}

// Storage place and initialisation of global signal identifier.
int SignalNetinfo::globalSignalId = 0;

void
SignalNetinfo::setDriverAddedFlag(){
  driverAddedFlag = true;
}

void
SignalNetinfo::setDriverAddedFlag(const bool newValue){
  driverAddedFlag = newValue;
}

bool
SignalNetinfo::getDriverAddedFlag() const {
  return driverAddedFlag;
}

bool
SignalNetinfo::getSourceBaseDeleteFlag() const {
  return sourceBaseDeleteFlag ;
}

void
SignalNetinfo::setSourceBaseDeleteFlag( const bool newValue ){
  sourceBaseDeleteFlag = newValue;
}

void
SignalNetinfo::addAdditionalDriver( SignalBase *driver ){
  additionalDrivers.addDriver(driver);
}

void
SignalNetinfo::addAdditionalDrivers( const AdditionalDrivers &toAdd ){
  additionalDrivers.addDrivers( toAdd );
}

bool
SignalNetinfo::isResolvedSignal() const {
  return (source && source->getResolutionFnId() != SourceBase::DEFAULT_RF_ID);
}

void
SignalNetinfo::copyId( const SignalNetinfo &that ){
  if( isCompositeType() ){
    for( int i = 0; i < length().getIntValue(); i++ ){
      ASSERT( dynamic_cast<SignalNetinfo *>( &getField(i) ) != 0 );
      ASSERT( dynamic_cast<const SignalNetinfo *>( &that.getField(i) ) != 0 );
      dynamic_cast<SignalNetinfo &>(getField(i)).copyId( dynamic_cast<const SignalNetinfo &>(that.getField(i)) );
    }
  }
  else{
    id = that.getId();
  }
}

void
SignalNetinfo::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "SignalNetinfo id = " << id << "  driverAddedFlag = ";
  if (driverAddedFlag)
    os << "T";
  else
    os << "F";
  os << "  source = " << source;
  ObjectBase::debugPrintDetails( os, indent);

  return;
}

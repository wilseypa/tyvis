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

#include "Signal.hh"
#include "SourceData.hh"
#include "AdditionalDrivers.hh"
#include "VCD.hh"

Signal::Signal() :
  SignalBase(),
  source(0),
  drvVal(0){
  newSignal(1);
}

Signal::Signal( const TypeInfo &initTypeInfo ) :
  SignalBase( initTypeInfo, string("") ),
  source(0),
  drvVal( initTypeInfo.create() ){
  newSignal(3);
}

Signal::Signal( const CompositeTypeInfo &initTypeInfo,
		const string &initName ) :
  SignalBase( initTypeInfo, initName ),
  source(0),
  drvVal( initTypeInfo.create() ){
  newSignal(2);
}

Signal::Signal( const TypeInfo &initTypeInfo,
		const string &initName ) :
  SignalBase( initTypeInfo, initName ),
  source(0),
  drvVal( initTypeInfo.create() ){
  newSignal(3);
}

Signal::Signal( const CompositeTypeInfo &initTypeInfo,
		const string &initName,
		const RValue &initValue ) :
  SignalBase( initTypeInfo, initName, initValue ),
  source(0),
  drvVal( initValue.clone() ){
  newSignal(4);
}

Signal::Signal( const TypeInfo &initTypeInfo,
		const string &initName,
		const RValue &initValue ) :
  SignalBase( initTypeInfo, initName, initValue ),
  source(0),
  drvVal( initValue.clone() ){
  newSignal(5);
}

Signal::Signal( const Signal &that ) : 
  SignalBase( that ),
  source( that.source ),
  drvVal( that.drvVal->clone() ){
  newSignal(6);
}

void
Signal::newSignal(int i) {
//  cout << "Created signal : " << getName() << " from constructor " << i << endl;
}

Signal::~Signal() {
  delete drvVal;
  drvVal = 0;
}

bool
Signal::isResolved() const {
  ASSERT( source != 0 );
  return (source->getResolutionFnId() != SourceBase::DEFAULT_RF_ID);
}

RValue &
Signal::operator=( const RValue &that ){
  updateDrvVal( that );
  updateEffVal( that );

  return *this;
}

void
Signal::updateDriver( const int sigId, 
		      VHDLKernel *srcProcess,
		      const RValue *data, 
		      const ArrayInfo *sInfo, 
		      const ArrayInfo *dInfo ){
//  if( ( sInfo == NULL && dInfo == NULL ) ||
//      ( *sInfo == defaultInfo()  && *dInfo == defaultInfo() )){
    // Update the correct driver.
    if( source == NULL ){
      cerr << "Error: signal with name |" << getName() << "| and id |"
	   << getSigId() << "| has no driver but was asked to update.  This "
	   << "is an unrecoverable condition." << endl;
	abort();
    }
    if ( source->getDriver( srcProcess ) != NULL ){
      source->getDriver( srcProcess )->updateData( *data );
    } 
    else {
//      ASSERT( getAdditionalDrivers().length() > 0 ); //DNS driver could be absent
      if (getAdditionalDrivers().length() > 0 )
        getAdditionalDrivers().updateDriver( sigId, srcProcess, data, sInfo, dInfo );
    }
//  } 
//  else {
//    cerr << "Attempt to update driver of non-scalar signal" << endl;
//    abort();
//  }
}

void
Signal::disconnectDriver(VHDLKernel *srcId) {
  // disconnect the correct driver.
  SourceData* driver = source->getDriver(srcId);
  ASSERT(driver != NULL);
  driver->disconnect();
}

 
RValue *
Signal::readDriver(VHDLKernel * sigsrc) {
  return source->getDriver(sigsrc)->getData();
}


void
Signal::updateDrvVal( const RValue &newData,
		      const ArrayInfo *sInfo, 
		      const ArrayInfo *dInfo ){
  if( sInfo == NULL && dInfo == NULL ){
    delete drvVal;
    drvVal = newData.clone();
  } 
  else if ( newData.length(UniversalInteger(1)) != drvVal->length(UniversalInteger(1)) ){
   // if we're not assigning to the entire bit_vector
#ifdef VHDLDBG
    cout << name << "Data length is " << sInfo->length() << ", my length is " 
	 << drvVal.length();
    cout << ".  Assigning data |";
    data.print(cout);
    cout << "| to sig " << name << " slice " << *sInfo << endl;
#endif
    dynamic_cast<VHDLData *>(drvVal)->assignSlice(&newData, sInfo, dInfo);
  }
  else { // assignment is for.operator== arrays
    delete drvVal;
    drvVal = newData.clone();
  }
}


RValue *
Signal::readDrvVal() {
  return drvVal;
}

void
Signal::updateEffVal( const RValue &newValue, 
		      const ArrayInfo *sInfo,
		      const ArrayInfo *dInfo) {
  if ( (sInfo && *sInfo != defaultInfo() ) ||
       (dInfo && *dInfo != defaultInfo() ) ) {
    // if we're not assigning to the entire bit_vector
#ifdef VHDLDBG
    cout << name << "NewValue length is " << sInfo->length() << ", my length is " 
	 << effVal.length();
    cout << ".  Assigning newValue |" << newValue 
	 << "| to sig " << name << " slice " << *sInfo << endl;
#endif
    abort();
    //    dynamic_cast<VHDLData *>(readVal())->assignSlice(&newValue, sInfo, dInfo);
  }
  else { // assignment is for.operator== arrays
    updateVal( newValue );
  }
}

RValue &
Signal::readEffVal() {
  return const_cast<RValue &>(readVal());
}

void
Signal::setSource( SourceBase *newSource ){
  source = newSource;
}

Value
Signal::resolve( VHDLKernel *processPtr ){
  Value retval;
  if( isCompositeType() ){
    for( int i = 0; i < length().getIntValue(); i++ ){
      ASSERT( dynamic_cast<SignalBase *>(&getField(i)) != 0 );
      retval = dynamic_cast<SignalBase &>(getField(i)).resolve( processPtr );
    }
  }
  else{
    ASSERT( source != 0 );
    retval = source->resolve( processPtr, getType() );
  }
  return retval;
}

SignalBase *
Signal::findSigInBlock( int sigId, VHDLKernel *srcId ){
  if( getSigId() == sigId ){
    if (source->getDriver(srcId) != NULL ){
      return this;
    }
  }
  
  return NULL;
}

RValue *
Signal::clone() const {
  return new Signal( *this );
}

void
Signal::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "source = " << source;
  os << NL(indent) << "Signal driving value:";
  drvVal->debugPrintDetails( os, indent+5);
  SignalBase::debugPrintDetails( os, indent);

  return;
}

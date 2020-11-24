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

#include <Debug.h>
#include <algorithm>
#include <functional>
#include "FanDest.hh"
#include "SignalBase.hh"
#include "SignalNetinfo.hh"
#include "SigEvent.hh"
#include "SubElementFactory.hh"
#include "VHDLKernel.hh"
#include "Transaction.hh"
#include "VCD.hh"

SignalBase::SignalBase() : 
  transactionList( *this ),
  id( -1 ),
  sensitive( false ),
  busResFn( -1 ),
  downTypeConversionFnListPtr( 0 ),
  transactionSequenceNumber( 0 ),
  myParentSignal(0),
  idMap(0){
}

SignalBase::SignalBase( const TypeInfo &initTypeInfo,
			const string &initName ) :
  ObjectBase( SubElementFactory( initTypeInfo, this, initName, SubElementFactory::SIGNAL )),
  transactionList( *this ),
  id( -1 ),
  sensitive( false ),
  busResFn( -1 ),
  downTypeConversionFnListPtr( 0 ),
  transactionSequenceNumber( 0 ),
  myParentSignal(0),
  idMap(0){
}

SignalBase::SignalBase( const TypeInfo &initTypeInfo,
			const string &initName,
			const RValue &initValue ) :
  ObjectBase( SubElementFactory( initTypeInfo, this, initName, SubElementFactory::SIGNAL ) ),
  transactionList( *this ),
  id( -1 ),
  sensitive( false ),
  busResFn( -1 ),
  downTypeConversionFnListPtr( 0 ),
  transactionSequenceNumber( 0 ),
  myParentSignal(0),
  idMap(0){
    updateVal( initValue );
}

SignalBase::SignalBase( const SignalBase &that ) : 
  ObjectBase( that ),
  transactionList( *this ),
  id( that.id ),
  sensitive( that.sensitive ),
  busResFn( that.busResFn ),
  fanOut( that.fanOut ),
  downTypeConversionFnListPtr( that.downTypeConversionFnListPtr ),
  additionalDrivers( that.additionalDrivers ),
  transactionSequenceNumber( that.transactionSequenceNumber ),
  myParentSignal( that.getParentSignal() ),
  idMap(0){}

SignalBase::~SignalBase( ) {
  if (idMap)
    delete idMap;
}

SignalBase &
SignalBase::operator=(const SignalBase &that) {
  id            = that.id;
  sensitive     = that.sensitive;
  busResFn      = that.busResFn;
  attributeList = that.attributeList;
  allAttributes = that.allAttributes;

  // Copy Fan-out list
  fanOut       = that.fanOut;
  additionalDrivers = that.additionalDrivers;  
  downTypeConversionFnListPtr = that.downTypeConversionFnListPtr;
  
  //  parentCompositeObject = that.parentCompositeObject;
  
  // Copy necessary transactions from one list to another...
  transactionList.copy( that.transactionList );
  myParentSignal = that.getParentSignal();
  idMap = 0;

  return *this;
}

// Add a VHDLKernel process object to the fan out list.
// SignalBase&
// SignalBase::addToFanOut( VHDLKernel *objid ){
//   // If there exist a fan out list,
  
//   // Check here for the destination process id and the signal id already in
//   // the list.
//   bool found = false;
//   for(unsigned int j = 0; j < fanDest.length(); j++){
//     // If there is an object having the correct process Id, and if the
//     // signal identifier is different, the object signal identifier matches,
//     // then we have an object in the fan out list which is.operator==ent to the
//     // one added.
    
//     if ( fanDest[j]->getProcessName() == objid->getName() ){
//       found = true;
//       break;
//     }
//   }
//   if( !found ){
//     fanDest.push_back( new FanDest(objid->getName()) );
//   }
//   return *this;
// }

// // Remove a VHDLKernel process object from the fan out list.

// SignalBase&
// SignalBase::Remove(VHDLKernel* objid){
//   return Remove(objid, id);
// }

// SignalBase&
// SignalBase::Remove( VHDLKernel *objid, int  sigid ){
//   // If there exist a fan out list,
  
//   // Check here for the destination process id and the signal id already in
//   // the list.
  
//   vector<FanDest *>::iterator pos = fanDest.end();
//   for( unsigned int j = 0; j < fanDest.length(); j++ ){
//     // If there is an object having the correct process Id, and if the
//     // signal identifier is different, the object signal identifier matches,
//     // then we have an object in the fan out list which is.operator==ent to the
//     // one added.
//     if ( fanDest[j]->getProcessName() == objid->getName() ){
//       advance( pos, j );
//       break;
//     }
//   }
  
//   if( pos != fanDest.end() ){
//     FanDest *erased = *pos;
//     fanDest.erase( pos );
//     delete erased;
//   }

//   return *this;
// }

void 
SignalBase::setAttrib( AttribType typ, SignalBase &attrib ){
  Attribute *newAttribute = new Attribute( typ, attrib );
  newAttribute->getSignal().initializeImplicitSignal(typ);
  attributeList.push_back( newAttribute );
}

SignalBase*
SignalBase::findSigInBlock(int, VHDLKernel *) {
  cerr << "ERROR: SignalBase::findSigInBlock(int sigId, int srcId) called" << endl;
  return NULL;
}

void
SignalBase::setSourceBaseDeleteFlag(bool){
  cerr << "ERROR: SignalBase::setSourceBaseDeleteFlag(bool) called" << endl;
}

SourceBase*
SignalBase::getSource() {
  cerr << "ERROR: SignalBase::getSource() called" << endl;
  return NULL;
}

bool
SignalBase::isResolved() const {
  cerr << "SignalBase::isResolved() called.  Aborting." << endl;
  abort();
  return false;
}


void
SignalBase::updateVal( const RValue &newVal ){
  if( isCompositeType() ){
    RangeIterator localIterator = range(UniversalInteger(1)).iterator();
    RangeIterator valIterator = newVal.range(UniversalInteger(1)).iterator();
    while( localIterator.hasNext() ){
      //      ASSERT( valIterator.hasNext() );
      RValue &target = (*this)[localIterator.next()];
      const RValue &subElementData = newVal[valIterator.next()];
      dynamic_cast<LValue &>(target).updateVal( subElementData );
    }
  }
  else{
    ObjectBase::updateVal( newVal );
  }
}

ArrayInfo*
SignalBase::readArrayInfo() {
  cerr << getName() << " readArrayInfo ERROR!" << endl;
  return NULL;
}

void
SignalBase::setElaborationInfo( const SignalNetinfo &netInfo ){
//   cerr << "SignalBase " << getName() 
//        << " setElaborationInfo with SignalNetinfo " << netInfo.getName() <<
//        "  ID = " << netInfo.getId() << " at " << this << endl;
  id = netInfo.getId();
  if( isCompositeType() ){
    ASSERT( length() == netInfo.length() );
    if (GlobalControl::getVCD()) {
      GlobalControl::getVCD()->registerCompositeSignal (id, getName(),
                     left().getIntValue(), right().getIntValue() );
    }
    for( int i = 0; i < length().getIntValue(); i++ ){
      ASSERT( dynamic_cast<SignalBase *>(&getField(i)) != 0 );
      ASSERT( dynamic_cast<const SignalNetinfo *>(&netInfo.getField(i)) != 0 );
      dynamic_cast<SignalBase &>(getField(i)).setElaborationInfo( dynamic_cast<const SignalNetinfo &>(netInfo.getField(i)) );
    }
    if (GlobalControl::getVCD()) {
      GlobalControl::getVCD()->finishCompositeSignal();
    }
  }
  else{
    setSource(netInfo.getSource());    
    delete downTypeConversionFnListPtr;
    downTypeConversionFnListPtr = new Block();
    *downTypeConversionFnListPtr = netInfo.getDownTypeConversionFnList();

    sensitive = true;
    fanOut = netInfo.getFanOut();
//    cout << "DNS: in signal " << id << " for signal " << getName() << " with SignalNetinfo at " <<
//      &netInfo << " and fan size = " << fanOut.size() << endl;
    if (GlobalControl::getVCD()) {
      vector<int> fan;
      fanOut.getIds(fan);
      GlobalControl::getVCD()->registerSignal (id, fan , getName(), readEffVal());
    }
    additionalDrivers           = netInfo.getAdditionalDrivers(); 
  }
}

void
SignalBase::sendTransactionToFanoutList( VHDLKernel *sourceProcess, 
					 const RValue &newData,
					 const VHDLVTime &delay, 
					 const VHDLVTime &rejTime,
					 const ArrayInfo &destInfo, 
					 const ArrayInfo &srcInfo,
					 const Transaction *newTransaction ){
  SigEvent *transEvent = generateEvent( sourceProcess,
					newTransaction, 
					delay,
					rejTime, 
					newData, 
					destInfo, 
					srcInfo );
  transEvent->setSigSrc( sourceProcess->getVHDLProcessId() );
#ifdef VHDLDBG
  cout << "Sending event " << *transEvent << endl;
#endif

  if (fanOut.size() > 0) {
    fanOut.sendEvent( transEvent, sourceProcess );
  }
  else {
    FanDest self(sourceProcess, id );
    self.sendEvent(transEvent, sourceProcess );
  }

  delete transEvent;
}

void
SignalBase::compositeAssignSignal( VHDLKernel *sourceProcess, 
				   const RValue &newData,
				   const VHDLVTime &delay, 
				   const VHDLVTime &rejTime,
				   const ArrayInfo &destInfo, 
				   const ArrayInfo &srcInfo ){
  int fieldLength = 0;
  if( destInfo.length() == 0 ){  // default/null Info() used as destInfo
    fieldLength = length().getIntValue();
  }
  else{
    fieldLength = destInfo.length();
  }

  for( int i = 0; i < fieldLength; i++){
    ASSERT( dynamic_cast<SignalBase *>(&getField(i)) != 0 );
    if( newData.length().getIntValue() == 1 && newData.length().getIntValue() < srcInfo.length() ){
      //We have an aggregate
      dynamic_cast<SignalBase &>(getField(i)).assignSignal( sourceProcess,
  							    newData.getField(0),
							    delay,
							    rejTime,
							    destInfo,
							    srcInfo );
    }
    else{
      dynamic_cast<SignalBase &>(getField(i)).assignSignal( sourceProcess,
  							    newData.getField(i),
							    delay,
							    rejTime,
							    destInfo,
							    srcInfo );
    }
  }
}

void
SignalBase::assignSignal( VHDLKernel *sourceProcess, 
			  const RValue &newData,
			  const VHDLVTime &delay, 
			  const VHDLVTime &rejTime,
			  const ArrayInfo &destInfo, 
			  const ArrayInfo &srcInfo ){
  
  ASSERT ( sourceProcess != NULL );
  
  if( isCompositeType() ){
    compositeAssignSignal( sourceProcess,
			   newData,
			   delay,
			   rejTime,
			   destInfo,
			   srcInfo );
  }
  else{
    VHDLVTime transactionTime(sourceProcess->getTimeNow() + delay);
    Transaction *newTransaction = new Transaction( sourceProcess,
						   transactionTime,
						   getNextSequenceNumber(),
						   dynamic_cast<const VHDLData *>(newData.readVal().clone()));
    sendTransactionToFanoutList( sourceProcess,
				 newData,
				 delay,
				 rejTime,
				 destInfo,
				 srcInfo,
				 newTransaction );
    
    if( !transactionList.empty() ){
      transactionList.clean( sourceProcess->getTimeNow() );
      transactionList.doMarking( transactionTime,
				 newTransaction,
				 rejTime );
    }
    delete newTransaction;
  }
}

bool
SignalBase::matchId (int mId){
  bool match;
  if( isCompositeType() ){
    if (!idMap) {
      idMap = new map<int, SignalBase *>;
      addSigMap(idMap);
    }
    if (idMap->find(mId) != idMap->end())
      match = true;
    else
      match = false;
  }
  else
  {
    match = (id == mId);
  }
  return match;
}
  
SigEvent*
SignalBase::generateEvent( VHDLKernel *sender,
			   const Transaction *transaction, 
			   const VHDLVTime &, 
			   const VHDLVTime &rejTime,
			   const RValue    &sigValue, 
			   const ArrayInfo &destInfo,
			   const ArrayInfo &srcInfo ){
  ASSERT( transaction != 0 );

  // This method generates an event for a given destination when a transaction
  // gets posted on a signal.
  
  //ASSERT (srcInfo.length() == destInfo.length());

  SigEvent *newEvent = new SigEvent( sender->getTimeNow(), // Should be "now()"
				     transaction->getTime(),
				     sender,
				     sender,
				     EXPLICIT,
				     0,
				     id,
				     rejTime,
				     srcInfo,
				     destInfo,
				     id,
				     transaction->getSequenceNumber(),
				     NORMALTRANSACTION,
				     sigValue );
  
  return newEvent;
}


void
SignalBase::cancelTransaction( const Transaction* transaction ){
  // A transaction posted on this signal is being removed due to the marking
  // process. We now send a cancel message to each process indicating that a 
  // given transaction is being cancelled.
  VHDLKernel *sourceProcess = transaction->getSourceProcess();

  if( transaction->getTime() <= sourceProcess->getTimeNow() ){
    // cerr << "Warning: Attempt to cancel transaction at time "
    // << transaction->getTime() << " at simulation time " 
    // << process->getTimeNow() << ".  This operation cannot be "
    // << "performed since it violates semantics of DES and is ignored.\n";
    return;
  }
 
  SigEvent *transEvent = generateEvent( sourceProcess,
					transaction, 
					VHDLVTime::getVHDLVTimeZero(), 
					VHDLVTime::getVHDLVTimeZero(),
					SAVANT_BOOLEAN_TRUE, 
					nullInfo(),
					nullInfo() );
  transEvent->setTransactionType( CANCELTRANSACTION );
  // Ask the process to dispatch this cancel Transaction event for us...
  transEvent->setSigSrc( sourceProcess->getVHDLProcessId() );
  fanOut.sendEvent( transEvent, sourceProcess );
  delete transEvent;
}

unsigned int
SignalBase::getNextSequenceNumber(){
  return transactionSequenceNumber++;
}

void
SignalBase::initializeImplicitSignal( AttribType attribType ) {
  UniversalInteger tmp_integer(0);
  switch(attribType) {
  case LAST_EVENT:
  case LAST_ACTIVE:
    updateVal(PhysicalLiteral(std_standard_timeTypeInfo(), -1));
    break;
  case EVENT:
  case ACTIVE:
    updateVal(SAVANT_BOOLEAN_FALSE);
    break;
  case QUIET:
  case STABLE:
    updateDrvVal(SAVANT_BOOLEAN_TRUE, NULL, NULL);
    updateEffVal(SAVANT_BOOLEAN_TRUE, NULL, NULL);
    break;
  case TRANSACTION:
    updateDrvVal(tmp_integer, NULL, NULL);
    updateEffVal(tmp_integer, NULL, NULL);
    break;
  case LAST_VALUE:
    //    updateVal(leftValue());  // Should happen automagically
    break;
  default:
    cerr << "Yet to take care of initializing Implicit attribute: " << attribType << endl;
    break;
  }
}

void
SignalBase::resolveAndUpdate( VHDLKernel *process ){
  if( isCompositeType() ){
    for( int i = 0; i < length().getIntValue(); i++ ){
      ASSERT( dynamic_cast<SignalBase *>(&getField(i)) != 0 );
      dynamic_cast<SignalBase &>(getField(i)).resolveAndUpdate( process );
    }
  }
  else{
    if( getSource() != 0 ){
      process->updateSignal( this, true );
    }
  }
}

SignalBase *
SignalBase::locateSig( int sigId ){
  SignalBase *retval = 0;
  if( isCompositeType()){
    if (!idMap) {
      idMap = new map<int, SignalBase *>;
      addSigMap(idMap);
    }
    if (idMap->find(sigId) != idMap->end())
      retval = (*idMap)[sigId];
  }
  else if( id == sigId ){
    retval = this;
  }
  return retval;
}

void
SignalBase::addSigMap(map<int, SignalBase *> *idM) {
  if( isCompositeType()){
    int j = length().getIntValue();
    for( int i = 0; i < j; i++ ){
      ASSERT( dynamic_cast<SignalBase *>(&getField(i)) != 0 );
      dynamic_cast<SignalBase &>(getField(i)).addSigMap(idM);
    }
  }
  else {
    (*idM)[id] = this;
  }
}

SignalBase *
SignalBase::getParentSignal() const {
  return myParentSignal;
}

void
SignalBase::setParentSignal( SignalBase *newParent ){
  myParentSignal = newParent;
}

void
SignalBase::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "Signal id = " << id << "  Sensitive? ";
  if (sensitive)
    os << "Y";
  else
    os << "N";
  os << "  Resolution function: " << busResFn;
  ObjectBase::debugPrintDetails( os, indent);

  return;
}

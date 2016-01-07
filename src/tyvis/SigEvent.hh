#ifndef SIG_EVENT_HH
#define SIG_EVENT_HH

// Copyright (c) Clifton Labs, Inc.  All Rights Reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "tyvis/tyvis-config.h"
#include <DeserializerManager.h>
#include "tyvis/VHDLEvent.hh"
#include "tyvis/VHDLKernel.hh"
#include "tyvis/VCD.hh"

/** class for sending signals around */
class SigEvent: public VHDLEvent {
public:
  SigEvent( const VHDLVTime &initSendTime,
	    const VHDLVTime &initReceiveTime,
	    SimulationObject *initSender,
	    SimulationObject *initReceiver,
	    SigType initType,
	    int initSigId,
	    int initSigSrc,
	    const VHDLVTime &initRejTime,
	    const ArrayInfo &initSBound,
	    const ArrayInfo &initDBound,
	    int initSenderSigId,
	    unsigned int initSequenceNumber,
	    TransactionType initTransType,
	    const RValue &initData ) :
    VHDLEvent( initSendTime, initReceiveTime, initSender, initReceiver ),
    myType( initType ),
    sigId( initSigId ),
    sigSrc( initSigSrc ),
    rejTime( initRejTime ),
    sBound( initSBound ),
    dBound( initDBound ),
    senderSigId( initSenderSigId ),
    sequenceNumber( initSequenceNumber ),
    transactionType( initTransType ),
    myData( initData.clone() ){}

  SigEvent( const VHDLVTime &initSendTime,
	    const VHDLVTime &initReceiveTime,
	    SimulationObject *initSender,
	    SimulationObject *initReceiver,
	    const RValue &initData ) : 
    VHDLEvent( initSendTime, initReceiveTime, initSender, initReceiver ),
    myType( EXPLICIT ),
    sigId( -1 ),
    sigSrc( -1 ),
    rejTime( VHDLVTime::getVHDLVTimeZero() ),
    sBound( nullInfo() ),
    dBound( nullInfo() ),
    senderSigId( -1 ),
    sequenceNumber( 0 ),
    transactionType( NORMALTRANSACTION ),
    myData( initData.clone() ){}
  
  explicit SigEvent( const SigEvent &s ) : 
    VHDLEvent( s ),
    myType( s.myType ),
    sigId( s.sigId ),
    sigSrc( s.sigSrc ),
    rejTime( s.rejTime ),
    sBound( s.sBound ),
    dBound( s.dBound ),
    senderSigId( s.senderSigId ),
    sequenceNumber( s.sequenceNumber ),
    transactionType( s.transactionType ),
    myData( s.getData().clone() ){}
  
  ~SigEvent(){
    delete myData;
    myData = 0;
  }

  void serialize( SerializedInstance *si ) const {
    VHDLEvent::serialize( si );
    si->addInt( myType );
    si->addInt( sigId );
    si->addInt( sigSrc );
    si->addSerializable( &rejTime );
    si->addSerializable( &sBound );
    si->addSerializable( &dBound );
    si->addInt( senderSigId );
    si->addUnsigned( sequenceNumber );
    si->addInt( transactionType );
    si->addSerializable( dynamic_cast<const Serializable *>(myData) );
  }

  static Serializable *deserialize( SerializedInstance *si ){
    const VHDLVTime *sendTime = dynamic_cast<VHDLVTime *>(si->getSerializable());
    const VHDLVTime *recvTime = dynamic_cast<VHDLVTime *>(si->getSerializable());
    unsigned int senderSimManID = si->getUnsigned();
    unsigned int senderSimObjID = si->getUnsigned();
    unsigned int receiverSimManID = si->getUnsigned();
    unsigned int receiverSimObjID = si->getUnsigned();    
    const unsigned int eventId = si->getUnsigned();
    SigType type = SigType( si->getInt() );
    int sigId = si->getInt();
    int sigSrc = si->getInt();
    const VHDLVTime *rejTime =  dynamic_cast<VHDLVTime *>(si->getSerializable());
    const ArrayInfo *sBound = dynamic_cast<ArrayInfo *>(si->getSerializable());
    const ArrayInfo *dBound = dynamic_cast<ArrayInfo *>(si->getSerializable());
    int senderSigId = si->getInt();
    int sequenceNumber = si->getUnsigned();
    TransactionType transactionType = TransactionType( si->getInt() );
    const RValue *data = dynamic_cast<RValue *>(si->getSerializable());

    ObjectID sender(senderSimObjID, senderSimManID);
    ObjectID receiver(receiverSimObjID, receiverSimManID);
    
    return new SigEvent( *sendTime, 
			 *recvTime,
			 sender,
			 receiver,
			 eventId,
			 type,
			 sigId,
			 sigSrc,
			 *rejTime,
			 *sBound,
			 *dBound,
			 senderSigId,
			 sequenceNumber,
			 transactionType,
			 *data );    
  }

  int getSigId() const { return sigId; }
  void setDestSigId( int newDestSigId ){ sigId = newDestSigId; }
  int getSigSrc() const { return sigSrc; }
  void setSigSrc( int newSigSrc ){ sigSrc = newSigSrc; }

  VHDLVTime getRejTime() const { return rejTime; }
  ArrayInfo getSourceBounds() const { return sBound; }
  ArrayInfo getDestBounds() const { return dBound; } 
  int getSenderSigId() const { return senderSigId; }
  unsigned int getSequenceNumber(){ return sequenceNumber; }
  void setTransactionType( TransactionType newType ){ transactionType = newType; }

  bool isCancelTransactionEvent() const { 
    if( transactionType == CANCELTRANSACTION ){ 
      return true; 
    }
    else{ 
      ASSERT( transactionType == NORMALTRANSACTION ); 
      return false; 
    }
  }

  bool isSigEvent() const { return true; }
  
  bool isNullTransaction(){ 
    if( myType == NULL_TRANSACTION ){
      return true;
    }
    else{
      return false;
    }
 }

  void execute( VHDLKernel *onProcess ){
    if (sigId == -100) {
      cout << "DNS: in execute with signal " << sigId;
      cout << " null = " << isNullTransaction() << "  myType = " << myType << endl;
    }
    if( isNullTransaction() == false ){
      if (GlobalControl::getVCD()) {
        if (transactionType == NORMALTRANSACTION && myType == EXPLICIT && sigId >= 0)
        {
          GlobalControl::getVCD()->updateSignal( sigId, myData,
                    dynamic_cast<const VHDLVTime &>(getReceiveTime()) );
        }
      }
      onProcess->updateDriver( this );
    }
    else{
      onProcess->disconnectDriver( this );
    }
  }

  static const string &getSigEventType(){
    static string sigEventType = "SigEventType";
    return sigEventType;
  }

  const string &getDataType() const {
    return getSigEventType();
  }

  const RValue &getData() const {
    return *myData;
  }

  static void registerDeserializer(){
    DeserializerManager::instance()->registerDeserializer( getSigEventType(),
							   &SigEvent::deserialize );
  }

  SigEvent *clone() const {
    return new SigEvent( *this );
  }

  unsigned int getEventSize() const {
    return sizeof(SigEvent);
  }

protected:

private:
  /**
     Constructor used by deserializer.
  */
  SigEvent( const VHDLVTime &initSendTime,
	    const VHDLVTime &initReceiveTime,
	    const ObjectID &initSender,
	    const ObjectID &initReceiver,
	    const unsigned int eventId,
	    SigType initType,
	    int initSigId,
	    int initSigSrc,
	    const VHDLVTime &initRejTime,
	    const ArrayInfo &initSBound,
	    const ArrayInfo &initDBound,
	    int initSenderSigId,
	    unsigned int initSequenceNumber,
	    TransactionType initTransType,
	    const RValue &initData ) :
    VHDLEvent( initSendTime, initReceiveTime, initSender, initReceiver, eventId ),
    myType( initType ),
    sigId( initSigId ),
    sigSrc( initSigSrc ),
    rejTime( initRejTime ),
    sBound( initSBound ),
    dBound( initDBound ),
    senderSigId( initSenderSigId ),
    sequenceNumber( initSequenceNumber ),
    transactionType( initTransType ),
    myData( initData.clone() ){}


  SigType         myType;
  int             sigId;
  int             sigSrc;
  VHDLVTime       rejTime;
  ArrayInfo       sBound;
  ArrayInfo       dBound;
  int             senderSigId;
  unsigned        int sequenceNumber;
  TransactionType transactionType;
  const RValue  *myData;
};

inline ostream&
operator<<(ostream& os, const SigEvent& s) {
  os << static_cast<const Event &>(s)
     << "\ns.getSenderSigId(): " << s.getSenderSigId()
     << "\ns.getSigId(): " << s.getSigId() 
     << "\ns.getSigSrc(): " << s.getSigSrc() 
     << "\nrejTime: " << s.getRejTime() 
     << "\nbound: " << s.getSourceBounds()
     << "\ndbound: " << s.getDestBounds()
     << "\ndata: " << s.getData() << "\n";
  
  return os;
}

inline int
SigEventCompareRecv(const SigEvent* a, const SigEvent* b) {
  register int retVal;  
  if ( a->getReceiveTime() > b->getReceiveTime() ) {
    retVal = 1;
  }
  else if ( a->getReceiveTime() < b->getReceiveTime()) {
    retVal = -1;
  }
  else {
    retVal = 0;
  }
  return retVal;
}


#endif

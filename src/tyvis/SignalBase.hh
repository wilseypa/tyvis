#ifndef SIGNALBASE_HH
#define SIGNALBASE_HH

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

#include "tyvis/tyvis-config.h"
#include <vector>
#include "VHDLKernel.hh"
#include "DefaultAttributes.hh"
#include "TransactionList.hh"
#include "AdditionalDrivers.hh"
#include "FanOutList.hh"
#include <map>

using std::vector;
using std::map;

class Block;
class VHDLKernel;
class SourceBase;
class SigEvent;
class Transaction;
class SignalNetinfo;

class SignalBase : public ObjectBase {
public:
  /**  Default constructor. */
  SignalBase();

  SignalBase( const TypeInfo &initTypeInfo );


  /** Constructor */
  SignalBase( const TypeInfo &initTypeInfo,
	      const string &initName );

  /** Constructor */
  SignalBase( const TypeInfo &initTypeInfo,
	      const string &initName,
	      const RValue &initValue );


  /** Constructor */  
  SignalBase( const SignalBase& sb );
  
  virtual ~SignalBase();

//   /** fanout list size.  This variable holds the current number of entries in
//       the fanout list (fanDest). */
//   int getFanOut() const {
//     return fanDest.size();
//   }

  virtual bool isImplicitSignal() const { abort(); return false; }

  int getSigId() const { return id; }

  virtual SigType getType() const = 0;

  int getNumAttributes() const { return attributeList.size(); }
  vector<Attribute *> &getAttributeList(){ return attributeList; }
  DefaultAttributes &getAllAttributes(){ return allAttributes; }
  const DefaultAttributes &getAllAttributes() const { return allAttributes; }
  int getBusResFn() const { return busResFn; }
  bool isSensitive() const { return sensitive; }


  using ObjectBase::operator=;
  SignalBase& operator=(const SignalBase& sb);

//   /** Add VHDLKernel object to the fan out list.  Adds the given VHDLKernel
//       process object to the fan out list. Optionally may the signal identifier
//       be specified. */
//   SignalBase &addToFanOut( VHDLKernel * );

//   /** Remove VHDLKernel object to the fan out list.  Removes the given
//       VHDLKernel process object to the fan out list. Optionally may the signal
//       identifier be specified. */
//   SignalBase &Remove( VHDLKernel * );
//   SignalBase &Remove( VHDLKernel *, int sigid );
  
  virtual bool isResolved() const; 

  virtual bool is_signal() const { return true; }
  
//   void setBusKind() { type = G_BUS;  }
  
  virtual ArrayInfo* readArrayInfo();
  
  void updateVal( const RValue &newVal );

  virtual void updateDriver( const int, 
			     VHDLKernel *srcProcess,
			     const RValue *, 
			     const ArrayInfo *,
			     const ArrayInfo * ) = 0;

  virtual void disconnectDriver( VHDLKernel * ){ 
    cerr << getName() << " disconnectDriver(VHDLKernel *) ERROR!" << endl;
    abort();
  }
  
  virtual RValue *readDriver( VHDLKernel * ){ 
    cerr << getName() << " readDriver ERROR!" << endl;
    abort();
    return NULL;
  }
  
  virtual void updateDrvVal( const RValue &,
			     const ArrayInfo* = NULL,
			     const ArrayInfo* = NULL ){ 
    cerr << getName() << " updateDrvVal ERROR!" << endl;
    abort();
  }
  
  virtual RValue *readDrvVal() { 
    cerr << getName() << " readDrvVal ERROR!" << endl;
    abort();
    return NULL;
  }
  
  virtual void updateEffVal(const RValue &,
			    const ArrayInfo* = NULL,
			    const ArrayInfo* = NULL) { 
    cerr << getName() << "SignalBase::updateEffVal called - aborting!" << endl;
    abort();
  }
  
  virtual RValue &readEffVal() { 
    static UniversalInteger empty(0);
    cerr << getName() << " readEffVal ERROR!" << endl;
    abort();
    return empty;
  }
  
  virtual const VHDLVTime& readTimeVal() {
    cerr << getName() << " readTimeVal ERROR!" << endl;
    abort();
    // this is bogus, but it should never be called
    return VHDLVTime::getVHDLVTimePositiveInfinity();
  }
  
  virtual VHDLVTime &updateTimeVal( const VHDLVTime& v =
				    VHDLVTime::getVHDLVTimePositiveInfinity() ){
    cerr << getName() << " updateTimeVal ERROR!" << endl;
    abort();
    return const_cast<VHDLVTime &>(v);
  }

  virtual void setSource(SourceBase*) {
   cerr << "ERROR: SignalBase::setSource(SourceBase *) called" << endl;
   abort();
  } 

  void setElaborationInfo( const SignalNetinfo &obj );

  void setAttrib( AttribType typ, SignalBase &attrib );

  virtual bool isCompositeResolvedSignal() const {
    cerr << "ERROR: SignalBase::isCompositeResolvedSignal() called " << endl;
    return false;
  }

  virtual SignalBase *findSigInBlock( int sigId, VHDLKernel *srcId );
  virtual void setSourceBaseDeleteFlag(bool flag);

  virtual SourceBase *getSource();
  
  /** Assigning a value to a signal does not immediately change the value
      of the signal. Instead, only a transaction get's posted on a
      signal. Posting a transaction on a signal involves 2 main steps:

      (a) the transaction is conveyed to all processes that are sensitive to
      the signal

      (b) marking of the signal transactions is done.

      This method triggers/implements the above two steps:
      
      Post the signal transaction to all the processes that are sensitive
      to this signal i.e., to all the process entries in the fanout of this
      signal:

      Create a new transaction to be posted for this signal at the given
      transaction time... 
  */
  void assignSignal( VHDLKernel *srcId, 
		     const RValue &newVal,
		     const VHDLVTime &delay, 
		     const VHDLVTime &rejTime, 
		     const ArrayInfo &destInfo,
		     const ArrayInfo &srcInfo );
  bool matchId ( int mId );
  /**
     Called by TransactionList during marking.
  */
  void cancelTransaction( const Transaction * );

  void resolveAndUpdate(VHDLKernel*);

  SignalBase *locateSig( int sigId );
  void addSigMap(map<int, SignalBase *> *);

  /**
     If this is a subelement of a composite type, this call will return a
     pointer to the parent.
  */
  SignalBase *getParentSignal() const;
  void setParentSignal( SignalBase *newParent );
  
protected:
  SigEvent *generateEvent( VHDLKernel *sender,
			   const Transaction *transaction,
			   const VHDLVTime &delay, 
			   const VHDLVTime &rejTime,
			   const RValue    &sigValue, 
			   const ArrayInfo &destInfo,
			   const ArrayInfo &srcInfo );

  void initializeImplicitSignal( AttribType attribType );

  AdditionalDrivers &getAdditionalDrivers(){ return additionalDrivers; }

  virtual void debugPrintDetails( ostream &os, int indent ) const;
  
private:
  /**
     Our transaction list.
  */
  TransactionList transactionList;
  
  /** global signal ID */
  int id;            

  map<int, SignalBase *> *idMap;

  /**  does it belong to the processes sensitivity list? */
  bool sensitive;

  /** Index of busResFn for this signal */
  int busResFn;

  /** fanout list.  This contains the simulation object IDs that this
      signal gets transmitted to.  This data should never change during
      a simulation, so it is allocated only once, during initState, and
      deleted after the simulation is over.  The copy constructor and
      operator= simply copy the pointer to the new object. */
  FanOutList fanOut;
  vector<Attribute *> attributeList;
  DefaultAttributes allAttributes;

  Block *downTypeConversionFnListPtr;
  AdditionalDrivers additionalDrivers;

  unsigned int getNextSequenceNumber();
  unsigned int transactionSequenceNumber;

  SignalBase *myParentSignal;

  void cleanTransactionList( const VHDLVTime &beforeTime );

  /**
     Sends the transaction to everyone in our fanout list.
  */
  void sendTransactionToFanoutList( VHDLKernel *sourceProcess, 
				    const RValue       &newData,
				    const VHDLVTime    &delay, 
				    const VHDLVTime    &rejTime,
				    const ArrayInfo    &destInfo, 
				    const ArrayInfo    &srcInfo,
				    const Transaction *newTransaction );

  void compositeAssignSignal( VHDLKernel *srcId, 
			      const RValue &newVal,
			      const VHDLVTime &delay, 
			      const VHDLVTime &rejTime, 
			      const ArrayInfo &destInfo,
			      const ArrayInfo &srcInfo );
};

#endif

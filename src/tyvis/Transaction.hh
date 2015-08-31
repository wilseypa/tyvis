#ifndef TRANSACTION_HH
#define TRANSACTION_HH

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
#include <new>
#include "tyvis/VHDLData.hh"
#include "tyvis/VHDLVTime.hh"
class VHDLKernel;

class Transaction {
  friend class SignalBase;
public:
  Transaction(const Transaction &);
  ~Transaction(){ delete value; }
  
  Transaction& operator==(const Transaction &) const;

  VHDLKernel *getSourceProcess() const { return sourceProcess; }
  const VHDLVTime &getTime() const { return transactionTime; }
  const unsigned int getSequenceNumber() const { return sequenceNumber; }
  const VHDLData *getValue() const { return value; }

  void *operator new( size_t size );
  void operator delete( void *toDelete );

protected:
  
private:
  VHDLKernel         *sourceProcess;
  const VHDLData     *value;
  const VHDLVTime    transactionTime;
  const unsigned int sequenceNumber;

  /** To create a transaction, ask the SignalBase for a new transaction
      entry by calling createNewTransaction() method... */
  Transaction( VHDLKernel *sourceProcess,
	       const VHDLVTime &,
	       const unsigned int,
	       const VHDLData * );

  static int getNextNumber();
};

#endif

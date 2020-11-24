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

#include "Transaction.hh"
#include <AllocatorStack.h>

Transaction::Transaction(const Transaction &src) :
  sourceProcess( src.getSourceProcess() ),
  value(src.getValue()),  
  transactionTime(src.getTime()),
  sequenceNumber(src.getSequenceNumber()){}


Transaction::Transaction( VHDLKernel *initSourceProcess,
			  const VHDLVTime &time, 
			  const unsigned int sNo,
			  const VHDLData *val) :
  sourceProcess( initSourceProcess ),
  value(val),  
  transactionTime(time),
  sequenceNumber(sNo){}

void *
Transaction::operator new( size_t ){
  return AllocatorStack<sizeof(Transaction)>::pop(); 
}
void 
Transaction::operator delete( void *toDelete ){
  AllocatorStack<sizeof(Transaction)>::push(toDelete); 
}


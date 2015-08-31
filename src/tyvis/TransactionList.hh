#ifndef TRANSACTION_LIST_HH
#define TRANSACTION_LIST_HH

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
using std::vector;

#include "tyvis/Transaction.hh"
class VHDLVTime;

class TransactionList {
public:
  TransactionList( SignalBase &initSignal );
  virtual ~TransactionList();

  bool empty() const;

  /**
     This method does housekeeping on our transaction list.  We can throw
     away transactions that have occured before our current simulation
     time.
  */
  void clean( const VHDLVTime &eraseBefore );
 
 /** Marking is explained in VHDL'93 LRM in section 8.4.1.  */
 void doMarking( const VHDLVTime &transactionTime,
		 Transaction *newTransaction,
		 const VHDLVTime &rejectTime );
  
  /**
     Destroys the current transaction list, and copies passed in list.
  */
  void copy( const TransactionList &source );
  

private:
  /**
     Returns the transactions.
  */
  vector<Transaction *> &getTransactions(){ return myTransactions; }

  vector<Transaction *> myTransactions;
  SignalBase &mySignal;
};

#endif

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

#include "TransactionList.hh"
#include "SignalBase.hh"

TransactionList::TransactionList( SignalBase &initSignal ) 
  : mySignal( initSignal ){}

TransactionList::~TransactionList(){}

bool
TransactionList::empty() const {
  return myTransactions.empty();
}

class deleteIfEarlier : public std::unary_function<Transaction *, bool> {
public:
  deleteIfEarlier( const VHDLVTime &initTime ) : currentTime( initTime ){}

  bool operator()( Transaction *t ) const {
    if( t && t->getTime() < currentTime ){
      delete t;
      return true;
    }
    else{
      return false;
    }
  }
  
private:
  const VHDLVTime &currentTime;
};

void
TransactionList::clean( const VHDLVTime &eraseBefore ){
  vector<Transaction *>::iterator newEnd =
    remove_if( myTransactions.begin(),
	       myTransactions.end(),
	       deleteIfEarlier( eraseBefore ) );
  vector<Transaction *>::iterator e_i, e_end;
  e_i = newEnd;
  e_end = myTransactions.end();
  if (e_i != e_end) {
    ++e_i;
    for ( ; e_i != e_end ; ++e_i) {
      delete *e_i;
    }
  }
  myTransactions.erase( newEnd, myTransactions.end() );  
}

void
TransactionList::copy( const TransactionList &source ){
  clean( VHDLVTime::getVHDLVTimePositiveInfinity() );
  for( vector<Transaction *>::iterator i = 
	 const_cast<TransactionList &>(source).getTransactions().begin();
       i < const_cast<TransactionList &>(source).getTransactions().end();
       i++ ){
    myTransactions.push_back( new Transaction( *(*i) ) );
  }
}

void
TransactionList::doMarking( const VHDLVTime &transactionTime,
			    Transaction *newTransaction,
			    const VHDLVTime &rejectTime ){
  // Step 1: All old transactions that are projected to occur at or after the
  // time at  which the earliest transaction is projected to occur are
  // deleted from the projected output waveform:
  
  // Locate the first transaction that is projected to occur after this
  // transaction.. This is done by walking the list of transactions
  // backwards and comparing the times on each transaction with the time
  // at which the current transaction is scheduled.  If the transaction
  // time is greater than or.operator== to the transaction time, it is
  // cancelled.
  vector<Transaction *>::iterator earliestToCancel = myTransactions.end();
  for( vector<Transaction *>::iterator i = myTransactions.end() - 1;
       i >= myTransactions.begin();
       i-- ){
    if( ((*i)->getTime()) >= transactionTime ){
      mySignal.cancelTransaction( *i );
      earliestToCancel = i;
    }
    else {
      break;
    }
  }
    
  // At the end of the above while loop, the iterator "oldTransaction" is
  // either pointing to the end of the myTransactions or to the first
  // transaction whose (receive) time is less than the new Transaction's time
  // .ie. all transactions after oldTransaction have been cancelled and
  // removed from the transaction list.
  
  vector<Transaction *>::iterator e_i, e_end;
  e_i = earliestToCancel;
  e_end = myTransactions.end();
  if (e_i != e_end) {
    ++e_i;
    for ( ; e_i != e_end ; ++e_i) {
      delete *e_i;
    }
  }
  myTransactions.erase( earliestToCancel, myTransactions.end() );
  // Now we append the new transaction to the end of the transaction list.
  myTransactions.push_back(newTransaction);
  // The next few steps of marking are performed for signal assignments with
  // inertial delays.  Inertial signal assignments will have rejection time >
  // ZERO.  This fact is utilized to identify inertial signal assignments.

  if( rejectTime > VHDLVTime::getVHDLVTimeZero() ){
    vector<bool> markedFlags( myTransactions.size(), false );
    // Perform first & second step of marking for inertial delays; i.e., All
    // new transactions are marked, as well as those old transactions that
    // occur at less than the time of the first new waveform less the rejection
    // limit. Each unmarked transaction is marked if it immediately preceds a
    // marked transaction and has the same value as the marked transaction.

    // Note: Two steps of marking have been combined to eliminate another
    //       iteration through the transaction list (an optimization?).

    *(markedFlags.end() - 1) = true;  // Mark the newly appended transaction

    VHDLVTime markTime( newTransaction->getTime() - rejectTime );
    bool firstTrFlag = true;
    vector<bool>::iterator markIterator     = markedFlags.begin();
    vector<Transaction *>::iterator transaction = myTransactions.begin();
    while( markIterator != markedFlags.end() ){
      if ((*transaction)->getTime() < markTime ){
	(*markIterator) = true;

	// Check if the values of this transaction and the preceeding
	// transaction are the same. If so then mark the pervious transaction.
	if ( firstTrFlag == false &&
	     ((*transaction)->getValue() == (*(transaction - 1))->getValue())){
	  *(markIterator - 1) = true;
	}
      }
      // else  {
      //   break; // This is an optimization that can be done to save a few
      //          // iterations in the marking process since the myTransactions
      //          // is sorted -- once we hit a transaction whose time is greater
      //          // than "markTime" then all succeeding transactions will have time
      //          // greater than "markTime"
      markIterator++;
      transaction++;
      firstTrFlag = false; // We are no longer dealing with the first transaction.
    }

    // The last phase of marking -- The transaction that determines the current
    // value of the driver (.ie. the first transaction in the list) is marked,
    // and all unmarked transactions are then deleted.
    
    markIterator    = markedFlags.begin();
    transaction     = myTransactions.begin();

    // The transaction that determines the current driving value is marked!
    if ((*transaction)->getTime() == (*transaction)->getSourceProcess()->getTimeNow()) {
      (*markIterator) = true;
    }

    vector<Transaction *> newTransactionList;
    
    while (markIterator != markedFlags.end()) {
      if ((*markIterator) == false) {
	// Unmarked transaction..have to remove it from the list
	mySignal.cancelTransaction( *transaction );
        delete *transaction;
      }
      else {
	newTransactionList.push_back(*transaction);
      }
      
      markIterator++;
      transaction++;
    }

    // Now swap the transactionlist and newTransactionList; thereby get rid of
    // the unwanted transactions!
    myTransactions.swap(newTransactionList);
  }
}

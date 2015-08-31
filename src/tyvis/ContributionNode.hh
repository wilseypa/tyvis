#ifndef CONTRIBUTION_NODE_HH
#define CONTRIBUTION_NODE_HH

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
#include "tyvis/VHDLType.hh"
#include "tyvis/Terminal.hh"
#include "tyvis/Quantity.hh"
#include "tyvis/QuantityNode.hh"

class quantityNode;

/** This class describes a Node for terminal. Each terminal has through
    Quantities associated with it. All the through Quantities associated
    with a terminal are collected in a list of quantityNodes.
    Contributions for each of these Quantities are set depending on whether
    the current Terminal is a positive Terminal or a negative Terminal for
    the Quantity. */
class contributionNode {

public:

  /** Default constructor. */
  contributionNode();

  /** Default destructor. */
  ~contributionNode();

  /** Sets the Terminal for this contribution node.

      @param term terminal to be set. */
  void setTerminal(const Terminal& term);

  /** Returns ptrToQuantityNode. */
  quantityNode* getNode();

  /** Returns the Terminal to this contribution Node. */
  Terminal getTerminal();

  /** Returns the number of quantities associated with this node. */
  int getNoOfQtys();

  /** Adds a Quantity (associated with the terminal) to the list of quantityNodes.

      @param qty the quantity to be added. */
  void insertQty(VHDLType *qty);

  /** Helps traverse the list of quantityNodes in a contribution Node. Used
      for debugging purposes only. */
  void traverse();

  /** A function which sets the contribution for every quantityNode in the
      contribution Node. Essentially sets the contribution of each Quantity
      associated with the Terminal. */
  void setContribution();

protected:

private:

  /// Member which holds the Terminal. 
  Terminal terminal;

  /** Points to the first quantityNode in the list of quantity Nodes for
      the Terminal. */
  quantityNode *ptrToQuantityNode;

  /// Keeps count of number of Quantities associated with this Terminal. 
  int noOfQtys;

};
#endif


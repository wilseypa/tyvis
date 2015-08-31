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

#include <clutils/Debug.h>
#include "ContributionNode.hh"

using clutils::debug;

contributionNode::contributionNode() {
  ptrToQuantityNode = NULL;
  noOfQtys = 0;
}

contributionNode::~contributionNode(){}

void contributionNode::setTerminal(const Terminal &term) {
  terminal = term;
}

Terminal contributionNode::getTerminal() {
  return terminal;
}

int contributionNode::getNoOfQtys() {
  return noOfQtys;
}

void contributionNode::insertQty(VHDLType *qty) {
  if (ptrToQuantityNode == NULL) {
    ptrToQuantityNode = new quantityNode;
    ptrToQuantityNode->setQuantity(qty);
  }
  else {
    quantityNode *temp = new quantityNode;
    temp->setNext(ptrToQuantityNode);
    temp->setQuantity(qty);
    ptrToQuantityNode = temp;
  }
  noOfQtys++;
}

void contributionNode::setContribution() {
  debug << "-->Inside setContribution. " << endl;
  quantityNode *temp;
  temp = ptrToQuantityNode;
  if (temp == NULL) {
    // There is no through quantity associated with terminal.
    debug << "Warning/Check: There is no through quantity associated with terminal." << endl;
    terminal.print(debug);
  }
  else {
    while(temp != NULL) {
      // Setting Contribution for the Quantity.
      if (((((Quantity *)(temp->getQuantity())->getObject())->getPosTerminal())->getTerminalId()) 
	  == terminal.getTerminalId()) {
	temp->setContribution(1);
      }
      else {
	if (((((Quantity*)(temp->getQuantity())->getObject())->getNegTerminal())->getTerminalId())
	    == terminal.getTerminalId()) {
	  temp->setContribution(-1);
	}
      }
      temp = temp->getNext();
    }
  }
}

quantityNode* contributionNode::getNode() {
  return ptrToQuantityNode;
}
  
void contributionNode::traverse() {
  quantityNode *temp;  
  temp = ptrToQuantityNode;
  debug << "Terminal id: " << terminal.getTerminalId();
  if (temp == NULL) {
     debug << " There are no through quantities associated with this terminal" << endl;
  }
  else {
    while (temp != NULL) {
      debug << "Quantity ";
      ((Quantity*) (temp->getQuantity())->getObject())->print(debug);
      debug << "with contribution " << temp->getContribution() << endl;
      temp = temp->getNext();
    }
  }
} 

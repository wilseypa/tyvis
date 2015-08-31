#ifndef NODELIST_CC
#define NODELIST_CC

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

#include "NodeList.hh"
#include <iostream>

using std::endl;

nodeList::nodeList(void) {
  numEqu = 1;
  tailNode = new Node(0,EFFORT,0);
  headNode = tailNode;
}

nodeList::~nodeList(void) {
  Node* temp;
  temp = headNode;
  while (temp) {
    temp = headNode->next;
    delete headNode;
    headNode = temp;
  }
}

int nodeList::addNode(int nodeId, nodeConstants type) {
  if (tailNode) {
    tailNode->next = new Node(nodeId, type, numEqu);
    tailNode = tailNode->next;
  }
  else {
    tailNode = new Node(nodeId, type, numEqu);
    headNode = tailNode;
  }
  return numEqu++;
}

int 
nodeList::addNodeCond(int nodeId, nodeConstants type) {
  Node* tmp=headNode;
  
  if (!headNode) {
    return addNode(nodeId, type);
  }
  while (tmp && tmp->nodeId != nodeId) {
    tmp = tmp->next;
  }
  if (!tmp) {
    return addNode(nodeId, type);
  }
  return tmp->equNum;
}

int 
nodeList::find(int nodeId) {
  Node* tmp=headNode;

  while (tmp && tmp->nodeId != nodeId) {
    tmp = tmp->next;
  }
  if (!tmp) {
    cout << "nodeList: node not found" << endl;
    return NONODE;
  }
  return tmp->equNum;
}

int
nodeList::getSize(void) { 
  // number of nodes in the system
  return(numEqu - 1);
}

void
nodeList::display(void) {
  Node *tmp = headNode;
  cout << "Global index\tInternal index\tType" << endl;
  while (tmp != NULL) {
    if (tmp->nodeId !=0 ){
      cout << tmp->nodeId << "\t\t" << tmp->equNum << "\t\t";
      if (tmp->type == EFFORT) {
	cout << "VOLTAGE/EFFORT at node " << tmp->getName() << endl;
      }
      else {
	if (tmp->getName() != NULL) {
	  cout << "CURRENT/FLOW " <<tmp->getName()<< endl;
	}
	else {
          cout << "CURRENT/FLOW " <<"No name"<<endl;
	}
      }
    }
    tmp = tmp->next;
  }
}

int 
nodeList::addNodeCond(int nodeId, nodeConstants type, char* name_n) {
  Node* tmp=headNode;
  
  if (!headNode) {
    return addNode(nodeId, type,  name_n);
  }
  while (tmp && tmp->nodeId != nodeId) {
    tmp = tmp->next;
  }
  if (!tmp) {
    return addNode(nodeId, type, name_n);
  }
  return tmp->equNum;
}

int nodeList::addNode(int nodeId, nodeConstants type, char *name_n) {
  if (tailNode) {
    tailNode->next = new Node(nodeId, type, numEqu, name_n);
    tailNode = tailNode->next;
  }
  else  {
    tailNode = new Node(nodeId, type, numEqu, name_n);
    headNode = tailNode;
  }
  return numEqu++;
}

#endif

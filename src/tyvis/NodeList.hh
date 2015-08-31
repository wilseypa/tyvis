#ifndef NODELIST_HH
#define NODELIST_HH

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
#include "tyvis/Node.hh"
#include <iostream>

using std::cout;

class Node;

/** This class describes a list of nodes and specific function which
    operates on the list. */
class nodeList {

public:
  /** Number of nodes in the system. */
  int numEqu;         

  /** Constructor for the node class. */
  nodeList(void);
  
  /** Destructor for the node class. It removes the entire list. */
  ~nodeList(void);
  
  /** Get the head node from the list. */
  const Node* getHeadNode(void) { return headNode; };
  
  /** Add a node to the node list.

      @return The node number for a future node in the system.  To get the
       number of nodes in the system subtract 1 from this number.
	
      @param nodeId a user defined id.

      @param type type of the node(EFFORT or FLOW). */
  int addNode(int nodeId, nodeConstants type);
  

  /** Add a node to the list if it is not already present.

      @return The current position of the node in the rhs list.

      @param nodeId a used defined id.

      @param type type of the node(EFFORT or FLOW). */
  int addNodeCond(int nodeId, nodeConstants type);

  /** Add a node to the node list.

      @return The current position of the node in the rhs list.

      @param nodeId a user defined id

      @param type type of the node (EFFORT or FLOW).

      @param name_n name of the node. */
  int addNode(int nodeId, nodeConstants type, char *name_n);
  
  /** Add node to the list if it is not already present

      @return The current position of the node in the rhs list.

      @param nodeId a user defined id.
       
      @param type type of the node (EFFORT or FLOW).
    
      @param name_n name of the node. */
  int addNodeCond(int nodeId, nodeConstants type, char *name_n);

  /** This function looks up in the list for the exact node number.

      @return If the id name is found it returns the equation number.  If
       not found it returns the nodeList::NONODE constant.

      @param name id which needs to be looked up in the list. */
  int find(int nodeId);

  /** Returns the number of nodes in the system. */
  int getSize(void);
  
  /** A function useful for debuggin purposes.  It is used to display the
      terminal mapping in the matrix created for a particular analog
      island.  For every matrix that is created in the system, the mapped
      numbers always start from zero and have as many nodes as needed. */
  void display(void);

protected:

private:
  /// The head of the list.
  Node*  headNode;

  /// The last element in the list.
  Node*  tailNode;

};
#endif

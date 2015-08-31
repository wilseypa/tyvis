#ifndef NODE_HH
#define NODE_HH

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
#include <iostream>
#include "tyvis/Constants.hh"

/** This class helps to describe the RHS vector. */
class Node {
public:

  /** Constructor 1 

      @param nodeName unique user defined id.
      @param nodeType node type Effort or Flow.
      @param nodeEquNum unique equation number. */
  Node(int nodeName, nodeConstants nodeType, int nodeEquNum){
    next = NULL;
    type = nodeType;
    nodeId = nodeName;
    equNum = nodeEquNum;
    name_n = NULL;
  }

  /** Constructor 2 with name passed

      @param nodeName unique user defined id.
      @param nodeType node type Effort or Flow.
      @param nodeEquNum unique equation number.
      @param name_s string containing node name. */ 
  Node(int nodeName, nodeConstants nodeType, int nodeEquNum, char *name_s){
    next = NULL;
    type = nodeType;
    nodeId = nodeName;
    equNum = nodeEquNum;
    name_n = name_s;
  }

  /** Returns the pointer to the next node. */
  const Node* Next() const {
    if(next){
      return next;
    }
    return NULL;
  }

  /** Returns the type of the node. */
  const nodeConstants& Type() const {
    return type;
  }

  /** Sets the name of the current Node. 

      @param name_s character pointer to the name of the component. */
  void setName(char* name_s) {
      name_n = name_s;
  }
 
  /** returns the name of the Node. */
  char* getName() {
   return name_n;
  }
 
protected:

private:
  /// Pointer to the next node.
  Node* next;

  /// Type of the node.
  nodeConstants   type;

  /// unique number assosiated with each node.
  int   nodeId;

  /// Equation number.
  int   equNum;

  /// name of the node.
  char *name_n;
  
  friend class nodeList;
};

#endif

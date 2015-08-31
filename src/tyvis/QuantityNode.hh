#ifndef QUANTITY_NODE_HH
#define QUANTITY_NODE_HH

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
#include "tyvis/Quantity.hh"

/** This class is meant to be a node(cell) for a Quantity. It defines the
    functions needed to deal with the contribution of a Quantity. */
class quantityNode {

public:
  /** Default constructor. */
  quantityNode();

  /** Default destructor. */
  ~quantityNode();

  /** Assigns the Quantity for the node.

      @param q the quantity to be assigned. */
  void setQuantity(VHDLType *q);

  /** Assigns the contribution of the particular Quantity(node).

      @param con the contribution value. */
  void setContribution(int con);

  /** Sets the next member to the node passed.

      @param ptr pointer to the next quantityNode */
  void setNext(quantityNode *ptr);

  /** Returns next. */
  quantityNode* getNext();

  /** Returns the quantity associated with this node. */
  VHDLType* getQuantity();

  /** Returns the contribution of this node(Quantity). */
  int getContribution();

protected:

private:

  /// Member which holds the Quantity for this node.
  VHDLType *qty;

  /// This member is initialized to 0 and takes values +1 and -1.
  int contribution;

  /** Points to the next member in the list of Quantity nodes for a
      particular terminal. */
  quantityNode *next;

};

#endif

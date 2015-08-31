#ifndef QUANTITY_CC
#define QUANTITY_CC

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

#include "Quantity.hh"

Quantity::Quantity() { 
inTheSet = false;
}

Quantity::~Quantity() { }

Quantity::Quantity(char *name, quantityTypes qType, Terminal *pos, Terminal *neg, double val) {
  quantityType = qType;
  quantityName = name;
  setPosTerminal(pos);
  setNegTerminal(neg);
  value.val = val ;
  sourceCurrent = false;
  used = 0;
  inTheSet = false;
}

Quantity::Quantity(char *name, quantityTypes qType, double reltol, double abstol, Terminal *pos ,Terminal *neg,double val) {
  quantityType = qType;
  quantityName = name;
  setPosTerminal(pos);
  setNegTerminal(neg);
  value.val = val ;
  sourceCurrent = false;
  used = 0;
  inTheSet = false;
}

bool
Quantity::isLoaded() {
  return loadedInMatrix;
}
        
void
Quantity::setLoadedFlag(bool toSet) {
  loadedInMatrix = toSet;
}

void 
Quantity::setPosTerminal(Terminal *pos) {
  posTerminal = pos;
}

void 
Quantity::setNegTerminal(Terminal *neg) {
  negTerminal = neg;
}

Terminal* 
Quantity::getPosTerminal() const {
  return posTerminal;
}

Terminal* 
Quantity::getNegTerminal() const {
  return negTerminal;
}

enum ObjectBase::ObjectType
Quantity::getKind() const {
  return QUANTITY ;  
}
 
quantityTypes
Quantity::getType() const {
  return quantityType;
}

void 
Quantity::setName(char *name) {
   quantityName = new char [strlen(name)];
   strcpy(quantityName,name) ;
}

void 
Quantity::setUsed(void) {
   used = 1;
   if (posTerminal!=NULL) {
    posTerminal->setUsed();
   }
   if (negTerminal!=NULL) {
    negTerminal->setUsed();
   }   
}

void 
Quantity::setIndex(int num) {
  index=num;
}

int
Quantity::getIndex() {
  if ((index == 0) || (index == -1)) {
    cerr << quantityName << " index incorrect -> " << index << endl;
    cerr << "Positive Terminal: " << getPosNode()
         << " Negative Terminal: " << getNegNode() ;
    cerr << " Type: " <<  quantityType << endl;
    cerr << "Aborting further processing..." << endl;
    abort();
  }  
  return index;
}

void 
Quantity::updateVal(const VHDLData& val) {}

const VHDLData& 
Quantity::readVal() const {
  return value;
}

ObjectBase* 
Quantity::clone() const {
  Quantity *retval = new Quantity();
  *retval = *this;
  return ((ObjectBase *) retval);
}

int 
Quantity::getPosNode() {
  Terminal *term;
  term = getPosTerminal();
  if (term) {
    return term->getTerminalId();
  }
  else {
    cerr << "Positive terminal does not exist for " << quantityName << endl;  
    abort();
    return 0;
  }
}	

int 
Quantity::getNegNode() {
  Terminal *term;
  term = getNegTerminal();
  if (term) {
    return term->getTerminalId();
  }
  else {
    cerr << "Negative terminal does not exist for " << quantityName << endl;
    abort();
    return 0;
  }	
}	

void
Quantity::print(ostream &os) const {
  os << "Name : " << quantityName << endl;
  if (posTerminal) {
   os << "--Positive Terminal " << posTerminal->getTerminalId() << " " << posTerminal->getTerminalName() << endl;
   os << "---Positive ";
   posTerminal->print(os) ;
  }
  else {
    os<<"--Positive Terminal not defined for this quantity " << endl;
  }
  if (negTerminal) {
    os << "--Negative Terminal " << negTerminal->getTerminalId() << " " << negTerminal->getTerminalName() << endl;
    os << "---Negative ";
    negTerminal->print(os);
    os << endl;
  }
  else {
   os << "--Negative Terminal not defined for this quantity " << endl;
  }
}

double
Quantity::getValue (AMSKernel *ckt)  {
  double retval = 0.0;
  double *oldRhs = ckt->getOldRhs();
  double temp = presentVal;
  if (get_quantity_type() == ACROSS) {
    int pos = ckt->findNodeRHS(getPosNode());
    int neg = ckt->findNodeRHS(getNegNode());
    posNodeVal = oldRhs[pos];
    negNodeVal = oldRhs[neg];
    retval = value.val=oldRhs[pos] - oldRhs[neg];
  }
  else {
    int brEq = ckt->findNodeRHS(index);
    retval = value.val= oldRhs[brEq];
  }
  presentVal = retval;
  if(temp != presentVal) {
    previousVal = temp ;
  }
  return retval;
}

#endif

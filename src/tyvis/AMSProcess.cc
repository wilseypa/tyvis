#ifndef AMSPROCESS_CC
#define AMSPROCESS_CC

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

#include <iostream>  
#include <list>
#include <sstream>
#include <string>
#include <cstring>
#include "AMSProcess.hh"
#include "SignalNetinfo.hh"
#include "Signal.hh"
#include "NodeList.hh"
#include "Component.hh"

using std::cout;
using std::endl;
using std::ends;

extern list<VHDLType *> globalAcrossQuantityList;

AMSProcess::AMSProcess(char *island_name){
  name = island_name;
  outputfile.open("results");
}

AMSProcess::~AMSProcess() {
}

AMSKernel_state * 
AMSProcess::allocateState() {
  AMSKernel_state *p = new AMSKernel_state();
  return p;
}

void
AMSProcess::initialize() {
   executeVHDL();
}

void
AMSProcess::finalize() {
  getSolverState()->cleanState();
  outputfile.close();
}

void
AMSProcess::setup() {
  int numberOfEquations = 0;
  component *index = getComp();  

  list<Quantity *> *islandQuantities = getSetOfQuantities();

  cout << "Processing equations in " << name << endl;     
  while( index != NULL) {  
    ASSERT(index!=NULL);
    index->init();
    numberOfEquations++;
    // Build the set of all the quantities in this island.
    index->buildQuantitySet(islandQuantities);
    index = index->getNext();
  } // While there are equations to be processed.

  list<VHDLType *>::iterator acrossIterator;
  for(acrossIterator = globalAcrossQuantityList.begin();
      acrossIterator != globalAcrossQuantityList.end();
      acrossIterator++) {
    ASSERT( (*acrossIterator)->getObject()->getKind() == ObjectBase::QUANTITY);
    if ((((Quantity *)(*acrossIterator)->getObject())->getPosTerminal()->getUsed()==1)&&
	(((Quantity *)(*acrossIterator)->getObject())->getNegTerminal()->getUsed()==1)) {
      if (((Quantity *)(*acrossIterator)->getObject())->getInTheSet() == false) {
	islandQuantities->push_back( ((Quantity *)(*acrossIterator)->getObject()) );
      }
      cout << "ADD QTY TO LIST" << ((Quantity *)(*acrossIterator)->getObject())->getName() << endl;
    }
  }
  ostringstream str;
  str << "Time  ";
  list<Quantity *>::iterator currentQuantity;
  for (currentQuantity = islandQuantities->begin(); currentQuantity != islandQuantities->end();currentQuantity++) {
    if ((*currentQuantity)->getType()!=IMPLICIT) {
      int length = strlen((*currentQuantity)->getName());
      char *outputQuantityName = new char [length];
      strcpy(outputQuantityName,(*currentQuantity)->getName());
      str << outputQuantityName;
      str << "  ";
      delete [] outputQuantityName;
    }
  }
  string strValue = str.str();
  outputfile<<strValue<<endl; 
}

void
AMSProcess::printFunction() {
  list<Quantity *> *islandQuantities = getSetOfQuantities();
  if (islandQuantities == NULL) {
    cout << "Fatal Error: No qunatities in this island. Aborting ...." << endl;
    abort();
  }  
  ostringstream str;
  str << getSolverState()->getCurTime()*1e-15;
  list<Quantity *>::iterator currentQuantity;
  for (currentQuantity = islandQuantities->begin(); currentQuantity != islandQuantities->end();currentQuantity++) {
    if ((*currentQuantity)->getType()!=IMPLICIT) {
      str << "  ";
      str.setf(ios::scientific, ios::floatfield);
      str << (*currentQuantity)->getValue(((AMSKernel*)this));
    }
  }  
  string strValue = str.str();
  outputfile<<strValue<<endl;
}  


void
AMSProcess::executeProcess(){
  simulate(100000000);
}

#endif



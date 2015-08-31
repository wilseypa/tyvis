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
#include <vector>
#include "AMSApplication.hh"
#include "Hierarchy.hh"

using std::cout;
using std::endl;
using std::vector;

class _savant_entity_elab;
class VHDLApplication;

extern Hierarchy * hier;

AMSApplication::AMSApplication( _savant_entity_elab *initDesign) : VHDLApplication(initDesign) {
  setAMSDesign(initDesign);
  ASSERT(initDesign != 0); 
}

void 
AMSApplication::setAMSDesign(_savant_entity_elab *toSet) {
  AMSDesign = static_cast<_savant_entity_ams_elab *>(toSet);
}

int
AMSApplication::initialize(vector<string> &parameters) {
  hier = new Hierarchy;
  AMSDesign->instantiate(hier);
  AMSDesign->connect(0, 0);
  cout << "Elaborating AMS constructs...." << endl;
  // Putting code in for the elaboration of AMS elements in the design.
  AMSDesign->connectTerminals(0);
  AMSDesign->assignIndex();
  AMSDesign->formCharacteristicExpressions();
  AMSDesign->formStepLimitSpecs();
  AMSDesign->printBasicSet();
  AMSDesign->createAMSProcess();
  cout << "Elaboration of AMS Constructs completed." << endl;
  return 0;
}


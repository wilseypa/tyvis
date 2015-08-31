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

#include "PortMap.hh"
#include "PortAssociation.hh"
#include "_savant_entity_elab.hh"

PortMap::PortMap() {}

PortMap::~PortMap() {
  PortAssociation *currentAssociation = portAssociationList.getElement();
  while( currentAssociation != 0 ){
    delete currentAssociation;
    currentAssociation = portAssociationList.getNextElement();
  }
}

void
PortMap::addPortAssociation( SignalNetinfo *formal, SignalNetinfo *actual, PortType modeType) {
  portAssociationList.add( new PortAssociation(actual, formal, modeType) );
}

const PortAssociation *
PortMap::getFirstAssociation(){
  return portAssociationList.getElement();
}

const PortAssociation *
PortMap::getNextAssociation(){
  return portAssociationList.getNextElement();
}


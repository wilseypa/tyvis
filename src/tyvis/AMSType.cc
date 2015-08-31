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

#include <list>
#include "AMSType.hh"
#include "vhdl_ams.hh"
#include "Terminal.hh"
#include "Quantity.hh"

using std::list;

extern list<VHDLType *> globalImplicitQuantityList;
extern list<VHDLType *> globalThroughQuantityList;
extern list<VHDLType *> globalAcrossQuantityList;

// Need to check if TypeInfo is required as the last parameter.
// In total 4 cases exist for quantities....
// Constructors for Quantities with tolerances.
// IMPLICIT quantities would have reltol and abstol fields set to 0. they are
// used neways.
// FREE/ACROSS/THROUGH quantities would have default/assigned tolerance values
// passed.

AMSType::AMSType(ObjectBase::ObjectType objType, char * name,
		 double reltol, double abstol, quantityTypes qType, VHDLType* pos, VHDLType* neg) {
  ASSERT(objType == ObjectBase::QUANTITY);
  switch ( qType ) {
  case IMPLICIT:
    setObject( new Quantity(name,qType,NULL,NULL) );
    globalImplicitQuantityList.push_back(this) ;
    break;
  case FREE:
    setObject( new Quantity(name,qType,reltol,abstol,NULL,NULL) );
    break;
  case THROUGH:
    if (!pos && !neg) {
      setObject( new Quantity(name,qType,reltol,abstol,NULL,NULL) );
    }
    else if (pos && neg) {
	setObject( new Quantity(name,qType,reltol,abstol,(Terminal *)(pos->getObject()),(Terminal *)(neg->getObject())) );
    }
    else {
      cerr << " Terminal associations for quantity incomplete" << endl;
    }
    globalThroughQuantityList.push_back(this);
    break;
  case ACROSS:
    if (!pos && !neg) {
      setObject( new Quantity(name,qType,reltol,abstol,NULL,NULL) );
    }
    else if (pos && neg) {
	setObject( new Quantity(name,qType,reltol,abstol,(Terminal *)(pos->getObject()),(Terminal *)(neg->getObject())) );
    }
    else {
	cerr << " Terminal associations for quantity incomplete" << endl;
    }
    globalAcrossQuantityList.push_back(this);
    break;
  default:
    cerr << "ERROR : Unknown Quantity type" << endl;
    exit(-1);
  }
}

AMSType::AMSType(ObjectBase::ObjectType objType, char * name, double reltol, double abstol, 
		 quantityTypes qType, VHDLType* pos, VHDLType* neg, double val) {
  ASSERT(objType == ObjectBase::QUANTITY);
  switch ( qType ) {
  case IMPLICIT:
    cerr << "Wrong Quantity type being initialized" << endl;
    break;
  case FREE:
    setObject( new Quantity(name,qType,reltol,abstol,NULL,NULL,val) );
    break;
  case THROUGH:
    if (!pos && !neg) {
      setObject( new Quantity(name,qType,reltol,abstol,NULL,NULL,val) );
    }
    else if (pos && neg) {
	setObject( new Quantity(name,qType,reltol,abstol,(Terminal *)(pos->getObject()),(Terminal *)(neg->getObject()),val) );
      }
    else {
	cerr << "Terminal associations for quantity incomplete." << endl;
    }
    globalThroughQuantityList.push_back(this);
    break;
  case ACROSS:
    if (!pos && !neg) {
      setObject( new Quantity(name,qType,reltol,abstol,NULL,NULL,val) );
    }
    else if (pos && neg) {
	setObject( new Quantity(name,qType,reltol,abstol,(Terminal *)(pos->getObject()),(Terminal *)(neg->getObject()),val) );
    }
    else {
	cerr<<" Terminal associations for quantity incomplete."<<endl;
    }
    globalAcrossQuantityList.push_back(this);
    break;
  default:
    cerr << "ERROR : Unknown Quantity type" << endl;
    exit(-1);
  }
}

// Constructor for terminal. it is associated only with its name.
// Contributions are handled in the terminal class.
// there is another constructor for terminal with an additional parameter.
// looks like it is a terminal number. it may be needed soon.

AMSType::AMSType(ObjectBase::ObjectType objType, char * name) {
  ASSERT(objType == ObjectBase::TERMINAL);
  setObject( new Terminal(name) );
}

AMSType::AMSType(ObjectBase::ObjectType objType, char * name ,int id) {
  ASSERT(objType == ObjectBase::TERMINAL);
  setObject( new Terminal(name,id) );
}

#ifndef _SAVANT_ENTITY_AMS_ELAB_HH
#define _SAVANT_ENTITY_AMS_ELAB_HH

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
#include <list>
#include "tyvis/_savant_entity_elab.hh"
#include "tyvis/Quantity.hh"
#include "tyvis/Terminal.hh"
#include "tyvis/ContributionNode.hh"
#include "tyvis/Component.hh"
#include "tyvis/AMSProcess.hh"

using std::list;

class AMSType;
class Quantity;
class Terminal;

/** This class handles the elaboration of AMS constructs. */
class _savant_entity_ams_elab : public _savant_entity_elab {
public:
  /// Empty constructor.
  _savant_entity_ams_elab();

  /// Empty destructor.
  ~_savant_entity_ams_elab() {}

  /** Handles port mapping of terminals from formal to actuals. This virtual function 
      is not defined for this class and is only defined for some entity/component/configuration
      in the AMS model. */
  virtual void connectTerminals(int, ...);
  
  /** Copies the terminal info of the actual to the formal. */
  void setTerminalInfo(VHDLType& formal, VHDLType& actual);
  
  /** Sets the positive terminal of a quantity. 

      @param toSet quantity, whose positive Terminal needs to be set.
      @param plusTerminal the positive Terminal of the Quantity.
      @param qName name of the Quantity. */
  void setPlusTerminal(VHDLType *toSet, VHDLType *plusTerminal, char *qName);

  /** Sets the negative terminal of a quantity.
      
      @param toSet quantity, whose negative terminal needs to be set.
      @param minusTerminal the negative Terminal of the Quantity. */ 
  void setMinusTerminal(VHDLType *toSet, VHDLType *minusTerminal);

  /** This function accomplishes assignment of unique Id to through
      quantities, and evaluation of contributions for through
      quantities. Unique Id is also assigned to free equations, free
      quantities and implicit quantities.  Terminals have already been set
      a unique Id when they are instantiated. */
  void assignIndex();

  /** Elaborates simultaneous statements to help form the basic set of equations. This
      virtual function is not defined for this class. */
  virtual void formCharacteristicExpressions();

  /** Creates a continuous process for every island in the AMS kernel */
  void createAMSProcess();

  /** Elaborates step limit specifications. */
  virtual void formStepLimitSpecs();
  
  /** Elaborates break statements. Helps in creating the set of 
      equations belonging to the break set */
  virtual void formBreakSet();

  /** A simple display function to view the set of characteristic equations */ 
  void printBasicSet();

  /** Adds the particular component(equation) into the basic set
        
      @param cmp the equation of class Component, which needs to be added
      to the set of equations. */
  void add(component *cmp);
protected:
  /// Used in connecting up terminals for a port map.
  AMSType **terminalInfo;

private:
  /// This static variable is used to maintain a list of all the equations.
  static component *basicSet;

  /// Returns the freeEquationId. freeEquationId stores the index (from
  /// globalAMSId) for the first free equation.  
  int getFreeEquationId();
  
};

#endif

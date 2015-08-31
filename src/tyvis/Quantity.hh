#ifndef QUANTITY_HH
#define QUANTITY_HH

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
#include "tyvis/Terminal.hh"
#include "tyvis/SignalBase.hh"
#include "tyvis/vhdl_ams.hh"
#include "tyvis/UniversalReal.hh"
#include "tyvis/Node.hh"
#include "tyvis/vhdl.hh"   
#include "tyvis/AMSKernel.hh"
  
class AMSKernel;  
class AMSKernel_state;

/** Quantities are defined in this class. Quantities are classified as
    ACROSS, THROUGH, FREE and IMPLICIT. Every Quantity has its positive and
    negative terminal defined except if it is a FREE quantity when it is
    set to null.  This class also helps in maintaining the current value of
    the Quantity.  A Quantity object is usually created using a AMSType
    Constructor. */
class Quantity : public SignalBase {

public:
  /** Default constructor. */
  Quantity();

  /** Constructor for Quantities without tolerance specifications.

      @param name name of the Quantity
      @param qType a member of the enumeration quantityTypes.
      @see vhdl_ams
      @param pos positive Terminal associated with this Quantity.
      @param neg negative Terminal associated with this Quantity.
      @param val value with which the Quantity needs to be initialized. */
  Quantity(char *name, quantityTypes qType, Terminal* pos, Terminal* neg, double val = 0.0);

  /** Constructor for Quantities with tolerances. 
 
      @param name name of the Quantity
      @param qType a member of the enumeration quantityTypes.
      @see vhdl_ams
      @param reltol relative tolerance of this Quantity.
      @param abstol absolute tolerance of this Quantity.
      @param pos positive Terminal associated with this Quantity.
      @param neg negative Terminal associated with this Quantity.
      @param val value with which the Quantity needs to be initialized.	*/
  Quantity(char *name, quantityTypes qType, double reltol, double abstol, Terminal* pos, Terminal* neg, double val = 0.0);

  /// Default destructor.
  ~Quantity();

  /** Returns QUANTITY as the ObjectType.

      @see ObjectBase */
  ObjectType getKind() const ;

  /** Returns the enumerated type of the Quantity. */
  quantityTypes getType() const ;
  
  /** Returns the name of the Quantity. */
  char* getName() {
   return quantityName;
  }

  /** Returns the used flag. */
  int getUsed() {
    return used;
  }

  /** Returns the positive Terminal for the Quantity. */  
  Terminal *getPosTerminal() const;

  /** Returns the negative Terminal for the Quantity. */
  Terminal *getNegTerminal() const;
  
  /** Sets the positive Terminal for the Quantity.

      @param pos positive Terminal to be set. */
  void setPosTerminal(Terminal *pos);

  /** Sets the negative Terminal for the Quantity.

      @param neg negative Terminal for the Quantity */
  void setNegTerminal(Terminal *neg);
  
  /** Sets the quantity name. 

      @param name name of the Quantity. */
  void setName(char *name);

  /** Set the quantity index.

      @param num index to be set. */
  void setIndex(int num);

  /** Returns the Quantity's index. */
  int getIndex();

  /** Returns the loadedInMatrix flag. */
  bool isLoaded();

  /** Set the loadedInMatrix flag.

      @param toSet the bool value to be set. */
  void setLoadedFlag(bool toSet);
 
  /** Any class inheriting from ObjectBase has to override the following
      function. Returns the value of the Quantity */
  const RValue &readVal() const;

  /** Any class inheriting from ObjectBase has to override the following
      function.  Updates the value of the object.

      @param val the value to be updated with */
  void updateVal( const RValue &val );

  /** Any class inheriting from ObjectBase has to override the following
      function. This method creates another object of the same type as itself. */
  ObjectBase* clone() const ;
 
  /** Returns the positive node for the quantity. */
  int getPosNode();

  /** Returns the negative node for the quantity. */
  int getNegNode();

  /** Prints the quantity name and its terminal associations.

      @param os stream buffer to be printed on. */
  void print(ostream& os) const ;

  /** Calculates and returns the value of the quantity. 

      @param ckt a pointer to the current simulation kernel */
  double getValue(AMSKernel* ckt);

  /** Checks if quantity is a source. */
  bool getSourceCurrent() {
   return sourceCurrent;
  }

  /** Sets source current flag.

      @param opt the bool value to be set. true if the quantity is a source current. */
  void setSourceCurrent(bool opt) {
   sourceCurrent = opt;
  }

  /** Flag is set for through quantity when it is used. */
  void setUsed();

  /** Returns quantity type ie ACROSS, THROUGH, etc .*/
  quantityTypes get_quantity_type() const {
   return quantityType;
  }
 
  /** Sets the boolean variable inTheSet.

      @param toSet the bool value to be set. true if the Quantity is in the set. */
  void setInTheSet(bool toSet) {
   inTheSet = toSet;
  }

  /** Returns the bool variable inTheSet. */
  bool getInTheSet() {
   return inTheSet;
  }  

protected:

private:
  /// Stores quantity name. 
  char* quantityName;

  /// Flag to check if Quantity is loaded in the matrix. 
  bool loadedInMatrix;

  /// Stores the current value for the quantity. 
  double presentVal;

  /// Positive node. 
  double posNodeVal;

  /// Negative node for the quantity.
  double negNodeVal;

  /// Flag to indicate if Quantity is currently in the set for this island.
  bool inTheSet;

  /// Holds the value of the Quantity. 
  UniversalReal value;

  /// Attribute for the enumeration type of the quantity.
  quantityTypes quantityType;

  /// Positive terminal for this quantity. 
  Terminal *posTerminal;

  /// Negative terminal for this quantity. 
  Terminal *negTerminal;

  /// Global index assigned to this quantity during elaboration. 
  int index;

  /// Flag to indicate if quantity is used. 
  int used;

  /// Flag to indicate if quantity is source current. 
  bool sourceCurrent;

  /// Stores the previousl value of the quantity. 
  double previousVal;
};

#endif


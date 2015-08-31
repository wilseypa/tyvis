#ifndef AMSTYPE_HH
#define AMSTYPE_HH

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
#include "tyvis/UniversalReal.hh"
#include "tyvis/vhdl_ams.hh"

/** This class handles scalar Quantity and Terminal types of UniversalReal */
class AMSType : public UniversalReal {

public:
  AMSType() {}
  ~AMSType() {}

  //@{
  /** The following are the constructors for Quantity and Terminal instantiations. 
      The first two constructs appear for Quantity instantiations and the next two 
      for Terminal instantiations.

      Most of the parameters are common to the constructors.

      @param objType a member of the enumeration ObjectType, either QUANTITY or TERMINAL. 
      @param name name of the Quantity/Terminal.
      @param reltol relative tolerance value for the Quantity/Terminal.
      @param abstol absolute tolerance value for the Quantity/Terminal.
      @param qType a member of the enumeration quantityTypes.
      @see vhdl_ams 
      @param pos postive terminal for the Quantity.
      @param neg negative terminal for the Quantity.
      @param val initial value of the Quantity. 
      @param id terminal Id of the Terminal.  */

  /// Constructor for quantities with tolerances.
  AMSType(ObjectBase::ObjectType objType, char *name, double reltol, 
         double abstol, quantityTypes qType, VHDLType* pos, VHDLType* neg);

  /// Constructor for quantities which are intialized, includes tolerances.
  AMSType(ObjectBase::ObjectType objType, char *name, double reltol, double abstol,
          quantityTypes qType, VHDLType* pos, VHDLType* neg, double val);

  /// Constructor for terminals
  AMSType(ObjectBase::ObjectType, char *);

  /** Constructor for terminals which need their object to be initialized with a terminal number.
      Useful while creating a new Quantity like "Is" */ 
  AMSType(ObjectBase::ObjectType objType, char *name, int id);

  //@}

protected:

private:

};

#endif

#ifndef VHDLPROCESS_HH
#define VHDLPROCESS_HH

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
#include "tyvis/VHDLKernel.hh"
#include "tyvis/vhdl.hh"
#include "tyvis/EventSigs.hh"

class StringLiteral;

/** This class implements the LRM definition of a VHDL'93 process.  The
   code defined inside of the method "executeVHDL" represents the
   sequential code defined by a VHDL process.  Some forms of VHDL
   sequential statements have been defined within this class as a method
   call.  */

class VHDLProcess : public VHDLKernel {
public:
  /**
     Constructor.
     
     @param name - simply passed on to parent class.
   */
  VHDLProcess( const std::string &name ) : VHDLKernel(name) {};

  /**
     Destructor.
   */
  ~VHDLProcess() {};

  /**
     This method is an upcall to generated code.  It gets called after the
     signal updation phase occurs.  Therefore, all input signals have been
     updated, and the code executed in the upcall needs to access these
     signals and other elements of this process's state, and generate
     output signal updates.
  */
  virtual void executeVHDL() override {};  // VHDL process simulation code


protected:
  virtual std::vector<std::shared_ptr<warped::Event>> assignSignal( const std::string& name,
			     int value, 
			     unsigned int delay,
			     const VTime& timestamp ) override;
  

private:
};

#endif


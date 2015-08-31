#ifndef AMSPROCESS_HH
#define AMSPROCESS_HH

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
#include <iostream>
#include <list>
#include "tyvis/AMSKernel.hh"

using std::cout;

extern int globalobjectid;
template <class type>
class set;

/** This class describes an Analog Process. Highest entity in the AMS
    architecture.  Acts as an interface between warped/tyvis and AMS */
class AMSProcess : public AMSKernel {

public:
  /** Default Constructor. */
  AMSProcess(char *);

  /** Default Destructor. */
  ~AMSProcess();

  /** Create a new AMSKernel state for this island. */
  AMSKernel_state *allocateState();

  /** Initialize calls execute, which executes the DC phase of the
      simulation cycle. */ 
  void initialize();
 
  /** Finalize routine to clean up before exiting. */
  void finalize();
 
  /** Setup loads and initializes each of the equations in this island. */
  void setup();

  /** Prints the values of the explicit Quantities into the results file at
      each time point. */
  void printFunction();

  /** Handles transient analysis */
  void executeProcess();

protected:

private:

  /// Island name. 
  char *name;

  /// Output file handle. 
  ofstream outputfile ;

};

#endif

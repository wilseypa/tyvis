#ifndef AMSAPPLICATION_HH
#define AMSAPPLICATION_HH

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
#include "tyvis/VHDLApplication.hh"
#include "tyvis/_savant_entity_ams_elab.hh"

class _savant_entity_ams_elab;

/** This class defines the initialization methods required by the AMS
    Kernel. */
class AMSApplication : public VHDLApplication {

public:

  /** Constructor. 

      @param initDesign elaboration class pointer, helps elaborate all constructs*/
  AMSApplication( _savant_entity_elab *initDesign );

  /// Destructor. 
  ~AMSApplication() {}

  /** Downcasts the _savant_entity_elab object to _savant_entity_ams_elab. 
      This helps in having a deisng pointer to elaborate the AMS constructs.

      @param toSet pointer which needs to be downcast to _savant_entity_ams_elab */ 
  void setAMSDesign(_savant_entity_elab *toSet);

  /** This method is invoked by the kernel so that the AMSApplication can
      make any initializations or processing that is required by it.

      @param args The command line parameters.
      @return Error code (non-zero return value implies an error).

      The command line parameters passed to this method are those command
      line parameters that the warped kernel did not use/recognize.  The
      command line parameters do not contain those parameters that are used
      by warped. */
  int initialize( vector <string> &args ); 
  
protected:

private:
  /// A design pointer to elaborate all the AMS constructs.
  _savant_entity_ams_elab *AMSDesign;
};

#endif

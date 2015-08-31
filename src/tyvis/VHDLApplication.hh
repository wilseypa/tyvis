
#ifndef VHDLAPPLICATION_H
#define VHDLAPPLICATION_H

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
#include <warped/Application.h>
#include <vector>
#include <string>
#include "tyvis/VHDLVTime.hh"

using std::vector;
using std::string;

class _savant_entity_elab;

/** The VHDLApplication. */
class VHDLApplication : public Application {
public:
   /**@name Public Class Methods of VHDLApplication. */
  //@{

  /** Constructor. */
  VHDLApplication( _savant_entity_elab *initDesign );

  /** This method is invoked by the kernel so that the vHDLApplication can
      make any initializations or processing that is required by it.  In
      general it is strongly suggested that the vHDLApplication must
      perform all the necessary checks in this method so that the
      simulation can be aborted (as early as possible) in case of errors.

      @param args The command line parameters
      @return Error code (non-zero return value implies an error)
      
      The command line parameters passed to this method are those command
      line parameters that the warped kernel did not use/recognize.  The
      command line parameters do not contain those parameters that are used
      by warped.*/
  virtual int initialize( vector <string> &args );
  
  /** This method is invoked by the kernel to obtain an
      application-specified partitioning of simulation objects. */
  const PartitionInfo *getPartitionInfo( unsigned int numProcessorsAvailable );

  /** This method is invoked by the kernel to find out how many simulation
      objects are involved in this simulation.
      
      @param MgrId The id of the simulation manager
      @return number of simulation objects. (-1) indicates error  */
  int getNumberOfSimulationObjects(int mgrId) const ;
  
  /** This method is invoked by the kernel so that the vHDLApplication can
      wind up and perform any cleanups etc.

      @return Error code (non-zero return value indicates error) */
  int finalize() ;

  /** This method is invoked by the kernel inorder to display the list of
      valid paramters for the vHDLApplication modules that a user can
      use/needs to specify. */  
  string getCommandLineParameters() const ;
  
  void registerDeserializers();

  /** Returns positive infinity in this application's time definition. */
  const VTime &getPositiveInfinity(){ return VHDLVTime::getVHDLVTimePositiveInfinity(); }

  /** Returns zero in this application's time definition. */
  const VTime &getZero(){ return VHDLVTime::getVHDLVTimeZero(); }

  /** a specified time in this application's time definition. */
  const VTime &getTime(string &timeString);

  /// Destructor.
  ~VHDLApplication() {}

  //@} // End of public class methods of VHDLApplication

protected:
  /// Default constructor.
  VHDLApplication() {}

private:
  _savant_entity_elab *design;
};
#endif

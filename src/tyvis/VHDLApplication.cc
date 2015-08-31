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

#include "vhdl.hh"
#include "VHDLApplication.hh"
#include "VHDLKernel.hh"
#include "Hierarchy.hh"
#include <clutils/StringHashMap.h>
#include <warped/PartitionInfo.h>
#include <warped/RoundRobinPartitioner.h>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include "_savant_entity_elab.hh"
#include "VCD.hh"

using std::flush;
using std::istringstream;

extern vector<VHDLKernel*> listOfProcesses;
Hierarchy * hier;


VHDLApplication::VHDLApplication( _savant_entity_elab *initDesign )
  : design( initDesign ){
  ASSERT( design != 0 );
}

int
VHDLApplication::initialize( vector<string> & args){
  if (args.size() > 1) {
    string vcdName = "";
    if (args.size() == 2) {
      vcdName = args[1];
    }
    else {
      vector<string>::iterator args_i, args_end;
      args_i = args.begin();
      args_end = args.end();
      args_i++;
      for ( ; args_i != args_end ; ++args_i ) {
        if ( *args_i == "-vcd" ) {
          if ( ++args_i == args_end ) {
            cerr << "-vcd specified without file name" << endl;
          }
          else {
            vcdName = *args_i;
          }
          break;
        }
      }
    }
    if ( vcdName == "" ) {
      cerr << "Command line arguments present, but unrecognizable.  No VCD file produced" << endl;
      cerr << "Syntax for outputting VCD file is: " << args[0] << " -vcd <file>" << endl;
    }
    else {
      GlobalControl::setVCD( new VCD( vcdName ) );
    }
  }
  cout << "Elaborating Design..." << flush;
  hier = new Hierarchy;
  char n = '\0';
  design->instantiate(hier, string(""), &n);
  design->connect(0, 0);
  cout << "Done" << endl;
  cout << "Hierarchy of design:" << endl;
  cout << "--------------------" << endl;
  hier->printHierarchy(cout, 3);
  cout << "--------------------" << endl;
  if (GlobalControl::getVCD()) {
    GlobalControl::getVCD()->setHierarchy(hier);
  }
  delete hier;
  return 0;
}

int
VHDLApplication::getNumberOfSimulationObjects(int) const {
  return VHDLKernel::getGlobalObjectId();
}

const PartitionInfo *
VHDLApplication::getPartitionInfo( unsigned int numProcessorsAvailable ){
  const PartitionInfo *retval = 0;

  Partitioner *myPartitioner = new RoundRobinPartitioner;
  retval = myPartitioner->partition( (vector<SimulationObject *> *)&listOfProcesses, 
				     numProcessorsAvailable );
  delete myPartitioner;

  return retval;
}

void
VHDLApplication::registerDeserializers(){
  registerTyvisDeserializers();
}

int
VHDLApplication::finalize()  {
  VCD *vcd = GlobalControl::getVCD();
  if (vcd) {
    vcd->finalize();
    delete vcd;
    vcd = 0;
  }
  delete design;

  return 0;
}

const VTime &
VHDLApplication::getTime(string &timeString) {
  istringstream is(timeString);
  LONG nf;
  is >> nf;
  nf *= 1000000000;
  static VHDLVTime t(nf, 0);
  return t;
}

string 
VHDLApplication::getCommandLineParameters() const { return ""; }

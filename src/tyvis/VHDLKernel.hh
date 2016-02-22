#ifndef VHDLKERNEL_HH
#define VHDLKERNEL_HH

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

#include <vector>
#include <set>
#include <warped.hpp>

#ifndef MYVHDLSTATE
#define MYVHDLSTATE
WARPED_DEFINE_LP_STATE_STRUCT(MyState) {
   bool state_;
};
#endif //MYVHDLSTATE

/** This class includes the methods and data required by a VHDL-Verilog Object to
   interface to warped2.  It is intended to be the glue between a generic VHDL-Verilog
   and Warped2.  */
class VHDLKernel : public warped::LogicalProcess {
public:
  VHDLKernel( const std::string &name)
     : warped::LogicalProcess(name),
       state_() {}
  virtual ~VHDLKernel() {};

  // Some of the new methods that is required by warped 2.0 API.
  // Actually there's no "reportError" in the warped2 API but I
  // leave it here because it would be good to implement some error
  // reporting in the future
  virtual void reportError( const std::string &, int severity ) {};

  virtual void executeVHDL( ){ abort(); }
  
  const std::string& getName() const { return name_; }

  warped::LPState& getState() override final { return this->state_; }

  void addSignal( const std::string& name, int value = 0 ) { signals_[name] = value; }
  void addOutput( const std::string& local_sig,
        const std::string& submod,
        const std::string& mod_sig ) {
     hierarchy_[local_sig].push_back( std::make_pair( submod,
              mod_sig));
  }

protected:
  virtual std::vector<std::shared_ptr<warped::Event>> assignSignal( const std::string& name,
			     const int value, 
			     unsigned int delay,
			     const VTime& timestamp ) = 0;

  // Element state
  MyState state_;
  // map< signal_name, value >
  std::map<std::string, int> signals_;
  // map< signal_name_here, vector< pair< subelement_name, name_signal_in_subelement > > >
  std::map<std::string, std::vector<std::pair<std::string, std::string>>> hierarchy_;
};

#endif //VHDLKERNEL_HH

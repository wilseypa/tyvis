#include "NANDG_state.hh"
#include "VHDLKernel.hh"

class NANDG : public VHDLKernel<NANDG_state> {
public:
  VTime TPD_HL;  // generic
  VTime TPD_LH;  // generic

  NANDG();
  ~NANDG();
  void executeVHDL();
};

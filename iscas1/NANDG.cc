#include "NANDG.hh"

NANDG::NANDG() {
  TPD_HL = 1;
  TPD_LH = 1;
}


NANDG::~NANDG() {
}


void NANDG::executeVHDL() {
#ifdef VHDLDBG
  cout << name << " executing VHDL at " << getLVT() << endl;
#endif

  if (state.current->waitLabel == 0) goto wait0;

  begin:
  state.current->VAL.val = 
    !((state.current->IN1.effVal == 0) && (state.current->IN2.effVal == 0));

  if (state.current->VAL.val != state.current->EX_VALUE.val) {
    state.current->EX_VALUE.val = state.current->VAL.val;
    if (state.current->VAL.val != 0) {
      VHDL_assignSignal(state.current->OUT1, 0, state.current->VAL.val,
			TPD_HL, TPD_HL);
    }
    else if (state.current->VAL.val == 0) {
      VHDL_assignSignal(state.current->OUT1, 0, state.current->VAL.val,
			TPD_LH, TPD_LH);
    }
    else {
    }
  }

  VHDL_executeWait(0);
  return;

  wait0:
  if (!VHDL_resumeWait(0)) return;
  goto begin;
}

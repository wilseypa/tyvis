#ifndef NANDG_STATE_HH
#define NANDG_STATE_HH

#include "BasicState.hh"
#include "vhdl.hh"
#include "ExpSignal.hh"
#include "Variable.hh"
#include "Wait.hh"

class NANDG_state : public BasicState {
  friend ostream& operator<<(ostream&, const NANDG_state&);

public:
  NANDG_state();
  ~NANDG_state(){};
  void initState();
  NANDG_state& operator=(NANDG_state&);
  void print(ostream&);
  SigBase* locateSig(int);

  // type, drivers, upassocs, downassocs, fanout, implicits
  Signal<BitVector<1>, 1, 0, 0, 0, 0> IN1; // formal
  Signal<BitVector<1>, 1, 0, 0, 0, 0> IN2; // formal
  Signal<BitVector<1>, 1, 1, 0, 0, 0> OUT1; // formal, associates up to OUT1_ACTUAL
  Signal<BitVector<1>, 1, 0, 1, 0, 0> IN1_ACTUAL; // associates down to IN1
  Signal<BitVector<1>, 1, 0, 1, 0, 0> IN2_ACTUAL; // associates down to IN2
  Signal<BitVector<1>, 1, 0, 0, 2, 0> OUT1_ACTUAL; // fanout this guy
  // the fanout speced is the max of all instances--those smaller need to 
  // manually set when the component is instantiated

  Variable<BitVector<1> > VAL;
  Variable<BitVector<1> > EX_VALUE;

  const int numWaits;
  Wait<NANDG_state> wait[1];

  int waitLabel;
  bool waitExpired;
};


inline 
NANDG_state::NANDG_state() : IN1("IN1"), IN2("IN2"), OUT1("OUT1"), 
IN1_ACTUAL("IN1_ACTUAL"), IN2_ACTUAL("IN2_ACTUAL"),OUT1_ACTUAL("OUT1_ACTUAL"),
numWaits(1) {
};

inline void
NANDG_state::initState() {
  waitLabel = -1;
  waitExpired = true;

  wait[0].sensSize = 2;
//  wait[0].sensList[0] = ; // unique
//  wait[0].sensList[1] = ; // unique

//  IN1.id = ; // unique
  IN1.sensitive = true;
  IN1.source[0] = "0";
  IN1.drvVal = "0";
  IN1.effVal = "0";

//  IN2.id = ; // unique
  IN2.sensitive = true;
  IN2.source[0] = "0";
  IN2.drvVal = "0";
  IN2.effVal = "0";

//  OUT1.id = ; // unique
  OUT1.source[0] = "0";
  OUT1.drvVal = "0";
  OUT1.effVal = "0";
//  OUT1.upAssoc.sig = ; // unique
//  OUT1.upAssoc.src = 0; // unique

//  IN1_ACTUAL.id = ; // unique
  IN1_ACTUAL.source[0] = "0";
  IN1_ACTUAL.drvVal = "0";
  IN1_ACTUAL.effVal = "0";
//  IN1_ACTUAL.downAssoc = ; // unique

//  IN2_ACTUAL.id = ; // unique
  IN2_ACTUAL.source[0] = "0";
  IN2_ACTUAL.drvVal = "0";
  IN2_ACTUAL.effVal = "0";
//  IN2_ACTUAL.downAssoc = ; // unique

//  OUT1_ACTUAL.id = ; // unique
  OUT1_ACTUAL.source[0] = "0";
  OUT1_ACTUAL.drvVal = "0";
  OUT1_ACTUAL.effVal = "0";
//  OUT1_ACTUAL.fanDest[0] = ; // unique
//  OUT1_ACTUAL.fanDest[1] = ; // unique--not all instances have this

  VAL.id = 0;
  VAL.val = "0";

  EX_VALUE.id = 1;
  EX_VALUE.val = "0";
}


inline ostream&
operator<<(ostream& os, const NANDG_state& s) {
  s.IN1.print(os);
  s.IN2.print(os);
  s.OUT1.print(os);
  s.IN1_ACTUAL.print(os);
  s.IN2_ACTUAL.print(os);
  s.OUT1_ACTUAL.print(os);
  s.VAL.print(os);
  s.EX_VALUE.print(os);
  for (register int i = 0; i < s.numWaits; i++) {
    os << "Wait " << i << ": \n  sensList: ";
    s.wait[i].print(os);
    os << "\ntimeout is: " << (s.wait[i].timeout)(&s) << " fs\n";
  }
  return os;
}


inline SigBase*
NANDG_state::locateSig(int sigId) {
  if (sigId == IN1.id) return &IN1;
  else if (sigId == IN2.id) return &IN2;
  else if (sigId == OUT1.id) return &OUT1;
  else if (sigId == IN1_ACTUAL.id) return &IN1_ACTUAL;
  else if (sigId == IN2_ACTUAL.id) return &IN2_ACTUAL;
  else if (sigId == OUT1_ACTUAL.id) return &OUT1_ACTUAL;
  else {
    cout << " NANDG_state::locateSig ERROR: signal " << sigId
	 << " not in state!" << endl;
  }
  abort();
};


inline NANDG_state&
NANDG_state::operator=(NANDG_state& s) {
  (BasicState&)*this = (BasicState&)s;
  IN1 = s.IN1;
  IN2 = s.IN2;
  OUT1 = s.OUT1;
  IN1_ACTUAL = s.IN1_ACTUAL;
  IN2_ACTUAL = s.IN2_ACTUAL;
  OUT1_ACTUAL = s.OUT1_ACTUAL;
  VAL = s.VAL;
  EX_VALUE = s.EX_VALUE;
  for (register int i = 0; i < numWaits; i++) wait[i] = s.wait[i];
  waitLabel = s.waitLabel;
  waitExpired = s.waitExpired;
  return *this;
}
#endif

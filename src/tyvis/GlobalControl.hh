#ifndef GLOBALCONTROL_H
#define GLOBALCONTROL_H
 
// Author: Dave Shirley, PSSI            dnshirl@sandia.gov
// Creation Date: 01/15/08

#include "tyvis/tyvis-config.h"

class VCD;

class GlobalControl {
public:

  static VCD *vcd;

  static VCD * getVCD( ) { return vcd;}
  static void setVCD( VCD * vcdIn ) { vcd = vcdIn;}
};
#endif

#ifndef VCD_H
#define VCD_H

// Author: Dave Shirley, PSSI            dnshirl@sandia.gov
// Creation Date: 01/15/08

#include "tyvis/tyvis-config.h"
#include "tyvis/VHDLVTime.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/SignalNetinfo.hh"
#include "GlobalControl.hh"
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>

using std::string;
using std::map;
using std::set;
using std::vector;

class Hierarchy;

class VCD {
public:

  VCD( const string &fileOut );
  ~VCD( );
  void setHierarchy(Hierarchy *hier);
  int finalize( );
  void registerCompositeSignal (int id, const string &name, int left, int right );
  void finishCompositeSignal ( );
  void registerSignal( int id, vector<int> fanIds, const string &name, RValue &value );
  void registerSignalInfo( const SignalNetinfo &, const SignalNetinfo &);
  void updateSignal( int sigId, const RValue &newValue );
  void updateSignal( int sigId, const RValue *newValue, const VHDLVTime &time );
  void setBase (string base);

private:
  void outputTime( LONG dTime);

  string outFile;
  map<int, string> idName;
  map<int, string> idLabel;
  std::ofstream out;
  bool head;
  bool vars;
  bool values;
  LONG rTime;                       // Report time
  map<int, RValue *> rValue;  // Report value (to be reported at rTime
  map<int, string> cValue;    // Current value string (already reported)
  string base_id;
  map<string, set<int> > idOrder;
  map<int, set<int> *> equivIds;
  vector<const SignalNetinfo *> parents;
  vector<const SignalNetinfo *> children;
  vector<string> hierOrder;
  set<RValue *> allocSet;
  int compositeId;
  map<int, int> compositeSize;
  map<int, string> compositeName;
  map<int, int> toComposite;
  map<int, int> firstCompositeId;
  const vector<string> *bitImage;
};

#endif

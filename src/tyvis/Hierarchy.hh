#ifndef HIERARCHY_HH
#define HIERARCHY_HH

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
#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;
using std::cout;

class Hierarchy
{
public:
  Hierarchy();
  ~Hierarchy();

  string & addFrame (const string & parent, const char *current);
  string & addFrame (const string & parent, const char *myName, const char *current);
  void printHierarchy (std::ostream &, int indent);
  void getOrder (vector<string> & order);

private:
  void insertLeaf (const string &);
  void printEntity (std::ostream &, string & name, int currIndent, int indent);
  void addOrder (vector<string> & order, string & base, string & curr);

  map<string, Hierarchy> children;
};

#endif /* HIERARCHY_HH */

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

#include "Hierarchy.hh"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

Hierarchy::Hierarchy() {
  children.clear();
}

Hierarchy::~Hierarchy() {
}

string & Hierarchy::addFrame (const string & parent, const char *current) {

  static string rval;

  if (current != string("")) {
    rval = parent + "--" + current;
    insertLeaf(rval);
  }
  else
    rval = parent;
// Useful for debugging run time failure: cout << "In addFrame with " << rval << endl;

  return rval;
}

string & Hierarchy::addFrame (const string & parent, const char *myName, const char *current) {
  if (parent == "" && *current == '\0') {
    return (addFrame (parent, myName));
  }
  return (addFrame (parent, current));
}

void Hierarchy::printHierarchy (std::ostream &os, int indent) {
  string null = "";
  printEntity (os, null, -indent, indent);
}

void Hierarchy::getOrder (vector<string> & order) {
  string base, curr;
  order.clear();
  base = "";
  curr = "";
  addOrder (order, base, curr);
}

void Hierarchy::insertLeaf(const string & path) {

  string name, nextPath;
  string::size_type s, e;

  s = path.find_first_not_of('-');
  if (s == string::npos)
    return;
  e = path.find_first_of('-', s);
  if (e == string::npos) {
    name = path.substr(s);
    nextPath = "";
  }
  else {
    name = path.substr(s, e-s);
    nextPath = path.substr(e);
  }
  children[name].insertLeaf(nextPath);
}

void Hierarchy::printEntity (std::ostream &os, string &name, int currIndent, int indent) {

  int nextIndent = indent + currIndent;

  if (name != "") {
    for ( int i=0 ; i<currIndent ; ++i )
      os << " ";
    os << name << std::endl;
  }
  map<string, Hierarchy>::iterator c_iter, c_end;
  c_iter = children.begin();
  c_end = children.end();
  for ( ; c_iter != c_end ; ++c_iter ) {
    string child = (*c_iter).first;
    (*c_iter).second.printEntity( os, child, nextIndent, indent );
  }
}

void Hierarchy::addOrder (vector<string> & order, string & base, string & curr) {

  string myName;
  if (order.size() != 0 || curr != "") {
    myName = base + "--" + curr;
    order.push_back(myName);
  }

  map<string, Hierarchy>::iterator c_iter, c_end;
  c_iter = children.begin();
  c_end = children.end();
  for ( ; c_iter != c_end ; ++c_iter ) {
    string child = (*c_iter).first;
    (*c_iter).second.addOrder( order, myName, child);
  }
}

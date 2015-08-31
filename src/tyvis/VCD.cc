
// Author: Dave Shirley, PSSI            dnshirl@sandia.gov
// Creation Date: 01/15/08

#define VCDMAIN
#include "SignalNetinfo.hh"
#include "VCD.hh"
#include "Hierarchy.hh"
#include "STDTypes.hh"
#include "time.h"
#include "EnumerationLiteral.hh"
#include <typeinfo>

// #define VCDDEBUG
#define TRANSLATE_ALDEC

VCD::VCD( const string &fileOut ) :
   outFile (fileOut),
   out(fileOut.c_str()),
   head (false),
   vars (false),
   values (false),
   rTime (0),
   base_id ("--main"),
   compositeId (-1),
   bitImage (0)
{
#ifdef VCDDEBUG
  cout << "In VCD::VCD, outFile = " << outFile << endl;
#endif
}

VCD::~VCD( ) {
#ifdef VCDDEBUG
  cout << "In VCD::~VCD" << endl;
#endif
  map<int, RValue *>::iterator rv_i, rv_end;
  rv_i = rValue.begin();
  rv_end = rValue.end();
  for ( ; rv_i != rv_end ; ++rv_i ) {
    if ((*rv_i).second)
      delete (*rv_i).second;
  }
  set<int>::iterator f_i, f_end;
  map<int, set<int> *>::iterator e_i, e_end;
  e_i = equivIds.begin();
  e_end = equivIds.end();
  for ( ; e_i != e_end ; ++e_i ){
    if ( (*e_i).second ) {
      set<int>::iterator d_i, d_end;
      set<int> *del = (*e_i).second;
      d_i = del->begin();
      d_end = del->end();
      for ( ; d_i != d_end ; ++d_i ){
        equivIds[*d_i] = 0;
      }
      delete del;
    }
  }
}

void
VCD::setHierarchy(Hierarchy *hier){
  hier->getOrder(hierOrder);
#ifdef VCDDEBUG
  vector<string>::iterator h_i, h_end;
  h_i = hierOrder.begin();
  h_end = hierOrder.end();
  cout << "Size = " << hierOrder.size() << endl;
  cout << "Hierarchy Order:" << endl;
  for ( ; h_i != h_end ; ++h_i ) {
    cout << (*h_i) << endl;
  }
#endif
  base_id = hierOrder[0];
}

int
VCD::finalize( ) {
#ifdef VCDDEBUG
  cout << "In VCD::finalize" << endl;
#endif
  const time_t now = time( NULL );
  outputTime( now );
  out << endl << "$comment" << endl << "      Finished dumping at " << ctime(&now) << "$end" << endl;
  out.close();
  return 0;
}

void
VCD::registerCompositeSignal (int id, const string &name, int left, int right ){
  compositeId = id;
  ostringstream cName;
  cName << name << "[" << left << ":" << right << "]";
  compositeName[id] = cName.str();
  if (right > left)
    compositeSize[id] = right - left + 1;
  else
    compositeSize[id] = left - right + 1;
  if (compositeSize[id] > 100)
    compositeId = -1;
}

void
VCD::finishCompositeSignal ( ){
  compositeId = -1;
}

void
VCD::registerSignal( int id, vector<int> fanIds, const string &name, RValue &value ) {
  int i;
  if ( typeid(value) == typeid(EnumerationLiteral) ){
    const vector<string> *image = dynamic_cast<const EnumerationTypeInfo &>(value.getTypeInfo()).getImage();
    if (bitImage == 0 && image->size() == 9){
      if ((*image)[0] == "'U'" &&
          (*image)[1] == "'X'" &&
          (*image)[2] == "'0'" &&
          (*image)[3] == "'1'" &&
          (*image)[4] == "'Z'" &&
          (*image)[5] == "'W'" &&
          (*image)[6] == "'L'" &&
          (*image)[7] == "'H'" &&
          (*image)[8] == "'-'" ){
        bitImage = image;
      }
    }
    if ( image != bitImage )
      return;
  }
#ifdef VCDDEBUG
  cout << "Registering signal " << id << " with value: ";
  value.print(cout);
  cout << endl;
#endif
  if (!head) {
    const time_t now = time( NULL );
    out << "$date" << endl << "      " << ctime(&now) << "$end" << endl;
    out << "$version" << endl << "      Habanero 0.x" << endl << "$end" << endl;
    out << "$timescale" << endl << "      1fs" << endl << "$end" << endl;
    head = true;
  }
  if (idName.find(id) == idName.end()) {
    idName[id] = name;
    rValue[id] = value.clone();
    allocSet.insert(rValue[id]);
    cValue[id] = "";
    idOrder[base_id].insert(id);
#ifdef VCDDEBUG
    cout << "In VCD::registerSignal, setting id = " << id << " to '" << name << "'" << endl;
#endif
  }
  else {
    if (idName[id] != name) {
      cerr << "Error: multiple names for id = " << id << " '" << name <<
              "' and '" << idName[id] << "'" << endl;
      ASSERT(false);
    }
  }
  if (compositeId >= 0 ){
    if ( typeid(value) == typeid(EnumerationLiteral) ){
      toComposite[id] = compositeId;
    }
  }

  int nfan = fanIds.size();
  set<int> f;
  f.insert(id);
  for ( i=0 ; i<nfan ; ++i )
    f.insert(fanIds[i]);
  set<int>::iterator f_i, f_end;
  set<int> *s = 0;
  f_i = f.begin();
  f_end = f.end();
  for ( ; f_i != f_end ; ++f_i ){
    if ( equivIds.find(*f_i) != equivIds.end() ) {
      s = equivIds[*f_i];
      break;
    }
  }
  if ( !s ) {
    s = new set<int>;
    s->clear();
  }
  f_i = f.begin();
  for ( ; f_i != f_end ; ++f_i )
    s->insert(*f_i);
  f_i = f.begin();
  for ( ; f_i != f_end ; ++f_i ){
    if (equivIds.find(*f_i) != equivIds.end() && equivIds[*f_i] != s ) {
      set<int>::iterator d_i, d_end;
      set<int> *del = equivIds[*f_i];
      d_i = del->begin();
      d_end = del->end();
      for ( ; d_i != d_end ; ++d_i ){
        equivIds[*d_i] = s;
      }
      delete del;
    }
    equivIds[*f_i] = s;
  }

  return;
}

void
VCD::registerSignalInfo( const SignalNetinfo &parent, const SignalNetinfo &child ){
  parents.push_back(&parent);
  children.push_back(&child);

  return;
}

void
VCD::updateSignal( int sigId, const RValue &newValue ) {
  if (idName.find(sigId) != idName.end() && newValue != *(rValue[sigId]) ) {
#ifdef VCDDEBUG
    cout << "Updating " << idLabel[sigId] << " (" << sigId << ") with ";
    newValue.print(cout);
    cout << " (";
    newValue.vcdPrint(cout);
    cout << ") for type " << typeid(newValue).name();
    cout << endl;
#endif
    *(rValue[sigId]) = newValue;
  }
}

void
VCD::updateSignal( int sigId, const RValue *newValue, const VHDLVTime &time ) {
#ifdef VCDDEBUG
    cout << "At time " << time.getMajor() << "  ";
    cout << "Updating " << idLabel[sigId] << " (" << sigId << ") with ";
    newValue->print(cout);
    cout << " (";
    newValue->vcdPrint(cout);
    cout << ") for type " << typeid(*newValue).name();
    cout << endl;
#endif
  if (!vars) {
    std::vector<string> curr, next;
    out << "$scope module " << hierOrder[0].substr(2) << " $end" << endl;
    int ser = 0;
    string label;
    unsigned int i, j, k, p;
    string::size_type n;
    vector<string>::iterator h_i, h_end;

    unsigned int numPairs = parents.size();
    idLabel.clear();
    if (numPairs > 0) {
      map<int, string> sigHier;
      string p_str, ch_str, p_name, work;
      unsigned unresolved;
      h_i = hierOrder.begin();
      h_end = hierOrder.end();
      for ( ; h_i != h_end ; ++h_i ) {
        string nx = *h_i;
        set<int> &sigs = idOrder[nx];
        set<int>::iterator sig_i, sig_end;
        sig_i = sigs.begin();
        sig_end = sigs.end();
        for ( ; sig_i != sig_end ; ++sig_i ) {
          sigHier[*sig_i] = nx;
        }
      }
      set<int>::iterator f_i, f_end;
      set<int> *s;
      for ( i=0 ; i<numPairs ; ++i ){
        s = 0;
        if (equivIds.find(children[i]->getId()) != equivIds.end() ){
          s = equivIds[children[i]->getId()];
          if ( equivIds.find(parents[i]->getId()) != equivIds.end() ){
            if ( equivIds[children[i]->getId()] != equivIds[parents[i]->getId()] ) {
              f_i = equivIds[parents[i]->getId()]->begin();
              f_end = equivIds[parents[i]->getId()]->end();
              for ( ; f_i != f_end ; ++f_i ) {
                s->insert(*f_i);
              }
              set<int>::iterator d_i, d_end;
              set<int> *del = equivIds[parents[i]->getId()];
              d_i = del->begin();
              d_end = del->end();
              for ( ; d_i != d_end ; ++d_i ){
                equivIds[*d_i] = s;
              }
              delete del;
            }
          }
          else {
            equivIds[parents[i]->getId()] = s;
          }
        }
        else if ( equivIds.find(parents[i]->getId()) != equivIds.end() ){
          s = equivIds[parents[i]->getId()];
          equivIds[children[i]->getId()] = s;
        }
        else {
          s = new set<int>;
          s->clear();
          equivIds[children[i]->getId()] = s;
          equivIds[parents[i]->getId()] = s;
        }
        s->insert(parents[i]->getId());
        s->insert(children[i]->getId());
      }
      while (numPairs > 0) {
        unresolved = 0;
        string::size_type p, p_net;
        for ( i=0 ; i<numPairs ; ++i ){
          if (sigHier.find(children[i]->getId()) != sigHier.end() ){
            if (sigHier.find(parents[i]->getId()) == sigHier.end() ){
              p_name = parents[i]->getName();
              p_net = p_name.find("<netInfo>");
              while ( p_net != string::npos ){
                if (p_name.size() > p_net+9)
                  p_name = p_name.substr(0, p_net) + p_name.substr(p_net+9);
                else
                  p_name = p_name.substr(0, p_net);
                p_net = p_name.find("<netInfo>");
              }
              idName[parents[i]->getId()] = p_name;
              rValue[parents[i]->getId()] = rValue[children[i]->getId()]->clone();
              allocSet.insert(rValue[parents[i]->getId()]);
              cValue[parents[i]->getId()] = "";
              ch_str = sigHier[children[i]->getId()];
              p = ch_str.rfind("--");
              p_str = ch_str.substr(0,p);
              idOrder[p_str].insert(parents[i]->getId());
              sigHier[parents[i]->getId()] = p_str;
            }
          }
          else {
            parents[unresolved] = parents[i];
            children[unresolved] = children[i];
            ++unresolved;
          }
        }
        if (numPairs == unresolved) {
/* These are generally inconsequential:
          cout << "Unresolved signal pairs: " << numPairs << endl;
          for ( i=0 ; i<numPairs ; ++i ){
            cout << parents[i]->getName() << " and " << children[i]->getName() << endl;
          }
*/
          numPairs = 0;
        }
        else
          numPairs = unresolved;
      }
    }
    
    h_i = hierOrder.begin();
    h_end = hierOrder.end();
    curr.clear();
    curr.push_back(hierOrder[0].substr(2));
    for ( ; h_i != h_end ; ++h_i ) {
      next.clear();
      string nx = *h_i;
      set<int> &sigs = idOrder[nx];
      p = 0;
      while (p < nx.size()-2) {
        n = nx.substr(p+2).find("--");
        if (n == string::npos)
          n = nx.size()-p-2;
        next.push_back(nx.substr(p+2,n));
        p += n+2;
      }
      i = curr.size();
      if (next.size() < i)
        i = next.size();
      j = 0;
      for (k=0 ; k<i ; ++k) {
        if ( curr[k] == next[k])
          j = k;
        else
          break;
      }
      for (k = j+1 ; k<curr.size() ; ++k)
        out << "$upscope $end" << endl;
      for (k=j+1 ; k<next.size() ; ++k)
        out << "$scope module " << next[k] << " $end" << endl;
      curr = next;
      set<int>::iterator sig_i, sig_end, sig_next;
      sig_i = sigs.begin();
      sig_end = sigs.end();
      for ( ; sig_i != sig_end ; ++sig_i ) {
        int id = *sig_i;
        RValue *init = rValue[id];
        ostringstream testType;
        init->vcdPrint(testType);
        if ( testType.str() != "INVALID" ) {
          i = 0;
          label = "";
          k = ++ser;
          for ( j=0 ; k>0 ; ++j ) {
            --k;
            char next = static_cast<char>(33+k%94);
            label.append(1, next);
            k /= 94;
          }
          if (toComposite.find(id) != toComposite.end() ){
            int compId = toComposite[id];
            firstCompositeId[compId] = id;
            idLabel[compId] = label;
            out << "$var wire ";
            out << compositeSize[compId];
            out << " " << label << " " << compositeName[compId] << " $end" << endl;
            sig_next = sig_i;
            while (toComposite.find(id) != toComposite.end() && compId == toComposite[id] ){
              if ( idLabel.find(id) == idLabel.end() )
                idLabel[id] = "";
              sig_i = sig_next;
              sig_next++;
              if (sig_next != sig_end) {
                id = *sig_next;
              }
              else {
                compId = -1;
              }
            }
          }
          else {
            if ( idLabel.find(id) == idLabel.end() ){
              idLabel[id] = label;
              std::set<int>::iterator e_i, e_end;
              e_i = equivIds[id]->begin();
              e_end = equivIds[id]->end();
              for ( ; e_i != e_end ; ++e_i ){
                idLabel[*e_i] = label;
              }
            }
            else {
              --ser;
            }
            rValue[id] = init;
            out << "$var ";
            if (typeid(*init) == typeid(UniversalInteger)) {
              out << "integer";
              out << " " << 8*sizeof(int) << " ";
            }
            else if (typeid(*init) == typeid(UniversalReal)) {
              out << "real";
              out << " 1 ";
            }
            else if (typeid(*init) == typeid(EnumerationLiteral)) {
              out << "wire";
              out << " 1 ";
            }
            else {
              out << "VCD_DESIGNATION_UNKNOWN for type: " << typeid(*init).name();
            }
            out << idLabel[id] << " " << idName[id] << " $end" << endl;
          }
        }
      }
    }
    for (k = 0 ; k<curr.size() ; ++k)
      out << "$upscope $end" << endl;
    out << "$enddefinitions $end" << endl;
    vars = true;
  }
  LONG dTime = time.getMajor();
  if (dTime > rTime) {
    outputTime( dTime );
  }
  if ( idLabel.find(sigId) != idLabel.end() )
  {
    if (rValue.find(sigId) == rValue.end() || *(rValue[sigId]) != newValue->readVal() ){
      std::set<int>::iterator e_i, e_end;
      RValue *nr = newValue->readVal().clone();
      allocSet.insert(nr);
      e_i = equivIds[sigId]->begin();
      e_end = equivIds[sigId]->end();
      for ( ; e_i != e_end ; ++e_i ){
        rValue[*e_i] = nr;
      }
    }
  }

#ifdef VCDDEBUG
  static int count = 0;
  count++;
  cout << "(count = " << count << ") ";
  if (count == 5901 || count == 5902) {
  }
  if ( idName.find(sigId) == idName.end() ) {
    cout << "**UNKNOWN**";
  }
  else {
    cout << idName[sigId];
  }
  cout << " (id:" << sigId << ") to value: ";
  cout << *newValue << " at time: " << time << endl;
#endif

  return;
}

void   
VCD::setBase( string base ) {
  base_id = "";
  unsigned int blen = base.size();
  string::size_type p = 0;
  while (p != string::npos && p<blen) {
    if (base.substr(p,2) == "--") {
      p += 2;
    }
    else {
      string::size_type end_name = base.substr(p).find("--");
      if (end_name == string::npos)
        end_name = blen-p;
      base_id += "--" + base.substr(p,end_name);
      p += end_name;
    }
  }
#ifdef VCDDEBUG
  cout << "base_id = " << base_id << endl;
#endif
}

void
VCD::outputTime( LONG dTime ) {
  map<int, RValue *>::iterator r_i, r_end, r_next;
  map<string, string> curr;
  if ( rValue.size() > 0 ) {
    string r;
    r_i = rValue.begin();
    r_end = rValue.end();
    for ( ; r_i != r_end ; ++r_i ) {
      int id = (*r_i).first;
      if (toComposite.find(id) != toComposite.end() ){
        int compId = toComposite[id];
        bool changed = false;
        r_next = r_i;
        while (toComposite.find(id) != toComposite.end() && compId == toComposite[id] ){
          r_i = r_next;
          ostringstream vcdOut;
          (*r_i).second->vcdPrint(vcdOut);
          r = vcdOut.str();
#ifdef TRANSLATE_ALDEC
          if ( r == "U" || r == "W" || r == "X" || r == "-" ){
            r = "x";
          }
          else if ( r == "L" ){
            r = "0";
          }
          else if ( r == "H" ){
            r = "1";
          }
          else if ( r == "Z" ){
            r = "z";
          }
#endif
          if ( r != cValue[id] ) {
            cValue[id] = r;
            changed = true;
          }
          r_next++;
          if (r_next != r_end) {
            id = (*r_next).first;
          }
          else {
            break;
          }
        }
        if (changed) {
          string compValue = "b";
          bool lead = true;
          int j=firstCompositeId[compId];
          for (int i=0 ; i<compositeSize[compId] ; i++, j++) {
            if ( !lead || cValue[j] != "0" || i == compositeSize[compId]-1 ){
              compValue += cValue[j];
              lead = false;
            }
          }
          compValue += " ";
          curr[idLabel[compId]] = compValue;
        }
      }
      else {
        ostringstream vcdOut;
        (*r_i).second->vcdPrint(vcdOut);
        r = vcdOut.str();
#ifdef TRANSLATE_ALDEC
        if ( r == "U" || r == "W" || r == "X" || r == "-" ){
          r = "x";
        }
        else if ( r == "L" ){
          r = "0";
        }
        else if ( r == "H" ){
          r = "1";
        }
        else if ( r == "Z" ){
          r = "z";
        }
#endif
        if ( r != cValue[id] ) {
          curr[idLabel[id]] = r;
          cValue[id] = r;
        }
      }
    }
    rValue.clear();
  }
  set<RValue *>::iterator as_i, as_end;
  as_i = allocSet.begin();
  as_end = allocSet.end();
  for ( ; as_i != as_end ; ++as_i ){
    delete (*as_i);
  }
  allocSet.clear();
  if ( curr.size() > 0 ) {
    out << "#" << rTime << endl;
    if ( rTime == 0 )
      out << "$dumpvars" << endl;
    map<string, string>::iterator c_i, c_end;
    set<string> done;
    c_i = curr.begin();
    c_end = curr.end();
    for ( ; c_i != c_end ; ++c_i ) {
      if ( done.find((*c_i).first) == done.end() ){
        out << (*c_i).second << (*c_i).first << endl;
        done.insert((*c_i).first);
      }
    }
    if ( rTime == 0 )
      out << "$end" << endl;
  }
  rTime = dTime;
}

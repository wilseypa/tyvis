#ifndef _ELEMENT_ASSOCIATION_HH
#define _ELEMENT_ASSOCIATION_HH

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
#include "tyvis/ArrayInfo.hh"
#include "tyvis/UniversalInteger.hh"

class ElementAssociation {
public:
  ArrayInfo choice;
  RValue* value;
  bool cloned;
  ~ElementAssociation() { if (cloned) delete value; }
  ElementAssociation(): choice(others()), value(0), cloned(false) { }

  ElementAssociation(ArrayInfo a, RValue *v): choice(a), value(v), cloned(false) { }
  ElementAssociation(ArrayInfo a, const RValue &v): choice(a), value(v.clone()),
    cloned(true) { }

  ElementAssociation( int position, RValue *v ): 
    choice(position, ArrayInfo::to, position),
    value(v), cloned(false) {}
  
  ElementAssociation(int position, const RValue &v):
    choice(position, ArrayInfo::to, position),
    value(v.clone()),
    cloned(true){ }

  ElementAssociation(UniversalInteger position, RValue* v): 
                     choice(position.getIntValue(), ArrayInfo::to, position.getIntValue()),
		     value(v), cloned(false) {}
};

#endif //ifndef _ELEMENT_ASSOCIATION_HH

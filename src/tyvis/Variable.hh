#ifndef VARIABLE_HH
#define VARIABLE_HH

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
#include "tyvis/vhdl.hh"
#include "tyvis/ObjectBase.hh"
#include "tyvis/ArrayInfo.hh"

class Variable : public ObjectBase {
public:

  Variable( const TypeInfo &initTypeInfo,
	    const string &initName );

  Variable( const TypeInfo &initTypeInfo,
	    const string &initName,
	    const RValue &initValue );

  Variable( const Variable &that );
 
  Variable( const RValue &that );
  
  ~Variable(); 

  using ObjectBase::operator=;
  Variable &operator=( const Variable &that );

  
  Variable *clone() const;

  static ObjectBase *createSubelement( const TypeInfo &, int fieldIndex );

//@{
  /**
     Since variable assignment is used outside the scope of a process in
     some places (like for initialization inside of state classes), these
     methods are static.
  */
  static void assign( const LValue &dest, const RValue &newData );
  static void assign( LValue &destObject, const RValue &srcObject, 
		      const ArrayInfo &dinfo, const ArrayInfo &sinfo);
//@}

private:
  Variable();
};


#endif

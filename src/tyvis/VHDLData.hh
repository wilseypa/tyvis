#ifndef VHDLDATA_HH
#define VHDLDATA_HH

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
#include <warped/Serializable.h>
#include "tyvis/RValue.hh"
#include <string>
using std::string;

class ArrayInfo;
class AccessObject;
class EnumerationLiteral;
class UniversalInteger;

class VHDLData : public Serializable, public RValue {
public:
  virtual void print(ostream& os) const;

  virtual void assignSlice( const RValue *, 
			    const ArrayInfo *, 
			    const ArrayInfo * );

  virtual void assignSlice(const RValue *, 
			   const ArrayInfo * );

  string toString() const;
  RValue &operator=( const RValue & ){ return *this; }

  bool isCompositeType() const;

  const RValue &operator[]( int index ) const;
  RValue &operator[]( int index );
  const RValue &operator[]( const RValue &index ) const;
  RValue &operator[]( const RValue &index );
  const RValue &getField( int index ) const;
  RValue &getField( int index );
  const RValue &getField( const RValue &index ) const;
  RValue &getField( const RValue &index );

  const string &getUnit() const;

  bool isPhysicalType() const;
  LONG getValue() const;
  int getUnits() const;

  virtual bool operator==( const RValue & ) const = 0;
  virtual bool operator!= (const RValue & ) const = 0;
  virtual bool operator> (const RValue &) const = 0;
  virtual bool operator>= (const RValue &) const = 0;
  virtual bool operator< (const RValue &) const = 0;
  virtual bool operator<= (const RValue &) const = 0;
  
  virtual const RValue &readVal() const { return *this; }

  virtual ~VHDLData(){}

  const EnumerationLiteral &vhdlEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlNotEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlGreaterThan( const RValue & ) const;
  const EnumerationLiteral &vhdlGreaterThanEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlLessThan( const RValue & ) const;
  const EnumerationLiteral &vhdlLessThanEqual( const RValue & ) const;

  Value vhdlConcatenate( const RValue & ) const;

  RValue &all();
  const RValue &all() const;

  Value length( const RValue &index ) const;

protected:
  VHDLData(){}
};

void extractToDelimiter(char *source, int delimiter, char *destination);


#endif


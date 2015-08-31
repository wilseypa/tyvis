#ifndef ENUMERATION_LITERAL_HH
#define ENUMERATION_LITERAL_HH

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
#include "tyvis/IntegerLiteral.hh"
class EnumerationTypeInfo;
class TypeInfo;

class EnumerationLiteral: public IntegerLiteral {
public:
  EnumerationLiteral();
  EnumerationLiteral(const EnumerationLiteral&);

  EnumerationLiteral( const EnumerationTypeInfo &,
		      const string &value );

  EnumerationLiteral( const EnumerationTypeInfo &,
		      int value );

  EnumerationLiteral( const EnumerationTypeInfo &,
		      const RValue &value );
  
  EnumerationLiteral( const EnumerationTypeInfo & );

  virtual ~EnumerationLiteral();

  RValue &operator=( const RValue & );
  EnumerationLiteral &operator=( const EnumerationLiteral & );

  const EnumerationLiteral &leftValue();

  Value vhdlAnd( const RValue &rhs ) const;
  Value vhdlNot() const;
  Value vhdlOr( const RValue & ) const;
  Value vhdlNand( const RValue & ) const;
  Value vhdlNor( const RValue & ) const;
  Value vhdlXor( const RValue & ) const;
  Value vhdlXnor( const RValue & ) const;
  Value vhdlConcatenate( const RValue & ) const;
  
  Value vhdlUnaryPlus( ) const;
  Value vhdlUnaryMinus( ) const;
  
  Value vhdlPlus( const RValue & ) const;
  Value vhdlMinus( const RValue & ) const;
  Value vhdlMultiply( const RValue & ) const;
  Value vhdlDivide( const RValue & ) const;
  Value vhdlMod( const RValue & ) const;
  Value vhdlRem( const RValue & ) const;
  Value vhdlPow( const RValue & ) const;

  static const EnumerationLiteral &getEnumerationZero();
  static const EnumerationLiteral &getEnumerationOne();
  static const EnumerationLiteral &getEnumerationTrue();
  static const EnumerationLiteral &getEnumerationFalse();
  static const EnumerationLiteral &getEnumerationReadMode();
  static const EnumerationLiteral &getEnumerationWriteMode();

  static const EnumerationLiteral &toBoolean( const RValue &toConvert );
  static const EnumerationLiteral &toBoolean( bool toConvert );

  const TypeInfo &getTypeInfo() const;

  RValue *clone() const;

  const string &getDataType() const;
  static const string &getEnumerationLiteralDataType();
  void serialize( SerializedInstance * ) const;

  void print( ostream &os ) const;
  string toString() const;
  void vcdPrint( ostream &os ) const;

protected:
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  const EnumerationTypeInfo *myTypeInfo;
};

#endif

#ifndef VHDL_VTIME_HH
#define VHDL_VTIME_HH

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
#include <warped.hpp>
#include <VTime.hpp>
#include <iostream>
#include <sstream>

using std::string;

/** The class VHDLVTime. */
class VHDLVTime : public VTime {
public:
  /**@name Public Class Methods of VHDLVTime. */
  //@{

  /** Constructor. **/
  VHDLVTime( unsigned int major, unsigned int minor, unsigned int reg ) : VTime(major, minor, reg) {}

  /** Get Major VTime.
	
      @return Major VTime.
  */
  unsigned int getMajor() const { return myMajor; }

  /** Get Minor VTime.
	
      @return Minor VTime.
  */
  unsigned int getMinor() const { return myMinor; }

  const VHDLVTime &operator=( const VHDLVTime &vhdlTime ){
    myMajor = vhdlTime.getMajor();
    myMinor = vhdlTime.getMinor();
    return *this;
  }

  /// Overloaded operator
  bool operator!=( const VTime &right ) const {
    return !operator==( right );
  }

  /// Overloaded operator
  bool operator<=( const VTime &right ) const {
    if( *this < right || *this == right ){
      return true;
    }
    else{
      return false;
    }
  }

  /// Overloaded operator
  bool operator>=( const VTime &right ) const {
    if( *this > right || *this == right ){
      return true;
    }
    else{
      return false;
    }
  }

  static const VHDLVTime &getVHDLVTimeZero();
  static const VHDLVTime &getVHDLVTimePositiveInfinity();

  /** Get definition of zero.

      @return VHDLVTime definition of Zero.
  */
  const VHDLVTime &getZero() const { return getVHDLVTimeZero(); }

  /** Get definition of Positive Infinity.
	
      @return VHDLVTime definition of Positive Infinity.
  */
  const VHDLVTime &getPositiveInfinity() const { return getVHDLVTimePositiveInfinity(); }

  /// Destructor.
  virtual ~VHDLVTime(){}

  static const string &getVHDLVTimeDataType(){
    static string vTimeDataType( "VHDLVTime" );
    return vTimeDataType;
  }

  /**
     @see Serializable#getDataType
  */
  const string &getDataType() const {
    return getVHDLVTimeDataType();
  }

  const string toString() const {
    std::ostringstream os;
    os << "(" << getMajor() << ", " << getMinor() << ")";
    return os.str();
  }

  // This needs to be implemented.
  const unsigned int getApproximateIntTime() const {
    return myMajor;
  }
  
  //@} // End of Public Class Methods of VHDLVTime. 

private:
  /**@name Private Class Methods of VHDLVTime. */
  //@{
  
  /// Default Constructor.
  VHDLVTime() : VTime(0,0,0) {}
  // Prevent VHDLVTimes from being allocated or deleted on stack.

  //@} // End of Private Class Methods of VHDLVTime.

  /**@name Private Class Attributes of VHDLVTime. */
  //@{

  /// Major VHDLVTime.
  unsigned int myMajor;

  /// Minor VTime.
  unsigned int myMinor;

  //@} // End of Private Class Attributes of VTime.
};

inline
std::ostream &operator<<( std::ostream &os, const VHDLVTime &vt ){
  os << vt.toString();
  return os;
}

#endif

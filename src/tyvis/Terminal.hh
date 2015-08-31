#ifndef TERMINAL_HH
#define TERMINAL_HH

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
#include "tyvis/ObjectBase.hh"
#include "tyvis/UniversalReal.hh"

/** Terminals are defined using this class. Every terminal has a unique Id
    and a name assigned to it. The contributionNode class deals with the
    contributions associated with each terminal.  A terminal object is
    usually created using a AMSType constructor. */
class Terminal : public ObjectBase {

public:

  /** Default constructor. */
  Terminal();      

  /** Constructor. 

      @param name name of the terminal */
  Terminal(char *name);
  
  /** Constructor which allows assignment of a terminal Id. Helps
      for quantities like "Is".

      @param name name of the terminal
      @param newId Id for the terminal */
  Terminal(char *name, int newId);

  /** Default destructor. */
  ~Terminal();

  /** Gets the Id of the terminal.

      @return the terminal Id of the terminal. */	
  int getTerminalId();

  /** Sets the terminal name of the terminal.

      @param toSet the name to be set to the terminal. */
  void setTerminalName(char *toSet) {
   strcpy(terminalName,toSet);
  }

  /** Sets the terminal Id.

      @param toSet the terminalId that needs to be set */
  void setTerminalId( int toSet) {
   terminalId = toSet;
  }

  /** Gets the ObjectType of the terminal.

      @return a member of the enumeration ObjectType, ie "TERMINAL" */
  ObjectType getKind() const ;

  /// Returns the terminal name.
  char* getTerminalName() {
    return terminalName;
  }

  /// Sets the used flag for this terminal.
  void setUsed() {
    used = 1;
  }
 
  /// Returns the used flag for this terminal.
  int getUsed() {
    return used;
  }

  /** Any class inheriting from ObjectBase has to override the following
      function.  Updates the value of the object. Not used in the Terminal class. */
  void updateVal( const RValue & );

  /** Any class inheriting from ObjectBase has to override the following
      function.  Returns the value of the object. Not used in the Terminal class. */
  const RValue &readVal() const;

  /** Prints the information about the terminal. Used for debugging.

    @param os the stream buffer to be printed on. */ 
  void print(ostream& os) const;

  /** Overloaded = operator.

      @return a reference to the terminal
      @param term the source terminal */
  Terminal& operator=(const Terminal& term);  
  
  /** Any class inheriting from ObjectBase has to override the following
      function. This method creates another object of the same type as itself. 
      Not used in Terminal class. */
  ObjectBase* clone() const;

protected:

private:
  /// Name of the Terminal.
  char *terminalName;

  /// Used flag. 
  int used;

  /// Unique ID for the terminal.
  int terminalId;

  /// retval is used with the readval function. 
  UniversalReal value;

};

#endif

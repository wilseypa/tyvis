#ifndef __ENTITY_ELAB_HH__
#define __ENTITY_ELAB_HH__

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
#include "tyvis/PortMap.hh"
#include <PluginBase.h>
#include <vector>
using std::vector;

class SignalNetinfo;
class ObjectBase;
class ScalarType;
class Record;
class Array;
class TypeConvert;
class VHDLKernel;
class Hierarchy;
class Block;
class ObjectBase;
class AdditionalDrivers;

extern ResolutionFnPtr *savantResolutionFn;
extern TypeConversionFnPtr *savantTypeConversionFn;

class _savant_entity_elab : public PluginBase {
public:
  _savant_entity_elab();
  virtual ~_savant_entity_elab();

  static int typeConversionFnIdCounter;
  static int resolutionFnIdCounter;

  virtual void instantiate( Hierarchy *hier );
  virtual void instantiate( Hierarchy *hier, const string base, const char *name );
  virtual void createNetInfo();
  virtual void connect(int inputsignals, int outputsignals, ...);
//   virtual void Add( ObjectBase &, ObjectBase & );
//   virtual void Add( ObjectBase &, SignalNetinfo& sig );
//   virtual void Add( ObjectBase &, VHDLKernel * );
//   virtual void Add( ObjectBase &, VHDLKernel *, int sigid );
//   virtual void Add( ObjectBase &, SignalNetinfo *ptr );
  virtual void Addall( SignalNetinfo &dest, const SignalNetinfo &src );
  virtual void collectFanout( SignalNetinfo*, const SignalNetinfo & );
  virtual std::string get_base() {return(std::string(""));}  
  
  //@{ methods required for implementing resolution functions and type
  // conversion functions in tyvis/savant

  /** This method sets the resolution function id of the source of this
      signal. */
  void setResolutionFunctionId(ObjectBase &, ResolutionFnPtr);  
  
  /** When a ResolutionFunctionPtr is added to the global
      resolution function pointer table, the id at which this pointer is
      stored is returned. */
  static int addResolutionFn(ResolutionFnPtr resolve);
  static int addTypeConversionFn(TypeConversionFnPtr upConvert);

  void setUpConversionFunctionId(ObjectBase &, TypeConversionFnPtr);

  /** This method sets the down type conversion function id of the source of
      this signal. */
  void setDownConversionFunctionId( SignalNetinfo&, TypeConversionFnPtr );

  /** If the source of the corresponding SignalNetinfo is not initialized,
      we new a SourceData for it and assign the data to it.  If it is
      initialized and it has 0 children, then the source is a SourceData
      node.  Then, we "new" a SourceInfo node there, copy the contents of
      the old SourceData node as the new node's child, add the new child,
      and delete the old SourceData node.  This is done so as to remove the
      burden from the elaboration code that is generated. */
  void addChild( SignalNetinfo *sigInfo, ObjectBase *data, VHDLKernel* processPtr);
  void addChild( SignalNetinfo &sigInfo, ObjectBase &data, VHDLKernel* processPtr);
  void addChild( SignalNetinfo &sigInfo, ObjectBase &data);
  void addChild( SignalNetinfo *sigInfo, SignalNetinfo* driverInfo);

  /** This method does the setup for the TypeConversion to be done during the
      runtime. It just holds the poniters to various temporary and permanent
      information needed during the TypeConversion. */
  void addUpConvertDriver(ObjectBase & sigInfo, ObjectBase &data, TypeConvert *typeConvertPtr);
  void addUpConvertDriver(SignalNetinfo *sigInfo, SignalBase *data, TypeConvert *typeConvertPtr);

  /** adds the driverInfo to the block existing in the SignalNetinfo class.
      first checks whether atleast one does not have additionalDriverList. If so
      it new's a Block and adds this driverInfo passes it to the subelements other
      wise, it just passes the driverInfo so that it can be added to the already
      existing block in the SignalNetinfo object.
  */
  void addDriver( SignalNetinfo &sigInfo, SignalBase &driverInfo );
  void addDriver( SignalNetinfo &sigInfo, const AdditionalDrivers &toAdd );
  //@}

  /** We are having a data structure of type Block to hold the pointers of the
      temporary signals which occur in the Type Conversion. This routine
      will return TRUE if the element already has the Block allocated for that
      element if it is a ScalarType. If it is a Composite Type, then it will
      return TRUE only if all the sub elements of the Composite Type have the
      Block already allocated. Even if one of the elements of the Composite Type
      does not have a Block allocated, it will return FALSE. */
  bool isBlockPresent( SignalNetinfo &sigInfo );

  void setSourceToRoot( SignalNetinfo &);

  void setSourceInfo( SignalNetinfo &, const RValue & );

  void addDownTypeConversionFn( SignalNetinfo &, SignalNetinfo &, TypeConversionFnPtr );
  Block *getDownTypeConversionFnList( SignalNetinfo &);

  /** The component that is instantiated in an architecture declaration
      is bound to some real entity(architecture) The following function
      is used to get the information from the real entity(architecture)
      pair to logical component */
  virtual void buildSignalTree();

  virtual void copyGenericsToGlobals(){};

  /** The following two function check to ensure that the types of the
      signals being passed to the actual setSourceInfo() method are of the
      same type.  Different types of signals can arise if sub-elements in
      composite signals are assigned thro' the port maps. Here is a quick
      example

      component comp port (i : bit_vector(0 to 2)); end component;
      comp_inst : comp port map (i(0) => bit0; i(1) => bit1; i(2) => bit2);

      In this case 3 signals will be passed in connect. Now, these signals
      need to be combined into a bit_vector and then passed to the actual
      methods. 
  */

  /** Check if source and destination have the same number of elements in them
      If they do well and good. If they don't then create a new sig_netinfo
      and use as many parameters as necessary and return the number of the
      next parameter to use back to the calling process. */
  int checkSetSourceInfo( SignalNetinfo &, int, vector<RValue *> & );


  /** Check if source and destination have the same number of elements in
      them If they do well and good. If they don't then create a new
      sig_netinfo and use as many parameters as necessary and return the
      number of the next parameter to use back to the calling process. */
  int checkAdd( SignalNetinfo &, int, vector<RValue *> & );

  const string getPluginType() const { return "tyvis entity elab object"; }
  const string getPluginName() const { return "_savant_entity_elab"; }

  PortMap *getPortMapAspect() { return &portMapAspect; }

protected:
  void addToFanOut( RValue * );
  vector<RValue *> fanOutInfo;

  //@}

private:
  PortMap portMapAspect;
};

#endif

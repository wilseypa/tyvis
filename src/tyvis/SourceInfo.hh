#ifndef SOURCEINFO_HH
#define SOURCEINFO_HH

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
#include "tyvis/Types.hh"
#include "tyvis/SourceBase.hh"
#include <warped/warped.h>
#include <clutils/StringHashMap.h>

extern ResolutionFnPtr *savantResolutionFn;
extern TypeConversionFnPtr *savantTypeConversionFn;
class VHDLKernel;
class VHDLProcess;

extern const int CHILD_NOT_ADDED;

class SourceInfo : public SourceBase {
public:
  SourceInfo();
  ~SourceInfo();

  SourceInfo &operator=( const SourceInfo & );

  /** Up and down type conversion function pointerss. */
  TypeConversionFnId_t upConvertFnId;
  TypeConversionFnId_t downConvertFnId;

  /** Resolution function pointer. */
  ResolutionFnId_t resolveFnId;

  /** Methods to deal with up and down type conversion functions and
      resolution functions. */
  void setUpTypeConversionFnId(TypeConversionFnId_t);
  void setDownTypeConversionFnId(TypeConversionFnId_t);
  void setResolutionFnId(ResolutionFnId_t);

  TypeConversionFnId_t getUpTypeConversionFnId(void) { return upConvertFnId; };
  TypeConversionFnId_t getDownTypeConversionFnId(void){ 
    return downConvertFnId;
  };
  ResolutionFnId_t getResolutionFnId(void){ return resolveFnId; };

  /** Methods to act on the number of children of this node. */
  int getNumChildren() const;

  int getNumActiveChildren() const;

  bool _is_child_present(VHDLKernel *);

  bool _is_driver_connected();
  
  /** Methods to act on the child(ren) of this node.  addChild(...)
      takes a drivers, sets it as the next child and returns the number
      of children for this node. */
  int addChild(SourceBase *);
  int addChild(ObjectBase *, VHDLKernel *);
  SourceBase *getChild(const int) const;
  SourceBase& operator[](VHDLKernel *);

  /** SourceInfo::getDriver searches the tree for the SourceData node that
      points to the driver requested by the id and returns the node.  It
      aborts if the requested driver is not available in the tree. */
  SourceData *getDriver( VHDLKernel * );

  SourceData *getDriveratIndex(int);
  
  int getTableSize(void) const;

  /** Method that resolves all the drivers of this driver recursively.
      This method will call the resolve of all the children and resolve
      itself. */
  Value resolve( VHDLKernel*, SigType typ = EXPLICIT );
  
  void addDriversToRootDriverTable( SourceBase * );

  /** This method is used to build the fast access tables in the source
      data structure.  The tables are required only in the root node of the
      tree.  If this is not the root node, it passes the argument to its
      parent.  If this is the root node, it allocates space for the new
      data, copies the old data, deletes the space allocated before, and
      finally, adds the data to the tables. */
  void addToRootDriverTable( SourceData * );
  int getNumDriversInTree(void) const { return rootDriverTable.size(); }

  /**
     The following methods override pure virtuals in SourceBase.  However,
     calling any of them on a SourceInfo will cause a runtime error.
  */
  //@{
  SignalBase *getData();
  void setData( SignalBase * );
  void updateData( const RValue &newData );
  //@}
  
  
  void print(ostream&) const;

private:
  /** Children of this node. */
  vector<SourceBase *> children;

  /** The following tables are used for fast access of the drivers of this
  signal.  The id of a driver is the id of the process which contains the
  driver.  The "drivers" contains the list of all the ids of the
  drivers in this tree.  The index of this table is used to access the
  drivers in the tree without actually traversing the tree using the
  "childHash".  This speeds up access of the drivers.  The method
  buildTable() is used to create these tables. */
  vector<VHDLKernel *> rootDriverTable;

  /**
     Our index of children.  We the index is case sensitive.
  */
  StringHashMap<SourceBase *, true> childIndex;
};

#endif

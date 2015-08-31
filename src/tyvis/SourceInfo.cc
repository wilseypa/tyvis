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

#include "Aggregate.hh"
#include "ObjectBase.hh"
#include "SourceInfo.hh"
#include "SourceData.hh"
#include "SourceTypeConvert.hh"
#include "VHDLProcess.hh"

const int CHILD_NOT_ADDED = -99;

SourceInfo::SourceInfo(void) :
  SourceBase(),
  upConvertFnId( DEFAULT_TCF_ID ),
  downConvertFnId( DEFAULT_TCF_ID ),
  resolveFnId( DEFAULT_RF_ID ){}

SourceInfo::~SourceInfo() {
  for ( int childNo = 0 ; childNo < getNumChildren() ; childNo++ ){
    delete children[childNo];
  }
}

SourceInfo&
SourceInfo::operator = (const SourceInfo& source) {
  for(int i = 0; i < source.getNumChildren(); i++ ){
    *(children[i]) = *(source.getChild(i)); // This recursively copies the
  }				               // grandchildren as well!!
  return (*this);
}

void 
SourceInfo::setUpTypeConversionFnId(TypeConversionFnId_t fnId) {
  upConvertFnId = fnId;
}

void 
SourceInfo::setDownTypeConversionFnId(TypeConversionFnId_t fnId) {
  downConvertFnId = fnId;
}

void 
SourceInfo::setResolutionFnId(ResolutionFnId_t fnId) {
  resolveFnId = fnId;
}

int 
SourceInfo::getNumChildren() const {
  return children.size();
}

int
SourceInfo::getNumActiveChildren() const {
  int retval = 0;

  for( int i = 0; i < getNumChildren(); i++ ){
    if( children[i]->_is_driver_connected() == true ){
      retval++;
    }
  }
  return retval;
}

bool
SourceInfo::_is_child_present(VHDLKernel *childId) {
  if( parent != NULL ){
    return parent->_is_child_present(childId);
  } 
  else {
    for( unsigned int i = 0; i < rootDriverTable.size(); i++ ){
      if( rootDriverTable[i] == childId ){
	return true;
      }
    }
    return false;
  }
}

bool
SourceInfo::_is_driver_connected() {
  // Return true only if atleast one child can contribute a driver!!
  
  for( int i=0; i < getNumChildren(); i++ ){
    if( children[i]->_is_driver_connected() == true ) {
      return true;
    }
  }
  return false;
}

int 
SourceInfo::addChild(SourceBase *newChild) {
  if( newChild->getNumChildren() == 0 ){
    if( dynamic_cast<SourceInfo *>(newChild) != 0 ){
      return CHILD_NOT_ADDED;
    }
  }
  int childPresent = 0;
  if( dynamic_cast<SourceData *>(newChild) != 0 ){
    if( _is_child_present(newChild->getSourceId()) ){
      cerr << "Try to add another driver for the same process"
	   << " with id" << newChild->getSourceId() << endl;
      childPresent = 1;
    }
    else {
      childPresent = 0;
    }
  }
  else if( dynamic_cast<SourceInfo *>(newChild) != 0 ){
    SourceInfo *siPtr = (SourceInfo*)newChild;
    for( unsigned int i = 0; i < rootDriverTable.size(); i++ ){
      if(  siPtr->_is_child_present(rootDriverTable[i]) ){
	childPresent = 1;
	cerr << "Try to add another driver for the same process"
	     << " with SourceInfo containing id" << rootDriverTable[i] << endl;
	break;
      }
    }
  }
  
  if( childPresent == 1 ){
    return CHILD_NOT_ADDED;
  }
  else {
    children.push_back( newChild );
    newChild->setParent(this);

    // only if the newChild is a SOURCE_DATA or SOURCE_INFO then the
    // the drivers are collected at the root of the resolution tree.
    if( dynamic_cast<SourceTypeConvert *>(newChild) == 0 ){
      addDriversToRootDriverTable(newChild);
    }
    
    return getNumChildren();
  }
}

int 
SourceInfo::addChild(ObjectBase *newDriver, VHDLKernel* childId) {
  SourceData *newChild = NULL;

  if( _is_child_present(childId) ){
    return CHILD_NOT_ADDED;
  }

  newChild = new SourceData();
  newChild->setParent(this);
  newChild->addChild(newDriver, childId);

  children.push_back( newChild );

  if( childId != ANONYMOUS_PROCESS_ID ){
    addToRootDriverTable( newChild );
  }
  
  return getNumChildren();
}

SourceBase *
SourceInfo::getChild(const int childId) const {
  ASSERT(childId < getNumChildren());
  return children[childId];
}

SourceBase&
SourceInfo::operator[] (VHDLKernel *childId) {
  ASSERT(childId != NULL);
  return *getDriver(childId);
}

void
SourceInfo::addToRootDriverTable(SourceData *leafNode) {
  if( parent != NULL ){		// This is not the root node.
    parent->addToRootDriverTable(leafNode);
  }
  else if (leafNode->getSourceId() ){			// This is the root node. DNS: if clause added
    rootDriverTable.push_back( leafNode->getSourceId() );
    if( leafNode->getSourceId() != SourceBase::ANONYMOUS_PROCESS_ID ){
      childIndex.insert( leafNode->getSourceId()->getName(), leafNode );
    }
  }
  else {
    cerr << "NULL leafNode->getSourceId() found in SourceInfo::addToRootDriverTable" << endl;
  }
}

void 
SourceInfo::addDriversToRootDriverTable(SourceBase *newNode) {
  if( parent != NULL ){
    parent->addDriversToRootDriverTable(newNode);
  } 
  else {
    for(int i = 0; i < newNode->getTableSize(); i++ ){
      addToRootDriverTable(newNode->getDriveratIndex(i));
    } // for
  } // else
}

Value
SourceInfo::resolve( VHDLKernel *processPtr, SigType typ ){
  Value retval;
  
  int activeChildren = getNumActiveChildren();
  Aggregate valueArray;

  int i = 0;
  int j = 0;
  if(activeChildren == 0 ){	// Sanity check.
    // This could happen for signals of kind bus or register
    // When the kind is a bus then pass a NULL array to the resolution
    // function to find the effcetive value whereas in case of a register
    // the value does not change 
    if( typ == G_BUS ){
      // Pass a NULL array to the resolution function
      ASSERT(resolveFnId != SourceBase::DEFAULT_RF_ID);
      retval = ((savantResolutionFn[resolveFnId])( static_cast<VHDLProcess *>(processPtr),
						   valueArray));
    }
    return retval;
  }

  TypeConversionFnId_t fnId = SourceBase::DEFAULT_TCF_ID;
  RValue *tempPtr = NULL;  
  // Two counters are used to point to the drivers.
  // "i" refers to all the drivers in the simulation
  // "j" refers to the set of all connected Drivers during the
  // current simulation period.
  for(j=0, i = 0; i < getNumChildren(); i++ ){
    if( children[i]->_is_driver_connected() == true ){
      //Call the resolution function if the driver is connected
      valueArray.append( children[i]->resolve(processPtr) );
      //Call the type conversion if the driver is connected
      // First we have to do the up-type-conversion. Then we will pass on the
      // resultant vector to the resolution function. The down-type
      // conversion will be taken care of by the fanout, since this is a
      // sequence of functions operating on a data.
      tempPtr = NULL;
      fnId = children[i]->getUpTypeConversionFnId();
      if( fnId != SourceBase::DEFAULT_TCF_ID ){
// 	tempPtr = valueArray[j];
// 	valueArray[j] = (savantTypeConversionFn[fnId])( static_cast<VHDLProcess *>(processPtr),
// 							valueArray[j] ).clone();
// 	delete tempPtr;
	abort();
      }
      j++;
    }
  }
  
  if( j != activeChildren ){
    // Insanity check!!!
    cerr << "Internal Error : Number of active children != number of"
	 << " drivers.\n";
    activeChildren = j;
    abort();
  }
  
  //Call the Resolution function after calling the resolve of the 
  //Drivers
  if( activeChildren > 1 ){
    if( resolveFnId != SourceBase::DEFAULT_RF_ID ){
      retval = (savantResolutionFn[resolveFnId])( static_cast<VHDLProcess *>(processPtr),
						  valueArray);
    }
    else {
      cerr << "Number of active children for a signal is > 1. But it "
	   << " is not a resolved subtype!\n";
      retval = valueArray[1];
      abort();
    }
  } 
  else {
    retval = valueArray.getField(0);
  }

  return retval;
}

void
SourceInfo::print(ostream& os) const {
  static int indent = 0;
  int i;

  for(i = 0; i < indent; i++ ){
    os << " ";
  }

  indent += 4;			// Indent each child by 4 spaces.
  
  os << "getNumChildren()(" << getNumChildren() << ") "
     << "upFn(" << upConvertFnId << ") "
     << "downFn(" << downConvertFnId << ") "
     << "resolveFn(" << resolveFnId << ")" << endl;

  for(i = 0; i < getNumChildren(); i++ ){
    children[i]->print(os);
  }

  indent -= 4;
}

SourceData*
SourceInfo::getDriver( VHDLKernel* id ){
  SourceData *retval = 0;

  unsigned int i;
  if( parent != NULL ){
    retval = parent->getDriver(id);
  } 
  else {
    for(i = 0; i < rootDriverTable.size(); i++ ){
      if( rootDriverTable[i] == id ){
	break;
      }	// if
    } // for
    if( i == rootDriverTable.size() ){
      //For the following VHDL code
      //  type two_array is array (1 to 2) of integer;
      //  signal v : two_array;
      //       proc1 : process
      // 	begin
      
      // 	v(1) <= 1 after 10 ns;
      //       wait;
      
      //       end process;
      //There should be only one driver added to the signal since it is a
      //unresolved signal. But the source of the signal for each sub-element
      //can be different. Currently the elaborator ensures that only one driver
      // is added to the any unresolved signal. But the source of the signal's
      //sub-element is not determined at elaboration time, and hence the
      //following fix
      if( i == 1 ){
	rootDriverTable[0]  = id;
	SourceData *source = static_cast<SourceData *>(childIndex.find( id->getName() ));
	if( source != 0 ){
	  source->setSourceId(id);
	  retval = source;
	}
      }
    } 
    else {
      retval = static_cast<SourceData *>(childIndex.find(id->getName()));
    }
  }
  return retval;
}

SourceData *
SourceInfo::getDriveratIndex(int position) {
  vector<SourceBase *> *elements = childIndex.getElementVector();
  SourceData *retval = static_cast<SourceData *>( (*elements)[position] );
  delete elements;
  return retval;
}

int
SourceInfo::getTableSize(void) const
{
  vector<SourceBase *> *elements = childIndex.getElementVector();
  int size = elements->size();
  delete elements;
  return  size;
}

SignalBase *
SourceInfo::getData(){
  cerr << "Error - SourceInfo::getData called!" << endl;
  abort();
  return 0;
}

void
SourceInfo::setData( SignalBase * ){
  cerr << "Error - SourceInfo::setData called!" << endl;
  abort();
}

void
SourceInfo::updateData( const RValue & ){
  cerr << "Error - SourceInfo::updateData called!" << endl;
  abort();
}

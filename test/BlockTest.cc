
#include "BlockTest.hh"
#include "tyvis/Block.hh"

// Create anything needed for the tests here
void BlockTest::setUp() {    
  ptr = new int;
  ptr2 = new int;
    
  *ptr = 5;
  *ptr2 = 42;
}

// Release any allocated resources here
void BlockTest::tearDown() {
  delete ptr;
  delete ptr2;
}

void BlockTest::testAddRemoveElement() {
  // Test the add element & getNumberOfElements
  block.addElement(ptr);
  CPPUNIT_ASSERT_MESSAGE("getNumberOfElements or addElement failing!\n", block.getNumberOfElements() == 1);
  block.addElement(ptr2);
  CPPUNIT_ASSERT_MESSAGE("getNumberOfElements or addElement failing!\n",  block.getNumberOfElements() == 2);
    
  // Test the [] operator
  CPPUNIT_ASSERT_MESSAGE( "[] operator failing", block[0] == ptr);
  CPPUNIT_ASSERT_MESSAGE( "[] operator failing", block[1] == ptr2);
    
  // Test the getElement
  CPPUNIT_ASSERT_MESSAGE( "get element failing", block.getElement(0) == ptr);

  // Test the remove element
  block.removeElement(0);
  CPPUNIT_ASSERT_MESSAGE( "removeElement failing!\n", block[0] == ptr2);
}

void BlockTest::testEqual() {
  Block newBlock;    
  
  // Create a new block
  newBlock.addElement(ptr);
  newBlock.addElement(ptr2);

  // assign it to the old block
  block = newBlock;

  // check for identical elements
  CPPUNIT_ASSERT(block[0] == ptr);
  CPPUNIT_ASSERT(block[1] == ptr2);
}

void BlockTest::testReset() {
  block.addElement(ptr2);
  block.reset();
  CPPUNIT_ASSERT_MESSAGE("Reset, addelement, or getNumberOfElements failing!", block.getNumberOfElements() == 0);
}

#ifndef VHDL_EVENT_TEST_HH
#define VHDL_EVENT_TEST_HH

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>
using std::string;

class VHDLVTime;
class VHDLEvent;

class VHDLEventTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( VHDLEventTest);
  CPPUNIT_TEST_SUITE_END();

private:
  static void checkDefaults( const VHDLEvent *toTest );

protected:
  static const VHDLVTime &getDefaultSendTime();
  static const VHDLVTime &getDefaultReceiveTime();
  static const string &getDefaultSender();
  static const string &getDefaultReceiver();

public:
  static void testConstructor( const VHDLEvent *toTest );
  static void testSerialization( const VHDLEvent *toTest, const string &dataType );
  static void testEventCompare( const VHDLEvent *toTest );

};

#endif


#include <QtTest/QtTest>

#include <grids/protocol.h>
#include <grids/event.h>
#include <kaleidoscope/define.h>

#include <iostream>

class ProtocolTest: public QObject {
  Q_OBJECT
  public:
  ProtocolTest();
  ~ProtocolTest();
	
private slots:
  void constructorTest();
  void connectToNodeTest();
  void createRoomTest();
  void createObjectTest();
  void createObjectWithPosition();
  void createObjectWithID();
  void updateObjectTest();
  void getAllRoomsTest();

private:
  Grids::Protocol proto;
  Grids::GridsID test_room_id;
  Grids::GridsID test_object_id;
};

ProtocolTest::ProtocolTest(){

}

ProtocolTest::~ProtocolTest(){
  proto.stopEventLoopThread();
}

// ===================================
// TESTS
// ===================================

void ProtocolTest::constructorTest() {
  // Nothing here
 std::cerr << "LOLOLO\n";

}

void ProtocolTest::connectToNodeTest() {
  int wait_timer = 0;
  const int wait_limit = CALLBACK_SECONDS;
  QSignalSpy state_spy(&proto, SIGNAL(protocolInitiated(Event*)));
  QVERIFY(state_spy.isValid());
  QCOMPARE(state_spy.count(), 0);

  QVERIFY(proto.connectToNode(GRIDS_SERVER));
  QCOMPARE(proto.runEventLoopThreaded(), 0);

  while( wait_timer < wait_limit) {
	if( state_spy.count() > 0 )
	  break;

        QTest::qSleep(1000);
	wait_timer++;		
  }

  QCOMPARE(state_spy.count(), 1);
}

void ProtocolTest::createRoomTest() {
  int wait_timer = 0;
  const int wait_limit = CALLBACK_SECONDS;
  Grids::Value test_m;
  test_m["_method"] = GRIDS_CREATE_ROOM;

  QSignalSpy state_spy(&proto, SIGNAL(receiveEvent(Event*)));
  QVERIFY(state_spy.isValid());
  QCOMPARE(state_spy.count(), 0);

  proto.sendRequest( GRIDS_CREATE_ROOM, &test_m );

  while( wait_timer < wait_limit) {
	if( state_spy.count() > 0 )
	  break;
        QTest::qSleep(1000);
	wait_timer++;
  }

  // Don't access a null pointer, segfaulting
  if( proto.last_event){
	QCOMPARE(tr(GRIDS_CREATE_ROOM), tr(proto.last_event->getEventType().c_str()));
	QCOMPARE(tr(GRIDS_CREATE_ROOM), tr(proto.last_event->getArgs()[ "_method" ].asString().c_str()));		
	QVERIFY(proto.last_event->getArgs()[ "id" ].asString().length() > 10);

	test_room_id = proto.last_event->getArgs()[ "id" ].asString();
  } else {
	QFAIL("Response Timeout");
  }
}

void ProtocolTest::createObjectTest() {
		int wait_timer = 0;
		const int wait_limit = CALLBACK_SECONDS;
		std::string debug_string = GRIDS_CREATE_OBJECT;
		unsigned int test_attr = 12345556u;
		QSignalSpy state_spy(&proto, SIGNAL(receiveEvent(Event*)));
		QVERIFY(state_spy.isValid());
		QCOMPARE(state_spy.count(), 0);

		Grids::Value test_m;
		test_m[ "_method" ] = GRIDS_CREATE_OBJECT;
		test_m[ "room_id" ] = test_room_id;
		test_m[ "attr" ][ "lol" ][ "hi" ] = test_attr;
					
		proto.sendRequest( debug_string, &test_m );

		while( wait_timer < wait_limit) {
		  if( state_spy.count() > 0 )
				break;
                        QTest::qSleep(1000);
			wait_timer++;		
		}
		
		QVERIFY( proto.last_event->getArgs()[ "id" ].asString().length() > 10 );
		test_object_id = proto.last_event->getArgs()[ "id" ].asString();

		QVERIFY(GRIDS_CREATE_OBJECT == proto.last_event->getEventType());
		QVERIFY(GRIDS_CREATE_OBJECT == proto.last_event->getArgs()[ "_method" ].asString());

		QCOMPARE(test_attr, proto.last_event->getArgs()[ "req" ][ "attr" ][ "lol" ][ "hi" ].asUInt());
}

void ProtocolTest::createObjectWithPosition() {
  int wait_timer = 0;
  const int wait_limit = CALLBACK_SECONDS;
  std::string debug_string = GRIDS_CREATE_OBJECT;
  Grids::Vec3D test_pos = Grids::Vec3D( 4.4, 5.5, 6.6 );
  Grids::Vec3D test_rot = Grids::Vec3D( 1.23, 5.678, 4.0987 );
  Grids::Vec3D test_scl = Grids::Vec3D( 642.1234, 66431.1266, 7993.23885 );
  QSignalSpy state_spy(&proto, SIGNAL(receiveEvent(Event*)));
  QVERIFY(state_spy.isValid());
  QCOMPARE(state_spy.count(), 0);
		
  Grids::Value test_m;
  test_m[ "_method" ] = debug_string;
  test_m[ "room_id" ] = test_room_id;
  test_m[ "pos" ][ 0u ] = test_pos.X;
  test_m[ "pos" ][ 1u ] = test_pos.Y;
  test_m[ "pos" ][ 2u ] = test_pos.Z;
  test_m[ "rot" ][ 0u ] = test_rot.X;
  test_m[ "rot" ][ 1u ] = test_rot.Y;
  test_m[ "rot" ][ 2u ] = test_rot.Z;		
  test_m[ "scl" ][ 0u ] = test_scl.X;
  test_m[ "scl" ][ 1u ] = test_scl.Y;
  test_m[ "scl" ][ 2u ] = test_scl.Z;					

  proto.sendRequest( debug_string, &test_m );

  while( wait_timer < wait_limit) {
	if(state_spy.count() > 0)
	  break;
        QTest::qSleep(1000);
	wait_timer++;		
  }

  qFuzzyCompare( (double)(test_pos.X), proto.last_event->getArgs()[ "req" ][ "pos" ][ 0u ].asDouble() ); 
  qFuzzyCompare( (double)(test_pos.Y), proto.last_event->getArgs()[ "req" ][ "pos" ][ 1u ].asDouble() ); 
  qFuzzyCompare( (double)(test_pos.Z), proto.last_event->getArgs()[ "req" ][ "pos" ][ 2u ].asDouble() ); 

  qFuzzyCompare( (double)(test_rot.X), proto.last_event->getArgs()[ "req" ][ "rot" ][ 0u ].asDouble() ); 
  qFuzzyCompare( (double)(test_rot.Y), proto.last_event->getArgs()[ "req" ][ "rot" ][ 1u ].asDouble() ); 
  qFuzzyCompare( (double)(test_rot.Z), proto.last_event->getArgs()[ "req" ][ "rot" ][ 2u ].asDouble() ); 

  qFuzzyCompare( (double)(test_scl.X), proto.last_event->getArgs()[ "req" ][ "scl" ][ 0u ].asDouble() ); 
  qFuzzyCompare( (double)(test_scl.Y), proto.last_event->getArgs()[ "req" ][ "scl" ][ 1u ].asDouble() ); 
  qFuzzyCompare( (double)(test_scl.Z), proto.last_event->getArgs()[ "req" ][ "scl" ][ 2u ].asDouble() ); 
}

void ProtocolTest::createObjectWithID(){
  int wait_timer = 0;
  const int wait_limit = CALLBACK_SECONDS;
  std::string debug_string = GRIDS_CREATE_OBJECT;
  unsigned int test_attr = 12345556u;
  Grids::GridsID test_id = "DAA10054-7683-11DE-BB26-3FC64C661FD7";
  QSignalSpy state_spy(&proto, SIGNAL(receiveEvent(Event*)));
  QVERIFY(state_spy.isValid());
  QCOMPARE(state_spy.count(), 0);
		
  Grids::Value test_m;
  test_m[ "_method" ] = debug_string;
  test_m[ "room_id" ] = test_room_id;
  test_m[ "id" ] = test_id;
  test_m[ "attr" ][ "lol" ][ "hi" ] = test_attr;
					
  proto.sendRequest( debug_string, &test_m );

  while( wait_timer < wait_limit) {
	if( state_spy.count() > 0 )
	  break;
        QTest::qSleep(1000);
	wait_timer++;		
  }
		
  QCOMPARE( tr(test_id.c_str()), tr(proto.last_event->getArgs()[ "id" ].asString().c_str()) );
}

void ProtocolTest::updateObjectTest(){
  int wait_timer = 0;
  const int wait_limit = CALLBACK_SECONDS;
  std::string debug_string = GRIDS_UPDATE_OBJECT;
  unsigned int test_attr = 992223u;
  QSignalSpy state_spy(&proto, SIGNAL(receiveEvent(Event*)));
  QVERIFY(state_spy.isValid());
  QCOMPARE(state_spy.count(), 0);

  Grids::Value test_m;
  test_m[ "_method" ] = debug_string;
  test_m[ "room_id" ] = test_room_id;
  test_m[ "id" ] = test_object_id;
  test_m[ "attr" ][ "lol" ][ "foo" ] = test_attr;
  test_m[ "attr" ][ "lol" ][ "hi" ] = test_attr;
					
  proto.sendRequest(debug_string, &test_m);

  while( wait_timer < wait_limit) {
	if(state_spy.count() > 0)
	  break;
        QTest::qSleep(1000);
	wait_timer++;		
  }
		
  QCOMPARE( tr(debug_string.c_str()), tr(proto.last_event->getEventType().c_str()));		
  QCOMPARE( tr(debug_string.c_str()), tr(proto.last_event->getArgs()[ "_method" ].asString().c_str()));

  QCOMPARE( test_attr, proto.last_event->getArgs()[ "req" ][ "attr" ][ "lol" ][ "foo" ].asUInt() );
  QCOMPARE( test_attr, proto.last_event->getArgs()[ "req" ][ "attr" ][ "lol" ][ "hi" ].asUInt() );
}

void ProtocolTest::getAllRoomsTest(){

}


QTEST_MAIN(ProtocolTest)

#include "protocoltest.moc"

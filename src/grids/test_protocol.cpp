



#include <grids/protocol.h>
#include <grids/event.h>
#include <grids/test_define.h>

#include <gtest/gtest.h>

namespace{


	class ProtocolTest : public ::testing::Test {

	protected:			

		virtual void SetUp(){
			proto->setConnectedCallback( &connectedCallback, this ); 
			proto->setEventCallback( &receiveEvent, this );			

			rec_bool = false;
			con_bool = false;
		}

		static void SetUpTestCase() {
			proto = new Grids::Protocol();
		}
		
		static void TearDownTestCase() {
			proto->closeConnection();
			proto->stopEventLoopThread();

			delete proto;
		}
	
		static void receiveEvent( Grids::Protocol *, Grids::Event *, void *); // Grids protocol object hooks into this
          static void connectedCallback( Grids::Protocol *, Grids::Event *, void *);

		static Grids::Protocol* proto;
		static Grids::GridsID test_room_id;
		static Grids::GridsID test_object_id;

	public:
		Grids::Event rec_event;
		Grids::Event con_event;
		bool rec_bool;
		bool con_bool;
	};
 
	Grids::Protocol* ProtocolTest::proto = NULL;
	Grids::GridsID ProtocolTest::test_room_id;
	Grids::GridsID ProtocolTest::test_object_id;
	
	void ProtocolTest::receiveEvent( Grids::Protocol* in_proto, Grids::Event* in_event, void* self ){
		std::cout << "receiveEvent" << std::endl;

		((ProtocolTest*)self)->rec_bool = true;
		((ProtocolTest*)self)->rec_event = (in_event == NULL ) ? Grids::Event() : Grids::Event( *in_event );
	}

	void ProtocolTest::connectedCallback( Grids::Protocol* in_proto, Grids::Event* in_event, void* self ){
		std::cout << "connectedCallback" << std::endl;
		
		((ProtocolTest*)self)->con_bool = true;		
		((ProtocolTest*)self)->con_event = (in_event == NULL) ? Grids::Event( ) : Grids::Event( *in_event );		
	}
		
	TEST_F(ProtocolTest, constructorTest ){
		// Nothing here		
	}

	TEST_F(ProtocolTest, connectToNodeTest){
		EXPECT_TRUE( proto->connectToNode( GRIDS_SERVER ) );

		proto->runEventLoopThreaded();
		
		EXPECT_TRUE( proto->getThreadId() );
		
		int wait_timer = 0;
		int wait_limit = CALLBACK_SECONDS;
		
		while( wait_timer < wait_limit ){
			if( con_bool )
				break;
			sleep( 1 );
			wait_timer++;
		}
		
		ASSERT_TRUE( con_bool );
	}
	
	TEST_F(ProtocolTest, createRoomTest){
		int wait_timer = 0;
		const int wait_limit = CALLBACK_SECONDS;
		std::string debug_string = GRIDS_CREATE_ROOM;

		Grids::Value test_m;
		test_m["_method"] = GRIDS_CREATE_ROOM;
				
		proto->sendRequest( debug_string, &test_m );

		while( wait_timer < wait_limit) {
			if( rec_bool )
				break;

			sleep( 1 );
			wait_timer++;		
		}
		
		EXPECT_TRUE( rec_bool );
		EXPECT_STREQ( GRIDS_CREATE_ROOM, rec_event.getEventType().c_str() );		
		EXPECT_STREQ( GRIDS_CREATE_ROOM, rec_event.getArgs()[ "_method" ].asString().c_str() );		
		ASSERT_GT( rec_event.getArgs()[ "id" ].asString().length(), 10 );
		
		test_room_id = rec_event.getArgs()[ "id" ].asString();
	}

	TEST_F(ProtocolTest, createObjectTest ){
		int wait_timer = 0;
		const int wait_limit = CALLBACK_SECONDS;
		std::string debug_string = GRIDS_CREATE_OBJECT;
		unsigned int test_attr = 12345556u;

		Grids::Value test_m;
		test_m[ "_method" ] = GRIDS_CREATE_OBJECT;
		test_m[ "room_id" ] = test_room_id;
		test_m[ "attr" ][ "lol" ][ "hi" ] = test_attr;
					
		proto->sendRequest( debug_string, &test_m );

		while( wait_timer < wait_limit) {
			if( rec_bool )
				break;

			sleep( 1 );
			wait_timer++;		
		}
		
		EXPECT_TRUE( rec_bool );

		ASSERT_GT( rec_event.getArgs()[ "id" ].asString().length(), 10 );
		test_object_id = rec_event.getArgs()[ "id" ].asString();

		EXPECT_STREQ( GRIDS_CREATE_OBJECT, rec_event.getEventType().c_str() );		
		EXPECT_STREQ( GRIDS_CREATE_OBJECT, rec_event.getArgs()[ "_method" ].asString().c_str() );

		EXPECT_EQ( test_attr, rec_event.getArgs()[ "req" ][ "attr" ][ "lol" ][ "hi" ].asUInt() );
	}
	
	TEST_F(ProtocolTest, createObjectWithID ){
		int wait_timer = 0;
		const int wait_limit = CALLBACK_SECONDS;
		std::string debug_string = GRIDS_CREATE_OBJECT;
		unsigned int test_attr = 12345556u;
		Grids::GridsID test_id = "1234-2345-1234";
		
		Grids::Value test_m;
		test_m[ "_method" ] = debug_string;
		test_m[ "room_id" ] = test_room_id;
		test_m[ "id" ] = test_id;
		test_m[ "attr" ][ "lol" ][ "hi" ] = test_attr;
					
		proto->sendRequest( debug_string, &test_m );

		while( wait_timer < wait_limit) {
			if( rec_bool )
				break;

			sleep( 1 );
			wait_timer++;		
		}
		
		EXPECT_STREQ( test_id.c_str(), rec_event.getArgs()[ "id" ].asString().c_str() );
	}
	
	TEST_F(ProtocolTest, updateObjectTest ){
		int wait_timer = 0;
		const int wait_limit = CALLBACK_SECONDS;
		std::string debug_string = GRIDS_UPDATE_OBJECT;
		unsigned int test_attr = 992223u;

		Grids::Value test_m;
		test_m[ "_method" ] = debug_string;
		test_m[ "room_id" ] = test_room_id;
		test_m[ "id" ] = test_object_id;
		test_m[ "attr" ][ "lol" ][ "foo" ] = test_attr;
		test_m[ "attr" ][ "lol" ][ "hi" ] = test_attr;
					
		proto->sendRequest( debug_string, &test_m );

		while( wait_timer < wait_limit) {
			if( rec_bool )
				break;

			sleep( 1 );
			wait_timer++;		
		}

		EXPECT_TRUE( rec_bool );
		
		EXPECT_STREQ( debug_string.c_str(), rec_event.getEventType().c_str() );		
		EXPECT_STREQ( debug_string.c_str(), rec_event.getArgs()[ "_method" ].asString().c_str() );

		EXPECT_EQ( test_attr, rec_event.getArgs()[ "req" ][ "attr" ][ "lol" ][ "foo" ].asUInt() );
		EXPECT_EQ( test_attr, rec_event.getArgs()[ "req" ][ "attr" ][ "lol" ][ "hi" ].asUInt() );
	}	   


} // end namespace

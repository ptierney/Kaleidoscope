
#include <grids/protocol.h>
#include <grids/event.h>
#include <kaleidoscope/define.h>

#include <gtest/gtest.h>
#include <SDL/SDL_net.h>

#include <string>
#include <iostream>
#include <assert.h>

namespace {

	class ProtocolTest : public ::testing::Test {

	protected:			

		virtual void SetUp(){
			proto->setConnectedCallback( &connectedCallback, this ); 
			proto->setEventCallback( &receiveEvent, this );			

			rec_bool = false;
			con_bool = false;
		}

		static void SetUpTestCase() {
			if (SDLNet_Init() != 0) {
				std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
				assert( false );
			}

			proto = new Grids::Protocol();
		}
		
		static void TearDownTestCase() {
			proto->closeConnection();
			proto->stopEventLoopThread();

			delete proto;

			SDLNet_Quit();
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
		
	TEST_F(ProtocolTest, createObjectWithPosition){
		int wait_timer = 0;
		const int wait_limit = CALLBACK_SECONDS;
		std::string debug_string = GRIDS_CREATE_OBJECT;
		Grids::Vec3D test_pos = Grids::Vec3D( 4.4, 5.5, 6.6 );
		Grids::Vec3D test_rot = Grids::Vec3D( 1.23, 5.678, 4.0987 );
		Grids::Vec3D test_scl = Grids::Vec3D( 642.1234, 66431.1266, 7993.23885 );
		
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

		proto->sendRequest( debug_string, &test_m );

		while( wait_timer < wait_limit) {
			if( rec_bool )
				break;
			sleep( 1 );
			wait_timer++;		
		}

		EXPECT_FLOAT_EQ( (double)(test_pos.X), rec_event.getArgs()[ "req" ][ "pos" ][ 0u ].asDouble() ); 
		EXPECT_FLOAT_EQ( (double)(test_pos.Y), rec_event.getArgs()[ "req" ][ "pos" ][ 1u ].asDouble() ); 
		EXPECT_FLOAT_EQ( (double)(test_pos.Z), rec_event.getArgs()[ "req" ][ "pos" ][ 2u ].asDouble() ); 

		EXPECT_FLOAT_EQ( (double)(test_rot.X), rec_event.getArgs()[ "req" ][ "rot" ][ 0u ].asDouble() ); 
		EXPECT_FLOAT_EQ( (double)(test_rot.Y), rec_event.getArgs()[ "req" ][ "rot" ][ 1u ].asDouble() ); 
		EXPECT_FLOAT_EQ( (double)(test_rot.Z), rec_event.getArgs()[ "req" ][ "rot" ][ 2u ].asDouble() ); 

		EXPECT_FLOAT_EQ( (double)(test_scl.X), rec_event.getArgs()[ "req" ][ "scl" ][ 0u ].asDouble() ); 
		EXPECT_FLOAT_EQ( (double)(test_scl.Y), rec_event.getArgs()[ "req" ][ "scl" ][ 1u ].asDouble() ); 
		EXPECT_FLOAT_EQ( (double)(test_scl.Z), rec_event.getArgs()[ "req" ][ "scl" ][ 2u ].asDouble() ); 
	}
	
	TEST_F(ProtocolTest, createObjectWithID ){
		int wait_timer = 0;
		const int wait_limit = CALLBACK_SECONDS;
		std::string debug_string = GRIDS_CREATE_OBJECT;
		unsigned int test_attr = 12345556u;
		Grids::GridsID test_id = "DAA10054-7683-11DE-BB26-3FC64C661FD7";
		
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

	TEST_F(ProtocolTest, getAllRoomsTest){
		int wait_timer = 0;
		const int wait_limit = CALLBACK_SECONDS;
		std::string debug_string = GRIDS_LIST_ROOMS;		
		
		proto->sendRequest( GRIDS_LIST_ROOMS, NULL );
		
		sleep( 3 );
	}

} // end namespace

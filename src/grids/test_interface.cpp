
#include <grids/interface.h>
#include <grids/event.h>
#include <grids/utility.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/device.h>

#include <gtest/gtest.h>

#include <vector>
#include <iostream>

namespace{

	class InterfaceTest : public ::testing::Test {
	protected:
		virtual void SetUp(){

		}

		static void SetUpTestCase() {
			d = new Kal::Device();
		}
		
		static void TearDownTestCase() {
			delete d;
		}

		static Kal::Device* d;

		Grids::Utility utility;
		
		Grids::GridsID test_room;
	};

	Kal::Device* InterfaceTest::d = NULL;

	TEST_F(InterfaceTest, constructorTest){
		EXPECT_STREQ( DEFAULT_SERVER, d->getInterface()->getServer().c_str() );
	}
	
	TEST_F(InterfaceTest, requestCreateRoomTest){	
		std::vector< Grids::GridsID > temp_rooms;
		Grids::GridsID temp_room_id = d->getInterface()->createMyRoom(CALLBACK_SECONDS);
		
		EXPECT_TRUE( utility.checkUUIDValidity( temp_room_id ) );		
	
		temp_rooms = d->getInterface()->getKnownRooms();
		bool room_in_vector = false;
		
		for( int i = 0; i < temp_rooms.size(); i++ ){
			if( temp_rooms[i] == temp_room_id )
				room_in_vector = true;
		}

		EXPECT_TRUE( room_in_vector );
		EXPECT_EQ( temp_room_id, d->getInterface()->getMyRoom() );		
		
		test_room = temp_room_id;
	}
	
	TEST_F(InterfaceTest, requestCreateObjectTest){
		Grids::Value attr;
		Grids::GridsID new_id;
		Grids::Vec3D pos_temp = Grids::Vec3D( 1.1f, 2.2f, 3.3f );
		std::string str_temp = "Foo-Bar-Lol";
		
		attr[ "type" ] = "SimpleVector"; 
		attr[ "pos" ][ 0u ] = pos_temp.X;
		attr[ "pos" ][ 1u ] = pos_temp.Y;
		attr[ "pos" ][ 2u ] = pos_temp.Z;
		attr[ "foo" ] = str_temp;

		new_id = d->getInterface()->requestCreateObject( &attr );
		
		EXPECT_TRUE( d->getGridsUtility()->checkUUIDValidity( new_id ) );
	}

} // end namespace

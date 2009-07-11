

#include <grids/interface.h>
#include <grids/event.h>
#include <grids/utility.h>
#include <grids/test_define.h>

#include <gtest/gtest.h>

#include <vector>

namespace{

	class InterfaceTest : public ::testing::Test {
	protected:
		virtual void SetUp(){

		}

		static void SetUpTestCase() {
			inter = new Grids::Interface();
		}
		
		static void TearDownTestCase() {
			
			delete inter;
		}

		static Grids::Interface* inter;
		Grids::Utility utility;
		
		Grids::GridsID test_room;
	};

	Grids::Interface* InterfaceTest::inter = NULL;

	TEST_F(InterfaceTest, ConstructorTest){
		EXPECT_STREQ( DEFAULT_SERVER, inter->getServer().c_str() );
	}
	
	TEST_F(InterfaceTest, requestCreateRoomTest){	
		std::vector< Grids::GridsID > temp_rooms;
		Grids::GridsID temp_room_id = inter->requestCreateRoom();		
		
		EXPECT_TRUE( utility.checkUUIDValidity( temp_room_id ) );		
	
		temp_rooms = inter->getRooms();
		bool room_in_vector = false;
		
		for( int i = 0; i < temp_rooms.size(); i++ ){
			if( temp_rooms[i] == temp_room_id )
				room_in_vector = true;
		}

		EXPECT_TRUE( room_in_vector );
		
		test_room = temp_room_id;
	}
	
	TEST_F(InterfaceTest, requestCreateObjectTest){
		Grids::Value attr;
		Grids::Vec3D pos_temp = Grids::Vec3D( 1.1f, 2.2f, 3.3f );
		std::string str_temp = "Foo-Bar-Lol";
		
		attr[ "pos" ][ 0u ] = pos_temp.X;
		attr[ "pos" ][ 1u ] = pos_temp.Y;
		attr[ "pos" ][ 2u ] = pos_temp.Z;
		attr[ "foo" ] = str_temp;

		inter->requestCreateObject( test_room, &attr );
		
		SUCCEED();
	}

} // end namespace

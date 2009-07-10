

#include <grids/event.h>

#include <gtest/gtest.h>

namespace{

	class EventTest : public ::testing::Test {

	protected:			
		virtual void SetUp(){
			test_val[ "foo" ][ "bar" ][ "lol" ][ "dongs" ] = 111u;
			test_event_type = "Lol.Dongs";
		}

		std::string test_event_type;
		Grids::Value test_val;
	};
		
	TEST_F(EventTest, ConstructorTest){
		Grids::Event temp_evt;
		Grids::Value temp_val = temp_evt.getArgs();
				
		EXPECT_STREQ( GRIDS_NULL_EVENT, temp_evt.getEventType().c_str() );
		EXPECT_TRUE( temp_evt.getArgs().empty() );		
		EXPECT_TRUE( temp_evt.getArgsPtr()->empty() );		
		EXPECT_NE( NULL, (unsigned int)temp_evt.getArgsPtr() );
		
		temp_evt = Grids::Event( test_event_type );
		
		EXPECT_STREQ( test_event_type.c_str(), temp_evt.getEventType().c_str() );
		EXPECT_TRUE( temp_evt.getArgs().empty() );		
		EXPECT_TRUE( temp_evt.getArgsPtr()->empty() );		
		EXPECT_NE( NULL, (unsigned int)temp_evt.getArgsPtr() );
		
		temp_evt = Grids::Event( test_event_type, test_val );
		
		EXPECT_STREQ( test_event_type.c_str(), temp_evt.getEventType().c_str() );
		EXPECT_FALSE( temp_evt.getArgs().empty() );		
		EXPECT_FALSE( temp_evt.getArgsPtr()->empty() );		
		EXPECT_NE( NULL, (unsigned int)temp_evt.getArgsPtr() );
		
		EXPECT_EQ( 111u, temp_evt.getArgs()[ "foo" ][ "bar" ][ "lol" ][ "dongs" ].asUInt() );
		EXPECT_EQ( 111u, (*(temp_evt.getArgsPtr()))[ "foo" ][ "bar" ][ "lol" ][ "dongs" ].asUInt() );

		Grids::Event temp_evt_2( temp_evt );

		EXPECT_EQ( 111u, temp_evt_2.getArgs()[ "foo" ][ "bar" ][ "lol" ][ "dongs" ].asUInt() );
		EXPECT_EQ( 111u, (*(temp_evt_2.getArgsPtr()))[ "foo" ][ "bar" ][ "lol" ][ "dongs" ].asUInt() );

		Grids::Event* temp_evt_3 = new Grids::Event( temp_evt );
		Grids::Event* temp_evt_4 = new Grids::Event( *temp_evt_3 );
		delete temp_evt_3;

		EXPECT_EQ( 111u, temp_evt_4->getArgs()[ "foo" ][ "bar" ][ "lol" ][ "dongs" ].asUInt() );
		EXPECT_EQ( 111u, (*(temp_evt_4->getArgsPtr()))[ "foo" ][ "bar" ][ "lol" ][ "dongs" ].asUInt() );
				
	}

	TEST_F(EventTest, getSetEventTypeTest){
		Grids::Event temp_evt;
		std::string temp_string = "foo.bar";

		temp_evt.setEvent( temp_string );

		EXPECT_STREQ( temp_string.c_str(), temp_evt.getEventType().c_str() );

	}

	TEST_F(EventTest, getSetArgsTest) {
		Grids::Event temp_evt;
		
		temp_evt.setArgs( test_val );
		
		EXPECT_EQ( 111u, temp_evt.getArgs()[ "foo" ][ "bar" ][ "lol" ][ "dongs" ].asUInt() );
	}

}


#include <grids/object.h>
#include <kaleidoscope/device.h>
#include <grids/utility.h>

#include <gtest/gtest.h>

#include <string>

namespace {

	class ObjectTest : public ::testing::Test {
	protected:
		virtual void SetUp(){
		}
		
		virtual void TearDown(){
		}
		
		static void SetUpTestCase() {
			d = new Kal::Device();

			util = new Grids::Utility();
			val = new Grids::Value();
			(*val)[ "id" ] = util->getNewUUID();
			(*val)[ "req" ][ "attr" ][ "foo" ] = attr_foo;
			(*val)[ "req" ][ "attr" ][ "bar" ] = attr_bar;
		}
		
		static void TearDownTestCase() {
			delete d;
			delete util;
			delete val;
		}
		
		Grids::Object* obj;
		static Grids::Value* val;
		static Grids::Utility* util;

		static Kal::Device* d;
		static int attr_foo;
		static std::string attr_bar;
	};
		
	Kal::Device* ObjectTest::d = NULL;
	Grids::Value* ObjectTest::val = NULL;
	Grids::Utility* ObjectTest::util = NULL;

	int ObjectTest::attr_foo = 12341234;
	std::string ObjectTest::attr_bar = "aoeuaoeuaoeu";

	TEST_F(ObjectTest, constructorTest ){
		Grids::Value temp_val;
		//temp_val[ "req" ][ "attr" ][ "foo" ] = 12312;
		//temp_val[ "req" ][ "attr" ][ "bar" ] = "aoeuaoeu";
		/*	
		obj = new Grids::Object(d, &temp_val);

		EXPECT_EQ( attr_foo, (*(obj->getAttr()))[ "foo" ].asInt() );
		EXPECT_EQ( attr_bar, (*(obj->getAttr()))[ "bar" ].asString() );

		delete obj;
		*/
		SUCCEED();
	}

} // end namespace

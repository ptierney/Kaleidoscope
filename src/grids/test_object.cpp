
#include <grids/object.h>
#include <kaleidoscope/device.h>
#include <grids/utility.h>

#include <gtest/gtest.h>


namespace {

	class ObjectTest : public ::testing::Test {
	protected:
		virtual void SetUp(){
			obj = new Grids::Object( d, );
		}
		
		virtual void TearDown(){
			delete obj;
		}
		
		static void SetUpTestcase() {
			d = new Kal::Device();
			util = new Grids::Utility();
			val = new Grids::Value();
			(*val)[ "id" ] = util->getNewUUID();
			val->[ "req" ][ "attr" ][ "foo" ] = attr_foo;
		}
		
		static void TearDownTestCase() {
			delete d;
			delete util;
			delete value;
		}
		
		Grids::Object* obj;

		static Kal::Device* d;
		static Grids::Value* val;
		static Grids::Utility* util;
		
		static int attr_foo;
	};
		
	Kal::Device* ObjectTest::d = NULL;
	Grids::Value* ObjectTest::val = NULL;
	Grids::Utility* ObjectTest::util = NULL;
	int ObjectTest::attr_foo = 12341234;

	TEST_F(ObjectTest, constructorTest ){


	}

} // end namespace



#include <grids/utility.h>

#include <gtest/gtest.h>



namespace {

	class GridsUtilityTest : public ::testing::Test {
	protected:

		Grids::Utility test_utility;
	};

	TEST_F(GridsUtilityTest, checkValidityTest){
		std::string real_uuid_upper = "4AA311CA-6DBB-11DE-B84C-43FC4C661FD7";
		std::string real_uuid_lower = "3a4a2146-58de-4bdd-b44b-d411d278f8fc";
		std::string fake_uuid_1 = "4444444";
		std::string fake_uuid_2 = "4AA311II-6QQQ-11DE-B84C-43FC4C661FD7";

		EXPECT_TRUE( test_utility.checkUUIDValidity( real_uuid_upper ) );
		EXPECT_TRUE( test_utility.checkUUIDValidity( real_uuid_lower ) );
		EXPECT_FALSE( test_utility.checkUUIDValidity( fake_uuid_1 ) );
		EXPECT_FALSE( test_utility.checkUUIDValidity( fake_uuid_2 ) );
	}
	   
	TEST_F(GridsUtilityTest, getNewUUIDTest){
		std::string uuid_1 = test_utility.getNewUUID();
		std::string uuid_2 = test_utility.getNewUUID();
		
		EXPECT_STRNE( uuid_1.c_str(), uuid_2.c_str() );
		
		EXPECT_TRUE( test_utility.checkUUIDValidity( uuid_1 ) );
		EXPECT_TRUE( test_utility.checkUUIDValidity( uuid_2 ) );
	}
	
	




} // end namespace



#include <grids/SDLObject.h>

#include <gtest/gtest.h>

namespace {

	class SDLObjectTest : public ::testing::Test {
	protected:

		static void SetUpTestCase() {

		}
		
		static void TearDownTestCase() {

		}

		static int SDLObjectTestEntryPoint( void* );
		
		void runLoop();
		
		static SDL_Thread* thread_1;
		static SDL_Thread* thread_2;
	};

	int SDLObjectTest::SDLObjectTestEntryPoint( void* arg ){
		SDLObjectTest* gb = (SDLObjectTest*)arg;
		gb->runLoop();
		return 0;
	}
	
	void SDLObjectTest::runLoop(){

	}	

	TEST_F(SDLObjectTest, lockingTest){
		//TO DO: Write a locking test
		FAIL();
	}

} // end namespace

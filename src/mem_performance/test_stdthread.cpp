//
//
//

#include <gtest/gtest.h>

#include <thread>


static void threaded_method() {
	//
}


///
/// In Raspbian there is Clang compiler error causing threads to crash with message:
///			pure virtual method called
///			terminate called without an active exception
///
TEST(stdthread, run) {
	std::thread worker( &threaded_method );
	worker.join();
    EXPECT_TRUE( true );
}

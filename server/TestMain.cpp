#include "gtest/gtest.h"
#include "util/Log.h"

int main(int argc, char** argv)
{
    Log::init(LogLevel::DEBUG);
    
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
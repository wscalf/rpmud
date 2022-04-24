#include "util/UUID.h"
#include "gtest/gtest.h"

TEST(unit_tests, uuid_equals)
{
    UUID first = UUID::create();
    UUID second = first;
    EXPECT_EQ(first, second);
}

TEST(unit_tests, uuid_not_equals)
{
    UUID first = UUID::create();
    UUID second = UUID::create();
    EXPECT_NE(first, second);
}
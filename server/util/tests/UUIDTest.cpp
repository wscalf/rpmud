#include "util/UUID.h"
#include "gtest/gtest.h"

TEST(unit_tests, uuid_equals)
{
    UUID first = UUID::create();
    UUID second = first;
    EXPECT_EQ(first, second);
}

TEST(unit_tests, uuids_are_random)
{
    UUID first = UUID::create();
    UUID second = UUID::create();
    EXPECT_NE(first, second);
}

TEST(unit_tests, uuid_empty_equals_empty)
{
    EXPECT_EQ(UUID::empty, UUID::empty);
}

TEST(unit_tests, uuid_empty_tostr_correct)
{
    EXPECT_EQ("00000000-0000-0000-0000-000000000000", UUID::empty.toStr());
}

TEST(unit_tests, uuid_empty_not_equals_random)
{
    EXPECT_NE(UUID::empty, UUID::create());
}
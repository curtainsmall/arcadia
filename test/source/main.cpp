#include "gtest/gtest.h"

TEST(HelloTest, BasicAssertions)
{
    EXPECT_STRNE("hellp", "world");
    EXPECT_EQ(6 * 7, 42);
}

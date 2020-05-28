#include "gtest/gtest.h"

#include <stdexcept>

#include "Stack.hpp"

TEST(TestStack, TestSwapTN)
{
    auto s = Stack<uint16_t>(5);
    s.Push(1);
    s.Push(2);
    s.Push(3);

    EXPECT_EQ(s.SwapTN(), 0);
    EXPECT_EQ(s.Top(), 2);
    s.Pop();
    EXPECT_EQ(s.Top(), 3);

    // Not enough elements
    s.Pop();
    EXPECT_EQ(s.SwapTN(), -1);
    s.Pop();
    EXPECT_EQ(s.SwapTN(), -1);
}

TEST(TestStack, TestCommon)
{
    auto s = Stack<uint16_t>(5);
    EXPECT_EQ(s.Depth(), 0);
    s.Push(1);
    EXPECT_EQ(s.Depth(), 1);
    EXPECT_EQ(s.Top().value_or(0xff), 1);
    EXPECT_EQ(s.Pop().value_or(0xff), 1);
    EXPECT_EQ(s.Pop().value_or(0xff), 0xff);
    EXPECT_EQ(s.Top().value_or(0xff), 0xff);
    EXPECT_EQ(s.Depth(), 0);
}

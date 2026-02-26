#include <gtest/gtest.h>
#include <string>
#include "../src/solution.h"

TEST(SwapTest, MySwapBasic) {
    int a = 3, b = 7;
    my_swap(a, b);
    EXPECT_EQ(a, 7);
    EXPECT_EQ(b, 3);
}

TEST(SwapTest, MySwapSameValue) {
    int a = 5, b = 5;
    my_swap(a, b);
    EXPECT_EQ(a, 5);
    EXPECT_EQ(b, 5);
}

TEST(SwapTest, MySwapTempBasic) {
    int a = 10, b = 20;
    my_swap_temp(a, b);
    EXPECT_EQ(a, 20);
    EXPECT_EQ(b, 10);
}

TEST(SwapTest, MySwapTempNegative) {
    int a = -1, b = 1;
    my_swap_temp(a, b);
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, -1);
}

TEST(SortThree, AlreadySorted) {
    int a = 1, b = 2, c = 3;
    sort_three(a, b, c);
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(c, 3);
}

TEST(SortThree, Reversed) {
    int a = 3, b = 2, c = 1;
    sort_three(a, b, c);
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(c, 3);
}

TEST(SortThree, Mixed) {
    int a = 2, b = 3, c = 1;
    sort_three(a, b, c);
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(c, 3);
}

TEST(SortThree, Duplicates) {
    int a = 5, b = 5, c = 3;
    sort_three(a, b, c);
    EXPECT_EQ(a, 3);
    EXPECT_EQ(b, 5);
    EXPECT_EQ(c, 5);
}

TEST(GenericSwap, Integers) {
    int a = 1, b = 2;
    generic_swap(a, b);
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 1);
}

TEST(GenericSwap, Strings) {
    std::string a = "hello", b = "world";
    generic_swap(a, b);
    EXPECT_EQ(a, "world");
    EXPECT_EQ(b, "hello");
}

TEST(GenericSwap, Doubles) {
    double a = 1.5, b = 2.5;
    generic_swap(a, b);
    EXPECT_DOUBLE_EQ(a, 2.5);
    EXPECT_DOUBLE_EQ(b, 1.5);
}

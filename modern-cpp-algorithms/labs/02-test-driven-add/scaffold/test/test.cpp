#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(AddTest, PositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(100, 200), 300);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(add(-1, -2), -3);
}

TEST(AddTest, Zero) {
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(0, 5), 5);
}

TEST(SubtractTest, Basic) {
    EXPECT_EQ(subtract(5, 3), 2);
    EXPECT_EQ(subtract(3, 5), -2);
}

TEST(SubtractTest, Zero) {
    EXPECT_EQ(subtract(0, 0), 0);
    EXPECT_EQ(subtract(7, 0), 7);
}

TEST(MultiplyTest, Basic) {
    EXPECT_EQ(multiply(3, 4), 12);
    EXPECT_EQ(multiply(-2, 3), -6);
}

TEST(MultiplyTest, Zero) {
    EXPECT_EQ(multiply(0, 100), 0);
    EXPECT_EQ(multiply(100, 0), 0);
}

TEST(ClampTest, WithinRange) {
    EXPECT_EQ(clamp(5, 0, 10), 5);
}

TEST(ClampTest, BelowRange) {
    EXPECT_EQ(clamp(-5, 0, 10), 0);
}

TEST(ClampTest, AboveRange) {
    EXPECT_EQ(clamp(15, 0, 10), 10);
}

TEST(ClampTest, AtBoundaries) {
    EXPECT_EQ(clamp(0, 0, 10), 0);
    EXPECT_EQ(clamp(10, 0, 10), 10);
}

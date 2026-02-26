#include <gtest/gtest.h>
#include "../src/solution.h"

// --- next_greater ---

TEST(NextGreater, Basic) {
    auto r = next_greater({2,1,2,4,3});
    EXPECT_EQ(r[0], 4);
    EXPECT_EQ(r[1], 2);
    EXPECT_EQ(r[2], 4);
    EXPECT_EQ(r[3], -1);
    EXPECT_EQ(r[4], -1);
}

TEST(NextGreater, Ascending) {
    auto r = next_greater({1,2,3,4});
    EXPECT_EQ(r[0], 2);
    EXPECT_EQ(r[1], 3);
    EXPECT_EQ(r[2], 4);
    EXPECT_EQ(r[3], -1);
}

TEST(NextGreater, Descending) {
    auto r = next_greater({4,3,2,1});
    EXPECT_EQ(r[0], -1);
    EXPECT_EQ(r[1], -1);
    EXPECT_EQ(r[2], -1);
    EXPECT_EQ(r[3], -1);
}

// --- daily_temperatures ---

TEST(DailyTemperatures, Typical) {
    auto r = daily_temperatures({73,74,75,71,69,72,76,73});
    EXPECT_EQ(r[0], 1);
    EXPECT_EQ(r[1], 1);
    EXPECT_EQ(r[2], 4);
    EXPECT_EQ(r[3], 2);
    EXPECT_EQ(r[4], 1);
    EXPECT_EQ(r[5], 1);
    EXPECT_EQ(r[6], 0);
    EXPECT_EQ(r[7], 0);
}

TEST(DailyTemperatures, NeverWarmer) {
    auto r = daily_temperatures({30,20,10});
    EXPECT_EQ(r[0], 0);
    EXPECT_EQ(r[1], 0);
    EXPECT_EQ(r[2], 0);
}

// --- largest_rectangle ---

TEST(LargestRectangle, Classic) {
    EXPECT_EQ(largest_rectangle({2,1,5,6,2,3}), 10);
}

TEST(LargestRectangle, Uniform) {
    EXPECT_EQ(largest_rectangle({3,3,3,3}), 12);
}

TEST(LargestRectangle, SingleBar) {
    EXPECT_EQ(largest_rectangle({5}), 5);
}

TEST(LargestRectangle, Ascending) {
    EXPECT_EQ(largest_rectangle({1,2,3,4,5}), 9);
}

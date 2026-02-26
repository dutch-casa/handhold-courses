#include <gtest/gtest.h>
#include "../src/solution.h"
#include <algorithm>

TEST(TwoSumSorted, BasicCase) {
    auto [a, b] = two_sum_sorted({2, 7, 11, 15}, 9);
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
}

TEST(TwoSumSorted, NegativeNumbers) {
    auto [a, b] = two_sum_sorted({-3, -1, 0, 2, 4}, 1);
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 4);
}

TEST(TwoSumSorted, LargeIndices) {
    auto [a, b] = two_sum_sorted({1, 2, 3, 4, 5, 6}, 11);
    EXPECT_EQ(a, 5);
    EXPECT_EQ(b, 6);
}

TEST(ThreeSum, BasicCase) {
    auto result = three_sum({-1, 0, 1, 2, -1, -4});
    std::vector<std::vector<int>> expected = {{-1, -1, 2}, {-1, 0, 1}};
    for (auto& v : result) std::sort(v.begin(), v.end());
    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST(ThreeSum, AllZeros) {
    auto result = three_sum({0, 0, 0, 0});
    EXPECT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], (std::vector<int>{0, 0, 0}));
}

TEST(ThreeSum, NoSolution) {
    auto result = three_sum({1, 2, 3});
    EXPECT_TRUE(result.empty());
}

TEST(MaxWater, BasicCase) {
    EXPECT_EQ(max_water({1, 8, 6, 2, 5, 4, 8, 3, 7}), 49);
}

TEST(MaxWater, TwoElements) {
    EXPECT_EQ(max_water({1, 1}), 1);
}

TEST(MaxWater, Descending) {
    EXPECT_EQ(max_water({5, 4, 3, 2, 1}), 6);
}

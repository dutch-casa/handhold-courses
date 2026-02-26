#include <gtest/gtest.h>
#include "../src/solution.h"
#include <algorithm>
#include <set>

// Normalize results for comparison: sort each inner vector, then sort outer
static std::vector<std::vector<int>> normalize(std::vector<std::vector<int>> v) {
    for (auto& inner : v) std::sort(inner.begin(), inner.end());
    std::sort(v.begin(), v.end());
    return v;
}

TEST(Permutations, ThreeElements) {
    auto result = permutations({1, 2, 3});
    EXPECT_EQ(result.size(), 6u);
    // Verify all unique
    std::set<std::vector<int>> unique(result.begin(), result.end());
    EXPECT_EQ(unique.size(), 6u);
}

TEST(Permutations, SingleElement) {
    auto result = permutations({42});
    EXPECT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], std::vector<int>{42});
}

TEST(Permutations, TwoElements) {
    auto result = permutations({1, 2});
    EXPECT_EQ(result.size(), 2u);
    std::set<std::vector<int>> unique(result.begin(), result.end());
    EXPECT_TRUE(unique.count({1, 2}));
    EXPECT_TRUE(unique.count({2, 1}));
}

TEST(Combinations, C4_2) {
    auto result = normalize(combinations(4, 2));
    std::vector<std::vector<int>> expected = {
        {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}
    };
    EXPECT_EQ(result, normalize(expected));
}

TEST(Combinations, C3_1) {
    auto result = normalize(combinations(3, 1));
    std::vector<std::vector<int>> expected = {{1}, {2}, {3}};
    EXPECT_EQ(result, normalize(expected));
}

TEST(Combinations, C3_3) {
    auto result = normalize(combinations(3, 3));
    std::vector<std::vector<int>> expected = {{1, 2, 3}};
    EXPECT_EQ(result, normalize(expected));
}

TEST(Subsets, ThreeElements) {
    auto result = normalize(subsets({1, 2, 3}));
    std::vector<std::vector<int>> expected = {
        {}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}
    };
    EXPECT_EQ(result, normalize(expected));
}

TEST(Subsets, Empty) {
    auto result = subsets({});
    EXPECT_EQ(result.size(), 1u);
    EXPECT_TRUE(result[0].empty());
}

TEST(Subsets, SingleElement) {
    auto result = normalize(subsets({5}));
    std::vector<std::vector<int>> expected = {{}, {5}};
    EXPECT_EQ(result, normalize(expected));
}

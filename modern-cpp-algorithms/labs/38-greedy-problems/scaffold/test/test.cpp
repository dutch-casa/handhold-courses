#include <gtest/gtest.h>
#include "../src/solution.h"

// --- can_jump ---

TEST(CanJump, CanReach) {
    EXPECT_TRUE(can_jump({2,3,1,1,4}));
}

TEST(CanJump, CannotReach) {
    EXPECT_FALSE(can_jump({3,2,1,0,4}));
}

TEST(CanJump, SingleElement) {
    EXPECT_TRUE(can_jump({0}));
}

// --- min_jumps ---

TEST(MinJumps, Simple) {
    EXPECT_EQ(min_jumps({2,3,1,1,4}), 2);
}

TEST(MinJumps, SingleStep) {
    EXPECT_EQ(min_jumps({1,1,1,1}), 3);
}

TEST(MinJumps, AlreadyAtEnd) {
    EXPECT_EQ(min_jumps({0}), 0);
}

// --- gas_station ---

TEST(GasStation, Possible) {
    EXPECT_EQ(gas_station({1,2,3,4,5}, {3,4,5,1,2}), 3);
}

TEST(GasStation, Impossible) {
    EXPECT_EQ(gas_station({2,3,4}, {3,4,3}), -1);
}

// --- partition_labels ---

TEST(PartitionLabels, Classic) {
    auto r = partition_labels("ababcbacadefegdehijhklij");
    EXPECT_EQ(r, (std::vector<int>{9,7,8}));
}

TEST(PartitionLabels, AllSame) {
    auto r = partition_labels("aaaa");
    EXPECT_EQ(r, (std::vector<int>{4}));
}

TEST(PartitionLabels, AllDistinct) {
    auto r = partition_labels("abcd");
    EXPECT_EQ(r, (std::vector<int>{1,1,1,1}));
}

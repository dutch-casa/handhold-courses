#include <gtest/gtest.h>
#include "../src/solution.h"

// --- climb_stairs ---

TEST(ClimbStairs, BaseOne) {
    EXPECT_EQ(climb_stairs(1), 1);
}

TEST(ClimbStairs, BaseTwo) {
    EXPECT_EQ(climb_stairs(2), 2);
}

TEST(ClimbStairs, Five) {
    // 1+1+1+1+1, 1+1+1+2, 1+1+2+1, 1+2+1+1, 2+1+1+1, 2+2+1, 2+1+2, 1+2+2 => 8
    EXPECT_EQ(climb_stairs(5), 8);
}

TEST(ClimbStairs, Ten) {
    EXPECT_EQ(climb_stairs(10), 89);
}

// --- house_robber ---

TEST(HouseRobber, Basic) {
    // [2,7,9,3,1] => rob 2+9+1 = 12
    EXPECT_EQ(house_robber({2, 7, 9, 3, 1}), 12);
}

TEST(HouseRobber, TwoElements) {
    EXPECT_EQ(house_robber({1, 2}), 2);
}

TEST(HouseRobber, AllSame) {
    // [3,3,3,3] => rob indices 0,2 or 1,3 => 6
    EXPECT_EQ(house_robber({3, 3, 3, 3}), 6);
}

TEST(HouseRobber, SingleElement) {
    EXPECT_EQ(house_robber({42}), 42);
}

// --- coin_change ---

TEST(CoinChange, Example1) {
    // coins [1,5,6,9], amount 11 => 2 (5+6)
    EXPECT_EQ(coin_change({1, 5, 6, 9}, 11), 2);
}

TEST(CoinChange, Impossible) {
    EXPECT_EQ(coin_change({2}, 3), -1);
}

TEST(CoinChange, ZeroAmount) {
    EXPECT_EQ(coin_change({1, 2, 5}, 0), 0);
}

TEST(CoinChange, LeetCodeExample) {
    // coins [1,2,5], amount 11 => 3 (5+5+1)
    EXPECT_EQ(coin_change({1, 2, 5}, 11), 3);
}

// --- lis_length ---

TEST(LIS, Classic) {
    // [10,9,2,5,3,7,101,18] => LIS [2,3,7,101] length 4
    EXPECT_EQ(lis_length({10, 9, 2, 5, 3, 7, 101, 18}), 4);
}

TEST(LIS, AllIncreasing) {
    EXPECT_EQ(lis_length({1, 2, 3, 4, 5}), 5);
}

TEST(LIS, AllDecreasing) {
    EXPECT_EQ(lis_length({5, 4, 3, 2, 1}), 1);
}

TEST(LIS, SingleElement) {
    EXPECT_EQ(lis_length({7}), 1);
}

#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(NumIslands, SingleIsland) {
    std::vector<std::vector<char>> grid = {
        {'1', '1', '1'},
        {'1', '1', '0'},
        {'0', '0', '0'}
    };
    EXPECT_EQ(num_islands(grid), 1);
}

TEST(NumIslands, ThreeIslands) {
    std::vector<std::vector<char>> grid = {
        {'1', '0', '1'},
        {'0', '0', '0'},
        {'1', '0', '0'}
    };
    EXPECT_EQ(num_islands(grid), 3);
}

TEST(NumIslands, AllWater) {
    std::vector<std::vector<char>> grid = {
        {'0', '0'},
        {'0', '0'}
    };
    EXPECT_EQ(num_islands(grid), 0);
}

TEST(NumIslands, AllLand) {
    std::vector<std::vector<char>> grid = {
        {'1', '1'},
        {'1', '1'}
    };
    EXPECT_EQ(num_islands(grid), 1);
}

TEST(CountComponents, TwoComponents) {
    EXPECT_EQ(count_components(5, {{0, 1}, {1, 2}, {3, 4}}), 2);
}

TEST(CountComponents, AllDisconnected) {
    EXPECT_EQ(count_components(4, {}), 4);
}

TEST(CountComponents, FullyConnected) {
    EXPECT_EQ(count_components(3, {{0, 1}, {1, 2}}), 1);
}

TEST(CanFinish, NoCycle) {
    EXPECT_TRUE(can_finish(4, {{1, 0}, {2, 1}, {3, 2}}));
}

TEST(CanFinish, HasCycle) {
    EXPECT_FALSE(can_finish(2, {{0, 1}, {1, 0}}));
}

TEST(CanFinish, NoDeps) {
    EXPECT_TRUE(can_finish(3, {}));
}

TEST(CanFinish, DiamondNoCycle) {
    EXPECT_TRUE(can_finish(4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}));
}

TEST(CanFinish, LargeCycle) {
    EXPECT_FALSE(can_finish(3, {{0, 1}, {1, 2}, {2, 0}}));
}

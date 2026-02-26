#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(ShortestPath, DirectConnection) {
    std::vector<std::vector<int>> adj = {{1}, {0, 2}, {1}};
    EXPECT_EQ(shortest_path(adj, 0, 2), 2);
}

TEST(ShortestPath, SameNode) {
    std::vector<std::vector<int>> adj = {{1}, {0}};
    EXPECT_EQ(shortest_path(adj, 0, 0), 0);
}

TEST(ShortestPath, Unreachable) {
    std::vector<std::vector<int>> adj = {{1}, {0}, {3}, {2}};
    EXPECT_EQ(shortest_path(adj, 0, 3), -1);
}

TEST(ShortestPath, MultiplePaths) {
    // 0-1-2-3 and 0-3
    std::vector<std::vector<int>> adj = {{1, 3}, {0, 2}, {1, 3}, {2, 0}};
    EXPECT_EQ(shortest_path(adj, 0, 3), 1);
}

TEST(ShortestPath, LargerGraph) {
    // 0-1, 0-2, 1-3, 2-3, 3-4
    std::vector<std::vector<int>> adj = {{1, 2}, {0, 3}, {0, 3}, {1, 2, 4}, {3}};
    EXPECT_EQ(shortest_path(adj, 0, 4), 3);
}

TEST(OrangesRotting, BasicSpread) {
    std::vector<std::vector<int>> grid = {
        {2, 1, 1},
        {1, 1, 0},
        {0, 1, 1}
    };
    EXPECT_EQ(oranges_rotting(grid), 4);
}

TEST(OrangesRotting, Impossible) {
    std::vector<std::vector<int>> grid = {
        {2, 1, 1},
        {0, 1, 1},
        {1, 0, 1}
    };
    EXPECT_EQ(oranges_rotting(grid), -1);
}

TEST(OrangesRotting, AlreadyRotten) {
    std::vector<std::vector<int>> grid = {{0, 2}};
    EXPECT_EQ(oranges_rotting(grid), 0);
}

TEST(OrangesRotting, NoOranges) {
    std::vector<std::vector<int>> grid = {{0}};
    EXPECT_EQ(oranges_rotting(grid), 0);
}

TEST(OrangesRotting, MultipleSources) {
    std::vector<std::vector<int>> grid = {
        {2, 1, 1},
        {1, 1, 1},
        {1, 1, 2}
    };
    EXPECT_EQ(oranges_rotting(grid), 2);
}

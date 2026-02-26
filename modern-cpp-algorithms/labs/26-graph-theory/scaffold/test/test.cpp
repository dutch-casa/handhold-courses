#include <gtest/gtest.h>
#include "../src/solution.h"
#include <algorithm>
#include <climits>

// --- floyd_warshall ---

TEST(FloydWarshall, ThreeNodeCycle) {
    // 0->1(3), 1->2(2), 0->2(8), 2->0(1)
    std::vector<std::tuple<int,int,int>> edges = {{0,1,3},{1,2,2},{0,2,8},{2,0,1}};
    auto d = floyd_warshall(3, edges);
    EXPECT_EQ(d[0][0], 0);
    EXPECT_EQ(d[0][1], 3);
    EXPECT_EQ(d[0][2], 5);  // 0->1->2
    EXPECT_EQ(d[2][0], 1);
    EXPECT_EQ(d[1][0], 3);  // 1->2->0
}

TEST(FloydWarshall, NoPath) {
    // 0->1(1), node 2 is isolated => dist[0][2] = INT_MAX/2
    std::vector<std::tuple<int,int,int>> edges = {{0,1,1}};
    auto d = floyd_warshall(3, edges);
    EXPECT_EQ(d[0][1], 1);
    EXPECT_GE(d[0][2], INT_MAX / 2);
}

TEST(FloydWarshall, SingleNode) {
    auto d = floyd_warshall(1, {});
    ASSERT_EQ(static_cast<int>(d.size()), 1);
    EXPECT_EQ(d[0][0], 0);
}

// --- is_bipartite ---

TEST(IsBipartite, EvenCycle) {
    // 0-1-2-3-0 (4-cycle is bipartite)
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,0}};
    EXPECT_TRUE(is_bipartite(4, edges));
}

TEST(IsBipartite, OddCycle) {
    // Triangle 0-1-2-0 is not bipartite
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,0}};
    EXPECT_FALSE(is_bipartite(3, edges));
}

TEST(IsBipartite, EmptyGraph) {
    EXPECT_TRUE(is_bipartite(3, {}));
}

TEST(IsBipartite, DisconnectedOddCycle) {
    // Component 0 is isolated, component 1-2-3 is a triangle
    std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
    EXPECT_FALSE(is_bipartite(4, edges));
}

// --- find_bridges ---

TEST(FindBridges, SingleBridge) {
    // 0-1-2, 1-3 forms a tree; all edges are bridges
    // But only check the obvious one: a linear chain 0-1-2
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2}};
    auto bridges = find_bridges(3, edges);
    std::sort(bridges.begin(), bridges.end());
    ASSERT_EQ(static_cast<int>(bridges.size()), 2);
}

TEST(FindBridges, NoBridges) {
    // 4-cycle: 0-1-2-3-0; no bridges
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,0}};
    auto bridges = find_bridges(4, edges);
    EXPECT_TRUE(bridges.empty());
}

TEST(FindBridges, MixedGraph) {
    // 0-1, 1-2, 2-0 (triangle) plus bridge 0-3
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,0},{0,3}};
    auto bridges = find_bridges(4, edges);
    ASSERT_EQ(static_cast<int>(bridges.size()), 1);
    EXPECT_EQ(bridges[0], std::make_pair(0, 3));
}

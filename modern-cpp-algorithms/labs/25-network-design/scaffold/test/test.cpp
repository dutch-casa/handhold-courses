#include <gtest/gtest.h>
#include "../src/solution.h"
#include <climits>

// --- kruskal_mst ---

TEST(KruskalMST, SimpleSquare) {
    // 4 nodes, edges: 0-1(1), 0-2(4), 1-2(2), 1-3(5), 2-3(3)
    // MST picks: 0-1(1), 1-2(2), 2-3(3)  => total 6
    std::vector<std::tuple<int,int,int>> edges = {
        {0,1,1},{0,2,4},{1,2,2},{1,3,5},{2,3,3}
    };
    EXPECT_EQ(kruskal_mst(4, edges), 6);
}

TEST(KruskalMST, AlreadyTree) {
    // Path graph 0-1-2-3 with weights 1,1,1  => MST = 3
    std::vector<std::tuple<int,int,int>> edges = {{0,1,1},{1,2,1},{2,3,1}};
    EXPECT_EQ(kruskal_mst(4, edges), 3);
}

TEST(KruskalMST, SingleNode) {
    EXPECT_EQ(kruskal_mst(1, {}), 0);
}

TEST(KruskalMST, TwoNodes) {
    std::vector<std::tuple<int,int,int>> edges = {{0,1,7}};
    EXPECT_EQ(kruskal_mst(2, edges), 7);
}

// --- prim_mst ---

TEST(PrimMST, SimpleSquare) {
    // Same graph as Kruskal test => MST cost 6
    std::vector<std::vector<std::pair<int,int>>> adj(4);
    adj[0] = {{1,1},{2,4}};
    adj[1] = {{0,1},{2,2},{3,5}};
    adj[2] = {{0,4},{1,2},{3,3}};
    adj[3] = {{1,5},{2,3}};
    EXPECT_EQ(prim_mst(4, adj), 6);
}

TEST(PrimMST, SingleNode) {
    std::vector<std::vector<std::pair<int,int>>> adj(1);
    EXPECT_EQ(prim_mst(1, adj), 0);
}

// --- min_cost_connect ---

TEST(MinCostConnect, BasicCase) {
    // LeetCode 1135 example: n=3, [[1,2,5],[1,3,6],[2,3,1]]  => 6
    std::vector<std::vector<int>> conn = {{1,2,5},{1,3,6},{2,3,1}};
    EXPECT_EQ(min_cost_connect(3, conn), 6);
}

TEST(MinCostConnect, Impossible) {
    // n=4 but only one edge connecting two nodes => impossible
    std::vector<std::vector<int>> conn = {{1,2,3}};
    EXPECT_EQ(min_cost_connect(4, conn), -1);
}

TEST(MinCostConnect, SingleCity) {
    EXPECT_EQ(min_cost_connect(1, {}), 0);
}

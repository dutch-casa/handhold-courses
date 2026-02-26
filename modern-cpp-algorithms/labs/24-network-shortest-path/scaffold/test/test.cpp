#include <gtest/gtest.h>
#include "../src/solution.h"
#include <climits>

// --- dijkstra ---

TEST(Dijkstra, SimpleTriangle) {
    // 0->1 (4), 0->2 (1), 2->1 (2)  =>  dist = [0, 3, 1]
    std::vector<Edge> g = {{0,1,4},{0,2,1},{2,1,2}};
    auto d = dijkstra(g, 3, 0);
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[1], 3);
    EXPECT_EQ(d[2], 1);
}

TEST(Dijkstra, DisconnectedNode) {
    // node 3 is unreachable
    std::vector<Edge> g = {{0,1,2},{1,2,3}};
    auto d = dijkstra(g, 4, 0);
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[3], INT_MAX);
}

TEST(Dijkstra, SingleNode) {
    auto d = dijkstra({}, 1, 0);
    EXPECT_EQ(static_cast<int>(d.size()), 1);
    EXPECT_EQ(d[0], 0);
}

TEST(Dijkstra, LongerPath) {
    // 0->1(10), 0->2(3), 2->1(4), 1->3(2), 2->3(8)  =>  [0,7,3,9]
    std::vector<Edge> g = {{0,1,10},{0,2,3},{2,1,4},{1,3,2},{2,3,8}};
    auto d = dijkstra(g, 4, 0);
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[1], 7);
    EXPECT_EQ(d[2], 3);
    EXPECT_EQ(d[3], 9);
}

// --- bellman_ford ---

TEST(BellmanFord, SimpleGraph) {
    // 0->1(6), 0->2(7), 1->2(8), 1->3(-4), 2->4(9), 3->2(7), 4->3(2)
    // dist from 0: [0, 6, 7, 2, 16] (classic textbook)
    std::vector<Edge> e = {{0,1,6},{0,2,7},{1,2,8},{1,3,-4},{2,4,9},{3,2,7},{4,3,2}};
    auto d = bellman_ford(e, 5, 0);
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[1], 6);
    EXPECT_EQ(d[2], 7);
    EXPECT_EQ(d[3], 2);
    EXPECT_EQ(d[4], 16);
}

TEST(BellmanFord, NegativeCycle) {
    // 0->1(1), 1->2(-3), 2->0(1)  =>  negative cycle
    std::vector<Edge> e = {{0,1,1},{1,2,-3},{2,0,1}};
    auto d = bellman_ford(e, 3, 0);
    EXPECT_EQ(d[0], -1);
}

TEST(BellmanFord, Unreachable) {
    std::vector<Edge> e = {{0,1,5}};
    auto d = bellman_ford(e, 3, 0);
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[1], 5);
    EXPECT_EQ(d[2], INT_MAX);
}

// --- network_delay ---

TEST(NetworkDelay, BasicCase) {
    // LeetCode 743 example 1: [[2,1,1],[2,3,1],[3,4,1]], n=4, k=2  => 2
    std::vector<std::vector<int>> times = {{2,1,1},{2,3,1},{3,4,1}};
    EXPECT_EQ(network_delay(times, 4, 2), 2);
}

TEST(NetworkDelay, Impossible) {
    // Only one edge, k=2 cannot reach node 1
    std::vector<std::vector<int>> times = {{1,2,1}};
    EXPECT_EQ(network_delay(times, 2, 2), -1);
}

TEST(NetworkDelay, SingleNode) {
    EXPECT_EQ(network_delay({}, 1, 1), 0);
}

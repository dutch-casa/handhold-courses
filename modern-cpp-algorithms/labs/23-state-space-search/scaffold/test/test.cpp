#include <gtest/gtest.h>
#include "../src/solution.h"
#include <set>
#include <algorithm>

TEST(OpenLock, BasicCase) {
    EXPECT_EQ(open_lock({"0201", "0101", "0102", "1212", "2002"}, "0202"), 6);
}

TEST(OpenLock, OneMove) {
    EXPECT_EQ(open_lock({}, "0001"), 1);
}

TEST(OpenLock, StartIsDeadend) {
    EXPECT_EQ(open_lock({"0000"}, "8888"), -1);
}

TEST(OpenLock, TargetIsStart) {
    EXPECT_EQ(open_lock({}, "0000"), 0);
}

TEST(OpenLock, Impossible) {
    EXPECT_EQ(open_lock({"8888"}, "0009"), 1);
}

TEST(OpenLock, WrapAround) {
    // "0000" -> "9000" is one move (turn first wheel down)
    EXPECT_EQ(open_lock({}, "9000"), 1);
}

// Helper: verify topological order
static bool is_valid_topo(int n, const std::vector<std::pair<int, int>>& edges,
                          const std::vector<int>& order) {
    if (static_cast<int>(order.size()) != n) return false;
    std::vector<int> pos(n, -1);
    for (int i = 0; i < n; i++) {
        if (order[i] < 0 || order[i] >= n) return false;
        pos[order[i]] = i;
    }
    // Check all nodes present
    for (int i = 0; i < n; i++) {
        if (pos[i] == -1) return false;
    }
    // Check all edges respected
    for (auto& [from, to] : edges) {
        if (pos[from] >= pos[to]) return false;
    }
    return true;
}

TEST(TopoSort, LinearChain) {
    auto result = topo_sort(4, {{0, 1}, {1, 2}, {2, 3}});
    EXPECT_TRUE(is_valid_topo(4, {{0, 1}, {1, 2}, {2, 3}}, result));
}

TEST(TopoSort, Diamond) {
    std::vector<std::pair<int, int>> edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3}};
    auto result = topo_sort(4, edges);
    EXPECT_TRUE(is_valid_topo(4, edges, result));
}

TEST(TopoSort, HasCycle) {
    auto result = topo_sort(3, {{0, 1}, {1, 2}, {2, 0}});
    EXPECT_TRUE(result.empty());
}

TEST(TopoSort, NoEdges) {
    auto result = topo_sort(3, {});
    EXPECT_EQ(static_cast<int>(result.size()), 3);
    std::set<int> unique(result.begin(), result.end());
    EXPECT_EQ(unique.size(), 3u);
}

TEST(TopoSort, SingleNode) {
    auto result = topo_sort(1, {});
    EXPECT_EQ(result, std::vector<int>{0});
}

TEST(TopoSort, TwoNodeCycle) {
    auto result = topo_sort(2, {{0, 1}, {1, 0}});
    EXPECT_TRUE(result.empty());
}

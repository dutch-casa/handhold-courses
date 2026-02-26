#include <gtest/gtest.h>
#include "../src/solution.h"

// --- knapsack ---

TEST(Knapsack, Classic) {
    // weights [1,3,4,5], values [1,4,5,7], capacity 7 => 9 (items 1 and 3: weight 1+4=5? no)
    // items: {w=1,v=1},{w=3,v=4},{w=4,v=5},{w=5,v=7}
    // best: w=3+4=7, v=4+5=9
    EXPECT_EQ(knapsack({1, 3, 4, 5}, {1, 4, 5, 7}, 7), 9);
}

TEST(Knapsack, ExactCapacity) {
    EXPECT_EQ(knapsack({2, 3}, {3, 4}, 5), 7);
}

TEST(Knapsack, ZeroCapacity) {
    EXPECT_EQ(knapsack({1, 2}, {5, 6}, 0), 0);
}

TEST(Knapsack, NoItems) {
    EXPECT_EQ(knapsack({}, {}, 10), 0);
}

TEST(Knapsack, SingleItem) {
    EXPECT_EQ(knapsack({5}, {10}, 5), 10);
    EXPECT_EQ(knapsack({6}, {10}, 5), 0);
}

// --- rob_tree ---

// Helper to build a tree node
static TreeNode* node(int val, TreeNode* left = nullptr, TreeNode* right = nullptr) {
    auto* n = new TreeNode{val, left, right};
    return n;
}

TEST(RobTree, SingleNode) {
    auto* root = node(5);
    EXPECT_EQ(rob_tree(root), 5);
    delete root;
}

TEST(RobTree, ThreeNodes) {
    //     3
    //    / \
    //   2   3
    // Rob root and leaves: 3+2+3=8? No — root is adjacent to both children.
    // Rob root only: 3. Rob children: 2+3=5. Best: 5.
    auto* root = node(3, node(2), node(3));
    EXPECT_EQ(rob_tree(root), 5);
    delete root->left;
    delete root->right;
    delete root;
}

TEST(RobTree, FiveNodes) {
    //       3
    //      / \
    //     2   3
    //      \    \
    //       3    1
    // Rob 2's child (3) + rob 3's child (1) + optionally rob root
    // Skip root: max(rob 2, skip 2) + max(rob 3, skip 3)
    //   rob 2 = 2 + 0 + 0 = 2 (skip 2's children? 2 has only right=3)
    //     wait: rob 2 = 2 (rob node 2, skip its child 3) = 2
    //     skip 2 = max(rob 3-child, skip 3-child) = 3
    //   rob 3 = 3 (rob node 3, skip its child 1) = 3
    //     skip 3 = max(1, 0) = 1
    // Skip root = (max(2,3)) + (max(3,1)) = 3 + 3 = 6
    // Rob root = 3 + 0 + 0 + (skip left: 3) + (skip right: 1) = wait
    //   rob root = 3 + skip_left + skip_right = 3 + 3 + 1 = 7
    // max(6, 7) = 7
    auto* root = node(3,
        node(2, nullptr, node(3)),
        node(3, nullptr, node(1))
    );
    EXPECT_EQ(rob_tree(root), 7);
}

// --- tsp ---

TEST(TSP, ThreeCities) {
    // dist matrix (symmetric):
    //   0  10  15
    //  10   0  20
    //  15  20   0
    // Cycle 0->1->2->0: 10+20+15=45
    // Cycle 0->2->1->0: 15+20+10=45
    std::vector<std::vector<int>> dist = {
        {0, 10, 15},
        {10, 0, 20},
        {15, 20, 0}
    };
    EXPECT_EQ(tsp(dist), 45);
}

TEST(TSP, FourCities) {
    // Classic 4-city example
    std::vector<std::vector<int>> dist = {
        {0,  10, 15, 20},
        {10,  0, 35, 25},
        {15, 35,  0, 30},
        {20, 25, 30,  0}
    };
    // Optimal: 0->1->3->2->0 = 10+25+30+15 = 80
    EXPECT_EQ(tsp(dist), 80);
}

TEST(TSP, TwoCities) {
    std::vector<std::vector<int>> dist = {
        {0, 7},
        {7, 0}
    };
    // Must go 0->1->0 = 7+7 = 14
    EXPECT_EQ(tsp(dist), 14);
}

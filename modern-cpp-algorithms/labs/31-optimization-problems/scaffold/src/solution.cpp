#include "solution.h"
#include <vector>

// TODO: 2D DP table dp[i][w]; iterate items then capacity
int knapsack(const std::vector<int>& weights, const std::vector<int>& values, int capacity) {
    // TODO: dp[i][w] = max value using first i items within weight w
    return 0;
}

// TODO: post-order DFS returning {rob_this_node, skip_this_node}
int rob_tree(TreeNode* root) {
    // TODO: combine child results; take max at root
    return 0;
}

// TODO: bitmask DP — dp[mask][city] = min cost reaching city with visited set mask
int tsp(const std::vector<std::vector<int>>& dist) {
    // TODO: init dp[1][0]=0, fill transitions, return min of returning to city 0
    return 0;
}

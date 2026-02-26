#include "solution.h"
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>

int knapsack(const std::vector<int>& weights, const std::vector<int>& values, int capacity) {
    int n = static_cast<int>(weights.size());
    // dp[i][w] = max value using first i items within weight limit w
    std::vector<std::vector<int>> dp(
        static_cast<std::size_t>(n + 1),
        std::vector<int>(static_cast<std::size_t>(capacity + 1), 0)
    );
    for (int i = 1; i <= n; ++i) {
        int wi = weights[static_cast<std::size_t>(i - 1)];
        int vi = values[static_cast<std::size_t>(i - 1)];
        for (int w = 0; w <= capacity; ++w) {
            auto si = static_cast<std::size_t>(i);
            auto sw = static_cast<std::size_t>(w);
            dp[si][sw] = dp[si - 1][sw];
            if (wi <= w) {
                dp[si][sw] = std::max(dp[si][sw], dp[si - 1][static_cast<std::size_t>(w - wi)] + vi);
            }
        }
    }
    return dp[static_cast<std::size_t>(n)][static_cast<std::size_t>(capacity)];
}

// Returns {rob_this, skip_this} for a subtree
static std::pair<int, int> rob_dfs(TreeNode* node) {
    if (!node) return {0, 0};
    auto [l_rob, l_skip] = rob_dfs(node->left);
    auto [r_rob, r_skip] = rob_dfs(node->right);
    int rob_this  = node->val + l_skip + r_skip;
    int skip_this = std::max(l_rob, l_skip) + std::max(r_rob, r_skip);
    return {rob_this, skip_this};
}

int rob_tree(TreeNode* root) {
    auto [r, s] = rob_dfs(root);
    return std::max(r, s);
}

int tsp(const std::vector<std::vector<int>>& dist) {
    int n = static_cast<int>(dist.size());
    if (n == 0) return 0;
    int full_mask = (1 << n) - 1;
    // dp[mask][i] = min cost to reach city i having visited exactly the cities in mask
    std::vector<std::vector<int>> dp(
        static_cast<std::size_t>(full_mask + 1),
        std::vector<int>(static_cast<std::size_t>(n), INT_MAX)
    );
    dp[1][0] = 0;  // start at city 0
    for (int mask = 1; mask <= full_mask; ++mask) {
        for (int i = 0; i < n; ++i) {
            auto sm = static_cast<std::size_t>(mask);
            auto si = static_cast<std::size_t>(i);
            if (!(mask & (1 << i))) continue;
            if (dp[sm][si] == INT_MAX) continue;
            // Extend to each unvisited city j
            for (int j = 0; j < n; ++j) {
                if (mask & (1 << j)) continue;
                int next_mask = mask | (1 << j);
                auto snm = static_cast<std::size_t>(next_mask);
                auto sj  = static_cast<std::size_t>(j);
                int cost = dp[sm][si] + dist[si][sj];
                dp[snm][sj] = std::min(dp[snm][sj], cost);
            }
        }
    }
    // Complete the tour: return to city 0
    int result = INT_MAX;
    auto sfull = static_cast<std::size_t>(full_mask);
    for (int i = 1; i < n; ++i) {
        auto si = static_cast<std::size_t>(i);
        if (dp[sfull][si] != INT_MAX) {
            result = std::min(result, dp[sfull][si] + dist[si][0]);
        }
    }
    return result;
}

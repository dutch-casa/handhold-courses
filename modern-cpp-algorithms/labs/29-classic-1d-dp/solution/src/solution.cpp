#include "solution.h"
#include <vector>
#include <algorithm>
#include <climits>

int climb_stairs(int n) {
    if (n <= 1) return 1;
    int prev2 = 1, prev1 = 1;
    for (int i = 2; i <= n; ++i) {
        int cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int house_robber(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    if (nums.size() == 1) return nums[0];
    int prev2 = 0, prev1 = 0;
    for (int val : nums) {
        int cur = std::max(prev1, prev2 + val);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int coin_change(const std::vector<int>& coins, int amount) {
    // Sentinel: amount+1 is unreachable (can never need more coins than amount itself).
    std::vector<int> dp(static_cast<std::size_t>(amount + 1), amount + 1);
    dp[0] = 0;
    for (int a = 1; a <= amount; ++a) {
        for (int coin : coins) {
            if (coin <= a) {
                dp[static_cast<std::size_t>(a)] = std::min(
                    dp[static_cast<std::size_t>(a)],
                    dp[static_cast<std::size_t>(a - coin)] + 1
                );
            }
        }
    }
    int result = dp[static_cast<std::size_t>(amount)];
    return result > amount ? -1 : result;
}

int lis_length(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    int n = static_cast<int>(nums.size());
    std::vector<int> dp(static_cast<std::size_t>(n), 1);
    int best = 1;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[static_cast<std::size_t>(j)] < nums[static_cast<std::size_t>(i)]) {
                dp[static_cast<std::size_t>(i)] = std::max(
                    dp[static_cast<std::size_t>(i)],
                    dp[static_cast<std::size_t>(j)] + 1
                );
            }
        }
        best = std::max(best, dp[static_cast<std::size_t>(i)]);
    }
    return best;
}

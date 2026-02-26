#include "solution.h"
#include <vector>

// TODO: bottom-up tabulation — ways(n) = ways(n-1) + ways(n-2)
int climb_stairs(int n) {
    // TODO: handle base cases, fill dp table
    return 0;
}

// TODO: dp[i] = max(dp[i-1], dp[i-2] + nums[i])
int house_robber(const std::vector<int>& nums) {
    // TODO: track only the two most recent dp values
    return 0;
}

// TODO: dp[a] = min over coins of dp[a - coin] + 1
int coin_change(const std::vector<int>& coins, int amount) {
    // TODO: initialize dp[0]=0, rest to amount+1; return -1 if unreachable
    return 0;
}

// TODO: dp[i] = max LIS length ending at index i
int lis_length(const std::vector<int>& nums) {
    // TODO: for each i scan j<i where nums[j]<nums[i]
    return 0;
}

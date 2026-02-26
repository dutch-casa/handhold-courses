# Lab 29 — Classic 1D Dynamic Programming

Four foundational DP problems. Each one builds intuition for the bottom-up tabulation pattern: define subproblems, express the recurrence, fill a table.

## Tasks

- [ ] Implement `climb_stairs(n)` — count distinct ways to reach step `n` taking 1 or 2 steps at a time
- [ ] Implement `house_robber(nums)` — find the maximum sum of non-adjacent elements in `nums`
- [ ] Implement `coin_change(coins, amount)` — return the minimum number of coins that sum to `amount`, or `-1` if impossible
- [ ] Implement `lis_length(nums)` — return the length of the longest strictly increasing subsequence

## Hints

- `climb_stairs`: ways(n) = ways(n-1) + ways(n-2); base cases ways(0)=1, ways(1)=1
- `house_robber`: dp[i] = max(dp[i-1], dp[i-2] + nums[i]); only two previous values needed
- `coin_change`: dp[a] = min over each coin c of dp[a-c]+1; initialize dp[0]=0, rest to amount+1 (sentinel for impossible)
- `lis_length`: dp[i] = length of LIS ending at index i; for each j<i where nums[j]<nums[i], dp[i]=max(dp[i], dp[j]+1)

Use `Run Tests` to check your progress.

# Lab 31 — Optimization Problems

Three DP problems that combine combinatorial structure with optimization objectives: bounded item selection, tree-structured constraints, and exponential-state tour planning.

## Tasks

- [ ] Implement `knapsack(weights, values, capacity)` — maximum total value achievable by selecting items (each used at most once) without exceeding `capacity`
- [ ] Implement `rob_tree(root)` — house robber variant on a binary tree; cannot rob a node and its direct parent on the same night; return the maximum sum
- [ ] Implement `tsp(dist)` — given an n×n distance matrix, find the minimum-cost Hamiltonian cycle visiting all cities starting and ending at city 0

## Hints

- `knapsack`: 2D table dp[i][w] = max value using first i items with capacity w; iterate items outer, capacity inner; dp[i][w] = max(dp[i-1][w], dp[i-1][w-weights[i-1]] + values[i-1])
- `rob_tree`: post-order DFS returning a pair {rob_this, skip_this}; rob_this = node->val + left.skip + right.skip; skip_this = max(left.rob, left.skip) + max(right.rob, right.skip)
- `tsp`: bitmask DP; dp[mask][i] = min cost to visit cities in mask, ending at city i; start dp[1][0]=0; transitions: for each unvisited city j, dp[mask|(1<<j)][j] = min(dp[mask][i] + dist[i][j]); answer = min over i of dp[full_mask][i] + dist[i][0]

Use `Run Tests` to check your progress.

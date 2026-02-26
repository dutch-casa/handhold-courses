---
title: Advanced DP Patterns
---

# 0/1 knapsack

{{show: knapsack-code typewriter 2s linear}} 0/1 knapsack: n items, each with a weight and value. Capacity W. Maximize total value without exceeding capacity. Each item is used at most once — that's the 0/1 part.

{{focus: state}} {{zoom: 1.2x}} {{annotate: state "dp[i][w] = max value, i items, cap w"}} dp[i][w] is the maximum value using the first i items with capacity w. Two dimensions: items considered and capacity remaining.

{{zoom: 1x}} {{focus: recurrence}} {{zoom: 1.3x}} {{annotate: recurrence "Skip or take"}} dp[i][w] = max(dp[i-1][w], dp[i-1][w - wt[i]] + val[i]). Either skip item i (keep the best without it), or take it (deduct its weight, add its value). Can only take if weight fits.

{{zoom: 1x}} {{focus: fill-order}} {{zoom: 1.2x}} {{annotate: fill-order "Top-left to bottom-right"}} Fill row by row. Each row i depends only on row i-1. The backward inner loop (capacity W down to wt[i]) collapses this to 1D — the same item can't be counted twice.

{{zoom: 1x}} {{focus: none}} The backward loop is the signature of 0/1 knapsack. Forward loop = unbounded knapsack. Backward loop = each item used at most once.

```code:knapsack-code lang=cpp
// dp[i][w] = max value with items 0..i-1 and capacity w
int knapsack(const std::vector<int>& wt, const std::vector<int>& val, int W) {
    const int n = static_cast<int>(wt.size());
    // Space-optimized: single row, backward fill
    std::vector<int> dp(W + 1, 0);

    for (int i = 0; i < n; ++i) {
        // Backward: prevents item i from being used more than once
        for (int w = W; w >= wt[i]; --w) {
            dp[w] = std::max(dp[w],              // skip item i
                             dp[w - wt[i]] + val[i]);  // take item i
        }
    }
    return dp[W];
}
---
state: 2
recurrence: 10-11
fill-order: 8-9
```

# Complete knapsack

{{clear: slide}}

{{show: complete-code typewriter 1.5s linear}} Complete knapsack: same setup, but each item can be used unlimited times. One change to the inner loop direction flips the entire semantics.

{{focus: diff}} {{zoom: 1.2x}} {{annotate: diff "Forward loop"}} Change the inner loop from backward to forward. That's it. `for (int w = wt[i]; w <= W; ++w)`. When you update dp[w] using dp[w - wt[i]], that value has already been updated in this iteration — so the same item gets counted again.

{{zoom: 1x}} {{focus: why}} {{zoom: 1.3x}} {{annotate: why "Reuse is intentional"}} In 0/1 knapsack, reading dp[w - wt[i]] after updating smaller w would reuse the item. We prevent that by going backward. In complete knapsack, reuse is exactly what we want — so we go forward.

{{zoom: 1x}} {{focus: coin-change}} {{zoom: 1.2x}} {{annotate: coin-change "Coin change is this"}} Coin change is complete knapsack where values are all 1 and you minimize instead of maximize. The forward inner loop is the same.

{{zoom: 1x}} {{focus: none}} Two loops, one direction change, completely different problem. The asymmetry between 0/1 and complete knapsack is subtle but absolute.

```code:complete-code lang=cpp
// Complete knapsack: each item usable unlimited times
int completeKnapsack(const std::vector<int>& wt,
                     const std::vector<int>& val, int W) {
    std::vector<int> dp(W + 1, 0);

    for (int i = 0; i < static_cast<int>(wt.size()); ++i) {
        // Forward: allows item i to be used multiple times
        for (int w = wt[i]; w <= W; ++w) {
            dp[w] = std::max(dp[w],
                             dp[w - wt[i]] + val[i]);
        }
    }
    return dp[W];
}
---
diff: 7-8
why: 9-10
coin-change: 2
```

# Tree DP

{{clear: slide}}

{{show: tree-dp-code typewriter 2s linear}} Tree DP runs DFS and computes answers bottom-up from leaves to root. Each node's state depends on its children's states — classic post-order processing.

{{focus: shape}} {{zoom: 1.2x}} {{annotate: shape "Post-order DFS"}} The template: recurse on all children first, then aggregate their results to compute the current node's value. Leaves return base-case values directly.

{{zoom: 1x}} {{focus: example}} {{zoom: 1.3x}} {{annotate: example "Robbery on a tree"}} House robber on a tree: rob or skip each node. If you rob a node, you can't rob its children. dp[node] = pair<int, int> where first = max loot if skipped, second = max loot if robbed.

{{zoom: 1x}} {{focus: aggregate}} {{zoom: 1.2x}} {{annotate: aggregate "Aggregate children"}} For each child, if the current node is skipped, the child can be either robbed or skipped — take the max. If the current node is robbed, the child must be skipped.

{{zoom: 1x}} {{focus: none}} Tree DP is unavoidable for problems defined on trees. The post-order guarantee ensures every child is resolved before its parent needs the result.

```code:tree-dp-code lang=cpp
struct TreeNode {
    int val;
    std::vector<int> children;  // indices into node array
};

// Returns {max_if_skipped, max_if_robbed}
std::pair<int, int> dfs(const std::vector<TreeNode>& nodes, int u) {
    int skip = 0, rob = nodes[u].val;

    for (int child : nodes[u].children) {
        auto [child_skip, child_rob] = dfs(nodes, child);

        // If we skip u, child can be robbed or skipped
        skip += std::max(child_skip, child_rob);

        // If we rob u, child must be skipped
        rob += child_skip;
    }
    return {skip, rob};
}

int robTree(const std::vector<TreeNode>& nodes) {
    auto [s, r] = dfs(nodes, 0);
    return std::max(s, r);
}
---
shape: 7-8
example: 7-17
aggregate: 11-16
```

# Bitmask DP

{{clear: slide}}

{{show: tsp-code typewriter 2s linear}} Bitmask DP encodes a subset of n elements as an integer bitmask. Bit i is 1 if element i is in the subset. This enables DP over all 2^n subsets — exponential but tractable for small n.

{{focus: state}} {{zoom: 1.2x}} {{annotate: state "dp[mask][i] = cost ending at i"}} Classic example: Traveling Salesman Problem. dp[mask][i] = minimum cost to visit all cities in the bitmask and end at city i. State is which cities you've visited and where you are.

{{zoom: 1x}} {{focus: transition}} {{zoom: 1.3x}} {{annotate: transition "Add next city"}} Transition: for each unvisited city j (bit j is 0 in mask), dp[mask | (1 << j)][j] = min(dp[mask | (1 << j)][j], dp[mask][i] + dist[i][j]). Set bit j and move there.

{{zoom: 1x}} {{focus: base-fill}} {{zoom: 1.2x}} {{annotate: base-fill "Start at city 0"}} Base case: dp[1][0] = 0. Bitmask 1 means only city 0 is visited, and we're at city 0. Cost zero.

{{zoom: 1x}} {{focus: none}} Bitmask DP is O(2^n * n^2) for TSP. Feasible up to about n = 20. The bitmask is the key — it makes "which subset have I processed" into an integer index.

```code:tsp-code lang=cpp
// TSP: minimum cost tour visiting all cities exactly once
// dist[i][j] = cost from city i to city j
int tsp(const std::vector<std::vector<int>>& dist) {
    const int n = static_cast<int>(dist.size());
    const int FULL = (1 << n) - 1;
    constexpr int INF = std::numeric_limits<int>::max() / 2;

    // dp[mask][i] = min cost to visit cities in mask, ending at i
    std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INF));
    dp[1][0] = 0;  // start at city 0, only city 0 visited

    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int i = 0; i < n; ++i) {
            if (dp[mask][i] == INF) continue;
            if (!(mask & (1 << i))) continue;  // i not in mask

            for (int j = 0; j < n; ++j) {
                if (mask & (1 << j)) continue;  // j already visited
                const int next_mask = mask | (1 << j);
                dp[next_mask][j] = std::min(dp[next_mask][j],
                                            dp[mask][i] + dist[i][j]);
            }
        }
    }
    // Return to city 0
    int ans = INF;
    for (int i = 1; i < n; ++i) {
        if (dp[FULL][i] != INF)
            ans = std::min(ans, dp[FULL][i] + dist[i][0]);
    }
    return ans;
}
---
state: 8
base-fill: 9
transition: 16-19
```

# Digit DP

{{clear: slide}}

{{show: digit-code typewriter 2s linear}} Digit DP counts integers in [1, N] that satisfy some property. The key insight: process N digit by digit, tracking whether the current prefix is still "tight" against N.

{{focus: tight}} {{zoom: 1.2x}} {{annotate: tight "Tight constraint"}} The tight flag means every digit so far equals the corresponding digit of N. While tight, the next digit can't exceed N's next digit. Once you place a digit smaller than N's, you go loose — any digit 0-9 is valid from then on.

{{zoom: 1x}} {{focus: state}} {{zoom: 1.2x}} {{annotate: state "dp[pos][tight][...property]"}} State is: current digit position, tight constraint flag, and whatever property you're tracking (sum of digits, count of specific digits, divisibility, etc.).

{{zoom: 1x}} {{focus: memo}} {{zoom: 1.3x}} {{annotate: memo "Cache on (pos, tight, property)"}} Memoize on the full state tuple. The tight states are rarely reused (there's only one tight path), but the loose states are reused constantly — that's where the savings come from.

{{zoom: 1x}} {{focus: none}} Digit DP handles problems like "count numbers up to N with digit sum S" or "count numbers up to N divisible by K". The template is always: pos, tight, accumulated property.

```code:digit-code lang=cpp
// Count integers in [1, N] where digit sum equals target
// Returns the count using digit DP
struct DigitDP {
    std::string num;
    std::unordered_map<int, int> memo;

    // State: position, tight, current_sum, started
    int solve(int pos, bool tight, int sum, bool started) {
        if (pos == static_cast<int>(num.size())) {
            return started && sum == 0 ? 1 : 0;
        }

        int key = pos * 10000 + tight * 1000 + sum * 2 + started;
        if (memo.contains(key)) return memo[key];

        int limit = tight ? (num[pos] - '0') : 9;
        int result = 0;

        for (int d = 0; d <= limit; ++d) {
            bool next_tight = tight && (d == limit);
            result += solve(pos + 1, next_tight, sum - d, started || d > 0);
        }
        return memo[key] = result;
    }

    int count(int N, int target) {
        num = std::to_string(N);
        memo.clear();
        return solve(0, true, target, false);
    }
};
---
tight: 7-10
state: 7
memo: 13-14
```

# Optimization hints

{{clear: slide}}

{{show: opt-code typewriter 2s linear}} Three optimization techniques that turn correct but slow DP into production-quality code: space reduction, divide-and-conquer optimization, and monotonic deque optimization.

{{focus: space}} {{zoom: 1.2x}} {{annotate: space "Rolling rows"}} Space reduction: if dp[i][j] only uses dp[i-1][...], you only need two rows. If it only uses dp[i-1][j] and dp[i][j-1], one row with careful traversal. Profile before optimizing — don't shrink what you don't need to.

{{zoom: 1x}} {{focus: deque}} {{zoom: 1.3x}} {{annotate: deque "O(n) sliding window"}} Monotonic deque optimization: when dp[i] = dp[j] + cost(i, j) and cost is "convex" in a sense that allows a sliding window, you can maintain a monotonic deque of useful j values. Drops O(n^2) DP to O(n).

{{zoom: 1x}} {{focus: dc-opt}} {{zoom: 1.2x}} {{annotate: dc-opt "O(n log n) D&C"}} Divide-and-conquer optimization: when the optimal split point opt(i, j) is monotone in i — opt(i, j) <= opt(i+1, j) — you can use D&C to find all split points in O(n log n) instead of O(n^2).

{{zoom: 1x}} {{focus: none}} Most DP is O(n^2) or O(n^2 k). These optimizations attack the inner loop. Apply them only when the recurrence structure supports it — verify the preconditions first.

```code:opt-code lang=cpp
// Monotonic deque DP: dp[i] = min over j < i of (dp[j] + cost(i, j))
// where cost is the "sum of elements in window [j..i]"
// Useful for problems like minimum cost to partition into subarrays

int minCostPartition(const std::vector<int>& a, int k) {
    const int n = static_cast<int>(a.size());
    std::vector<int> dp(n + 1, std::numeric_limits<int>::max() / 2);
    dp[0] = 0;

    // Monotonic deque stores indices in order of increasing dp value
    std::deque<int> dq;
    dq.push_back(0);

    for (int i = 1; i <= n; ++i) {
        // Pop front if outside window
        while (!dq.empty() && dq.front() < i - k) dq.pop_front();

        // Optimal j is at front of deque
        if (!dq.empty()) dp[i] = dp[dq.front()] + a[i - 1];

        // Maintain deque: pop from back while front is not better
        while (!dq.empty() && dp[dq.back()] >= dp[i]) dq.pop_back();
        dq.push_back(i);
    }
    return dp[n];
}
---
space: 2
deque: 11-20
dc-opt: 2
```

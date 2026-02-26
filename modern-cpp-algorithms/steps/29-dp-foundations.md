---
title: DP Foundations
---

# What is DP

{{show: overlap-tree grow 0.5s spring}} Dynamic programming solves problems by breaking them into overlapping subproblems and caching the results. Two properties must hold: optimal substructure and overlapping subproblems.

{{focus: optimal-sub}} {{annotate: optimal-sub "Optimal substructure"}} Optimal substructure means the optimal solution to the whole problem contains optimal solutions to its subproblems. If you can't build the global optimum from local optima, DP won't work.

{{focus: overlap}} {{annotate: overlap "Overlapping subproblems"}} Overlapping subproblems means you'd compute the same subproblem multiple times in a naive recursion. That's the waste DP eliminates — compute once, cache forever.

{{focus: none}} Divide and conquer also has optimal substructure, but its subproblems don't overlap. DP is the answer when both conditions hold.

```data:overlap-tree type=tree
{"value": "fib(5)", "children": [{"value": "fib(4)", "children": [{"value": "fib(3)", "children": [{"value": "fib(2)"}, {"value": "fib(1)"}]}, {"value": "fib(2) dup"}]}, {"value": "fib(3) dup", "children": [{"value": "fib(2) dup2"}, {"value": "fib(1) dup"}]}]}
---
optimal-sub: fib(5)
overlap: fib(3) dup
```

# Top-down vs bottom-up

{{clear: slide}}

{{show: approaches grow 0.5s spring}} Two ways to implement DP. Top-down with memoization, or bottom-up with tabulation. Same asymptotic complexity, different constants and tradeoffs.

{{focus: top-down}} {{annotate: top-down "Memoization"}} Top-down keeps the recursive structure. You add a cache — a hash map or array indexed by state. Before computing, check the cache. After computing, store the result. Natural to write, easy to reason about.

{{focus: bottom-up}} {{annotate: bottom-up "Tabulation"}} Bottom-up starts from the base cases and fills a table in dependency order. No recursion, no call stack overhead, cache-friendly access patterns. Usually faster in practice.

{{focus: tradeoffs}} {{annotate: tradeoffs "When to pick"}} Pick top-down when the state space is sparse — you only visit a fraction of all states. Pick bottom-up when you need maximum performance or when the fill order is obvious.

{{focus: none}} Both approaches are valid. Most competitive programmers default to bottom-up. Most production code benefits from top-down's clarity.

```data:approaches type=array
["Top-down: recurse + cache (memoize)", "Bottom-up: fill table in order (tabulate)", "Same O() complexity — different constants", "Top-down: sparse states. Bottom-up: dense fill."]
---
top-down: 0
bottom-up: 1
tradeoffs: 3
```

# Fibonacci — naive to optimal

{{clear: slide}}

{{show: fib-code typewriter 2s linear}} Fibonacci is the canonical DP example. Watch it evolve from exponential to constant space.

{{focus: naive}} {{zoom: 1.2x}} {{annotate: naive "O(2^n)"}} Naive recursion recomputes everything. fib(40) makes over a billion calls. The call tree branches at every node — pure waste.

{{zoom: 1x}} {{focus: memo}} {{zoom: 1.2x}} {{annotate: memo "O(n) time/space"}} Add a cache. First call fills it, every repeat is O(1) lookup. The exponential tree collapses to a linear chain. Call stack depth is still O(n).

{{zoom: 1x}} {{focus: table}} {{zoom: 1.2x}} {{annotate: table "O(n) time/space"}} Bottom-up table. Fill from fib(0) up to fib(n). No recursion, sequential memory access, fast.

{{zoom: 1x}} {{focus: optimal}} {{zoom: 1.3x}} {{annotate: optimal "O(n) time, O(1) space"}} You only ever need the last two values. Two variables, one loop. Same answer, constant space. This is the final form.

{{zoom: 1x}} {{focus: none}} Every DP solution follows this arc: understand the recurrence, implement it correctly, then optimize space.

```code:fib-code lang=cpp
// Naive: O(2^n)
int fib_naive(int n) {
    if (n <= 1) return n;
    return fib_naive(n - 1) + fib_naive(n - 2);
}

// Memoization: O(n) time, O(n) space
int fib_memo(int n, std::unordered_map<int, int>& cache) {
    if (n <= 1) return n;
    if (cache.contains(n)) return cache[n];
    return cache[n] = fib_memo(n - 1, cache) + fib_memo(n - 2, cache);
}

// Tabulation: O(n) time, O(n) space
int fib_table(int n) {
    if (n <= 1) return n;
    std::vector<int> dp(n + 1);
    dp[0] = 0; dp[1] = 1;
    for (int i = 2; i <= n; ++i) dp[i] = dp[i-1] + dp[i-2];
    return dp[n];
}

// Optimal: O(n) time, O(1) space
int fib_opt(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int c = a + b;
        a = b; b = c;
    }
    return b;
}
---
naive: 2-4
memo: 7-10
table: 13-18
optimal: 21-28
```

# The 1D DP template

{{clear: slide}}

{{show: template-code typewriter 2s linear}} Almost every 1D DP problem fits one template. Master this shape and you'll recognize the pattern instantly.

{{focus: state-def}} {{zoom: 1.2x}} {{annotate: state-def "Define state"}} dp[i] means something specific. Write it in English before you write code. "dp[i] is the answer considering only the first i elements." If you can't state it clearly, you don't understand the subproblem.

{{zoom: 1x}} {{focus: base-case}} {{zoom: 1.2x}} {{annotate: base-case "Base cases"}} Base cases are the seeds. dp[0] and dp[1] are usually trivial — zero items means zero cost, one item means that item's value. Get these wrong and everything downstream is garbage.

{{zoom: 1x}} {{focus: recurrence}} {{zoom: 1.2x}} {{annotate: recurrence "Recurrence"}} The recurrence expresses dp[i] in terms of smaller indices. This is the heart of the problem. It encodes the choice — take or skip, extend or start fresh.

{{zoom: 1x}} {{focus: fill-order}} {{zoom: 1.2x}} {{annotate: fill-order "Fill order"}} Fill left to right for problems where dp[i] depends on dp[i-1], dp[i-2]. Fill order must respect dependencies — never use a value before it's computed.

{{zoom: 1x}} {{focus: none}} State definition → recurrence → base case → fill order. That sequence never changes.

```code:template-code lang=cpp
// The 1D DP template
// Step 1: define what dp[i] means in plain English
// Step 2: identify the recurrence
// Step 3: set base cases
// Step 4: fill in order

int solve(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    std::vector<int> dp(n + 1, 0);

    // Base cases
    dp[0] = /* trivial case for 0 elements */;
    dp[1] = /* trivial case for 1 element  */;

    // Fill: left to right, each cell depends on previous cells
    for (int i = 2; i <= n; ++i) {
        dp[i] = /* recurrence: express dp[i] in terms of dp[i-1], dp[i-2], ... */;
    }

    return dp[n];
}
---
state-def: 2-4
base-case: 10-11
recurrence: 14-15
fill-order: 18
```

# Climbing stairs

{{clear: slide}}

{{show: stairs-code typewriter 1.5s linear}} Climbing stairs: n steps, climb 1 or 2 at a time. How many distinct ways to reach the top? This is Fibonacci in disguise.

{{focus: state}} {{zoom: 1.2x}} {{annotate: state "dp[i] = ways to reach step i"}} The state is clear. dp[i] is the number of distinct ways to reach step i. To reach step i, you either came from i-1 (took 1 step) or from i-2 (took 2 steps).

{{zoom: 1x}} {{focus: recurrence}} {{zoom: 1.2x}} {{annotate: recurrence "Fibonacci recurrence"}} dp[i] = dp[i-1] + dp[i-2]. Exactly Fibonacci. The base cases are dp[1] = 1 and dp[2] = 2.

{{zoom: 1x}} {{focus: result}} {{zoom: 1.2x}} {{annotate: result "O(1) space"}} Since we only need the last two values, collapse to two variables. Same pattern as the optimal Fibonacci.

{{zoom: 1x}} {{focus: none}} Recognize the shape. Whenever dp[i] depends on the previous two values with addition, it's Fibonacci. The space-optimal form is always two variables.

```code:stairs-code lang=cpp
// dp[i] = number of ways to climb i stairs
int climbStairs(int n) {
    if (n <= 2) return n;

    // dp[1] = 1, dp[2] = 2 → O(1) space optimization
    int prev2 = 1;  // dp[i-2]
    int prev1 = 2;  // dp[i-1]

    for (int i = 3; i <= n; ++i) {
        int curr = prev1 + prev2;  // dp[i] = dp[i-1] + dp[i-2]
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
---
state: 2
recurrence: 9-10
result: 6-12
```

# House robber

{{clear: slide}}

{{show: robber-code typewriter 1.5s linear}} House robber: adjacent houses can't both be robbed. Maximize total loot. Classic skip-or-take DP.

{{focus: state}} {{zoom: 1.2x}} {{annotate: state "dp[i] = max loot to index i"}} dp[i] is the maximum loot you can steal from houses 0 through i. At each house, you make a binary decision.

{{zoom: 1x}} {{focus: recurrence}} {{zoom: 1.3x}} {{annotate: recurrence "Skip or take"}} dp[i] = max(dp[i-1], dp[i-2] + nums[i]). Either you skip house i and keep the best from i-1, or you rob house i and add its value to the best from i-2. Can't take i-1 if you take i.

{{zoom: 1x}} {{focus: base-case}} {{zoom: 1.2x}} {{annotate: base-case "Base cases"}} dp[0] = nums[0] — rob the only house. dp[1] = max(nums[0], nums[1]) — rob the better of two adjacent houses. Can't rob both.

{{zoom: 1x}} {{focus: none}} The skip-or-take pattern appears in 0/1 knapsack, LCS, and dozens of other problems. Recognize it and the recurrence writes itself.

```code:robber-code lang=cpp
// dp[i] = max loot considering houses 0..i
int rob(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n == 1) return nums[0];

    int prev2 = nums[0];                         // dp[i-2]
    int prev1 = std::max(nums[0], nums[1]);      // dp[i-1]

    for (int i = 2; i < n; ++i) {
        int curr = std::max(prev1,               // skip house i
                            prev2 + nums[i]);    // rob house i
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
---
state: 2
base-case: 6-7
recurrence: 9-12
```

# Coin change

{{clear: slide}}

{{show: coin-code typewriter 2s linear}} Coin change: given coin denominations and a target amount, find the minimum number of coins to make that amount. Unbounded knapsack variant.

{{focus: state}} {{zoom: 1.2x}} {{annotate: state "dp[i] = min coins for amount i"}} dp[i] is the minimum number of coins to make amount i exactly. Initialize to infinity — amounts that can't be made stay unreachable.

{{zoom: 1x}} {{focus: base-case}} {{zoom: 1.2x}} {{annotate: base-case "Zero costs nothing"}} dp[0] = 0. Making amount zero requires zero coins. Everything else starts at INT_MAX / 2 to avoid overflow when we add 1.

{{zoom: 1x}} {{focus: recurrence}} {{zoom: 1.3x}} {{annotate: recurrence "Try every coin"}} For each amount i, try every coin. If i >= coin and dp[i - coin] is reachable, then dp[i] = min(dp[i], dp[i - coin] + 1). Use each coin as many times as needed — that's the unbounded part.

{{zoom: 1x}} {{focus: fill-order}} {{zoom: 1.2x}} {{annotate: fill-order "Amount by amount"}} Fill left to right. When computing dp[i], dp[i - coin] is already filled for any valid coin. Dependencies are always to the left.

{{zoom: 1x}} {{focus: none}} This template handles all unbounded knapsack variants. Change "min coins + 1" to whatever your objective is and it adapts.

```code:coin-code lang=cpp
// dp[i] = minimum coins to make amount i
int coinChange(const std::vector<int>& coins, int amount) {
    constexpr int INF = std::numeric_limits<int>::max() / 2;
    std::vector<int> dp(amount + 1, INF);

    dp[0] = 0;  // base case: zero coins to make amount 0

    for (int i = 1; i <= amount; ++i) {
        for (int coin : coins) {
            if (i >= coin && dp[i - coin] != INF) {
                dp[i] = std::min(dp[i], dp[i - coin] + 1);
            }
        }
    }

    return dp[amount] == INF ? -1 : dp[amount];
}
---
state: 2
base-case: 5
recurrence: 8-12
fill-order: 7
```

# 2D vectors in C++

{{clear: slide}}

{{show: vec2d-code typewriter 2s linear}} 2D DP tables are `vector<vector<int>>`. Know how to create, initialize, and fill them correctly.

{{focus: declare}} {{zoom: 1.2x}} {{annotate: declare "Constructor syntax"}} `vector<vector<int>>(rows, vector<int>(cols, init_val))` creates a rows-by-cols grid filled with init_val. The inner vector<int>(cols, 0) is the row template, repeated rows times.

{{zoom: 1x}} {{focus: access}} {{zoom: 1.2x}} {{annotate: access "Row-major access"}} Access with dp[row][col]. With `noUncheckedIndexedAccess`, each dp[i] returns an optional-like element. Use `dp.at(i).at(j)` if you want bounds checking, or validate indices manually.

{{zoom: 1x}} {{focus: fill}} {{zoom: 1.3x}} {{annotate: fill "std::ranges::fill"}} To reset a 2D vector, nest fill calls or use a range. Alternatively, reinitialize in the constructor if you only need to do it once.

{{zoom: 1x}} {{focus: resize}} {{zoom: 1.2x}} {{annotate: resize "resize adds rows"}} resize changes the number of rows. Each new row is default-constructed — an empty vector. You'll need to resize each row separately or re-assign with the constructor idiom.

{{zoom: 1x}} {{focus: none}} The constructor idiom is the cleanest. Build it right the first time rather than resize and fill separately.

```code:vec2d-code lang=cpp
#include <vector>
#include <numeric>

// Declare and initialize: rows x cols, all zeros
const int rows = 4, cols = 5;
std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

// Access
dp[1][2] = 42;
int val = dp[1][2];

// Fill entire 2D table with a value
for (auto& row : dp) {
    std::ranges::fill(row, -1);
}

// Resize: add a row (each new row is empty by default)
dp.resize(rows + 1);
dp.back().resize(cols, 0);

// Common DP init: fill with "infinity"
constexpr int INF = std::numeric_limits<int>::max() / 2;
std::vector<std::vector<int>> cost(rows, std::vector<int>(cols, INF));
cost[0][0] = 0;  // base case
---
declare: 5
access: 8-9
fill: 12-14
resize: 17-18
```

---
title: Greedy algorithms
---

# Greedy choice property

{{show: greedy-intro grow 0.5s spring}} Greedy algorithms make the locally optimal choice at each step, hoping it leads to the globally optimal solution. Sometimes it works. Sometimes it doesn't. The trick is knowing when.

{{focus: greedy-works}} {{annotate: greedy-works "Provably optimal"}} When greedy works, it's because the problem has the greedy choice property: a locally optimal choice is always part of some globally optimal solution. No need to explore alternatives.

{{focus: greedy-fails}} {{annotate: greedy-fails "Counterexample"}} When greedy fails, it's because a locally good choice locks you into a globally bad path. The coin change problem with denominations {1, 3, 4} and target 6: greedy picks 4+1+1 (three coins), but 3+3 is two coins.

{{focus: none}} Greedy is powerful when it applies. The challenge is recognizing when it does.

```data:greedy-intro type=array
["Greedy: local best → global best", "Fails: {1,3,4} target 6 → 4+1+1 (3) vs 3+3 (2)"]
---
greedy-works: 0
greedy-fails: 1
```

# Activity selection

{{clear: slide}}

{{show: activity-code typewriter 2s linear}} The classic greedy problem. Given activities with start and end times, select the maximum number of non-overlapping activities.

{{focus: sort-step}} {{zoom: 1.2x}} {{annotate: sort-step "Sort by end time"}} Sort by end time. This is the greedy insight — activities that finish earliest leave the most room for later activities.

{{zoom: 1x}} {{focus: select-loop}} {{zoom: 1.2x}} {{annotate: select-loop "Greedy pick"}} Iterate. If the activity starts after the last selected one ends, take it. Otherwise skip. One pass, O(n log n) total.

{{zoom: 1x}} {{focus: none}} This is provably optimal. Any other selection that skips an earliest-ending compatible activity can be improved by swapping.

```code:activity-code lang=cpp
#include <vector>
#include <algorithm>

int max_activities(std::vector<std::pair<int,int>>& intervals) {
    std::sort(intervals.begin(), intervals.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    int count = 0;
    int last_end = -1;
    for (const auto& [start, end] : intervals) {
        if (start >= last_end) {
            count++;
            last_end = end;
        }
    }
    return count;
}
---
sort-step: 5-6
select-loop: 9-14
```

# Jump game

{{clear: slide}}

{{show: jump-code typewriter 1.5s linear}} Can you reach the last index? Each element tells you the maximum jump length from that position.

{{focus: farthest}} {{zoom: 1.2x}} {{annotate: farthest "Track reach"}} Track the farthest index reachable so far. At each position, update: farthest equals max of farthest and i plus nums at i.

{{zoom: 1x}} {{focus: check}} {{zoom: 1.2x}} {{annotate: check "Can't proceed"}} If i ever exceeds farthest, you're stuck. You can't reach this position, so you can't reach the end.

{{zoom: 1x}} {{focus: none}} One pass, O(n). The greedy insight: you never need to backtrack. If a position is reachable, the best strategy is to always extend your reach.

```code:jump-code lang=cpp
bool can_jump(const std::vector<int>& nums) {
    int farthest = 0;
    for (int i = 0; i < static_cast<int>(nums.size()); i++) {
        if (i > farthest) return false;
        farthest = std::max(farthest, i + nums[i]);
    }
    return true;
}
---
farthest: 2-4
check: 3
```

# Gas station

{{clear: slide}}

{{show: gas-code typewriter 2s linear}} Circular route with gas stations. Each station gives gas and costs fuel to reach the next. Find the starting station for a complete circuit, or negative one if impossible.

{{focus: total-check}} {{zoom: 1.2x}} {{annotate: total-check "Feasibility"}} First insight: if total gas is less than total cost, no solution exists. The deficit can't be made up anywhere.

{{zoom: 1x}} {{focus: reset-logic}} {{zoom: 1.2x}} {{annotate: reset-logic "Reset on deficit"}} Second insight: if you run out of gas at station j starting from station i, then no station between i and j works either. So reset the start to j plus one.

{{zoom: 1x}} {{focus: none}} One pass to check feasibility, one pass to find the start. O(n).

```code:gas-code lang=cpp
int gas_station(const std::vector<int>& gas, const std::vector<int>& cost) {
    int total = 0, tank = 0, start = 0;
    int n = static_cast<int>(gas.size());
    for (int i = 0; i < n; i++) {
        int net = gas[i] - cost[i];
        total += net;
        tank += net;
        if (tank < 0) {
            start = i + 1;
            tank = 0;
        }
    }
    return total >= 0 ? start : -1;
}
---
total-check: 2
reset-logic: 7-10
```

# Assign cookies

{{clear: slide}}

{{show: cookie-code typewriter 1.5s linear}} Children have greed factors, cookies have sizes. A child is content if cookie size is at least their greed. Maximize content children.

{{focus: sort-both}} {{zoom: 1.2x}} {{annotate: sort-both "Sort both"}} Sort children by greed, cookies by size. Start with the least greedy child and smallest cookie.

{{zoom: 1x}} {{focus: greedy-match}} {{zoom: 1.2x}} {{annotate: greedy-match "Match smallest"}} If the current cookie satisfies the current child, advance both. Otherwise, the cookie is too small for anyone remaining — skip it.

{{zoom: 1x}} {{focus: none}} This is optimal because giving a larger cookie to a less greedy child wastes capacity that might satisfy a greedier child later.

```code:cookie-code lang=cpp
int assign_cookies(std::vector<int> children, std::vector<int> cookies) {
    std::sort(children.begin(), children.end());
    std::sort(cookies.begin(), cookies.end());
    int child = 0, cookie = 0;
    while (child < static_cast<int>(children.size()) &&
           cookie < static_cast<int>(cookies.size())) {
        if (cookies[cookie] >= children[child]) child++;
        cookie++;
    }
    return child;
}
---
sort-both: 2-3
greedy-match: 6-8
```

# Proving greedy

{{clear: slide}}

{{show: proof-viz grow 0.5s spring}} How do you know greedy is correct? The exchange argument: show that any optimal solution can be transformed into the greedy solution without making it worse.

{{focus: step-1}} {{annotate: step-1 "Assume optimal"}} Start with any optimal solution O. If it already matches the greedy solution, done.

{{focus: step-2}} {{annotate: step-2 "Find difference"}} Find the first point where O differs from greedy. Greedy chose X, optimal chose Y.

{{focus: step-3}} {{annotate: step-3 "Swap and compare"}} Swap Y for X in the optimal solution. Show the result is no worse. Repeat until O matches greedy. Since each swap preserves optimality, greedy is optimal.

{{focus: none}} You don't always need a formal proof. But the exchange argument is the standard technique, and thinking through it helps you catch cases where greedy fails.

```data:proof-viz type=array
["1. Assume optimal solution O", "2. Find first difference: greedy chose X, O chose Y", "3. Swap Y→X in O: result ≥ O. Repeat."]
---
step-1: 0
step-2: 1
step-3: 2
```

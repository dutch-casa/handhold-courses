---
title: The backtracking template
---

# Brute force to pruning

{{show: decision-tree grow 0.5s spring}} Backtracking explores all possibilities — but it quits early when it knows a path can't lead to a solution. Brute force tries everything. Backtracking prunes.

{{focus: full-tree}} {{annotate: full-tree "All paths"}} The brute force decision tree. Every branch is explored. Exponential work.

{{focus: pruned}} {{annotate: pruned "Cut early"}} With pruning, dead-end branches are abandoned the moment they violate a constraint. Entire subtrees vanish. The savings can be enormous.

{{focus: none}} Backtracking is DFS on a decision tree with pruning. That's the whole idea. The rest is template.

```data:decision-tree type=tree
           *
         / | \
        a   b   c
       / \  |  / \
      ab ac ba ca cb
     /        \
    abc       cab
---
full-tree: *, a, b, c, ab, ac, ba, ca, cb, abc, cab
pruned: b, ba
```

# The decision tree

{{clear: slide}}

{{show: ceu-diagram grow 0.5s spring}} Every backtracking problem has three phases: choose, explore, unchoose. This is the recursion pattern.

{{focus: choose}} {{annotate: choose "Pick one"}} Choose: select the next candidate. Add it to the current partial solution. This extends the path down the tree.

{{focus: explore}} {{annotate: explore "Go deeper"}} Explore: recurse with the updated state. Dive deeper into the decision tree.

{{focus: unchoose}} {{annotate: unchoose "Undo"}} Unchoose: remove the candidate. Restore state to what it was before the choice. This is the "backtrack" — you undo the decision and try the next option.

{{focus: none}} Choose, explore, unchoose. The undo step is critical. Without it, the state accumulates and the search breaks. Clean undo means clean exploration.

```diagram:ceu-diagram
choose [service]
explore [service]
unchoose [service]
choose --add candidate--> explore
explore --recurse deeper--> choose
explore --return--> unchoose
unchoose --try next--> choose
---
choose: choose
explore: explore
unchoose: unchoose
```

# The backtracking template

{{clear: slide}}

{{show: template-code typewriter 2s linear}} This is the skeleton. Every backtracking problem fits into it.

{{focus: signature}} {{zoom: 1.2x}} {{annotate: signature "State params"}} The function takes the current state, the results collector, and whatever extra context the problem needs. The state is the partial solution being built.

{{zoom: 1x}} {{focus: base-case}} {{zoom: 1.3x}} {{annotate: base-case "Solution found"}} Base case: the partial solution is complete. Add it to results and return. For permutations, that's when the path has n elements. For subsets, it depends on the problem.

{{zoom: 1x}} {{focus: loop}} {{zoom: 1.2x}} {{annotate: loop "Try candidates"}} The loop iterates over all candidates at this decision point. Each iteration is one branch of the decision tree.

{{zoom: 1x}} {{focus: choose-step}} {{zoom: 1.3x}} {{annotate: choose-step "Choose"}} Choose: push the candidate onto the current path. The state grows.

{{zoom: 1x}} {{focus: recurse}} {{zoom: 1.2x}} {{annotate: recurse "Explore"}} Explore: recurse. The start index advances to avoid duplicates (for combinations) or stays the same (for permutations with reuse).

{{zoom: 1x}} {{focus: unchoose-step}} {{zoom: 1.3x}} {{annotate: unchoose-step "Unchoose"}} Unchoose: pop the candidate. The state shrinks back. Ready for the next candidate.

{{zoom: 1x}} {{focus: none}} Push, recurse, pop. That's the heartbeat of backtracking.

```code:template-code lang=cpp
#include <vector>

void backtrack(const std::vector<int>& candidates, int start,
               std::vector<int>& current, std::vector<std::vector<int>>& results) {
    if (/* base case: solution is complete */) {
        results.push_back(current);
        return;
    }

    for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
        // Choose
        current.push_back(candidates[i]);

        // Explore
        backtrack(candidates, i + 1, current, results);

        // Unchoose
        current.pop_back();
    }
}
---
signature: 3-4
base-case: 5-8
loop: 10
choose-step: 12
recurse: 15
unchoose-step: 18
```

# Permutations

{{clear: slide}}

{{show: perm-tree grow 0.5s spring}} Generate all orderings of n elements. Every permutation is a full path in the decision tree.

{{focus: level-0}} {{annotate: level-0 "Pick first"}} First position: three choices — 1, 2, or 3.

{{focus: level-1}} {{annotate: level-1 "Pick second"}} Second position: two remaining choices. The used elements are excluded.

{{focus: level-2}} {{annotate: level-2 "Pick third"}} Third position: one choice left. The permutation is complete.

{{focus: none}} n factorial total permutations. Each leaf is one complete ordering.

{{clear: slide}}

{{show: perm-code typewriter 2s linear}} Use a used-flags vector instead of a start index. Every position considers every unused element.

{{focus: used-check}} {{zoom: 1.2x}} {{annotate: used-check "Skip taken"}} If element i is used, skip it. This prevents repeating elements within a single permutation.

{{zoom: 1x}} {{focus: mark}} {{zoom: 1.3x}} {{annotate: mark "Reserve slot"}} Mark element i as used before recursing. This is the choose step.

{{zoom: 1x}} {{focus: unmark}} {{zoom: 1.2x}} {{annotate: unmark "Release slot"}} Unmark element i after recursing. This is the unchoose step. The element is available again for other branches.

{{zoom: 1x}} {{focus: full-perm}} {{annotate: full-perm "Leaf = result"}} When the current path has n elements, it's a complete permutation. Push it to results.

{{zoom: 1x}} {{focus: none}} Permutations: loop over all elements, skip used ones. Combinations: loop from start index. That's the difference.

```data:perm-tree type=tree
         *
       / | \
      1   2   3
     / \  |  / \
   12  13 21 31 32
   |   |  |  |   |
  123 132 213 312 321
---
level-0: 1, 2, 3
level-1: 12, 13, 21, 31, 32
level-2: 123, 132, 213, 312, 321
```

```code:perm-code lang=cpp
#include <vector>

void permute(const std::vector<int>& nums, std::vector<int>& current,
             std::vector<bool>& used, std::vector<std::vector<int>>& results) {
    if (current.size() == nums.size()) {
        results.push_back(current);
        return;
    }

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (used[i]) continue;

        used[i] = true;
        current.push_back(nums[i]);
        permute(nums, current, used, results);
        current.pop_back();
        used[i] = false;
    }
}

// Usage:
// std::vector<int> nums = {1, 2, 3};
// std::vector<int> current;
// std::vector<bool> used(nums.size(), false);
// std::vector<std::vector<int>> results;
// permute(nums, current, used, results);
---
used-check: 11
mark: 13
unmark: 16-17
full-perm: 5-8
```

# Combinations

{{clear: slide}}

{{show: combo-code typewriter 2s linear}} Choose k elements from n. Order doesn't matter — {1,2,3} and {3,2,1} are the same combination.

{{focus: start-idx}} {{zoom: 1.2x}} {{annotate: start-idx "Forward only"}} The start index prevents picking earlier elements. If you chose element 2, the next choice starts from element 3. This avoids duplicates by construction.

{{zoom: 1x}} {{focus: base}} {{zoom: 1.3x}} {{annotate: base "Size = k"}} Base case: the current path has k elements. That's a complete combination. Save it.

{{zoom: 1x}} {{focus: choose-explore}} {{zoom: 1.2x}} {{annotate: choose-explore "Push, recurse"}} Push the element, recurse with i plus 1 as the new start. Each level picks from a shrinking pool.

{{zoom: 1x}} {{focus: unchoose}} {{annotate: unchoose "Pop"}} Pop and try the next element. Standard backtrack.

{{zoom: 1x}} {{focus: none}} Combinations use the start index. Permutations use the used-flags vector. Both are the same template with different candidate logic.

```code:combo-code lang=cpp
#include <vector>

void combine(int n, int k, int start,
             std::vector<int>& current, std::vector<std::vector<int>>& results) {
    if (static_cast<int>(current.size()) == k) {
        results.push_back(current);
        return;
    }

    for (int i = start; i <= n; ++i) {
        current.push_back(i);
        combine(n, k, i + 1, current, results);
        current.pop_back();
    }
}

// combine(4, 2, ...) produces:
// {1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4}
---
start-idx: 10
base: 5-8
choose-explore: 11-12
unchoose: 13
```

# Subsets

{{clear: slide}}

{{show: subset-code typewriter 2s linear}} Generate all subsets of a set — the power set. Every node in the decision tree is a valid result, not just the leaves.

{{focus: collect-all}} {{zoom: 1.2x}} {{annotate: collect-all "Every node"}} No base case check needed for "is it complete." Every partial path is a valid subset. Push to results at every recursive call.

{{zoom: 1x}} {{focus: include}} {{zoom: 1.3x}} {{annotate: include "Take it"}} For each element, two choices: include it or skip it. The loop with start index handles both — including element i and then not including it (when the loop advances).

{{zoom: 1x}} {{focus: skip}} {{zoom: 1.2x}} {{annotate: skip "Leave it"}} When the current call ends without pushing more elements, that branch chose to skip the remaining elements. The subset is smaller.

{{zoom: 1x}} {{focus: none}} Subsets: 2 to the n results. Every subset from empty to full. The code is the combination template without the size check.

```code:subset-code lang=cpp
#include <vector>

void subsets(const std::vector<int>& nums, int start,
             std::vector<int>& current, std::vector<std::vector<int>>& results) {
    results.push_back(current);  // every partial path is a subset

    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        current.push_back(nums[i]);
        subsets(nums, i + 1, current, results);
        current.pop_back();
    }
}

// subsets({1, 2, 3}) produces:
// {}, {1}, {1,2}, {1,2,3}, {1,3}, {2}, {2,3}, {3}
---
collect-all: 5
include: 8-9
skip: 10
```

# Pruning strategies

{{clear: slide}}

{{show: prune-code typewriter 2s linear}} Pruning is what makes backtracking practical. Cut branches early based on constraints.

{{focus: size-prune}} {{zoom: 1.2x}} {{annotate: size-prune "Not enough left"}} Remaining elements check: if there aren't enough elements left to reach size k, skip. No point exploring a branch that can't produce a valid solution.

{{zoom: 1x}} {{focus: dup-prune}} {{zoom: 1.3x}} {{annotate: dup-prune "Skip duplicates"}} Duplicate skip: if the current element equals the previous one at the same decision level, skip it. This avoids generating duplicate subsets from arrays with repeated values. Sort the input first.

{{zoom: 1x}} {{focus: sum-prune}} {{zoom: 1.2x}} {{annotate: sum-prune "Over budget"}} Sum constraint: if adding the current element exceeds the target sum, skip it and everything after it (if sorted). All larger elements will also exceed the target.

{{zoom: 1x}} {{focus: none}} Good pruning can reduce exponential to near-polynomial in practice. The earlier you prune, the more subtrees you skip.

```code:prune-code lang=cpp
#include <vector>
#include <algorithm>

// Pruning 1: Not enough elements remaining
void combine_pruned(int n, int k, int start,
    std::vector<int>& cur, std::vector<std::vector<int>>& res) {
    if (static_cast<int>(cur.size()) == k) { res.push_back(cur); return; }

    // Prune: need k - cur.size() more elements, only n - i + 1 available
    for (int i = start; i <= n - (k - static_cast<int>(cur.size())) + 1; ++i) {
        cur.push_back(i);
        combine_pruned(n, k, i + 1, cur, res);
        cur.pop_back();
    }
}

// Pruning 2: Skip duplicates (nums must be sorted)
void subsets_no_dup(std::vector<int>& nums, int start,
    std::vector<int>& cur, std::vector<std::vector<int>>& res) {
    res.push_back(cur);
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        if (i > start && nums[i] == nums[i - 1]) continue;  // skip dup
        cur.push_back(nums[i]);
        subsets_no_dup(nums, i + 1, cur, res);
        cur.pop_back();
    }
}

// Pruning 3: Sum exceeds target (nums sorted ascending)
void combo_sum(std::vector<int>& nums, int target, int start,
    std::vector<int>& cur, std::vector<std::vector<int>>& res) {
    if (target == 0) { res.push_back(cur); return; }
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        if (nums[i] > target) break;  // prune: too large, and all after are larger
        cur.push_back(nums[i]);
        combo_sum(nums, target - nums[i], i, cur, res);
        cur.pop_back();
    }
}
---
size-prune: 10
dup-prune: 22
sum-prune: 34
```

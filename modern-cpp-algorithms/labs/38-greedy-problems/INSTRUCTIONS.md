# Lab 38 — Greedy Problems

Greedy algorithms make the locally optimal choice at each step, trusting it leads to a globally optimal solution. The hard part is proving the greedy choice is safe — these four problems are canonical examples where it is.

## Tasks

- [ ] Implement `can_jump(nums)` — each element is the maximum jump length; return true if you can reach the last index from index 0
- [ ] Implement `min_jumps(nums)` — return the minimum number of jumps to reach the last index; input is guaranteed to be reachable
- [ ] Implement `gas_station(gas, cost)` — find the starting gas station index for a circular trip where you collect gas[i] and spend cost[i] at each station; return -1 if impossible
- [ ] Implement `partition_labels(s)` — partition the string so each letter appears in at most one part; return a vector of part sizes (greedy: extend current partition to last occurrence of each character seen)

## Hints

- `can_jump`: track the farthest reachable index; at each position, if it exceeds current farthest, fail; otherwise update farthest with `i + nums[i]`
- `min_jumps`: track current jump boundary and next reachable boundary; increment jump count each time you cross the current boundary
- `gas_station`: if total gas < total cost, return -1; otherwise, simulate greedily — reset start to `i+1` whenever the running tank goes negative
- `partition_labels`: precompute the last index of each character; walk the string, tracking the max last-index seen; when current index equals that max, a partition ends

Use `Run Tests` to check your progress.

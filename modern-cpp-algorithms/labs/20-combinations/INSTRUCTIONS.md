# Combinations and Permutations — Backtracking

Backtracking generates all valid configurations by building candidates incrementally and abandoning paths that cannot lead to a solution.

## Tasks

- [ ] Implement `permutations(nums)` — return all permutations of the input vector.
- [ ] Implement `combinations(n, k)` — return all combinations of `k` numbers from `1..n`.
- [ ] Implement `subsets(nums)` — return all subsets (the power set) of the input vector.

## Hints

- `permutations`: swap-based backtracking or use a `used` boolean array. Fix one position at a time.
- `combinations`: start from a position, pick the next number, recurse with remaining. Prune when not enough numbers remain.
- `subsets`: for each element, include it or skip it. Alternatively, iterate and extend existing subsets.

Use `Run Tests` to check your progress.

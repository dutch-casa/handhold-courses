# Capacity Planning — Binary Search on Answer

When the answer itself is a number in a range and you can check feasibility, binary search the answer space.

## Tasks

- [ ] Implement `can_ship(weights, trucks, capacity)` — return `true` if all packages can be shipped in `trucks` trips with the given capacity. Packages must stay in order; each truck is loaded left-to-right until adding the next package would exceed capacity.
- [ ] Implement `min_capacity(weights, trucks)` — return the minimum truck capacity needed to ship all packages in exactly `trucks` trips. Binary search over the answer using `can_ship`.
- [ ] Implement `peak_of_mountain(arr)` — return the index of the peak element in a mountain array.

## Hints

- Lower bound for capacity: the heaviest single package. Upper bound: sum of all packages.
- `can_ship`: greedily fill each truck. If you need more trucks than allowed, return false.
- `peak_of_mountain` is the same algorithm as `find_peak` from the previous lab — practice makes permanent.

Use `Run Tests` to check your progress.

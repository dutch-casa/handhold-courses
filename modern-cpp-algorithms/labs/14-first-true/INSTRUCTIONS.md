# First True — Binary Search on Predicates

Binary search isn't just for sorted arrays. Any monotonic predicate over an ordered domain can be bisected.

## Tasks

- [ ] Implement `first_true(arr, pred)` — return the first index where `pred` is true on a sorted/monotonic array. Return `-1` if no element satisfies it.
- [ ] Implement `first_bad_version(n, is_bad)` — versions 1..n, `is_bad` flips from false to true at some point. Return the first bad version.
- [ ] Implement `find_peak(arr)` — given a mountain array (strictly increases then strictly decreases), return the index of the peak element.

## Hints

- `first_true`: classic lower-bound binary search. If `pred(mid)` is true, the answer is at `mid` or left of it.
- `first_bad_version`: identical structure — bisect on the predicate `is_bad(version)`.
- `find_peak`: compare `arr[mid]` with `arr[mid+1]` to decide which half contains the peak.

Use `Run Tests` to check your progress.

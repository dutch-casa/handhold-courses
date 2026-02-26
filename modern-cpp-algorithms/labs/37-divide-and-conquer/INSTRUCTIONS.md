# Lab 37 — Divide and Conquer

Divide and conquer breaks a problem into independent subproblems, solves them recursively, and combines the results. The key insight is that the combination step is where interesting work happens — inversions are counted during the merge, and majority elements are identified by cancellation across halves.

## Tasks

- [ ] Implement `merge_sort(nums)` — return a sorted copy of nums using the merge sort algorithm
- [ ] Implement `count_inversions(nums)` — return the number of pairs (i, j) where i < j but nums[i] > nums[j]; piggyback on the merge sort merge step
- [ ] Implement `majority_element(nums)` — return the element that appears more than n/2 times; it is guaranteed to exist; use Boyer-Moore voting or divide and conquer

## Hints

- `merge_sort`: split at midpoint, recurse on left and right, merge two sorted halves into result
- `count_inversions`: during the merge step, when you pick from the right half, every remaining element in the left half forms an inversion with it; add `(mid - left_index)` to the count
- `majority_element`: Boyer-Moore voting: maintain a candidate and a count; increment on match, decrement on mismatch; when count reaches zero, the next element becomes the new candidate

Use `Run Tests` to check your progress.

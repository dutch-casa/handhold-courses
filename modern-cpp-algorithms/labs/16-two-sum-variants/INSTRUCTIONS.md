# Two-Sum Variants — Two Pointers

Two pointers converge on sorted data. One from each end, adjusting based on the sum comparison.

## Tasks

- [ ] Implement `two_sum_sorted(nums, target)` — given a sorted array, return the 1-indexed pair of indices whose values sum to `target`. Exactly one solution exists.
- [ ] Implement `three_sum(nums)` — return all unique triplets that sum to zero. No duplicate triplets.
- [ ] Implement `max_water(heights)` — compute the maximum water a container can hold (container with most water problem).

## Hints

- `two_sum_sorted`: left pointer at 0, right at end. If sum < target, move left up. If sum > target, move right down.
- `three_sum`: sort first, fix one element, then run two-sum on the remainder. Skip duplicates at each level.
- `max_water`: area = min(left, right) * width. Move the shorter side inward.

Use `Run Tests` to check your progress.

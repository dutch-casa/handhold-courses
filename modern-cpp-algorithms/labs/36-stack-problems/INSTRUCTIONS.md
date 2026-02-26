# Lab 36 — Stack Problems

The monotonic stack is one of the most powerful tricks in competitive programming. By maintaining a stack that is always increasing or decreasing, you resolve "next greater/smaller" queries in O(n) instead of O(n²).

## Tasks

- [ ] Implement `next_greater(nums)` — for each element, find the next element to its right that is strictly greater; return -1 if none exists
- [ ] Implement `daily_temperatures(temps)` — for each day, return how many days you must wait until a warmer temperature; 0 if no warmer day ever comes
- [ ] Implement `largest_rectangle(heights)` — given a histogram of bar heights, return the area of the largest rectangle that fits within it

## Hints

- `next_greater`: maintain a stack of indices; when you see a larger value, pop and resolve all waiting indices
- `daily_temperatures`: same monotonic stack pattern — stack stores indices; resolve when current temp exceeds temp at stack top
- `largest_rectangle`: use a monotonic increasing stack of indices; when a bar is shorter than the top, pop and compute area using the popped bar as the height and the distance to the new top as the width; push a sentinel 0 at the end to flush

Use `Run Tests` to check your progress.

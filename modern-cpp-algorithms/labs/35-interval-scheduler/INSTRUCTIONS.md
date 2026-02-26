# Lab 35 — Interval Scheduler

Intervals appear everywhere: calendar events, CPU scheduling, memory allocation. Mastering interval problems means recognizing when to sort by start, when to use a min-heap for end times, and how to merge greedily.

## Tasks

- [ ] Implement `merge_intervals(intervals)` — sort by start, then greedily merge overlapping pairs; return the merged list
- [ ] Implement `insert_interval(intervals, new_interval)` — insert a new interval into a sorted, non-overlapping list and merge as needed; return the result
- [ ] Implement `min_meeting_rooms(intervals)` — return the minimum number of rooms needed so all meetings can run simultaneously

## Hints

- `merge_intervals`: sort by `first`, then walk linearly — if current start ≤ last merged end, extend; otherwise push a new interval
- `insert_interval`: collect intervals that come entirely before, merge all overlapping, then collect the rest
- `min_meeting_rooms`: sort start times and end times separately, use two pointers; increment rooms when a meeting starts before the earliest ending meeting finishes

Use `Run Tests` to check your progress.

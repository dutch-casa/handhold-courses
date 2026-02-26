# Substring Search — Sliding Window

Sliding windows maintain a moving range over a sequence, expanding and contracting to satisfy constraints.

## Tasks

- [ ] Implement `longest_unique_substr(s)` — return the length of the longest substring without repeating characters.
- [ ] Implement `min_window(s, t)` — return the minimum window substring of `s` that contains all characters of `t` (including duplicates). Return `""` if no such window exists.

## Hints

- `longest_unique_substr`: expand right, contract left when a duplicate enters the window. Track characters with a set or frequency map.
- `min_window`: expand right until all chars of `t` are covered, then contract left to minimize. Use frequency counts for both `t` and the current window.

Use `Run Tests` to check your progress.

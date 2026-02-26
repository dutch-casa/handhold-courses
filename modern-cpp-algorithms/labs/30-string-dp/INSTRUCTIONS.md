# Lab 30 — String Dynamic Programming

String DP with reconstruction. Building the DP table is half the work; tracing back through it to produce the actual result is the other half.

## Tasks

- [ ] Implement `lcs(a, b)` — return the longest common subsequence string (not just its length)
- [ ] Implement `edit_distance(a, b)` — return the minimum number of single-character insertions, deletions, or substitutions to transform `a` into `b`
- [ ] Implement `longest_palindrome(s)` — return the longest palindromic substring of `s`; if multiple share the maximum length, return the first one

## Hints

- `lcs`: fill an (m+1)×(n+1) table; dp[i][j] = dp[i-1][j-1]+1 if chars match, else max(dp[i-1][j], dp[i][j-1]); reconstruct by tracing back from dp[m][n]
- `edit_distance`: dp[i][j] = cost to convert a[0..i) to b[0..j); if chars match cost=0, else 1; take min(insert, delete, replace)
- `longest_palindrome`: expand-around-center in O(n²); check both odd-length (single center) and even-length (between two chars) expansions; track the longest window seen

Use `Run Tests` to check your progress.

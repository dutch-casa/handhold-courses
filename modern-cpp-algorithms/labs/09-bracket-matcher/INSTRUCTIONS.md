# Bracket Matcher

Check balanced brackets using `std::stack`.

## Tasks

- [ ] Implement `is_balanced(string_view s)` — return `true` if all `()`, `[]`, `{}` are properly balanced
- [ ] Implement `first_unbalanced(string_view s)` — return the index of the first unbalanced bracket, or `-1` if fully balanced

## Hints

Push opening brackets onto a stack. When you encounter a closing bracket, check that the top of the stack is the matching opener.

For `first_unbalanced`, track the index of each bracket. If a closing bracket doesn't match, return its index. If the stack is non-empty after processing all characters, return the index of the earliest unmatched opener.

Non-bracket characters should be ignored.

Use `Run Tests` in the testing panel to check your progress.

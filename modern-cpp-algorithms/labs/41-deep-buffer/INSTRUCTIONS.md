# Lab 41 — Deep Buffer

The gap buffer is the data structure powering most text editors. It maintains two contiguous segments of text with a "gap" at the cursor position, making insert and delete O(1) amortized at the cursor while hiding all complexity behind a clean interface.

## Tasks

- [ ] Implement `TextBuffer()` — default constructor; start with empty text and cursor at position 0
- [ ] Implement `insert(ch)` — insert a single character at the cursor position; cursor advances past it
- [ ] Implement `insert_string(text)` — insert all characters of text at the cursor; cursor ends past the last inserted character
- [ ] Implement `erase()` — delete the character immediately before the cursor (backspace behavior); no-op if cursor is at 0
- [ ] Implement `move_left()` — move cursor one position left; no-op at position 0
- [ ] Implement `move_right()` — move cursor one position right; no-op at end of text
- [ ] Implement `move_to(pos)` — move cursor to an absolute position (clamped to valid range)
- [ ] Implement `text()` — return the full text content as a string
- [ ] Implement `cursor_pos()` — return the current cursor position (0-indexed)

## Hints

- You do not need to implement an actual gap buffer — a `std::string` split at the cursor (left part + right part), or a single string with a cursor index, works correctly and is Rule of Zero compliant
- The simplest correct implementation: store `std::string left_` (everything before cursor) and `std::string right_` (everything after); `text()` returns `left_ + right_`; `cursor_pos()` returns `left_.size()`
- `insert(ch)`: append ch to `left_`
- `erase()`: pop the last char of `left_` if non-empty
- `move_left()`: move the last char of `left_` to the front of `right_`
- `move_right()`: move the first char of `right_` to the back of `left_`

Use `Run Tests` to check your progress.

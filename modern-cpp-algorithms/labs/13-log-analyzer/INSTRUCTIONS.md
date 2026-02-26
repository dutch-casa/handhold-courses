# Log Analyzer

Parse and filter log lines using string operations and algorithms.

## Tasks

- [ ] Implement `split_lines(string_view text)` — split text by newline into a vector of non-empty strings
- [ ] Implement `filter_by_level(const vector<string>& lines, string_view level)` — keep lines containing the given level string (e.g. `"ERROR"`)
- [ ] Implement `take_recent(const vector<string>& lines, int n)` — return the last `n` lines
- [ ] Implement `count_level(const vector<string>& lines, string_view level)` — count lines containing the level string

## Hints

For `split_lines`, use `std::istringstream` and `std::getline`. Skip empty lines.

For `filter_by_level`, check if each line contains the level substring using `std::string::find`.

For `take_recent`, if `n` exceeds the number of lines, return all lines. Use iterators or `std::vector::end() - n`.

`count_level` is similar to `filter_by_level` but just counts matches instead of collecting them.

Use `Run Tests` in the testing panel to check your progress.

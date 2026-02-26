# Word Counter

Count word frequencies in text using `string_view` and `std::format`.

## Tasks

- [ ] Implement `count_words(string_view text)` returning a `map<string, int>` of word frequencies
- [ ] Implement `most_frequent(const map<string, int>& counts)` returning the word with the highest count
- [ ] Implement `format_counts(const map<string, int>& counts)` returning a formatted string like `"apple: 3, banana: 1"`

## Hints

Use `std::istringstream` to split text into words.

`std::string_view` can be passed to `std::istringstream` by constructing a `std::string` from it.

For `format_counts`, iterate the map and use `std::format` to build the output string. Separate entries with `", "`.

Use `Run Tests` in the testing panel to check your progress.

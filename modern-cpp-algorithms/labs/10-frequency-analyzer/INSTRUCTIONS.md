# Frequency Analyzer

Build character, word, and bigram frequency maps from text.

## Tasks

- [ ] Implement `char_freq(string_view text)` — return a `map<char, int>` of character frequencies
- [ ] Implement `word_freq(string_view text)` — return a `map<string, int>` of word frequencies
- [ ] Implement `bigram_freq(string_view text)` — return a `map<string, int>` where each key is `"word1 word2"` representing consecutive word pairs
- [ ] Implement `most_common_bigram(string_view text)` — return the most frequent bigram as `"word1 word2"`

## Hints

For `char_freq`, iterate every character in the text (including spaces and punctuation).

For `word_freq`, use `std::istringstream` to split on whitespace.

For `bigram_freq`, split into words first, then slide a window of size 2 across consecutive words. Store each pair as `"word1 word2"` in the map key.

For `most_common_bigram`, iterate the bigram map and track the entry with the highest count.

Use `Run Tests` in the testing panel to check your progress.

# Lab 32 — Autocomplete Engine

Build a Trie (prefix tree) that supports prefix-based word lookup and retrieval. Tries trade memory for O(k) lookup time where k is key length, independent of dictionary size.

## Tasks

- [ ] Implement `Trie::insert(word)` — add a word to the trie
- [ ] Implement `Trie::search(word)` — return `true` if the exact word was inserted
- [ ] Implement `Trie::starts_with(prefix)` — return `true` if any inserted word begins with `prefix`
- [ ] Implement `Trie::autocomplete(prefix)` — return all inserted words beginning with `prefix`, sorted lexicographically
- [ ] Implement `Trie::remove(word)` — delete a word; return `true` if it existed; do not orphan shared prefixes

## Hints

- Use `struct TrieNode { std::array<std::unique_ptr<TrieNode>, 26> children{}; bool is_end = false; };`
- `insert` / `search` / `starts_with`: walk character by character; index = `c - 'a'`
- `autocomplete`: find the node at the end of the prefix, then DFS to collect all words where `is_end` is true
- `remove`: recursive DFS; unset `is_end` at the target; a node is safe to delete if it has no children and is not an endpoint of another word

Use `Run Tests` to check your progress.

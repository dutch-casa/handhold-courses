---
title: Trie
---

# Prefix tree idea

{{show: trie-viz grow 0.5s spring}} A trie — also called a prefix tree — stores strings by sharing their common prefixes. Instead of storing "apple", "app", "application" three times, they share the path a-p-p.

{{focus: root}} {{annotate: root "Common root"}} Every string starts from the same root node. Characters are stored on edges (or in child slots), not in nodes themselves. The path from root to a node spells out a prefix.

{{focus: shared}} {{annotate: shared "Shared prefixes"}} "app" and "apple" share the path a→p→p. The is_end flag on the 'p' node marks "app" as a complete word. "apple" continues to 'l'→'e' with its own is_end. No duplication of the shared prefix.

{{focus: complexity}} {{annotate: complexity "O(L) operations"}} Lookup, insert, and delete are all O(L) where L is the string length. Independent of the number of strings stored. Compare to a hash map: O(L) average but with hash collisions.

{{focus: none}} Tries are the right structure when you need prefix queries — autocomplete, IP routing tables, spell checkers. The prefix is the key feature hash maps can't match.

```data:trie-viz type=tree
{"value": "(root)", "children": [{"value": "a", "children": [{"value": "p", "children": [{"value": "p [end]", "children": [{"value": "l", "children": [{"value": "e [end]"}]}]}]}]}, {"value": "b", "children": [{"value": "a", "children": [{"value": "t [end]"}]}]}]}
---
root: (root)
shared: p [end]
complexity: (root)
```

# TrieNode in C++

{{clear: slide}}

{{show: node-code typewriter 2s linear}} The TrieNode struct. Children are an array of 26 unique pointers — one slot per lowercase letter. Null pointer means that child doesn't exist.

{{focus: children}} {{zoom: 1.2x}} {{annotate: children "Unique ownership"}} `std::array<std::unique_ptr<TrieNode>, 26>` gives automatic memory management. When a TrieNode is destroyed, all its children are destroyed recursively — no manual cleanup needed.

{{zoom: 1x}} {{focus: is-end}} {{zoom: 1.2x}} {{annotate: is-end "Word boundary"}} `bool is_end` marks that a complete word ends at this node. A node can be both a word end and an interior node — "app" ends here, "apple" continues through here.

{{zoom: 1x}} {{focus: index}} {{zoom: 1.2x}} {{annotate: index "Letter to index"}} `c - 'a'` maps 'a'→0, 'b'→1, ... 'z'→25. Simple, O(1). The array's fixed size means no hash function, no collision, deterministic cache behavior.

{{zoom: 1x}} {{focus: none}} The 26-slot array is the standard for lowercase-only tries. For unicode, use `unordered_map<char32_t, unique_ptr<TrieNode>>` instead — slower but handles arbitrary characters.

```code:node-code lang=cpp
#include <array>
#include <memory>
#include <string>

struct TrieNode {
    std::array<std::unique_ptr<TrieNode>, 26> children{};
    bool is_end = false;

    // Convenience: get child index for character c
    static int idx(char c) { return c - 'a'; }
};

class Trie {
    std::unique_ptr<TrieNode> root_ = std::make_unique<TrieNode>();
public:
    // Insert, search, startsWith, erase declared here
};
---
children: 5
is-end: 6
index: 9
```

# Insert

{{clear: slide}}

{{show: insert-code typewriter 1.5s linear}} Insert walks the trie character by character. If a child node doesn't exist for the current character, create it. At the end of the word, set is_end.

{{focus: walk}} {{zoom: 1.2x}} {{annotate: walk "Walk or create"}} Start at root. For each character, compute its index. If the child slot is null, allocate a new TrieNode there. Move down to that child. Never backtrack.

{{zoom: 1x}} {{focus: mark-end}} {{zoom: 1.2x}} {{annotate: mark-end "Mark completion"}} After processing all characters, set `node->is_end = true`. This is what distinguishes "app" (a word) from "ap" (just a prefix that hasn't been inserted as a word).

{{zoom: 1x}} {{focus: complexity}} {{zoom: 1.2x}} {{annotate: complexity "O(L) guaranteed"}} Each character creates at most one node. L characters, L node accesses or creations. Total memory across all inserts: O(total characters across all words). Shared prefixes pay once.

{{zoom: 1x}} {{focus: none}} Insert is clean and linear. The only allocation is `make_unique<TrieNode>()` for new paths — existing paths are just pointer traversals.

```code:insert-code lang=cpp
void insert(const std::string& word) {
    TrieNode* node = root_.get();

    for (char c : word) {
        const int i = TrieNode::idx(c);
        if (!node->children[i]) {
            node->children[i] = std::make_unique<TrieNode>();
        }
        node = node->children[i].get();
    }
    node->is_end = true;
}
---
walk: 3-8
mark-end: 9
complexity: 2
```

# Search and prefix check

{{clear: slide}}

{{show: search-code typewriter 1.5s linear}} Two queries: search checks for a complete word, startsWith checks for any prefix. They share the same walk logic — only the final check differs.

{{focus: shared-walk}} {{zoom: 1.2x}} {{annotate: shared-walk "Same traversal"}} Both functions walk the trie identically. Character by character, follow the child pointers. If any child is null, the path doesn't exist — return false immediately.

{{zoom: 1x}} {{focus: search-check}} {{zoom: 1.2x}} {{annotate: search-check "Exact word: is_end"}} search returns `node->is_end` at the end. The path exists (you didn't return early), but is this a complete word or just a prefix of a longer word? is_end tells you.

{{zoom: 1x}} {{focus: prefix-check}} {{zoom: 1.2x}} {{annotate: prefix-check "Prefix: just reach it"}} startsWith returns true if you reach the end of the prefix without hitting a null pointer. Any word in the trie with this prefix ensures the path exists.

{{zoom: 1x}} {{focus: none}} The asymmetry is subtle but important. A failed search on "app" when only "apple" is inserted returns false — is_end is false at the 'p' node. But startsWith("app") returns true — the path exists.

```code:search-code lang=cpp
// Returns true if word exists exactly in the trie
bool search(const std::string& word) const {
    const TrieNode* node = root_.get();
    for (char c : word) {
        const int i = TrieNode::idx(c);
        if (!node->children[i]) return false;
        node = node->children[i].get();
    }
    return node->is_end;
}

// Returns true if any inserted word starts with prefix
bool startsWith(const std::string& prefix) const {
    const TrieNode* node = root_.get();
    for (char c : prefix) {
        const int i = TrieNode::idx(c);
        if (!node->children[i]) return false;
        node = node->children[i].get();
    }
    return true;  // reached the prefix node — path exists
}
---
shared-walk: 3-7
search-check: 8
prefix-check: 17
```

# Delete

{{clear: slide}}

{{show: delete-code typewriter 2s linear}} Deletion is the tricky operation. You need to remove the word's is_end marker and clean up any nodes that are now orphaned — no longer part of any word.

{{focus: strategy}} {{zoom: 1.2x}} {{annotate: strategy "Recursive, bottom-up"}} Use recursion: descend to the end of the word, then on the way back up, check if each node should be removed. A node is removable if it has no children and is not the end of another word.

{{zoom: 1x}} {{focus: erase}} {{zoom: 1.2x}} {{annotate: erase "Clear is_end first"}} At the target node (end of the word), clear is_end. Then returning from recursion, a node is a candidate for deletion if it has no children and is not its own word end.

{{zoom: 1x}} {{focus: has-children}} {{zoom: 1.3x}} {{annotate: has-children "Check all slots"}} A node has children if any of its 26 child pointers is non-null. Use ranges::any_of to check. If any child exists, this node is still needed as a prefix node — don't delete it.

{{zoom: 1x}} {{focus: none}} The unique_ptr ownership makes deletion safe: assigning nullptr to the parent's child slot triggers the child's destructor recursively. No manual free needed.

```code:delete-code lang=cpp
// Returns true if the node at this path can be deleted
bool erase(TrieNode* node, const std::string& word, int depth) {
    if (!node) return false;

    if (depth == static_cast<int>(word.size())) {
        node->is_end = false;
        // Node can be deleted if it has no children
        return !hasChildren(node);
    }

    const int i = TrieNode::idx(word[depth]);
    if (erase(node->children[i].get(), word, depth + 1)) {
        node->children[i].reset();  // delete the child subtree
        return !node->is_end && !hasChildren(node);
    }
    return false;
}

bool hasChildren(const TrieNode* node) {
    return std::ranges::any_of(node->children,
        [](const auto& c) { return c != nullptr; });
}

void erase(const std::string& word) {
    erase(root_.get(), word, 0);
}
---
strategy: 2
erase: 5-8
has-children: 17-19
```

# Autocomplete

{{clear: slide}}

{{show: autocomplete-code typewriter 2s linear}} Autocomplete: given a prefix, return all words in the trie that start with that prefix. Walk to the prefix node, then DFS to collect all words below.

{{focus: find-prefix}} {{zoom: 1.2x}} {{annotate: find-prefix "Walk to prefix node"}} First, walk the trie to find the node where the prefix ends. If any character is missing, no words have that prefix — return empty.

{{zoom: 1x}} {{focus: collect-dfs}} {{zoom: 1.3x}} {{annotate: collect-dfs "DFS collect words"}} From the prefix node, run DFS. Maintain a current path string. At each is_end node, push the accumulated word to results. The prefix is prepended before DFS starts.

{{zoom: 1x}} {{focus: none}} This is O(P + W) where P is the prefix length and W is the total characters in all matching words. Efficient — you only traverse the subtree rooted at the prefix node, not the entire trie.

```code:autocomplete-code lang=cpp
std::vector<std::string> autocomplete(const std::string& prefix) const {
    const TrieNode* node = root_.get();

    // Walk to prefix node
    for (char c : prefix) {
        const int i = TrieNode::idx(c);
        if (!node->children[i]) return {};
        node = node->children[i].get();
    }

    // DFS from prefix node to collect all words
    std::vector<std::string> results;
    std::string current = prefix;

    std::function<void(const TrieNode*)> dfs = [&](const TrieNode* n) {
        if (n->is_end) results.push_back(current);

        for (int i = 0; i < 26; ++i) {
            if (!n->children[i]) continue;
            current.push_back('a' + i);
            dfs(n->children[i].get());
            current.pop_back();
        }
    };
    dfs(node);
    return results;
}
---
find-prefix: 3-7
collect-dfs: 12-22
```

# Compressed trie

{{clear: slide}}

{{show: compressed-viz grow 0.5s spring}} A compressed trie — or radix tree — merges chains of single-child nodes into one edge labeled with a string. It reduces node count from O(total characters) to O(number of strings).

{{focus: chains}} {{annotate: chains "Single-child chains"}} In a regular trie, "there" and "their" share 't'→'h'→'e'. Then they diverge. But if only these two words exist, the chain t→h→e is wasteful — three nodes, no branching, no information.

{{focus: merge}} {{annotate: merge "Merge to edge label"}} Compress it: one node, one edge labeled "the", then children 'r' and 'i'. Each edge carries a string instead of a single character. Branching nodes are preserved; linear chains collapse.

{{focus: tradeoffs}} {{annotate: tradeoffs "When to use"}} Regular tries are simpler to implement and good enough for most competitive programming. Radix trees shine in production IP routing tables and in-memory databases where millions of strings share prefixes and memory is tight.

{{focus: none}} The DSL/stdlib doesn't have a built-in radix tree. Implement one when profiling shows the trie's memory dominates. For interview problems, the regular trie is always sufficient.

```data:compressed-viz type=tree
{"value": "(root)", "children": [{"value": "\"the\" →", "children": [{"value": "re [end]"}, {"value": "ir [end]"}]}, {"value": "\"bat\" → [end]"}]}
---
chains: (root)
merge: "the" →
tradeoffs: (root)
```

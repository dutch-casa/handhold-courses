#include "solution.h"
#include <algorithm>

Trie::Trie() : root_(std::make_unique<TrieNode>()) {}

void Trie::insert(const std::string& word) {
    TrieNode* cur = root_.get();
    for (char c : word) {
        auto idx = static_cast<std::size_t>(c - 'a');
        if (!cur->children[idx]) {
            cur->children[idx] = std::make_unique<TrieNode>();
        }
        cur = cur->children[idx].get();
    }
    cur->is_end = true;
}

// Walk to the end of prefix/word; return nullptr if path is missing.
static const TrieNode* walk(const TrieNode* root, const std::string& s) {
    const TrieNode* cur = root;
    for (char c : s) {
        auto idx = static_cast<std::size_t>(c - 'a');
        if (!cur->children[idx]) return nullptr;
        cur = cur->children[idx].get();
    }
    return cur;
}

bool Trie::search(const std::string& word) const {
    const TrieNode* node = walk(root_.get(), word);
    return node && node->is_end;
}

bool Trie::starts_with(const std::string& prefix) const {
    return walk(root_.get(), prefix) != nullptr;
}

static void collect(const TrieNode* node, std::string& current, std::vector<std::string>& out) {
    if (node->is_end) out.push_back(current);
    for (int i = 0; i < 26; ++i) {
        auto si = static_cast<std::size_t>(i);
        if (node->children[si]) {
            current.push_back(static_cast<char>('a' + i));
            collect(node->children[si].get(), current, out);
            current.pop_back();
        }
    }
}

std::vector<std::string> Trie::autocomplete(const std::string& prefix) const {
    const TrieNode* node = walk(root_.get(), prefix);
    if (!node) return {};
    std::vector<std::string> results;
    std::string current = prefix;
    collect(node, current, results);
    std::ranges::sort(results);
    return results;
}

// Returns true if the node can be pruned (no children, not an endpoint).
static bool remove_rec(TrieNode* node, const std::string& word, int depth, bool& found) {
    if (depth == static_cast<int>(word.size())) {
        found = node->is_end;
        node->is_end = false;
        // Prune if no children remain
        return !node->is_end && std::ranges::none_of(node->children, [](const auto& c) { return c != nullptr; });
    }
    auto idx = static_cast<std::size_t>(word[static_cast<std::size_t>(depth)] - 'a');
    if (!node->children[idx]) return false;
    bool should_delete = remove_rec(node->children[idx].get(), word, depth + 1, found);
    if (should_delete) node->children[idx].reset();
    return !node->is_end && std::ranges::none_of(node->children, [](const auto& c) { return c != nullptr; });
}

bool Trie::remove(const std::string& word) {
    bool found = false;
    remove_rec(root_.get(), word, 0, found);
    return found;
}

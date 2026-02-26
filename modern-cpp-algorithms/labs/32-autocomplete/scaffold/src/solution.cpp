#include "solution.h"

Trie::Trie() : root_(std::make_unique<TrieNode>()) {}

// TODO: walk each character; create nodes as needed; set is_end at last character
void Trie::insert(const std::string& word) {
    // TODO
}

// TODO: walk each character; return false if any node is missing; check is_end at last node
bool Trie::search(const std::string& word) const {
    // TODO
    return false;
}

// TODO: same walk as search but do not require is_end
bool Trie::starts_with(const std::string& prefix) const {
    // TODO
    return false;
}

// TODO: navigate to prefix node then DFS to collect all words
std::vector<std::string> Trie::autocomplete(const std::string& prefix) const {
    // TODO
    return {};
}

// TODO: recursive remove; unset is_end; prune childless non-endpoint nodes
bool Trie::remove(const std::string& word) {
    // TODO
    return false;
}

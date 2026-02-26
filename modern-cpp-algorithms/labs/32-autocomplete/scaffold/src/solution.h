#pragma once
#include <array>
#include <memory>
#include <string>
#include <vector>

struct TrieNode {
    std::array<std::unique_ptr<TrieNode>, 26> children{};
    bool is_end = false;
};

class Trie {
public:
    Trie();

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    bool starts_with(const std::string& prefix) const;
    std::vector<std::string> autocomplete(const std::string& prefix) const;
    bool remove(const std::string& word);

private:
    std::unique_ptr<TrieNode> root_;
};

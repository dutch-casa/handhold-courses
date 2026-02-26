#include <gtest/gtest.h>
#include "../src/solution.h"
#include <algorithm>

TEST(Trie, InsertAndSearch) {
    Trie t;
    t.insert("apple");
    EXPECT_TRUE(t.search("apple"));
    EXPECT_FALSE(t.search("app"));
    EXPECT_FALSE(t.search("apples"));
}

TEST(Trie, StartsWith) {
    Trie t;
    t.insert("apple");
    EXPECT_TRUE(t.starts_with("app"));
    EXPECT_TRUE(t.starts_with("apple"));
    EXPECT_FALSE(t.starts_with("apples"));
    EXPECT_FALSE(t.starts_with("banana"));
}

TEST(Trie, SearchMissingWord) {
    Trie t;
    EXPECT_FALSE(t.search("anything"));
}

TEST(Trie, AutocompleteBasic) {
    Trie t;
    t.insert("apple");
    t.insert("app");
    t.insert("application");
    t.insert("banana");
    auto results = t.autocomplete("app");
    std::sort(results.begin(), results.end());
    ASSERT_EQ(static_cast<int>(results.size()), 3);
    EXPECT_EQ(results[0], "app");
    EXPECT_EQ(results[1], "apple");
    EXPECT_EQ(results[2], "application");
}

TEST(Trie, AutocompleteNoMatch) {
    Trie t;
    t.insert("cat");
    EXPECT_TRUE(t.autocomplete("dog").empty());
}

TEST(Trie, AutocompleteEmptyPrefix) {
    Trie t;
    t.insert("cat");
    t.insert("car");
    t.insert("bar");
    auto results = t.autocomplete("");
    EXPECT_EQ(static_cast<int>(results.size()), 3);
}

TEST(Trie, RemoveExisting) {
    Trie t;
    t.insert("apple");
    t.insert("app");
    EXPECT_TRUE(t.remove("apple"));
    EXPECT_FALSE(t.search("apple"));
    EXPECT_TRUE(t.search("app"));   // shared prefix must survive
}

TEST(Trie, RemoveMissing) {
    Trie t;
    t.insert("apple");
    EXPECT_FALSE(t.remove("banana"));
    EXPECT_TRUE(t.search("apple"));
}

TEST(Trie, RemoveAndAutocomplete) {
    Trie t;
    t.insert("apple");
    t.insert("app");
    t.insert("application");
    t.remove("app");
    auto results = t.autocomplete("app");
    std::sort(results.begin(), results.end());
    ASSERT_EQ(static_cast<int>(results.size()), 2);
    EXPECT_EQ(results[0], "apple");
    EXPECT_EQ(results[1], "application");
}

TEST(Trie, DuplicateInsert) {
    Trie t;
    t.insert("hello");
    t.insert("hello");
    EXPECT_TRUE(t.search("hello"));
    auto results = t.autocomplete("hel");
    EXPECT_EQ(static_cast<int>(results.size()), 1);
}

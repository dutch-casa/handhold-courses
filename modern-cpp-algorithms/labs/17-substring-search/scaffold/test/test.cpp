#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(LongestUniqueSubstr, BasicCase) {
    EXPECT_EQ(longest_unique_substr("abcabcbb"), 3);
}

TEST(LongestUniqueSubstr, AllSame) {
    EXPECT_EQ(longest_unique_substr("bbbbb"), 1);
}

TEST(LongestUniqueSubstr, Mixed) {
    EXPECT_EQ(longest_unique_substr("pwwkew"), 3);
}

TEST(LongestUniqueSubstr, EmptyString) {
    EXPECT_EQ(longest_unique_substr(""), 0);
}

TEST(LongestUniqueSubstr, AllUnique) {
    EXPECT_EQ(longest_unique_substr("abcdef"), 6);
}

TEST(MinWindow, BasicCase) {
    EXPECT_EQ(min_window("ADOBECODEBANC", "ABC"), "BANC");
}

TEST(MinWindow, ExactMatch) {
    EXPECT_EQ(min_window("a", "a"), "a");
}

TEST(MinWindow, NoMatch) {
    EXPECT_EQ(min_window("a", "aa"), "");
}

TEST(MinWindow, WholeString) {
    EXPECT_EQ(min_window("abc", "abc"), "abc");
}

TEST(MinWindow, DuplicateCharsInT) {
    EXPECT_EQ(min_window("aabbc", "abc"), "abbc");
}

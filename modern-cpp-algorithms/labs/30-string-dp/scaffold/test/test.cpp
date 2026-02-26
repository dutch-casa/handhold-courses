#include <gtest/gtest.h>
#include "../src/solution.h"

// --- lcs ---

TEST(LCS, Classic) {
    // "ABCBDAB" and "BDCAB" => "BCAB" or "BDAB" (length 4); we pin to one
    auto result = lcs("ABCBDAB", "BDCAB");
    EXPECT_EQ(static_cast<int>(result.size()), 4);
}

TEST(LCS, Identical) {
    EXPECT_EQ(lcs("abc", "abc"), "abc");
}

TEST(LCS, NoCommon) {
    EXPECT_EQ(lcs("abc", "xyz"), "");
}

TEST(LCS, OneEmpty) {
    EXPECT_EQ(lcs("", "hello"), "");
}

TEST(LCS, SingleChar) {
    EXPECT_EQ(lcs("a", "a"), "a");
}

// --- edit_distance ---

TEST(EditDistance, ClassicPair) {
    // "horse" -> "ros" => 3
    EXPECT_EQ(edit_distance("horse", "ros"), 3);
}

TEST(EditDistance, BothEmpty) {
    EXPECT_EQ(edit_distance("", ""), 0);
}

TEST(EditDistance, OneEmpty) {
    EXPECT_EQ(edit_distance("abc", ""), 3);
    EXPECT_EQ(edit_distance("", "abc"), 3);
}

TEST(EditDistance, Identical) {
    EXPECT_EQ(edit_distance("kitten", "kitten"), 0);
}

TEST(EditDistance, SingleSubstitution) {
    EXPECT_EQ(edit_distance("a", "b"), 1);
}

// --- longest_palindrome ---

TEST(LongestPalindrome, OddCenter) {
    // "babad" => "bab" or "aba"; both length 3
    auto r = longest_palindrome("babad");
    EXPECT_EQ(static_cast<int>(r.size()), 3);
}

TEST(LongestPalindrome, EvenCenter) {
    // "cbbd" => "bb"
    EXPECT_EQ(longest_palindrome("cbbd"), "bb");
}

TEST(LongestPalindrome, EntireString) {
    EXPECT_EQ(longest_palindrome("racecar"), "racecar");
}

TEST(LongestPalindrome, SingleChar) {
    EXPECT_EQ(longest_palindrome("a"), "a");
}

TEST(LongestPalindrome, AllSame) {
    EXPECT_EQ(longest_palindrome("aaaa"), "aaaa");
}

#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(CharFreq, Basic) {
    auto freq = char_freq("aab");
    EXPECT_EQ(freq['a'], 2);
    EXPECT_EQ(freq['b'], 1);
}

TEST(CharFreq, WithSpaces) {
    auto freq = char_freq("a b a");
    EXPECT_EQ(freq['a'], 2);
    EXPECT_EQ(freq[' '], 2);
    EXPECT_EQ(freq['b'], 1);
}

TEST(CharFreq, Empty) {
    auto freq = char_freq("");
    EXPECT_TRUE(freq.empty());
}

TEST(WordFreq, Basic) {
    auto freq = word_freq("the cat and the dog");
    EXPECT_EQ(freq["the"], 2);
    EXPECT_EQ(freq["cat"], 1);
    EXPECT_EQ(freq["and"], 1);
    EXPECT_EQ(freq["dog"], 1);
}

TEST(WordFreq, SingleWord) {
    auto freq = word_freq("hello");
    EXPECT_EQ(freq["hello"], 1);
    EXPECT_EQ(freq.size(), 1u);
}

TEST(WordFreq, Empty) {
    auto freq = word_freq("");
    EXPECT_TRUE(freq.empty());
}

TEST(BigramFreq, Basic) {
    auto freq = bigram_freq("a b c a b");
    EXPECT_EQ(freq["a b"], 2);
    EXPECT_EQ(freq["b c"], 1);
    EXPECT_EQ(freq["c a"], 1);
}

TEST(BigramFreq, TwoWords) {
    auto freq = bigram_freq("hello world");
    EXPECT_EQ(freq["hello world"], 1);
    EXPECT_EQ(freq.size(), 1u);
}

TEST(BigramFreq, SingleWord) {
    auto freq = bigram_freq("alone");
    EXPECT_TRUE(freq.empty());
}

TEST(BigramFreq, Empty) {
    auto freq = bigram_freq("");
    EXPECT_TRUE(freq.empty());
}

TEST(MostCommonBigram, Basic) {
    EXPECT_EQ(most_common_bigram("a b c a b"), "a b");
}

TEST(MostCommonBigram, AllUnique) {
    auto result = most_common_bigram("x y z");
    // Two bigrams "x y" and "y z" each with count 1; either is acceptable
    EXPECT_TRUE(result == "x y" || result == "y z");
}

TEST(MostCommonBigram, Empty) {
    EXPECT_EQ(most_common_bigram(""), "");
}

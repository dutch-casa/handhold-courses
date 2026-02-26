#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(WordCounter, SingleWord) {
    auto counts = count_words("hello");
    EXPECT_EQ(counts["hello"], 1);
    EXPECT_EQ(counts.size(), 1u);
}

TEST(WordCounter, MultipleWords) {
    auto counts = count_words("the cat sat on the mat");
    EXPECT_EQ(counts["the"], 2);
    EXPECT_EQ(counts["cat"], 1);
    EXPECT_EQ(counts["sat"], 1);
    EXPECT_EQ(counts["mat"], 1);
}

TEST(WordCounter, EmptyString) {
    auto counts = count_words("");
    EXPECT_TRUE(counts.empty());
}

TEST(MostFrequent, Basic) {
    std::map<std::string, int> counts = {{"apple", 3}, {"banana", 1}, {"cherry", 2}};
    EXPECT_EQ(most_frequent(counts), "apple");
}

TEST(MostFrequent, SingleEntry) {
    std::map<std::string, int> counts = {{"only", 5}};
    EXPECT_EQ(most_frequent(counts), "only");
}

TEST(FormatCounts, Basic) {
    std::map<std::string, int> counts = {{"apple", 3}, {"banana", 1}};
    EXPECT_EQ(format_counts(counts), "apple: 3, banana: 1");
}

TEST(FormatCounts, SingleEntry) {
    std::map<std::string, int> counts = {{"hello", 5}};
    EXPECT_EQ(format_counts(counts), "hello: 5");
}

TEST(FormatCounts, Empty) {
    std::map<std::string, int> counts;
    EXPECT_EQ(format_counts(counts), "");
}

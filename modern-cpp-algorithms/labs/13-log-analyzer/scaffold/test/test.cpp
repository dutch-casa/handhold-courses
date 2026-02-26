#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(SplitLines, Basic) {
    auto lines = split_lines("line1\nline2\nline3\n");
    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "line1");
    EXPECT_EQ(lines[1], "line2");
    EXPECT_EQ(lines[2], "line3");
}

TEST(SplitLines, SkipsEmpty) {
    auto lines = split_lines("a\n\nb\n\nc\n");
    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "a");
    EXPECT_EQ(lines[1], "b");
    EXPECT_EQ(lines[2], "c");
}

TEST(SplitLines, NoTrailingNewline) {
    auto lines = split_lines("first\nsecond");
    ASSERT_EQ(lines.size(), 2u);
    EXPECT_EQ(lines[0], "first");
    EXPECT_EQ(lines[1], "second");
}

TEST(SplitLines, Empty) {
    auto lines = split_lines("");
    EXPECT_TRUE(lines.empty());
}

TEST(FilterByLevel, ErrorOnly) {
    std::vector<std::string> lines = {
        "2024-01-01 INFO startup",
        "2024-01-01 ERROR disk full",
        "2024-01-01 WARN low memory",
        "2024-01-01 ERROR timeout"
    };
    auto result = filter_by_level(lines, "ERROR");
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "2024-01-01 ERROR disk full");
    EXPECT_EQ(result[1], "2024-01-01 ERROR timeout");
}

TEST(FilterByLevel, NoMatches) {
    std::vector<std::string> lines = {
        "INFO ok",
        "WARN caution"
    };
    auto result = filter_by_level(lines, "ERROR");
    EXPECT_TRUE(result.empty());
}

TEST(FilterByLevel, AllMatch) {
    std::vector<std::string> lines = {
        "ERROR one",
        "ERROR two"
    };
    auto result = filter_by_level(lines, "ERROR");
    EXPECT_EQ(result.size(), 2u);
}

TEST(TakeRecent, LastTwo) {
    std::vector<std::string> lines = {"a", "b", "c", "d"};
    auto result = take_recent(lines, 2);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "c");
    EXPECT_EQ(result[1], "d");
}

TEST(TakeRecent, MoreThanAvailable) {
    std::vector<std::string> lines = {"a", "b"};
    auto result = take_recent(lines, 10);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
}

TEST(TakeRecent, Zero) {
    std::vector<std::string> lines = {"a", "b", "c"};
    auto result = take_recent(lines, 0);
    EXPECT_TRUE(result.empty());
}

TEST(TakeRecent, Empty) {
    std::vector<std::string> lines;
    auto result = take_recent(lines, 5);
    EXPECT_TRUE(result.empty());
}

TEST(CountLevel, Mixed) {
    std::vector<std::string> lines = {
        "INFO start",
        "ERROR fail",
        "INFO ok",
        "ERROR crash",
        "ERROR panic"
    };
    EXPECT_EQ(count_level(lines, "ERROR"), 3);
    EXPECT_EQ(count_level(lines, "INFO"), 2);
    EXPECT_EQ(count_level(lines, "WARN"), 0);
}

TEST(CountLevel, Empty) {
    std::vector<std::string> lines;
    EXPECT_EQ(count_level(lines, "ERROR"), 0);
}

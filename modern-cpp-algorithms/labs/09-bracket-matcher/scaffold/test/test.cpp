#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(IsBalanced, EmptyString) {
    EXPECT_TRUE(is_balanced(""));
}

TEST(IsBalanced, SimpleParens) {
    EXPECT_TRUE(is_balanced("()"));
}

TEST(IsBalanced, SimpleBrackets) {
    EXPECT_TRUE(is_balanced("[]"));
}

TEST(IsBalanced, SimpleBraces) {
    EXPECT_TRUE(is_balanced("{}"));
}

TEST(IsBalanced, Nested) {
    EXPECT_TRUE(is_balanced("{[()]}"));
}

TEST(IsBalanced, Sequential) {
    EXPECT_TRUE(is_balanced("()[]{}"));
}

TEST(IsBalanced, WithContent) {
    EXPECT_TRUE(is_balanced("int main() { return a[0]; }"));
}

TEST(IsBalanced, Unmatched) {
    EXPECT_FALSE(is_balanced("(]"));
}

TEST(IsBalanced, UnclosedOpen) {
    EXPECT_FALSE(is_balanced("({)"));
}

TEST(IsBalanced, ExtraClose) {
    EXPECT_FALSE(is_balanced("())"));
}

TEST(IsBalanced, ExtraOpen) {
    EXPECT_FALSE(is_balanced("(()"));
}

TEST(FirstUnbalanced, Balanced) {
    EXPECT_EQ(first_unbalanced("()[]{}"), -1);
}

TEST(FirstUnbalanced, MismatchClose) {
    EXPECT_EQ(first_unbalanced("(]"), 1);
}

TEST(FirstUnbalanced, ExtraClose) {
    EXPECT_EQ(first_unbalanced("())"), 2);
}

TEST(FirstUnbalanced, UnclosedOpen) {
    EXPECT_EQ(first_unbalanced("("), 0);
}

TEST(FirstUnbalanced, NestedUnclosed) {
    EXPECT_EQ(first_unbalanced("{[}"), 2);
}

TEST(FirstUnbalanced, Empty) {
    EXPECT_EQ(first_unbalanced(""), -1);
}
